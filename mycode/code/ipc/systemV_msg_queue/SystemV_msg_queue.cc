#include <iostream>
#include <thread>
#include <semaphore.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/ipc.h>
using namespace std;

int number;
sem_t sem_id, sem_id2;

void test_process_sem() {
    cout << "start to test process semaphore" << endl;
    int nloop = 10, zero = 0;
    sem_t sem_id;

    // open a file and map it into memory
    int fd = open("./log.txt", O_RDWR | O_CREAT, S_IRWXU);
    write(fd, &zero, sizeof(int));
    int * ptr = (int *)mmap(nullptr, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    // create, initialize semaphore(POSIX interface)
    if (sem_init(&sem_id, 1, 1) < 0) {
        perror("semaphore initialization");
        exit(0);
    }
    auto pid = fork();
    if (pid == 0) {
        // child process
        cout << "enter in child process" << endl;
        for (auto i = 0; i < nloop; ++i) {
            sem_wait(&sem_id);
            cout << "child: " << (*ptr)++ << endl;
            sem_post(&sem_id);
        }
        cout << "exit in child process" << endl;
        exit(0);
    } else if (pid == -1) {
        perror("fork error");
        exit(-1);
    } else {
        // parent process
        for (auto i = 0; i < nloop; ++i) {
            sem_wait(&sem_id);
            cout << "parent: " << (*ptr)++ << endl;
            sem_post(&sem_id);
        }
        exit(0);
    }
}


int main() {
    test_process_sem();
    number = 1;
    sem_init(&sem_id, 0, 1); // 空闲
    sem_init(&sem_id2, 0, 0); // 忙

    // sem_id先是空闲，所以t_1先进入，然后在number++之后，释放了sem_id2,使得sem_id2>0
    // 此时线程t_2被唤醒，申请到sem_id2
    thread t_1([]()->void * {
        sem_wait(&sem_id);
        printf("thread_one have the semaphore\n");
        number++;

        printf("thread_one : number = %d\n", number);

        sem_post(&sem_id2);

        return NULL;});

    thread t_2([] () {
        sem_wait(&sem_id2);
        printf("thread_two have the semaphore \n");

        number--;
        printf("thread_two : number = %d\n", number);

        sem_post(&sem_id);

        return NULL;
    });

    t_1.join();
    t_2.join();

    //test_process_sem();
    return 0;
}




/////////////////////////// old code //////////////////////////
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/ipc.h>
//#include <sys/msg.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <errno.h>
//
//#define ERR_EXIT(m)                 \
//            do                      \
//            {                       \
//                perror(m);          \
//                exit(EXIT_FAILURE); \
//            }while(0)               \
//int main()
//{
//    int msgid;
//    msgid = msgget(1234,0666 | IPC_CREAT);
//    if(msgid == -1)
//    {
//        ERR_EXIT("msgget");
//    }
//    printf("msgget success\n");
//    return 0;
//}
