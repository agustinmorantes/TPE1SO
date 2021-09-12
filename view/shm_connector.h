#pragma once

#define SHM_NAME_LEN 128

void * attach_shm(const char * name);
void dettach_shm(void * shm);

