#ifndef TESTTIME_H
#define TESTTIME_H
#include <sys/time.h>
#include <unistd.h>

void
TimeAlarm(int sigal)
{
    static int nCount = 0;
    printf("test alarm: %d\n", nCount++);
    alarm(5);
}

void
TestTime()
{
    signal(SIGALRM, &TimeAlarm);
    printf("time start \n");
    alarm(5);
    while(1)
    {
        printf("Time Loop\n");
        pause();
    }
    printf("time end\n");
}









#endif
