#pragma once

typedef struct
{
    int pipe;
    pid_t pid;
} worker;

worker * summon_workers(int n);


