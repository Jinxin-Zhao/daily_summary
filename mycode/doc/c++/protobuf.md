### proto files explaination
### advantages & disadvantages
#### advantages
+ 性能好效率高： 占用空间比xml少3-10倍，序列化的时间效率比xml快20-100倍
+ 有代码生成机制
+ 支持向后和向前兼容： 当客户端和服务端同时使用一块协议的时候，当客户端在协议中增加一个字节，并不会影响客户端的使用
+ 支持多种编程语言： Protobuf目前支持Java、C++、Python、Go、Ruby等多种语言
+ 更重要的是，你甚至可以在无需重新部署程序的情况下更新数据结构。
#### disadvantages
+ 二进制格式存储，可读性差
+ 缺乏自描述
#### 编译
+ 安装完proto编译器protoc，就可通过protoc编译生成对应的头文件和源码文件。
```cpp
protoc -I$SRC_DIR --cpp_out=$DST_DIR message.proto
```
#### proto3
+ 文档网址(https://protobuf.dev/programming-guides/proto3/)
##### 字段定义
消息定义中的每个字段都有一个唯一的编号。这些字段编号用于在消息二进制格式中标识您的字段，并且在使用您的消息类型后不应更改。请注意，1 到 15 范围内的字段编号需要一个字节进行编码，包括字段编号和字段类型（您可以在Protocol Buffer Encoding中找到更多相关信息）。16 到 2047 范围内的字段编号占用两个字节。因此，您应该为非常频繁出现的消息元素保留数字 1 到 15。
+ singular： 一个message可以有0个或者1个此类消息（但不能超过1个），当用proto3语法时，当没有给字段指定字段规则时，这个就是默认规则。
+ optional：字段值指定与否都可以。如果没有指定一个 optional 的字段值，它就会使用默认值。标识是一个可选字段，发送方在发送消息的时候，可以有选择性的设置或不设置该值。对于接收方，如果能够识别，就进行相应的处理，如果无法识别，就直接忽略该字段，消息中的其他字段正常处理。
+ map：<key, value>, map类型的字段不可以是repeated类型。
+ required: 必须提供字段值，否则对应的消息会被认为是“未初始化的”。标识必需设置字段，发送方(或者说序列化前)必须设置该值。而对于接收方，则必须能够识别该字段。如果 libprotobuf 是以debug 模式编译的，序列化一个未初始化的消息（message）将会导致一个断言错误。例如 release 模式，该检查会被跳过，消息会被写入。然而，解析一个未初始化的消息仍然会失败（解析函数会返回 false）。除此之外，一个 required 的字段与一个 optional 的字段就没有区别了。
+ repeated: 标识该字段可以包含0或多个元素，其本质特性也是optional的，但是一次可以包含多个值(可以类比理解为一个动态的数组)。这里需要注意一下：基本数值类型的repeated的字段并没有被尽可能的高效编码。在编写过程中，应该使用附加的特殊选项[packed = true]来保证更高效的编码。
+ reserved: 如果您通过完全删除某个字段或将其注释掉来更新消息类型，未来的用户可以在对类型进行自己的更新时重用该字段编号。如果他们稍后加载相同的旧版本，这可能会导致严重问题.proto，包括数据损坏、隐私错误等。确保不会发生这种情况的一种方法是指定已删除字段的字段编号（和/或名称，这也可能导致 JSON 序列化问题）为reserved. 如果将来有任何用户尝试使用这些字段标识符，protocol buffer 编译器会抱怨。
<font color=blue size=5>默认值</font>
| 字段类型 | field_name |
| ------ | ------ |
| 字符串 | 空字符串 |
| 字节 | 空字节 |
| 布尔值 | false |
| 数字类型 | 0 |
| enums | 第一个定义的enum value必须是0 |
| 消息字段 | 未设置该字段，确切值取决于语言 |

##### 别名
+ 您可以通过将相同的值分配给不同的枚举常量来定义别名。为此，您需要将allow_alias选项设置为true，否则协议编译器将在找到别名时生成错误消息。
##### 导入定义
+ 您可以通过导入.proto来自其他文件的定义来使用它们。要导入另一个的定义，请在文件顶部添加一个 import 语句：
```proto
import "myproject/other_protos.proto";
```
##### 更新消息类型
+ 如果现有的消息类型不再满足您的所有需求 - 例如，您希望消息格式有一个额外的字段 - 但您仍然希望使用使用旧格式创建的代码，需遵循以下规则：
  - 不要更改任何现有字段的字段编号。
  - 如果您添加新字段，则使用“旧”消息格式的代码序列化的任何消息仍然可以由新生成的代码解析。您应该记住这些元素的默认值，以便新代码可以正确地与旧代码生成的消息交互。类似地，新代码创建的消息可以由旧代码解析：旧二进制文件在解析时会忽略新字段。有关详细信息，请参阅未知字段部分。
  - 只要在更新的消息类型中不再使用字段编号，就可以删除字段。您可能想要重命名该字段，可能添加前缀“OBSOLETE_”，或将字段编号设为保留，以便您的未来用户.proto不会意外重用该编号。
  - int32、uint32、int64、uint64和bool都是兼容的——这意味着您可以将字段从其中一种类型更改为另一种类型，而不会破坏向前或向后兼容性。如果从不适合相应类型的线路中解析出一个数字，您将获得与在 C++ 中将该数字强制转换为该类型相同的效果（例如，如果将 64 位数字读取为int32，它将被截断为 32 位）。
  - sint32并且sint64彼此兼容，但与其他整数类型不兼容。
  - string并且bytes只要字节是有效的 UTF-8 就兼容。
  - fixed32与sfixed32和fixed64兼容sfixed64。
##### Any
+ 消息类型允许您将Any消息用作嵌入类型，而无需它们的 .proto 定义。AnAny包含任意序列化消息 as bytes,要使用该Any类型，您需要导入 google/protobuf/any.proto.
```cpp
import "google/protobuf/any.proto";

message ErrorStatus {
  string message = 1;
  repeated google.protobuf.Any details = 2;
}

// PackFrom & UnpackTo()
// Storing an arbitrary message type in Any.
NetworkErrorDetails details = ...;
ErrorStatus status;
status.add_details()->PackFrom(details);

// Reading an arbitrary message from Any.
ErrorStatus status = ...;
for (const Any& detail : status.details()) {
  if (detail.Is<NetworkErrorDetails>()) {
    NetworkErrorDetails network_error;
    detail.UnpackTo(&network_error);
    ... processing network_error ...
  }
}
```
#### 使用Oneof
+ 如果您有一条包含多个字段的消息，并且最多同时设置一个字段，您可以强制执行此行为并使用 oneof 功能节省内存。oneof 字段与常规字段一样，除了一个 oneof 共享内存中的所有字段外，最多可以同时设置一个字段。设置 oneof 的任何成员会自动清除所有其他成员。可以添加任何类型的字段，但map字段和repeated字段除外。
  - oneof 不能是repeated。
  - 如果解析器在线路上遇到同一个成员的多个成员，则在解析的消息中只使用最后一个看到的成员。
  - 反射 API 适用于 oneof 字段。
  - 如果将 oneof 字段设置为默认值（例如将 int32 oneof 字段设置为 0），则会设置该 oneof 字段的“大小写”，并且该值将在线上序列化。
  - 如果您使用 C++，请确保您的代码不会导致内存崩溃。以下示例代码将崩溃，因为sub_message已通过调用该set_name()方法删除。
  - 再次在 C++ 中，如果您Swap()使用 oneofs 发送两条消息，则每条消息都将以另一个的 oneof 情况结束：在下面的示例中，msg1将具有 asub_message和msg2将具有name.
#### 示例
+ as you can see, every single field has a sole remark, it cannot change once used:
  - [1,15] remark will take one byte;
  - [16,2047] remark will take two bytes;
  the remark range is [1,536870911] except [19000-19999] which is reserved for system;
+ the first enum variable must be 0; 
+ 限定修饰符  数据类型  字段名称 = 字段标识编号  字段选项
  - ex： optional int32 id = 1 [default = 1]
+ 一个消息中各字段的标识号无需是连续的，只要确保其合法性
+ 一个消息中各字段标识号唯一，不可重复
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

message PhoneNumber {
    required string number = 1;
    optional PhoneType type = 2 [default = HOME];
}

message Family {
  string query = 1;
  int32 page_number = 2;
  int32 result_per_page = 3;

  enum PhoneType {
    MOBILE = 0;
    HOME = 1;
  }
  repeated PhoneNumber phone = 4;
  repeated Person person = 1;
}
// reserved
message Foo {
  reserved 2, 15, 9 to 11;
  reserved "foo", "bar";
} //不能在同一reserved语句中混合字段名称和字段编号

// alias
message MyMessage1 {
  enum EnumAllowingAlias {
    option allow_alias = true;
    UNKNOWN = 0;
    STARTED = 1;
    RUNNING = 1;
  }
}
message MyMessage2 {
  enum EnumNotAllowingAlias {
    UNKNOWN = 0;
    STARTED = 1;
    // RUNNING = 1;  // Uncommenting this line will cause a compile error inside Google and a warning message outside.
  }
}
// 类型嵌套
message SearchResponse {
  message Result {
    string url = 1;
    string title = 2;
    repeated string snippets = 3;
  }
  repeated Result results = 1;
}

// 在其他消息里重用Result类型
message SomeOtherMessage {
  SearchResponse.Result result = 1;
}

message SampleMessage {
  oneof test_oneof {
    string name = 4;
    SubMessage sub_message = 9;
  }
}
```

### grpc
+ 下载
  - 鉴于国外网速，国内同步镜像地址: git clone https://gitee.com/mirrors/grpc-framework grpc
+ 安装
  - https://developer.aliyun.com/article/819208