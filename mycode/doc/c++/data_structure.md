<!-- TOC -->

[1. priority_queue](#1-priority_queue)
[2. HashMap](#2-hashmap)
[3. Map](#3-map)
[4. DFS_BFS](#4-dfs_bfs)
[5. Prime_Kruskal](#5-prime_kruskal)

<!-- TOC -->

# 1. priority_queue
+ summary
    - priority_queue<Type, Container, Functional>，其中Type 为数据类型，Container为保存数据的容器，Functional 为元素比较方式。
    - Container必须是用**数组**实现的容器，比如vector,deque等等，**但不能用list**。STL里面默认用的是vector。
    - 比较方式默认用operator<，所以如果把后面2个参数缺省的话，优先队列就是**大顶堆（降序）**，队头元素最大。**特别注意pair的比较函数。**
+ priority_queue function:
    - normal type;
    ```
    #include <iostream>
    #include <queue>
    using namespace std;
    int main(){
    priority_queue<int> q;
prime_kruskal++i ) q.push(i)prime_kruskal
prime_kruskal
prime_kruskal;
        q.pop();
    }
    //output：
    //9 8 7 6 ...  0
    return 0;
    }
    ```
    - pair<> type;
    ```
    //先按照pair的first元素降序，first元素相等时，再按照second元素降序：
    #include<iostream>
    #include<vector>
    #include<queue>
    using namespace std;
    int main(){
    priority_queue<pair<int,int> > coll;
    pair<int,int> a(3,4);
    pair<int,int> b(3,5);
    pair<int,int> c(4,3);
    coll.push(c);
    coll.push(b);
    coll.push(a);
    while(!coll.empty())
    {
        cout<<coll.top().first<<"\t"<<coll.top().second<<endl;
        coll.pop();
    }
    // output:
    //  4	3
    //  3	5
    //  3	4
    return 0;
    }
    ```
    - make ascending order(最小堆)
    ```
    #include <iostream>
    #include <queue> 
    using namespace std;
    int main(){
    priority_queue<int, vector<int>, greater<int> > q;
    for( int i= 0; i< 10; ++i ) q.push(10-i);
    while( !q.empty() ){
        cout << q.top() << " ";
        q.pop();
    }
    // output：
    //1 2 3 ... 10
    return 0;
    }


    ///
    #include<iostream>
    #include<vector>
    #include<queue>
    using namespace std;
    int main(){
    priority_queue<pair<int,int>,vector<pair<int,int> >,greater<pair<int,int> > > coll;
    pair<int,int> a(3,4);
    pair<int,int> b(3,5);
    pair<int,int> c(4,3);
    coll.push(c);
    coll.push(b);
    coll.push(a);
    while(!coll.empty())
    {
        cout<<coll.top().first<<"\t"<<coll.top().second<<endl;
        coll.pop();
    }
    return 0;
    }
    ```
    - customized type;

    ```
    #include <iostream>
    #include <queue> 
    using namespace std;
    struct Node{
        int x, y;
        Node(int a=0, int b=0):
            x(a),y(b){}
    };
    //自定义类型重载operator<后，声明对象时就可以只带一个模板参数。
    bool operator<(Node a, Node b){//返回true时，说明a的    优先级低于b
        //x值较大的Node优先级低（x小的Node排在队前）
        //x相等时，y大的优先级低（y小的Node排在队前）
        if( a.x== b.x ) return a.y> b.y;
        return a.x> b.x; 
    }
    int main(){
        priority_queue<Node> q;
        for( int i= 0; i< 10; ++i )
        q.push( Node( rand(), rand() ) );
        while( !q.empty() ){
            cout << q.top().x << ' ' << q.top().y << endl;
        q.pop();
      }
      ///output:
        // 424238335 1957747793
        // 846930886 1804289383
        // 1102520059 783368690
        // 1189641421 596516649
        // 1303455736 304089172
        // 1350490027 1025202362
        // 1540383426 1365180540
        // 1649760492 719885386
        // 1714636915 1681692777
        // 1967513926 2044897763
      return 0;
    }


    ////
    //重写仿函数的例子（返回值排序与2.3.1相同，都是小顶堆。先按x升序，x相等时，再按y升序
    #include <iostream>
    #include <queue>
    using namespace std;
    struct Node{
        int x, y;
        Node( int a= 0, int b= 0 ):
            x(a), y(b) {}
    };
    struct cmp{
        bool operator() ( Node a, Node b ){//默认是less函数
            //返回true时，a的优先级低于b的优先级（a排在b的后面）
            if( a.x== b.x ) return a.y> b.y;      
            return a.x> b.x; }
    };
    int main(){
        priority_queue<Node, vector<Node>, cmp> q;
        for( int i= 0; i< 10; ++i )
        q.push( Node( rand(), rand() ) );
        while( !q.empty() ){
            cout << q.top().x << ' ' << q.top().y << endl;
            q.pop();
        }
        return 0;
    }
    ```
+ practical application:
    - timer management;
+ theoretical analysis:
../doc/c++/data_structure.mdnel ../doc/c++/data_structure.mdre: ../doc/c++/data_structure.md

# 2. HashMap
- unordered_map

# 3. Map
- prototype:
```cpp
template <class Key,class T,
          class Compare = less<key>,
          class Alloc = alloc>
class map{
    ......
};
```


- customized key;
    + key cannot be pointer and can only be object ($critical$);
    ```cpp
   std::map<NHSymbolkey, Stru_NHSymbol>*   pmapNHSymbolInfo1

   std::map<NHSymbolkey, Stru_NHSymbol*>*  pmapNHSymbolInfo2
   
   std::map<NHSymbolkey*, Stru_NHSymbol*>*  pmapNHSymbolInfo3

   其中，pmapNHSymbolInfo1，pmapNHSymbolInfo2中使用find正常，遍历也正常；
   pmapNHSymbolInfo3使用find查找不到对应的数据（数据已经存在，find不到，遍历可以找到）

   reason：std::map<NHSymbolkey*, Stru_NHSymbol*>*  pmapNHSymbolInfo2在find的时候是根据指针(数据地址)进行查找的。而在数据insert时，数据都是new的，每次new出的地址是不一样的，在find数据时，根据数据地址找不到不同地址的相同数据。通过遍历是取出地址中内容一一比较，这样能够找到数据。

   pmapNHSymbolInfo1、pmapNHSymbolInfo2两种方式都可以使用find方式查找数据，但是pmapNHSymbolInfo1中Stru_NHSymbol为对象，这样会使map占用空间比较大，pmapNHSymbolInfo2的Stru_NHSymbol为指针，存储时地址占用空间小，但是每次都是new处理来的，所有一定要记住使用完成后一定要delete，否则会出现内存泄露。
    ```

    + customized key find;
    ```cpp
    #include <iostream>
    #include <map>
    #include <string>

    using namespace std;
    //solution 1:
    struct AttrSubKey{
        string clientID;
        string instrumentID;
        int TID;

        bool operator< (const AttrSubKey & rValue) const {
            return clientID < rValue.clientID || instrumentID < rValue.instrumentID || TID < rValue.TID;
        }
    };

    typedef map<AttrSubKey,int> T_AttrSubMap;

    //solution 2 (also you can inherit unary_function):
    class AttrSubSetLessPred : public binary_function<AttrSubKey,AttrSubKey,bool>{
        public:
        bool operator() (const AttrSubKey & lValue,const AttrSubKey & rValue) const {
            return lValue.TID < rValue.TID || lValue.clientID < rValue.clientID || lValue.instrument < rValue.instrumentID;
        }
    }

    typedef map<AttrSubKey,int,AttrSubSetLessPred> T_AttrSubMap;

    int main(){
        T_AttrSubMap mm;
        AttrSubKey object;
        object.clientID = "zhaojx";
        object.instrumentID = "BTC";
        object.TID = 20;
        mm[object] = 1;

        object.clientID = "stl";
        object.instrumentID = "ETH";
        object.TID = 25;
        mm[object] = 2;

        auto it = mm.find(tmp);
        if(it != mm.end()){
            std::cout<<"found"<<std::endl;
        }else{
            std::cout<<"not found"<<std:endl;
        }
        
    }
    ```

 - map插入数据2中方式比较  
    std::map<NHSymbolkey, Stru_NHSymbol*>*  pmapNHSymbolInfo
      + pmapNHSymbolInfo->insert(std::make_pair(pNHSymbolkey, pNHSymbol));该方式的key如果出现重复，则会插入数据失败；
      + (*pmapNHSymbolInfo)[objNHSymbolkey] = pNHSymbol;该方式的key如果出现重复则直接覆盖掉原来的数据，永远不会出现插入失败的问题。