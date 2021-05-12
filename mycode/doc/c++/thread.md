# # chapter 3 sharing data betweening threads
### 3 ways to deal with problematic race conditions:
    - the simplest option is to wrap your data structure with a protection mechanism,to ensure that only the thread actually performing a modification can see the intermediate states where the invariants are broken;
    - another option is to modify the design of your data structure and its invariants so that modifications are done as series if indivisible changes,each of which preserves the invariants.This is generally referred to as lock-free programming;
    - another way of dealing with race conditions is to handle the updates to the data structure as a "transaction",just as updates to a database are done within a transaction.The required series of data modifications and reads is stored in a transaction log,and then committed in a single step.
      If the commit cannot proceed because the data structure has been modified by another thread,then the transaction is restarted. This is termed Software Transactional Memory. There's no direct support for STM in C++.

#### If there is no enforced ordering between two accesses to a single memory location from separate threads, one or both of those accesses is not atomic, and one or both is a write, this is a data race, and causes undefined behaviour

### further guidelines for avoiding deadlock
    + avoid nested locks;
    + avoid calling user-supplied code whilst holding a lock;
    + acquire locks in a fixed order;
    + use a lock hierarchy;

### std::unique_lock & std::lock_guard
    + std::unique_lock is more flexible than std::lock_guard,but there is a slight performance penalty when using std::unique_lock;
    + std::unique_lock instances do not have to own their associated mutexes, the ownership of a mutex can be transfered between instances by moving the instances around;
        - one possible use is to allow a function to lock a mutex and transfer ownership of that lock to the caller,

### don't do any really time consuming activities like file I/O whilst holding a lock;

### condition_variable & condition_variable_any
+ In both cases,they need to work with a mutex in order to provide appropriate synchronization:
    - the former is limited to working with the std::mutex;
    - whereas the latter can work with anything that meets some minimal criteria for being mutex-like;
+ Let's start with an example:
    ```cpp
    #include <iostream>
    #include <thread>
    #include <mutex>
    #include <condition_variable>

    mutex mtx;
    condition_variable cv;
    bool ready = false;

    void print_id(int id){
        unique_lock<mutex> lck(mtx);
        while(!ready){
            //wait() unlocks the mutex and puts the thread in a “waiting” state
            cv.wait(lck); 
            //after calling notify_all() by main thread, current thread with wait() is awakened and its wait() function locks the mutex again before return;
        }
        std::cout<<"thread" << id << std::endl;
    }

    /*void  print_id(int id){
        unique_lock<mutex> lck(mtx);
        cv.wait(lk,[]{return ready;});
        std::cout<< "thread" << id << std::endl;
    }*/

    void go(){
        unique_lock<mutex> lck(mtx);
        ready = true;
        cv.notify_all();
    }
    int main(){
        std::thread threads[10];
        for(auto i = 0; i < 10; ++i){
            threads[i] = std::thread(print_id,i);
        }
        std::cout<<"10 threads ready to race ...\n";
        //
        go();

        for(auto & th : threads){
            th.join();
        }
    }

    //conditoin_variable::notify_one(): awaken a 'waiting' status thread;
    //condition_variable::notify_all(): awaken all 'waiting' status threads;
    //condition_variable::wait_for(): put thread in 'waiting' status until a period of time ends or be awakened by notify_xxx();
    //condition_variable::wait_until(): put thread in 'waiting' status until the appointed time or be awakened by notify_xxx(); 
    ```
+ why using lock with condition_variable instead of mutex
  - for exception safe in multithreads:
  ```cpp
  std::mutex mtx;

  void foo(){
      mtx.lock();
      while(not_ready){
          //....
          cv.wait(mtx);
      }
      //think about exception before unlock(),the thread cannot unlock till the end and the other threads cannot get the resource;
      mtx.unlock();
  }

  //solution1:
  //use try...catch

  //solution2:
  void foo(){
      unique_lock<mutex> lck(mtx);
      whilr(not_ready){
          //...
          cv.wait(lck);
      }
  }
    //when exception happen the destructor will call unlock();
  ```
