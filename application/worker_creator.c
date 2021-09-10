#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "worker_manager.h"

static inline void create_pipe(int * pipefd) 
{
    if (pipe(pipefd) < 0)
    {
        perror("pipe");
        exit(1);
    }
}

static inline void dup_pipe_end(int oldfd, int newfd)
{
    if (dup2(oldfd,newfd) < 0)
    {
        perror("dup2");
        exit(1);
    }
}

static inline void close_pipe_end(int fd)
{
    if (close(fd) < 0) 
    {
        perror("close");
        exit(1);
    }
}

Worker * summon_workers(int n)
{
    int i;
    pid_t pid;
    int send_pipefd[2], receive_pipefd[2];
    char * arguments[2] = {"worker.out",0};
    Worker * workers = malloc((n+1)*sizeof(Worker));

    if (workers == NULL) 
    {
        perror("malloc");
        exit(1);
    }

    create_pipe(receive_pipefd);

    for (i = 0; i < n; i++)
    {
        create_pipe(send_pipefd);

        if ((pid = fork()) < 0)
        {
            perror("fork");
            exit(1);
        }
        
        if (pid == 0)
        {
            dup_pipe_end(send_pipefd[0],STDIN_FILENO);
            dup_pipe_end(receive_pipefd[1],STDOUT_FILENO);

            close_pipe_end(send_pipefd[1]);
            close_pipe_end(receive_pipefd[0]);
            execv("worker/worker.out", arguments);
            perror("execv");
            exit(1);
        }

        workers[i].pipe = send_pipefd[1];
        workers[i].pid = pid;
        workers[i].isPipeOpen = 1;

        close_pipe_end(send_pipefd[0]);
    }

    close_pipe_end(receive_pipefd[1]);

    workers[i].pipe = receive_pipefd[0];
    workers[i].pid = -1;
    workers[i].isPipeOpen = 1;

    return workers;
}