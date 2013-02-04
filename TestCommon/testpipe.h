#ifndef TESTPIPE_H
#define TESTPIPE_H

#include "apue.h"
#include <sys/wait.h>

static int pfd1[2], pfd2[2];

void TELL_WAIT()
{
    if(pipe(pfd1)< 0 || pipe(pfd2)< 0)
        err_sys("pipe error");
}

void TELL_PARENT(pid_t pid)
{
    if(write(pfd2[0],"c", 1) != 1)
        err_sys("write error");
}
void WAIT_PARENT()
{
    char c;
    if(read(pfd1[1],&c, 1) != 1)
        err_sys("read error");
    if(c != 'p')
        err_sys("read error ,WAIT_PARENT");
}

void TELL_CHILD(pid_t pid)
{
    if(write(pfd1[0], "p",1) !=1)
        err_sys("write error");
}

void WAIT_CHILD()
{
    char c;
    if(read(pfd2[1], &c, 1) != 1)
        err_sys("read error");
    if(c != 'c')
        err_sys("WAIT_CHILD, incorrect date");
}

int TestPipe()
{
    int n = 0;
    int fd[2]= {0};
    pid_t pid =0;
    char line[MAXLINE] = {'\0' };


    if(pipe(fd) < 0)
        err_sys("pipe error");
    if((pid = fork())<0)
        err_sys("fork error");
    else if(pid == 0)
    {
        printf("child process\n");
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }
    else if(pid > 0)
    {
//      sleep(20);
        printf("parent process\n");
        close(fd[0]);
        write(fd[1], "hello world\n", 12);
    }
    return 0;
}

#define DEF_PAGER "/bin/more"

int TestPipe2(const char* filepath)
{
    int n =0;
    int fd[2] = {0};
    pid_t pid = 0;

    char* pager=NULL, *argv0=NULL;
    char line[MAXLINE] ={'\0'};
    FILE *fp = NULL;


    if(filepath == NULL)
        err_quit("please input the pathname");
    if((fp = fopen(filepath, "r"))== NULL)
        err_sys("Can't open %s ", filepath);

    if(pipe(fd)<0)
        err_sys("pipe error");

    if((pid = fork()) < 0 )
        err_sys("fork error");
    else if(pid > 0) //parent proces
    {
        close (fd[0]);
        while (fgets(line ,MAXLINE, fp) != NULL)
        {
            n = strlen(line);
            if(write(fd[1], line, n ) != n)
                err_sys("write error to pipe");
        }
        if(ferror(fp))
            err_sys("fgets error");
        close(fd[1]);
        if(waitpid(pid, NULL, 0 ) < 0 )
            err_sys("waitpid error");
        exit(0);
    }
    else
    {
        close(fd[1]);
        if(fd[0] != STDIN_FILENO )
        {
            if(dup2(fd[0], STDIN_FILENO)!= STDIN_FILENO)
            {
                err_sys("dup error");
            }
            close(fd[0]);
        }
        if((pager = getenv("PAGER")) == NULL)
            pager = DEF_PAGER;
        if((argv0 == strrchr(pager, '/')) != NULL)
            argv0++;
        else
            argv0 = pager;
        if(execl(pager, argv0, (char*)0) < 0)
            err_sys("excel error for %s", pager);
    }
}

#define PAGER "${PAGER:-more}"

int TestPipe3(const char* filepath)
{
    char line[MAXLINE];
    FILE *fpin, *fpout;


    if(filepath == NULL)
        err_sys("filepath error");
    if((fpin = fopen(filepath, "r")) == NULL)
        err_sys("cann't open %s", filepath);
    if((fpout = popen(PAGER, "w")) == NULL)
        err_sys("fputs error to pipe");

    while (fgets (line, MAXLINE, fpin)!= NULL)
        if(fputs(line, fpout)== EOF)
            err_sys("fputs error to pipe");
    if(ferror(fpin))
        err_sys("fgets error");
    if(pclose(fpout)== -1)
        err_sys("pclose error");
    exit(0);
}

#endif
