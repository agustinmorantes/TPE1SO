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
    int n;
    
    if ((fd = popen("nproc --all","r")) == NULL)
    {
        perror("popen");
        exit(1);
    }
    fscanf(fd, "%d", &n);
    if (pclose(fd) < 0)
    {
        perror("pclose");
        exit(1);
    }

    n = argc <= n ? argc - 1 : n;

    worker * workers = summon_workers(n);

    return 0;
}