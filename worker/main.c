// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// Definimos un máximo al tamaño del path. Sabemos que existe PATH_MAX en limits.h, pero si nos pasan un path cercano a 4096 bytes,
// entonces no podemos asegurar que el write al pipe sea atómico (pues supera PIPE_BUF al incluir lo que debe devolver minisat).
#define MAX_PATH_SIZE 512
#define MAX_RESULT_SIZE 1024
#define FILE_ERROR "Error: not a valid file"
#define MINISAT_ERROR "Minisat Error, couldn't solve file."
#define COMMAND_FORMAT "minisat %s | grep -o -e \"Number of.*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\" | grep -o -e \"[0-9]\\+\\.[0-9]\\+\" -e \"[0-9]\\+\" -e \".*SAT\" | tr '\\n' '\\t'"

int main() {
    char pathName[MAX_PATH_SIZE]; 
    char cmd[MAX_PATH_SIZE + 150];
    char * buffer;
    size_t bufLen;
    pid_t pid = getpid();
    char toPrint[MAX_RESULT_SIZE];
    char * rta;
    while (scanf("%511s", pathName) != EOF)
    {
        if( access(pathName, F_OK) != 0 ) 
            rta = FILE_ERROR;
        else
        {
            sprintf(cmd, COMMAND_FORMAT, pathName);

            FILE* fd = popen(cmd, "r");
            if (fd == NULL)
            {
                perror("popen");
                exit(1);
            }

            buffer = NULL;
            bufLen = 0;
            getline(&buffer, &bufLen, fd);
            pclose(fd);
            if (buffer == NULL)
                rta = MINISAT_ERROR; 
            else
                rta = buffer;
        }

        snprintf(toPrint, MAX_RESULT_SIZE, "%d\t%s\t%s\n", pid, pathName, rta);
        printf("%s", toPrint);
        fflush(stdout);
        
    }
    free(buffer);
    return 0;
}
