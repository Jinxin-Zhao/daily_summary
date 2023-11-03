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

#define SHMSZ 27

char SEM_NAME[] = "sem_zjx";

int main() {
    // name the shared memory segment
    key_t key = 1000;
    // create & initialize semaphore
    sem_t * sem_id = sem_open(SEM_NAME, O_CREAT, 0644, 1);
    if (sem_id == SEM_FAILED) {
        perror("unable to create semaphore");
        sem_unlink(SEM_NAME);
        exit(-1);
    }
    // create the shared memory segment
    int shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("failure in shmget");
        exit(-1);
    }

    // attach this segment to virtual memory
    char * shm = (char*)shmat(shmid, nullptr, 0);
    char * s = shm;
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        sem_wait(sem_id);
        *s++ = ch;
        sem_post(sem_id);
    }
    // the below loop could be replaced by binary semaphore
    while (*shm != '*') {
        sleep(1);
    }
    sem_close(sem_id);
    sem_unlink(SEM_NAME);
    shmctl(shmid, IPC_RMID, 0);
    return EXIT_SUCCESS;
}
