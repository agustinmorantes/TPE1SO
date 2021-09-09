#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int * summon_workers(int n)
{
    int i;
    pid_t pid;
    int wpipefd[2], rpipefd[2];
    char * arguments[2] = {"worker.out",0};
    int * pipes = malloc((n+1)*sizeof(int));

    if (pipe(rpipefd) < 0)
    {
        perror("pipe");
        exit(1);
    }

    for (i = 0; i < n; i++)
    {
        if (pipe(wpipefd) < 0)
        {
            perror("pipe");
            exit(1);
        }

        if ((pid = fork()) < 0)
        {
            perror("fork");
            exit(1);
        }

        if (pid == 0)
        {
            if (dup2(wpipefd[0],STDIN_FILENO) < 0 || dup2(rpipefd[1],STDOUT_FILENO) < 0)
            {
                perror("dup2");
                exit(1);
            }

            if (close(wpipefd[1]) < 0 || close(rpipefd[0]) < 0) 
            {
                perror("close");
                exit(1);
            }
            
            execv("worker/worker.out", arguments);
            perror("execv");
            exit(1);
        }

        pipes[i] = wpipefd[1];

        if (close(wpipefd[0]) < 0) 
        {
            perror("close");
            exit(1);
        }
    }

    if (close(rpipefd[1]) < 0) 
    {
        perror("close");
        exit(1);
    }

    pipes[i] = rpipefd[0];

    return pipes;
}