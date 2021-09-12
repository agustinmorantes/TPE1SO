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

    void * shm = create_shm(argc - 1);

    Worker* workers = summon_workers(workerCount);

    sleep(2);
    printf("%s\n",SHM_NAME);

    manageWorkers(&argv[1], argc-1, workers, workerCount);

    free(workers);
    destroy_shm(shm);

    return 0;
}
