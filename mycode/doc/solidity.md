### Solidity
#### 数据型态
+ 在solidity里每个型态都有default值，不存在未定义和null；且分为三种型态： 数值(Value TYpes), 引用(Reference Types), 映射(Mapping types)
    ```solidity
    1. bool VarName = true | false;

    //2.  int, uint, intN(指定整数的空间大小，N必为8倍数，[8,256]), uintN
    // int = int256, uint = uint256

    //3.  address： 储存一个以太坊的地址，大小为20b
    // usage: address (payable) VarName = 0x....
    // payable关键字是指定允许转移以太到该地址，如果没有这个修饰字意味着不可以转钱到这个地址
    address X = 0x9A87...
    address payable Y = payable(0x...)

    //4. enum EnumName {element1, ...}
    enum Color {Blue, Green}
    Color C = color.Blue
    ```
    