// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "worker_manager.h"

Worker * summon_workers(int n)
{
    int i;
    pid_t pid;
    int send_pipefd[2], receive_pipefd[2];
    char * arguments[2] = {"worker.out",0};
    Worker * workers = malloc((n+1)*sizeof(Worker));

    if (workers == NULL) 
    {
        return NULL;
    }

    if (pipe(receive_pipefd) < 0)
    {
        free(workers);
        return NULL;
    }

    for (i = 0; i < n; i++)
    {
        if (pipe(send_pipefd) < 0)
        {
            free(workers);
            return NULL;
        }

        if ((pid = fork()) < 0)
        {
            free(workers);
            return NULL;
        }
        
        if (pid == 0)
        {
            if (dup2(send_pipefd[0],STDIN_FILENO) < 0 || dup2(receive_pipefd[1],STDOUT_FILENO) < 0)
            {
                free(workers);
                return NULL;
            }

            if (close(send_pipefd[1]) < 0 || close(receive_pipefd[0]) < 0) 
            {
                free(workers);
                return NULL;
            }
            
            execv("worker/worker.out", arguments);
            free(workers);
            return NULL;
        }

        workers[i].pipe = send_pipefd[1];
        workers[i].pid = pid;
        workers[i].isPipeOpen = 1;

        if (close(send_pipefd[0]) < 0) 
        {
            free(workers);
            return NULL;
        }
    }

    if (close(receive_pipefd[1]) < 0) 
    {
        free(workers);
        return NULL;
    }

    workers[i].pipe = receive_pipefd[0];
    workers[i].pid = -1;
    workers[i].isPipeOpen = 1;

    return workers;
}