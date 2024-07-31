##Flink

### Time
+ 当在流程序中引用时间(例如定义窗口)时，可以参考不同的时间概念：
    - Event Time: 创建事件的时间。它通常由事件中的时间戳描述。Flink通过时间戳分配器访问事件时间戳。
    - Ingestion Time: 事件在源操作员处进入FLink数据流的时间。
    - Processing Time: 执行基于时间的操作的每个操作员的本地时间。
    一个事件的处理时间 = Processing Time - Ingestion Time


### windows
+ Window是无限数据流处理的核心，Window将一个无限的stream拆分成有限大小的”buckets”桶，我们可以在这些桶上做计算操作。
+ 窗口化的Flink程序的一般结构如下，第一个代码段中是分组的流，而第二段是非分组的流。正如我们所见，唯一的区别是分组的stream调用keyBy(…)和window(…)，而非分组的stream中window()换成了windowAll(…)，这些也将贯穿都这一页的其他部分中。
+ 每个窗口都有"触发器" 和 "函数"（PeocesWindowFunction,ReduceFunction,AggregateFunction,FoldFunction）;
+ 聚合事件(例如，计数，总和)在流上的工作方式与批处理方式（有界流）不同。无界流上的聚合（计数，总和等）由窗口限定，比如在最后五分钟内计数 或 最后100个元素的和等场景。 

#### window assigners
窗口可以是时间驱动(如每30秒) 或数据驱动（如每100个元素），一般用到的三种窗口：
翻滚窗口（无重叠），滑动窗口（有重叠），会话窗口（由不活动的间隙打断），全局窗口

+ keyed windows
```java
stream.keyBy(...)          <-  keyed versus non-keyed windows
       .window(...)         <-  required: "assigner"
      [.trigger(...)]       <-  optional: "trigger" (else default trigger)
      [.evictor(...)]       <-  optional: "evictor" (else no evictor)
      [.allowedLateness()]  <-  optional, else zero
       .reduce/fold/apply() <-  required: "function"
```

+ non-keyed windows
```java
stream.windowAll(...)      <-  required: "assigner"
      [.trigger(...)]       <-  optional: "trigger" (else default trigger)
      [.evictor(...)]       <-  optional: "evictor" (else no evictor)
      [.allowedLateness()]  <-  optional, else zero
       .reduce/fold/apply() <-  required: "function"
```

#### window 生命周期
+ 当一个属于window的元素到达之后这个window就创建了，而当当前时间(事件或者处理时间)为window的创建时间跟用户指定的延迟时间相加时，窗口将被彻底清除。Flink 确保了只清除基于时间的window，其他类型的window不清除:
例如： 对于一个每5分钟创建无覆盖的(即 翻滚窗口)窗口，允许一个1分钟的时延的窗口策略，Flink将会在12:00到12:05这段时间内第一个元素到达时创建窗口，当水印通过12:06时，移除这个窗口。

#### window 分类
+ Tumbling windows
+ Sliding windows
+ Session windows
+ Global windows

#### window 方法
+ PeocesWindowFunction 
+ ReduceFunction  
+ AggregateFunction
+ FoldFunction
    - 不能用于会话窗口和可合并的窗口

#### 如何使用flatmap方法
```java
SingleOutputStreamOperator transStream = stream.flatMap(new FlatMapFunction<String, Tuple4<String, String, Integer, Long>>() {
			@Override
			public void flatMap(String input, Collector<Tuple4<String, String, Integer, Long>> collector) throws Exception {
				Transaction transaction = JSON.parseObject(input, Transaction.class);
				System.out.println(transaction);
				collector.collect(new Tuple4<>(transaction.getSymbol(), transaction.getType(), transaction.getCount(), transaction.getTimestamp()));
			}
		});
```
flatmap方法最终返回的是一个collector，而这个collector只有一层，当输入数据有嵌套的情况下，可以将数据平铺处理。

#### 如何使用filter方法
```java
SingleOutputStreamOperator<UrlInfo> filterSource = flatSource.filter(urlInfo -> {
            if(StringUtils.equals(UrlInfo.BAIDU,urlInfo.getDomain())){
                return true;
            }
            return false;
        });

        filterSource.addSink(new PrintSinkFunction<>());
```





### flink触发器
+ 触发器定义了window何时会被求值以及何时发送求值结果。
+ 默认的触发器将会在两种情况下触发
  处理时间：机器时间到达处理时间
  事件时间：水位线超过了窗口的结束时间
#### TriggerResult取值：
 每次调用触发器都会产生一个TriggerResult来决定窗口接下来发生什么。TriggerResult可以取以下结果：
 + CONTINUE: do nothing
 + FIRE: 如果window operator有ProcessWindowFunction这个参数，将会调用这个ProcessWindowFunction。如果窗口仅有增量聚合函数(ReduceFunction或者AggregateFunction)作为参数，那么当前的聚合结果将会被发送。窗口的state不变。
 + PURGE: 窗口所有内容包括窗口的元数据都将被丢弃。
 + FIRE_AND_PURGE: 先对窗口进行求值，再将窗口中的内容丢弃。