+ why using unique_lock<> instead of lock_gurad<>:
    - think about the source code of unique_lock() and lock_guard():
    ```cpp
    ~unique_lock() _NOEXCEPT {
        if(_Owns){
            _Pmtx->unlock();
        }
    }

    ////
    ~lock_guard() _NOEXCEPT{
        _MyMutex.unlock();
    }
    ```
  

### condition variable & future(one-off events)
+ if the waiting thread is only ever going to wait at once, when the condition is true it will never wait on this condition variable again, a condition variable might not be
the best choice of synchronization mechanism.
+ future: std::shared_future<>  && std::unique_future<>
    - std::unique_future should be your first choice wherever it is appropriate:
    std::shared_future only allows you to copy the data associated with the future;
    std::unique_future allows you to move the data out(using std::move). This can be important where the data is potentially expensive to copy, but cheap to move(such as a std::vector with a million elements),
    and is vital where the data cannot be copied,only moved(such as std::thread, or std::ifstream)
+ std::async
  let's start with an example:
  ```cpp
  #include <iostream>
  #include <future>

  bool is_prime(int x){
      std::cout<<"Calculating ...\n";
      for(auto i = 2; i < x; ++i){
          if(x % i == 0)
            return false;
      }
      return true;
  }

  int main(){
      std::future<bool> fut = async(is_prime,3232222313);
      //you can also use async like this:
      std::future<bool> fut = async(launch::async, is_prime,3232323213); //start a new thread asynchronously to call the function is_prime
      std::future<bool> fut = async(launch::defered, is_prime,3232323213); //defer to create thread to call is_prime till future object uses 'get()' or 'wait()';
      std::future<bool> fut = async(launch::async | launch::defered, is_prime,3232323213); //function pick mode automatically

      std::cout<<"checking ....\n";
      //if is_prime function already return, then fut can get the return vale
      //or the fut.get() will block till is_prime return;
      bool ret = fut.get();

      if(ret)
        std::cout<<"prime\n";
      else 
        std::cout<<"not prime\n";
     return 0;
  }
  ```

###packaged_task<> & std::promise
+ std::packaged_task cannot be copied, it ties a future to the result of a function call: when the function completes,the future is ready, and the associated data is the value returned by the function;

# Chapter 5 C++ Memory Model & atomic
### atomic variable
+ The Standard atomic types are not copyable or assignable in the conventional sense(they have no copy constructors or copy assignment operators).
    However,they do support assignment from and implicit conversion to the corresponding built-in types.
+ Each of the operations on the atomic types has an optional memory ordering argument which can be used to specify the required memory ordering semantics,the operations are divided into 3 categories:
    - [stroe operations]: which can have memory_order_relaxed, memeory_order_release, memory_order_seq_cst ordering;
    - [load operations]:  which can have memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_seq_cst ordering;
    - [read-modify-write]: which can have memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_release, memory_order_acq_rel, memory_order_seq_cst ordering;
  The default ordering for all operarations is memory_order_seq_cst;
