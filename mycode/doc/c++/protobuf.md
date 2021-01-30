### Thrift files explaination
#### Attention
+ Thift生成的server端是thread safe的，但是client端不是thread safe的，所以需要多个thread和server端通信,则每个thread需要initiate一个自己的client实例。
+ 如果服务器采用TNonblockingServer的话，客户端必须采用TFramedTransport。程序链接的时候需要thriftnb。
+ 默认TServerSocket和TSocket都设置了NoDelay为1，使得报文尽快发送出去，如果客户端和服务器间传输数据量较大，通过可以设置NoDelay为0来开启Nagel算法，缓存一段数据后再进行发送，减少报文数量.
TSocket默认开启了Linger，并设置linger time为0，这样close会丢弃socket发送缓冲区中的数据，并向对端发送一个RST报文，close不会被阻塞，立即返回。
TServerSocket默认关闭了Linger，close不会被阻塞，立即返回.
+ fb303作为handler的基类，里面预置了一些rpc方法，用于监控，包括系统状态，请求次数等状态信息。
  thrift文件中需要include "fb303.thrift"这样来将service导入目标thrift文件中。thrift编译后的代码只需要相应的Handler多重继承facebook::fb303::FacebookBase就好了。
  ```cpp
  class scribeHandler : virtual public scribe::thrift::scribeIf,
                                  public facebook::fb303::FacebookBase {
  ```
+ thrift支持完全async，生成代码的时候需要使用
   thrift --gen cpp:cob_style xxx.thrift


### proto files explaination
### advantages & disadvantages
#### advantages
+ 性能好效率高： 占用空间比xml少3-10倍，序列化的时间效率比xml快20-100倍
+ 有代码生成机制
+ 支持向后和向前兼容： 当客户端和服务端同时使用一块协议的时候，当客户端在协议中增加一个字节，并不会影响客户端的使用
+ 支持多种编程语言： Protobuf目前支持Java、C++、Python、Go、Ruby等多种语言
#### disadvantages
+ 二进制格式存储，可读性差
+ 缺乏自描述
#### usage
+ 限定修饰符  数据类型  字段名称 = 字段标识编号  字段选项
  - ex： optional int32 id = 1 [default = 1]
+ 一个消息中各字段的标识号无需是连续的，只要确保其合法性
+ 一个消息中各字段标识号唯一，不可重复
#### Qualifier
+ required: 标识必需设置字段，发送方(或者说序列化前)必须设置该值。而对于接收方，则必须能够识别该字段。
+ optional: 标识是一个可选字段，发送方在发送消息的时候，可以有选择性的设置或不设置该值。对于接收方，如果能够识别，就进行相应的处理，如果无法识别，就直接忽略该字段，消息中的其他字段正常处理。
+ repeated: 标识该字段可以包含0或多个元素，其本质特性也是optional的，但是一次可以包含多个值(可以类比理解为一个动态的数组)。这里需要注意一下：基本数值类型的repeated的字段并没有被尽可能的高效编码。在编写过程中，应该使用附加的特殊选项[packed = true]来保证更高效的编码。
#### proto3
+ as you can see, every single field has a sole remark, it cannot change once used:
  - [1,15] remark will take one byte;
  - [16,2047] remark will take two bytes;
  the remark range is [1,536870911] except [19000-19999] which is reserved for system;
+ the first enum variable must be 0; 
```cpp
enum IDE1 {
    IDEA = 0;
    CLION = 1;
}
```

+ the `repeated` keyword represents Array of some type
```cpp
message Person {
  required int32 age = 1;
  required string name = 2;
}

message Family {
  repeated Person person = 1;
}
```