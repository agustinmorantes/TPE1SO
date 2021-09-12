#pragma once

#include <semaphore.h>
#include "../shmLibrary/shm_manager.h"

typedef struct {
    int pipe;
    pid_t pid;
    int isPipeOpen;
} Worker;

Worker * summon_workers(int n);
int manageWorkers(const char** filepaths, int fileCount, Worker* workers, int workerCount, shmPointer shm);