#### std::atomic_flag
+ Objects of type std::atomic_flag must be initialized with ATOMIC_FLAG_INIT to clear the state. It is the only atomic type to require such special treatment for initialization and It's also the only type guaranteed to be lock free
(if the std::atomic_flag object has static storage duration,It's guaranteed to be statically initialized. It will always be initialized by the time of the first operation on the flag)
#### std::atomic_bool
+ This is a more fullfeatured boolean flag than std::atomic_flag,you can construct it from a non-atomic bool, so it can be initialized true or false.
   ```cpp
   std::atomic_bool b = true;
   ```
#### Storing a new value(or not) depending on the current value
+ 2 member functions: "compare_exchange_weak()", "compare_exchange_strong()";
  - return type of the functions above is a bool(true: if the store was performed, false otherwise);
  - it compares the value of the atomic variable with a supplied "expected" value and stores the supplied "desired" value if they are equal.    if the values are not equal, the "expected" value is updated with the actual value of the atomic variable.
  ```cpp
  bool expected = false;
  atomic_bool b = false;
  //if b is equal to "expected" value(which is false here,the first param in compare_exchange_weak()),then store the supplied "desired" value(true here,the second param in compare_exchange_weak())
  while(!b.compare_exchange_weak(expected,true) && !expected);
  ```
##### compare_exchange_weak()
+ the store might not be successful even if the original value was equal to the "expected" value(in which case the value of the variable is unchanged and the return value of compare_exchange_weak() is false).this is called a "spurious" failure.
##### compare_exchange_strong()
+ On the other hand,compare_exchange_strong() is guaranteed to only return false if the actual value was not equal to the "expected" value.
#### std::atomic < UDT>
+ you can't just use any user-defined type with std::atomic<> though; the type has to fulfill certain criteria. In order to use std::atomic<UDT> for some user-defined type UDT,this type must：
  - have a trivial copy-assignment operator. this means that the type must not have any virtual functions or virtual base class, and must use the compiler-generated copy-assignment operator.
  - every base class and non-static data member of user-defined type must also have a trivial copy-assignment operator. This essentially permits the compiler to use memcpy() or an equivalent operation for assignment operations,since there's no user-written code to run.
  - the UDT must be bitwise equality comparable,so there can be no user-defined equality comparison operator(compare instances for equality using memcmp()). this guarantee is required in order for compare/exchange operations to work.
  you **can't** create a **$std::atomic<std::vector<int>>$**, but you can use it with classes containing counters or flags or pointers,or even just arrays of simple data elements.
  ```cpp
  std::shared_ptr<my_data> p;
  void process_global_data(){
      std::shared_ptr<my_data> local = std::atomic_load(&p);
      process(local);
  }
  void update_global_data(){
      std::shared_ptr<my_data> local(new my_data);
      std::atomic_store(&p,local);
  }
  ```
#### Memory Ordering
##### The happen-before relation
+ if the one operation(A) occurs in a statement prior to another(B) in the source code, then A happens before B. What about the operations occur in the same statement? the answer is that they are unordered.
    ```cpp
    //the order of evaluation of arguments to a function call is unspecified
    void foo(int a,int b){
        std::cout<<a << "," << b << std::endl;
    }
    int get_num(){
        static int i = 0;
        return ++i;
    }
    int main(){
        foo(get_num(),get_num());
    }
    ```
##### Memory ordering for atomic operations
+ 6 memory ordering options represent 3 models: 
  - **sequentially-consistent ordering** (*memory_order_seq_cst*)  //default ordering unless you sepcify
  ```cpp
  #include <cstdatomic>
  #include <thread>
  #include <assert.h>

  std::atomic_bool x,y;
  std::atomic_int z;

  void write_x(){
      x.store(true,std::memory_order_seq_cst);  #2
  }
  void write_y(){
      y.store(true,std::memory_order_seq_cst);  #3
  }
  void read_x_then_y(){
      while(!x.load(std::memory_order_seq_cst));
      if(y.load(std::memory_order_seq_cst) #4
        ++z;
  }
  void read_y_then_x(){
      while(!y.load(std::memory_order_seq_cst));
      if(x.load(std::memory_order_seq_cst) #5
        ++z;
  }

  int main(){
      x = false;
      y = false;
      z = 0;
      std::thread a(write_x);
      std::thread b(write_y);
      std::thread c(read_x_then_y);
      std::thread d(read_y_then_x);
      a.join();
      b.join();
      c.join();
      d.join();
      assert(z.load() != 0); #1

      return 0;
  }
  ```
  if (#4) returned false then the store x must occur before the store to y, at the moment y can be 'false' or 'true' which means z can be 1 or 2.(这种情况x先于y被置成true，此时1. y如果还是false，不会执行++z，那么(#5)步骤后（此时y肯定被置成true了，要不然不会执行到#5），肯定会执行++z，此时z的结果是1； 2. y如果是true，继续执行++z,下面的(#5)肯定也会执行++z,此时z的结果是2)
  （#1）can never fire, since z can be 1 or 2 and under no circumstances can z be 0;
  - **relaxed ordering** (*memory_order_relaxed*)
  ```cpp
  #include <cstdatomic>
  #include <thread>
  #include <assert.h>

  std::atomic_bool x,y;
  std::atomic_int z;

  void write_x_then_y(){
      x.store(true,std::memory_order_relaxed); #4
      y.store(true,std::memory_order_relaxed); #5
  }
  void read_y_then_x(){
      while(!y.load(std::memory_order_relaxed)); #3
      if(x.load(std::memory_order_relaxed)) #2
        ++z;
  }

  int main(){
      x = false;
      y = false;
      z = 0;
      std::thread a(write_x_then_y);
      std::thread b(read_y_then_x);
      a.join();
      b.join();
      assert(z.load() != 0);  //can fire
  }
  ```
  (#2) may read false, even though the load of y(#3) read true and (#4) happens-before the store of y(#5). x and y are different variables, so there're no ordering guarantees relating the visibility of values arising from operations on each.
  以这种方式修改内存时，不需要保证该修改会不会及时被其它线程看到，也不对乱序做任何要求，因此当对公共变量以 relaxed 方式进行读写时，编译器，cpu 等是被允许按照任意它们认为合适的方式来加以优化处理的。
  - **acquire-release ordering** (*memory_order_consume*, *memory_order_acquire*, *memory_order_release*, *memory_order_acq_rel*) 
  ```cpp
  //memory_order_release,memory_order_acquire
  //In order to provide any synchronization,acquire and release operations must be paired up
  #include <cstdatomic>
  #include <thread>
  #include <assert.h>
  
  std::atomic_bool x,y;
  std::atomic_int z;

  void write_x_then_y(){
      x.store(true,std::memory_order_relaxed);  #3
      y.store(true,std::memory_order_release);  #2
  }
  void read_y_then_x(){
      while(!y.load(std::memory_order_acquire)); #1
      if(x.load(std::memory_order_relaxed))  #4
        ++z;
  }

  int main(){
      x = false;
      y = false;
      z = 0;
      std::thread a(write_x_then_y);
      std::thread b(read_y_then_x);
      a.join();
      b.join();
      assert(z.load() != 0);   #5
      return 0;
  }


    // 假设线程 A 执行如下指令：
    a.store(3);
    b.store(4);
    m.store(5, release);

    // 线程 B 执行如下:
    e.load();
    f.load();
    m.load(acquire);
    g.load();
    h.load();

    //线程B在读取完m之后，线程A在release之前的操作对于线程B都是可见(生效)的。
  ```
  the assert(#5) cannot fire. the "store" uses *memory_order_release*, and the "load" uses *memory_order_acquire*（这两个tag用来约定两个线程之间的相对行为)，必须配套使用，单独定义没有任何意义。（#1）可以确保（#3）的store操作对于线程b可见，所以最后（#4）一定成立，z不可能是0；
    - transitive synchronization with acquire-release ordering
    ```cpp
    std::atomic<int> data[5];
    std::atomic<bool> sync1(false),sync2(false);

    void thread_1(){
        data[0].store(42,std::memory_order_relaxed);
        data[0].store(97,std::memory_order_relaxed);
        data[0].store(17,std::memory_order_relaxed);
        data[0].store(-141,std::memory_order_relaxed);
        data[0].store(2003,std::memory_order_relaxed);
        sync1.store(true,std::memory_order_release); #3
    }
    void thread_2(){
        while(!sync1.load(std::memory_order_acquire)); #1
        sync2.store(std::memory_order_release); #2
    }

    void thread_3(){
        while(!sync2.load(std::memory_order_acquire));
        assert(data[0].load(std::memory_order_relaxed) == 42);
        assert(data[1].load(std::memory_order_relaxed) == 97);
        assert(data[2].load(std::memory_order_relaxed) == 17);
        assert(data[3].load(std::memory_order_relaxed) == -141);
        assert(data[4].load(std::memory_order_relaxed) == 2003);
        //the asserts above are right
    }
    ```
    the asserts in thread_3 won't fire.
    In this case, we could combine sync1 and sync2 into a single variable by using a read-modify-write operation with memory_order_acq_rel in thread_2. One option would be to use compare_exchange_strong() to ensure that the value is only updated once the store from thread_1 has been seen:
    ```cpp
    std::atomic<int> sync(0);
    void thread_1(){
        ...
        sync.store(1,std::memory_order_release);
    }
    void thread_2(){
        int expected = 1;
        while(!sync.compare_exchange_strong(expected,2,std::memory_order_acq_rel))
            expected = 1;
    }
    void thread_3(){
        while(sync.load(std::memory_order_acquire) < 2);
        ...
    }
    ```
    - memory_order_consume（强调代码间存在依赖关系,与memory_order_acquire相比获取语义更弱）
    memory_order_consume was part of the acquire-release ordering model which is all about data description.
    There are two new relations that deal with data dependencies: 
    **dependency-ordered-before**: this relation can apply between threads （ 多个线程之间的关系 ）. It's instroduced by using atomic load operations tagged with memory_order_consume. This is a special case of **memory_order_acquire** that limits the synchronized data to direct dependencies: 
    ```cpp
    atomic<int> Guard(0);
    int Payload = 0;

    //thread 1
    Payload = 42;
    Guard.store(1, memory_order_release);

    //thread 2
    g = Guard.load(memory_order_acquire);
    if (g != 0)
        p = Payload;
    ```
    **carries-a-dependency-to**: this relation applies strictlhy within a single thread ( 单个线程内的关系 ), if the result of an operation A is used as an operand for an operation B,then A carries-a-dependency-to B.
    ```cpp
    atomic<int*> Guard(nullptr);
    int Payload = 0;

    //thread 1
    Payload = 42;
    Guard.store(&Payload, memory_order_release);

    //thread 2
    g = Guard.load(memory_order_consume);
    if (g != nullptr)
        p = *g;
    ```
    **总结**： memory_order_acquire 和 memory_order_consume的目的是一样的：帮助非原子信息在线程间安全的传递
#### Fences(barriers)
+ These are operations that enforce memory ordering constraints without modifying any data, and are typically combined with atomic operations that use the **memory_order_relaxed** ordering constraints.
  ```cpp
  #include <cstdatomic>
  #include <thread>
  #include <assert.h>
  std::atomic_bool x,y;
  std::atomic_int z;

  void write_x_then_y(){
      x.store(true,std::memory_order_relaxed);  #3
      std::atomic_thread_fence(std::memory_order_release);  #1
      y.store(true,std::memory_order_relaxed);  #4
  }

  void read_y_then_x(){
      while(!y.load(std::memory_order_relaxed));  #5
      std::atomic_thread_fence(std::memory_order_acquire);  #2
      if(x.load(std::memory_order_relaxed)) #6
        ++z;
  }
  int main(){
      x = false;
      y = false;
      std::thread a(write_x_then_y);
      std::thread b(read_y_then_x);
      a.join();
      b.join();
      assert(z.load() != 0);  #7
      return 0;
  }
  ```
  the assert at #7 will not fire,the general idea with fences(内存屏障大致思想): 可以这么理解，拿上面这个例子来说，#1处的内存屏障（release)就等效于在#4处的store后面的tag是memory_order_release, 同样的#2处的acquire内存屏障就等效于#5处的tag是memory_order_acquire。

### atomic
+ atomic<int> a
  ```cpp
  a.store(1);
  a += 3;
  int pre = a.fetch_add(3); //pre is 1;
  ```
  - a += 3;  // a.fetch_add(3);
+ floating point atomic
  - No template specialisation for float and double;
  - operator+=, operator-= etc. do not exist;
  - store and load is fine;
  - compare_exchange is there, but not meaningful;
  ```cpp
  struct Synthesister{
      std::atomic<float> level;

      //GUI thread:
      void levelChanged(float newValue){
          level.store(newValue);
      }

      //real-time thread (another thread)
      void audioCallback(float * buffer, int numSamples) noexcept{
          for(int i = 0; i < numSamples; ++i){
              const float preValue = level.load();
              buffer[i] = level.load() * getNextAudioSample();
              // buffer[i] = preValue * getNextAudioSample();
          }
      }
  }
  ```
- an example to realize lock-free queue(single-consumer-single-producer)
  ```cpp
  template <typename T, size_t size>
  class LockFreeQueue{
      public:
        bool push(const T & newElement){
            auto oldWritePos = writePos.load();
            auto newWritePos = getPositionAfter(oldWritePos);
            if(newWritePos == readPos.load()){
                return false;
            }
            ringBuffer[oldWritePos] = newElement;

            write.store(newWritePos);
            return true;
        }

        bool pop(T & returnedElement){
            auto oldWritePos = writePos.load();
            auto oldReadPos = readPos.load();
            if(oldWritePos == oldReadPos)
            return false;

            returnedElement = std::move(ringBuffer[oldReadPos]);

            readPos.store(++oldReadPos);
            return true;
        }
      private:

        static constexpr size_t getPositionAfter(size_t pos) nonexcept{
            return ++pos == ringBufferSize ? 0 : pos;
        }

      static_constexpr size_t ringBufferSize = size + 1;
      std::array<T,ringBuffersize> ringBuffer;
      std::atomic<size_t> readPos = {0}, writePos = {0};

  }
  ```

- multiConsumers & multiProducer
  ```cpp
  template <typename T, size_t size>
  class LockFreeQueue{
      public:
        bool push(const T & newElement){
            auto oldWritePos = writePos.load();
            auto newWritePos = getPositionAfter(oldWritePos);
            if(newWritePos == readPos.load()){
                return false;
            }
            ringBuffer[oldWritePos].store(newElement);

            write.store(newWritePos);
            return true;
        }
        //still have bug
        bool pop(T & returnedElement){
            auto oldWritePos = writePos.load();
            auto oldReadPos = readPos.load();
            if(oldWritePos == oldReadPos)
            return false;

            //returnedElement = std::move(ringBuffer[oldReadPos]);

            while(true){
                returnedElement = ringBuffer[oldReadPos].load(); //Reader1 thread
                if(readPos.compare_exchange_strong(oldReadPos,getPositonAfer(oldReadPos)))
                return true;
                oldReadPos = readPos.load();  //Reader2 thread
            }
        }
      private:

        static constexpr size_t getPositionAfter(size_t pos) nonexcept{
            return ++pos == ringBufferSize ? 0 : pos;
        }

      static_constexpr size_t ringBufferSize = size + 1;
      //std::array<T,ringBuffersize> ringBuffer;
        std::array<std::atomic<T>, ringBufferSize> ringBuffer;
      std::atomic<size_t> readPos = {0}, writePos = {0};
        //define a struct including readingPos and WritePos?
        struct Pos_{
            short readPos {0};
            short writePos {0};
        }
        std::atomic<Pos_> pos {0,0};
  }



  ///fix bug of pop()
  bool pop(T & returnedElement){
            auto oldWritePos = writePos.load();
            auto oldReadPos = readPos.load();
            if(oldWritePos == oldReadPos)
            return false;

            //another bug in this scenario
            //r1 & r2 index the same location(which is the last element whose index is size-1);and the queue is empty after r1 finishes the reading work,then r2 reload the pos and now the index is 0 with no data, but r2 is in while loop and continue to read and assign value to returnedElement;
            while(true){
                returnedElement = ringBuffer[oldReadPos].load(); //Reader1、Reader2 threads; then queue is empty after Reader1 finished(readPos now index the location with no data , you cannot use assigenment now)
                if(readPos.compare_exchange_strong(oldReadPos,getPositonAfer(oldReadPos)))
                return true; //Reader1 is faster than Reader2, and already set ReadPos to newPos, then 
                oldReadPos = readPos.load();
            }
        }

        ///fix another bug of pop()
    bool pop(T & returnedElement){
            //another bug in this scenario
            //r1 & r2 index the same location(which is the last element whose index is size-1);and the queue is empty after r1 finishes the reading work,then r2 reload the pos and now the index is 0 with no data, but r2 is in while loop and continue to read and assign value to returnedElement;
            while(true){
                auto oldWritePos = writePos.load();
                auto oldReadPos = readPos.load();
                if(oldWritePos == oldReadPos)
                    return false;
                returnedElement = ringBuffer[oldReadPos].load(); //Reader1、Reader2 threads; then queue is empty after Reader1 finished(readPos now index the location with no data , you cannot use assigenment now)
                if(readPos.compare_exchange_strong(oldReadPos,getPositonAfer(oldReadPos)))
                    return true;
            }
        }
  ```

# Chapter 6 Designing structures for concurrency
+ The use of a mutex to protect a data structure does so by exlicitly preventing true concurrent access to the data it protects. This is called **serialization**： threads take turns to access the data protected by the mutex.

# Chapter 7 Designing Data Structure for Concurrency: Lock-free Concurrent Data Structure
+ It's worth bearing in mind that only std::atomic_flag is guaranteed not to use locks in the implementation,so on some platforms what appears to be lock-free code might actually be using locks internal to the C++ STL implementation.
#### Writing a thread-safe stack without locks
+ the simplest stack is just a linked list(采用头插法).
    ```cpp
    template<typename T>
    class stack{
        private:
            struct node{
                shared_ptr<T> data;
                node * next;
                node(T const & data_):data(new T(data_)){}
            };
            std::atomic<node*> head;
        public:
            void push(T const & data){
                node * const new_node = new node(data);
                new_node->next = head.load();
                //if "head" is not equal to "new_node->next",then set "new_node->next = head" and return false, else set "head = new_node"
                //比较失败就代表head节点已经被其他的线程修改，此时借助while循环和exchange机制，重新设定new_node->next的值为新的head节点，下次loop再来的时候就会比较成功，彼时便会set "head = new_node"
                //当然比较成功的话(head == new_node->next),直接会set "head = new_node",compare_exchange_weak会返回true
                while(!head.compare_exchange_weak(new_node->next,new_node));
            }

            //1. read the current value of head
            //2. read head->next
            //3. set head to head->next
            //4. return the data from the retrieved node
            //5. delete the retrived node

            //if one thread then proceeds all the way through to step 5 before the other gets to step 2, then the second thread will be dereferencing a dangling pointer.
            std::shared_ptr<T> pop(){
                node * old_head = head.load();
                while(old_head && !head.compare_exchange_weak(old_head,old_head->next));
                return old_head ? old_head->data : std::shared_ptr<T>();
            }

            //pop() with no pesky leaks
            std::atomic<unsigned> threads_in_pop;
            std::atomic<node *> to_be_deleted;
            static void delete_nodes(node * nodes){
                while(nodes){
                    node * next = nodes->next;
                    delete nodes;
                    nodes = next;
                }
            }

            std::shared_ptr<T> pop(){
                ++threads_in_pop;
                node * old_head = head.load();
                while(old_head && !head.compare_exchange_weak(old_head,old_head->next));
                std::shared_ptr<T> res;
                if(old_head){
                    res.swap(old_head->data);
                }
                node * nodes_to_delete = to_be_deleted.load();
                if(!--threads_in_pop){
                    if(to_be_deleted.compare_exchange_strong(nodes_to_delete,nullptr)){
                        delete_nodes(nodes_to_delete);
                    }
                    delete old_head;
                } else {
                    old_head->next = nodes_to_delete;
                    while(!to_be_deleted.compare_exchange_weak(old_head->next,old_head));
                }
                return res;
            }
    };
    ```