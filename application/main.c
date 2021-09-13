// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "worker_manager.h"
#include "../shmLibrary/shm_manager.h"

#define MAX_RESULT_SIZE 1024

static int getCpuCount() {
    FILE *fd;
    if ((fd = popen("nproc --all", "r")) == NULL) {
        perror("popen");
        return -1;
    }

    int cpuCount = 1;
    fscanf(fd, "%5d", &cpuCount);

    if (pclose(fd) < 0) {
        perror("pclose");
    }

    return cpuCount;
}

static void parseArgs(int argc, char* const argv[], int* fileCount, int* workerCount) {
    if (argc < 2) {
        fprintf(stderr, "Error: No files detected\n");
        exit(1);
    }

    int opt; int jFlag = 0;
    while((opt = getopt(argc, argv, "j:")) != -1) {
        if(opt == 'j') {
            if(sscanf(optarg, "%3d", workerCount) < 1 || *workerCount < 1) {
                fprintf(stderr, "Invalid worker count\n");
                exit(1);
            }
            jFlag = 1;
        }
        else {
            exit(1);
        }
    }

    *fileCount = argc - jFlag*2 - 1;

    if(!jFlag) {
        *workerCount = getCpuCount();
    }
    
    *workerCount = *fileCount < *workerCount ? *fileCount : *workerCount;
}

int main(int argc, char* const argv[]) {
    int fileCount = -1; int workerCount = -1;
    parseArgs(argc, argv, &fileCount, &workerCount);
    char* const *files = &argv[argc - fileCount];

    ShmPointer shm = create_shm(MAX_RESULT_SIZE*(argc - 1));

    Worker *workers = summon_workers(workerCount);

    printf("%s\n",getName(shm));
    fflush(stdout);
    sleep(2); // tiempo para que se conecte la vista

    manageWorkers(files, fileCount, workers, workerCount, shm);

    free(workers);
    destroy_shm(shm);

    return 0;
}
