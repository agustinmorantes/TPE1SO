// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "worker_manager.h"
#include "../shmLibrary/shm_manager.h"

#define MAX_RESULT_SIZE 1024

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr,"Error: No files detected\n");
        exit(1);
    }
    int workerCount;

    {
        FILE * fd;
        
        if ((fd = popen("nproc --all","r")) == NULL)
        {
            perror("popen");
            exit(1);
        }
        fscanf(fd, "%5d", &workerCount);
        if (pclose(fd) < 0)
        {
            perror("pclose");
            exit(1);
        }
    }

    workerCount = argc <= workerCount ? argc - 1 : workerCount;

    shmPointer shm = create_shm(MAX_RESULT_SIZE*(argc - 1));

    Worker* workers = summon_workers(workerCount);

    printf("%s\n",getName(shm));
    fflush(stdout);

    sleep(2); // tiempo para que se conecte la vista

    manageWorkers(&argv[1], argc-1, workers, workerCount, shm);

    free(workers);
    destroy_shm(shm);

    return 0;
}
