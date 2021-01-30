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
