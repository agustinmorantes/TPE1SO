#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main() {
    int pid = getpid();

    char buf[513];
    while(scanf("%512s", buf) != -1) {
        printf("%d\t%s\n", pid, buf);
        fflush(stdout);
    }
}
