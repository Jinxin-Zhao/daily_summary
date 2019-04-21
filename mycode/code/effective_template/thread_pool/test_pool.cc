#include "threadpool.h"
#include <unistd.h>
#include <sys/time.h>
#include <iostream>

const int64_t order_interval = 1000000;
static int64_t Order_num = 0;

int64_t GetCurrentMicroSec(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000000 + tv.tv_usec;
}

void fun1(int slp)
{
    int c = 0;
    while(1){
        int s_cur = GetCurrentMicroSec();
        printf("thread->[%lu]:start second: [%lu]\n",std::this_thread::get_id(),s_cur);
        int count = 30;
        while(count-- > 0){
            Order_num++;
            printf("*****Order_num****[%lu]",Order_num);
            printf(" thread->[%lu] enter fun1 and RequestOrderInsert![%d]\n" ,std::this_thread::get_id(),count);
        }
        int e_cur = GetCurrentMicroSec();
        int64_t interval = e_cur - s_cur;
        printf("thread->[%lu]:end second: [%lu],experienced [%lu] microsec\n",std::this_thread::get_id(),e_cur,interval);
        if(interval < order_interval){
            printf("==========thread->[%lu]:start to usleep========: [%lu]\n",std::this_thread::get_id(),(order_interval - interval));
            usleep(order_interval - interval);
        }
        printf("thread_id:[%lu],task done====start cycle:[%d]\n",std::this_thread::get_id(),c++);
    }
}

struct gfun {
    int operator()(int n) {
        printf("%d  hello, gfun !  %d\n" ,n, std::this_thread::get_id() );
        return 42;
    }
};

class A { 
public:
    static int Afun(int n = 0) {   //函数必须是 static 的才能直接使用线程池
        std::cout << n << "  hello, Afun !  " << std::this_thread::get_id() << std::endl;
        return n;
    }

    static std::string Bfun(int n, std::string str, char c) {
        std::cout << n << "  hello, Bfun !  "<< str.c_str() <<"  " << (int)c <<"  " << std::this_thread::get_id() << std::endl;
        return str;
    }
};

int main()
    try {
        std::threadpool executor{ 5 };
        //A a;
        //std::future<void> ff = executor.commit(fun1,0);
        //std::future<int> fg = executor.commit(gfun{},0);
        //std::future<int> gg = executor.commit(a.Afun, 9999); //IDE提示错误,但可以编译运行
        //std::future<std::string> gh = executor.commit(A::Bfun, 9998,"mult args", 123);
        //std::future<std::string> fh = executor.commit([]()->std::string { std::cout << "hello, fh !  " << std::this_thread::get_id() << std::endl; return "hello,fh ret !"; });

       //std::cout << " =======[start sleep]========= " << std::this_thread::get_id() << std::endl;
       //std::this_thread::sleep_for(std::chrono::microseconds(9000));

       //for (int i = 0; i < 5; i++) {
       //    executor.commit(fun1,i*100 );
       //}

       //std::cout << " =======[commit all]=========[thread_id]: " << std::this_thread::get_id()<< " [idlsize]: "<<executor.idlCount() << std::endl;

        //std::cout << " =======  sleep ========= " << std::this_thread::get_id() << std::endl;
        //std::this_thread::sleep_for(std::chrono::seconds(3));

        ///ff.get(); //调用.get()获取返回值会等待线程执行完,获取返回值
        //st::cout << fg.get() << "  " << fh.get().c_str()<< "  " << std::this_thread::get_id() << std::endl;

        //std::cout << " =======  sleep ========= " << std::this_thread::get_id() << std::endl;
        //std::this_thread::sleep_for(std::chrono::seconds(3));

        //std::cout << " =======  fun1,55 ========= " << std::this_thread::get_id() << std::endl;
        //executor.commit(fun1,55).get();    //调用.get()获取返回值会等待线程执行完

        //std::cout << "main thread: [" << std::this_thread::get_id() << "]end..." << std::endl;


        //std::threadpool pool(4);
        std::vector< std::future<void> > results;

        for (int i = 0; i < 5; ++i) {
            results.emplace_back(
                // pool.commit([i] {
                //     printf("hello :[%d]======thread_id:[%d]\n",i,std::this_thread::get_id());
                //     //std::cout << "hello " << i << std::endl;
                //     //std::this_thread::sleep_for(std::chrono::seconds(1));
                //     //std::cout << "world " << i << std::endl;
                //     return i*i;
                // })
                executor.commit(fun1,(i+1)*100)
            );
        }
        //std::cout << " =======  commit all2 ========= " << std::this_thread::get_id() << std::endl;
        printf("========commit all2========thread_id:[%lu]\n",std::this_thread::get_id());
        for (auto & result : results){
            result.get();
        }
            //printf("get:[%d]\n",result.get());
        std::cout << "main_thread end....."<<std::endl;
        return 0;
    }
catch (std::exception& e) {
    std::cout << "some unhappy happened...  " << std::this_thread::get_id() << e.what() << std::endl;
}