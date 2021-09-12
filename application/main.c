#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "worker_manager.h"
#include "shm_manager.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr,"Error: No files detected\n");
        exit(1);
    }

    FILE * fd;
    int workerCount;
    
    if ((fd = popen("nproc --all","r")) == NULL)
    {
        perror("popen");
        exit(1);
    }
    fscanf(fd, "%d", &workerCount);
    if (pclose(fd) < 0)
    {
        perror("pclose");
        exit(1);
    }

    workerCount = argc <= workerCount ? argc - 1 : workerCount;

    void* shm = create_shm(argc - 1);
    sem_t * shmSem = (sem_t*)shm;
    char * shmData = (char*)shm + sizeof(sem_t);

    Worker* workers = summon_workers(workerCount);

    printf("%s\n",SHM_NAME);
    fflush(stdout);

    sleep(2); // tiempo para que se conecte la vista

    manageWorkers(&argv[1], argc-1, workers, workerCount, shmData, shmSem);

    free(workers);
    destroy_shm(shm);

    return 0;
}
