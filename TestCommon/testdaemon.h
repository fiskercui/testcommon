#ifndef TESTDAEMON_H
#define TESTDAEMON_H

#include "apue.h"
#include <sys/resource.h>
#include <fcntl.h>
#include <syslog.h>

void
daemonize(const char* cmd)
{
    int i ,fd0, fd1, fd2;

    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    umask(0);
    if(getrlimit(RLIMIT_NOFILE, &rl) < 0)
        err_quit("%s: cann't get file limit", cmd);

    printf("befork fork\n");
    if((pid = fork()) < 0)
        err_sys("fork eror");
    else if(pid > 0){
        printf("parent exit1 \n");
        exit (0);
    }

    printf("fork chld1\n");
    //only left child process
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;


    if(sigaction(SIGHUP, &sa, NULL) < 0)
        err_sys("can;t ignore SIGHUP");

    if((pid = fork()) < 0)
        err_quit("cann't ignore SIGHUP");
    else if(pid > 0){
        printf("parent exit2 \n");
        exit(0);
    }

    printf("fork child2\n");
    if(chdir("/") < 0 )
        err_sys("cann;t change work dir");


    if(rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;

    for(int i=0; i<rl.rlim_max; i++)
        close(i);

    fd0 = open("dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    printf("open log\n");

//    while(1);
    if(fd0 != 0 || fd1 != 1 || fd2 !=2)
    {
        printf("log\n");
        syslog(LOG_ERR, "unexcept file descriptors %d %d %d ",fd0, fd1, fd2);
        exit(1);
    }
}

void AlreadyRun()
{

}

void TestDaemon(const char* szCmd)
{

}

#endif
