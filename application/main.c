#include <stdio.h>
#include <stdlib.h>
#include "worker_manager.h"

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
    printf("Worker count: %d\n", workerCount);

    Worker* workers = summon_workers(workerCount);

    manageWorkers(&argv[1], argc-1, workers, workerCount);

    free(workers);
    return 0;
}
