#ifndef TESTPROCESS_H
#define TESTPROCESS_H
#include <unistd.h>
#include "apue.h"

static void charatatime(char* str);


int TestProcess()
{
    pid_t pid;
    if((pid = fork()) <  0)
    {
        err_sys("fork error");
    }
    else if ( pid == 0)
    {
        charatatime("Output from child \n");
    }
    else
    {
        charatatime("Output from parent \n");
    }
}


static void charatatime(char* str)
{
    char* ptr = NULL;
    int c = 0;
    setbuf(stdout, NULL);
    for(ptr = str; (c=*ptr++)!=0; )
        putc(c, stdout);
}



int TestFork()
{
    pid_t pid;
    if((pid = fork()) < 0 )
    {
        err_sys("fork err");
    }
    else if(pid != 0 )
    {
        sleep(2);
        exit(2);
    }
    if((pid = fork()) < 0 )
    {
        err_sys("fork_err");
    }
    else if(pid !=0)
    {
        sleep(4);
        abort();
    }

    if((pid = fork()) < 0)
    {
        err_sys("fork_err");
    }
    else if(pid != 0)
    {

    }

}
#endif
