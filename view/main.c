#include <stdio.h>
#include <semaphore.h>
#include "shm_connector.h"

int main(int argc, char const *argv[]) {
    void * shm;

    if (argc > 1)
    {
        shm = attach_shm(argv[1]);
    }
    else
    {
        char shmName[SHM_NAME_LEN];
        scanf("%s", shmName);
        shm = attach_shm(shmName);
    }

    sem_t * sem = (sem_t *) shm;
    char * data = (char *) shm + sizeof(sem_t);

    dettach_shm(shm);

    return 0;
}

