#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char const *argv[]) {
    char * shmName;
    int shmSize;
    size_t shm_fd;
    void * ptr;

    if (argc > 1)
    {
        shmName = argv[1];
    }
    else
    {
        scanf("%s %s", shmName, shmSize);
    }


    shm_fd = shm_open(shmName, O_RDONLY, 0);
    ptr = mmap(NULL, shmSize, PROT_READ, MAP_SHARED, shm_fd, 0);


    return 0;
}

