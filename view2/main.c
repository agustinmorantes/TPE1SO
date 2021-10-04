// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_OUTPUT_LENGTH 1024
#define EOT 4

static inline int readLine(char* dst, char* src) {
    int len = 0;
    while(*src != '\n') {
        if(*src == EOT) return -1;
        dst[len++] = *src++;
    }
    dst[len++] = 0;
    return len;
}

int main(int argc, char const *argv[]) {
    char buf[MAX_OUTPUT_LENGTH]; int len = 0;

    int fd = open("/tmp/view2pipe", O_RDONLY);
    
    printf("PID\tFILE\t\t\tVARS\tCLAUSES\tTIME\t\tOUTPUT\n");

    while((len = read(fd, buf, 512)) > 0) {
        buf[len] = 0;
        printf("%s", buf);
    }

    return 0;
}

