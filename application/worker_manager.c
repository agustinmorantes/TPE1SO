// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "worker_manager.h"
#include "../shmLibrary/shm_manager.h"

#define MAX_FILEPATH_SIZE 1024
#define MAX_SHM_OUTPUT_LENGTH 2048
#define EOT 4

static inline Worker* getWorkerFromPid(int pid, Worker* workers, int workerCount) {
    for(int i = 0; i < workerCount; i++) {
        if(workers[i].pid == pid)
            return &workers[i];
    }
    return NULL;
}

int manageWorkers(char* const *filepaths, int fileCount, Worker* workers, int workerCount, ShmPointer shm) {
    int retcode = 0;
    int currentFile = 0;

    //First 2 files to each worker
    for(int j = 0; j < 2; j++) {
        for(int i = 0; i < workerCount && currentFile < fileCount; i++) {
            dprintf(workers[i].pipe, "%s\n", filepaths[currentFile++]);
        }
    }
    
    char * shmOutput = getData(shm);
    
    FILE* inPipe = fdopen(workers[workerCount].pipe, "r");
    if(inPipe == NULL) {
        perror("fdopen");
        return 1;
    }

    char* buf = NULL; size_t len = 0;
    while(getline(&buf, &len, inPipe) != -1) {
        shmOutput += snprintf(shmOutput, MAX_SHM_OUTPUT_LENGTH, "%s", buf);
        postsem(shm);

        int pid = -1;
        sscanf(buf, "%7d", &pid);
        if(pid == -1) {
            fprintf(stderr, "Couldn't read worker output\n");
            continue;
        }

        Worker* worker = getWorkerFromPid(pid, workers, workerCount);
        if(worker == NULL) {
            fprintf(stderr, "Couldn't find worker with given PID.\n");
            continue;
        }

        if(!worker->isPipeOpen) continue;
        
        if(currentFile < fileCount) {
            dprintf(worker->pipe, "%s\n", filepaths[currentFile++]);
        }
        else {
            close(worker->pipe);
            worker->isPipeOpen = 0;
        }
    }

    for(int i = 0; i < workerCount; i++) {
        wait(NULL);
    }

    *shmOutput = EOT;
    postsem(shm);


    free(buf);
    fclose(inPipe);
    return retcode;
}
