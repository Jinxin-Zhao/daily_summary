<!-- TOC -->

[1. SFINAE](#1-SFINAE)

<!-- TOC -->

# 1. SFINAE
## definition
+ SFINAE(Substitution failure is not an error). we'd better distinguish 'failure' from 'error'
    ```cpp
    struct A{};
    struct B : public A{};
    struct C{};

    void foo(A const &){}
    void foo(B const &){}

    void callFoo(){
        foo(A()); //match "foo(A const &)" successfully, but "foo(B const &)" failed
        foo(B()); //match "foo(B const &)" successfully, but "foo(A const &)" failed
        foo(C()); //error, neither funcs succeed
    }
    ```
## enable_if
+ Providing that we have 2 different types of counter function, one is a normal int counter, the other is an object counter which has a member function called 'increase'.
Now we want to package the 2 counter functions.
    ```cpp
    struct ICounter{
        virtual void increase() = 0;
        virtual ~ICounter() {}
    };

    struct Counter : public ICounter{
        void increase() override{

        }
    };

    template <typename T>
    void inc_counter(T & counterObj){
        counterObj.increase();
    }

    template <typename T>
    void inc_counter(T & intTypeCounter){
        ++intTypeCounter;
    }

    void doSomething(){
        Counter cntObj;
        unint32_t cntUI32;

        //
        inc_counter(cntObj);
        inc_counter(cntUI32);
    }
    ```
    the compiler will hint that the compilation of the code above is failed caused by redefinition.
    at this time we'd better use 'enable_if' to limit types of the 2 functions;

    ```cpp
    template <typename T>
    void inc_counter(T & counterObj,typename std::enable_if<is_base_of<T,ICounter>::value>::type * = nullptr);

    template <typename T>
    void inc_counter(T & counterInt,typename std::enable_if<std::is_integral<T>::value>::type * = nullptr);
    ```
    "is_integral<T>::value" return a compilor constant with bool type, it tells if the T is integral or not. and "enable_if<C>" denotes that if C(is_integral<T>::value) is True, then the "type"(std::enable_if<std::is_integral<T>::value>::type)  is deducted as a  "void" type, then the function will become:
    ```cpp
    void inc_counter<int>(int & couterInt, void * dummy = nullptr);
    ```
    if the C is False, then enable_if<false> has no "type", substitution failed, which means that the function will not be created.

    we can substitue the first function as this(no need to use enable_if unless it's necessary)
    ```cpp
    void inc_counter(ICounter & counterObj);
    ```

    you may write the code like this:
    ```cpp
    template <typename T>
    void inc_counter(ICounter & c){
    };

    template <typename T>
    void inc_counter(T & c){
        ++c;
    }

    void doSomething(){
        Counter cntObj;
        uint32_t cntUI32;

        inc_counter(cntObj); //1: match "inc_counter(T & c)"
        inc_counter(static_cast<ICounter&>(cntObj));  //2: match "inc_counter(ICounter &c)"
        inc_counter(cntUI32); //3 
    }
    ```
    about 1: cntObj needs to be casted to ICounter, so it matches the "inc_counter(T & c)" function, then execute "++cntObj" action, BOOM!
    
#### Use SFINAE
+ C++11 offer the Expression SFINAE, we can write the code without inheriting from ICounter:
    ```cpp
    struct Counter{
        void increase(){

        }
    }
    template <typename T>
    void inc_counter(T & intTypeCount,std::decay_t<decltype(++intTypeCounter)> *= nullptr) {
        ++intTypeCounter;
    }

    template <typename T>
    void inc_counter(T & counterObj,std::decay_t<decltype(counterObj.increase())>* = nullptr){
        counterObj.increase();
    }

    void doSomething(){
        Counter cntObj;
        uint32_t cntUI32;

        inc_counter(cntObj);
        inc_counter(cntUI32);
    }
    ```
+  In "Universal Reference" scence, we can only use SFINAE instead of inheriting & override methods.
  ```cpp
  //we can precisely solve lvalue & rvalue reference issue
  template <typename ArgT> void foo(ArgT && a);
  ```
  if we want to limit ArgT in float or types derived from float(which may instantiated as "foo(float &)", "foo(float &&)", "foo(float)"), we'd better write the code like this:
  ```cpp
  template <typename ArgT>
  void foo(ArgT && a,typename std::enable_if<is_same<std::decay_t<ArgT>,float>::value>::type * = nullptr);
  ```

# 2. Generic Libraries
+ Let's look how the for_each() algorithm is implemented:
  ```hpp
  template<typename Iter,typename Callable>
  void foreach(Iter current, Iter end, Callable op){
      while(current != end){
          op(*current);
          ++current;
      }
  }

  //since c++17
  template<typename Iter,typename Callable, typename... Args>
  void foreach(Iter current,Iter end,Callable op,Args const &... args){
      while(current != end){
          std::invoke(op, args..., *current);
          ++current;
      }
  }
  ```

  ```cpp
  int main(){
      foreach(primes.begin(),prime.end(),func); #1
      foreach(primes.begin(),prime.end(),&func); #2
      foreach(primes.begin(),prime.end(),FuncObj());
      foreach(primes.begin(),prime.end(),[](int i){std::cout << "lamda called for: " << i << '\n';});
  }
  ```

# 3. CRTP(curiously recurring template pattern)
+ 通过把派生类作为模板参数传递给其基类，无需通过虚函数。应用场景比如追踪一个类类型创建了多少个对象。
  ```cpp
  template <typename Derived>
  class CuriousBase{
      ...
  };

  template <typename T>
  class CuriousTemplate : public CuriousBase<CuriousTemplate<T>>{
      ...
  };

  int main(){
      CuriousTemplate<char> s1,s2;
      CuriousTemplate<wchar_t> ws;
  }
  ```
  两个CuriousTemplate各自继承的基类是不同的(基类根据不同类型实例化出不同的实体基类)
### The Barton-Nackman Trick
+ 这种技巧就是在类定义中定义友元函数。
    ```cpp
    template<typename T>
    class Wrapper
    {
    private:
        T object;
    public:
        Wrapper(T obj):object(obj){}
        friend void foo(Wrapper<T> const&){}
    };

    class S{
        //friend void foo(Wrapper<S> const&){}
    };

    int main(){
        S s;
        Wrapper<S> w(s);
        foo(w);
        foo(s);  //error
    }
    ```
    一个在类定义中定义的友元函数实际上其作用域是名字空间的。所以如果S类中的函数没有被屏蔽，那么编译器是会提示foo重复定义的错误的。
### Operator Implementations
+ 我们可以通过结合CRTP和Barton-Nackman来实现一个提供运算符重载的类。
  ```cpp
    template<typename Derived>
    class EqualityComparable
    {
    public:
        friend bool operator!=(Derived const& x1,Derived const x2)
        {
            return !(x1==x2);
        }
    };
    class X : public EqualityComparable<X>
    {
    public:
        friend bool operator==(X const& x1,X const& x2)
        {
            //比较并返回结果
        }
    };

    int main(){
        X x1,x2;
        if(x1 != x2){}
    }
  ```

    