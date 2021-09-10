#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "worker_manager.h"

#define MAX_FILEPATH_SIZE 1024

static inline Worker* getWorkerFromPid(int pid, Worker* workers, int workerCount) {
    for(int i = 0; i < workerCount; i++) {
        if(workers[i].pid == pid)
            return &workers[i];
    }
    return NULL;
}

int manageWorkers(const char** filepaths, int fileCount, Worker* workers, int workerCount) {
    int retcode = 0;
    int currentFile = 0;

    //First 2 files to each worker
    for(int j = 0; j < 2; j++) {
        for(int i = 0; i < workerCount && currentFile < fileCount; i++) {
            dprintf(workers[i].pipe, "%s\n", filepaths[currentFile++]);
        }
    }
    
    FILE* inPipe = fdopen(workers[workerCount].pipe, "r");
    char* buf = NULL; size_t len = 0;
    while(getline(&buf, &len, inPipe) != -1) {
        printf("%s", buf);

        int pid = -1;
        sscanf(buf, "%d", &pid);
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

    free(buf);
    fclose(inPipe);
    return retcode;
}