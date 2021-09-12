#pragma once

#define SHM_NAME "/THEBIGSHM"

void * create_shm(int fileCount);

void destroy_shm();