#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHMSZ 27

char SEM_NAME[] = "sem_zjx";

int main() {
    key_t key = 1000;
    sem_t * sem_id = sem_open(SEM_NAME, 0, 0644, 0);
    if (sem_id == SEM_FAILED) {
        perror("reader::unable to open sem");
        sem_close(sem_id);
        exit(-1);
    }
    // create the shared memory segment with this key
    int shmid = shmget(key, SHMSZ, 0666);
    if (shmid < 0) {
        perror("reader::failure in shmget");
        exit(-1);
    }

    // attach this segment to virtual memory
    char * shm = (char*)shmat(shmid, nullptr, 0);
    // start reading
    char * s = shm;
    for (s = shm; *s != '\0'; s++) {
        sem_wait(sem_id);
        putchar(*s);
        sem_post(sem_id);
    }
    //
    *shm = '*';
    sem_close(sem_id);
    shmctl(shmid, IPC_RMID, 0);
    return EXIT_SUCCESS;
}
