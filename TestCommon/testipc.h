#ifndef TESTIPC_H
#define TESTIPC_H

#include "apue.h"
#include <sys/shm.h>


#define ARRAY_SIZE 40000
#define MALLOC_SIZE 100000
#define SHM_SIZE 100000
#define SHM_MODE 0600

char array[ARRAY_SIZE];


int TestShm()
{
    void * testPtr = (void *)-1;
    printf("testPtr ", (unsigned long) (testPtr));
    int shmid;
    char* ptr, * shmptr;

    printf("array[] from %lx to %lx \n", (unsigned long) (&array[0]),
           (unsigned long) (&array[ARRAY_SIZE]));
    printf("statck around %lx \n", (unsigned long) (&shmid));

    ptr = (char*)malloc(MALLOC_SIZE);
    if(ptr == (char *)NULL)
       err_sys("malloc error");

    printf("malloced from %lx to %lx\n", (unsigned long) (ptr),
           (unsigned long) ptr+MALLOC_SIZE);

    if((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE))<0)
        err_sys("shmget error");

    if((shmptr = (char*)shmat(shmid , 0,0)) == NULL )
        err_sys("shmat error");

    printf("shared memory Attached from %lx to %lx\n", (unsigned long )shmptr,
           (unsigned long) shmptr + SHM_SIZE);
    if(shmctl(shmid,  IPC_RMID, 0) < 0)
        err_sys("shmctl error");
    return (0);
}


#endif
