#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

// Definimos un máximo al tamaño del path. Sabemos que existe PATH_MAX en limits.h, pero si nos pasan un path cercano a 4096 bytes,
// entonces no podemos asegurar que el write al pipe sea atómico (pues supera PIPE_BUF al incluir lo que debe devolver minisat).
#define MAX_PATH_SIZE 1024

int main() {
    char pathName[MAX_PATH_SIZE]; 
    char cmd[MAX_PATH_SIZE + 150];
    FILE * fd;
    char * buffer;
    size_t bufLen;
    pid_t pid = getpid();
    char toPrint[PIPE_BUF];
    int returnValue;
    char * rta;

    while (scanf("%s", pathName) != EOF)
    {
        sprintf(cmd, "minisat %s | grep -o -e \"Number of.*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\" | grep -o -e \"[0-9]\\+\\.[0-9]\\+\" -e \"[0-9]\\+\" -e \".*SAT\" | tr '\\n' '\\t'", pathName);

        fd = popen(cmd, "r");
        if (fd == NULL)
        {
            perror("POPEN");
            exit(1);
        }

        buffer = NULL;
        bufLen = 0;
        getline(&buffer, &bufLen, fd);
        returnValue = pclose(fd);
        rta = buffer;
        // printf("%d\n", returnValue);
        // if (returnValue != 10 && returnValue != 20)
        // {
        //     rta = "MinisatError";
        // }
        
        snprintf(toPrint, PIPE_BUF, "%d\t%s\t%s\n", pid, pathName, rta);
        printf("%s", toPrint);
        
    }
    free(buffer);
    return 0;
}
