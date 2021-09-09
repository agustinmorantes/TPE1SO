#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

inline void create_pipe(int * pipefd) 
{
    if (pipe(pipefd) < 0)
    {
        perror("pipe");
        exit(1);
    }
}

inline void dup_pipe_end(int oldfd, int newfd)
{
    if (dup2(oldfd,newfd) < 0)
    {
        perror("dup2");
        exit(1);
    }
}

inline void close_pipe_end(int fd)
{
    if (close(fd) < 0) 
    {
        perror("close");
        exit(1);
    }
}

int * summon_workers(int n)
{
    int i;
    pid_t pid;
    int wpipefd[2], rpipefd[2];
    char * arguments[2] = {"worker.out",0};
    int * pipes = malloc((n+1)*sizeof(int));

    create_pipe(rpipefd);

    for (i = 0; i < n; i++)
    {
        create_pipe(wpipefd);

        if ((pid = fork()) < 0)
        {
            perror("fork");
            exit(1);
        }
        
        if (pid == 0)
        {
            dup_pipe_end(wpipefd[0],STDIN_FILENO);
            dup_pipe_end(rpipefd[1],STDOUT_FILENO);

            close_pipe_end(wpipefd[1]);
            close_pipe_end(rpipefd[0]);
            execv("worker/worker.out", arguments);
            perror("execv");
            exit(1);
        }

        pipes[i] = wpipefd[1];

        close_pipe_end(wpipefd[0]);
    }

    close_pipe_end(rpipefd[1]);

    pipes[i] = rpipefd[0];

    return pipes;
}