#include "apue.h"
#include "testbase.h""
#include "testsocket.h"
#include "testprocess.h"
#include "testsignal.h"
#include "testpipe.h"
#include "testfile.h"
#include "testsocket.h"
#include "testipc.h"
#include "teststl.h"
#include "testdaemon.h"
#include "select.h"
#include "epoll.h"
#include "testtime.h"
#include <vector>

void
dump(int signo)
{
    char  buf[1024];
    char  cmd[1024];
    FILE *fh;
    snprintf(buf, sizeof(buf), "/proc/%d/cmdline",getpid());
    if(!(fh = fopen(buf, "r")))
        exit(0);
    if(!fgets(buf, sizeof(buf), fh))
        exit(0);
    fclose(fh);
    if(buf[strlen(buf)-1]  == '\n')
        buf[strlen(buf)-1]  = '\0';
    snprintf(cmd, sizeof(cmd), "gdb %s %d",buf, getpid());
    system(cmd);
    exit(0);
}

void
dummy_function()
{
    unsigned char* ptr = 0x00;
    *ptr               = 0x00;
}


int
main(int argc, char *argv[])
{
//  TestProcess();
//  TestFork();
//  TestSig();
//    TestPipe();
//    TestPipe2("cedet");
//  TestPipe3("abc.txt");
//  TestFile();
//    TestCopyFile("abc.txt", "abc.txt2");
    signal(SIGSEGV, &dump);
//    dummy_function();

    if(argc < 2)
        err_sys("usage [func]\n");
    if(strcmp(argv[1], "base") == 0)
    {
        TestBase();
    }
    else if(strcmp(argv[1], "time") == 0)
    {
        TestTime();
    }
    else if(strcmp(argv[1],"socket") == 0)
    {
        if(argc < 4)
            err_sys("error args\n");
        if(strcmp(argv[2],"-s")       == 0)
            TestSyncBlock(atol(argv[3]));
        else if(strcmp(argv[2], "-c") == 0)
            TestSyncBlockClient(atol(argv[3]));
    }
    else if(strcmp(argv[1],"udp") == 0)
    {
        if(argc < 4)
            err_sys("error args\n");
        if(strcmp(argv[2],"-s")       == 0)
            TestUDP(atol(argv[3]));
        else if(strcmp(argv[2], "-c") == 0)
            TestUDPClient(atol(argv[3]));
    }
    else if(strcmp(argv[1],"select") == 0)
    {
        do
        {
            uint32_t     port = atol(argv[2]);
            SelectServer server;
            server.init();
            server._listen(port);
            printf("listen socket ok\n");
            int          i    = 0;
            while(server.pulse())
            {
                printf("test:%d\n",i++);
                usleep(1);
            }
        }while(false);
    }
    else if(strcmp(argv[1], "epoll") == 0)
    {
        EpollServer server;
        server._listen(argv[2]);
        while(server.pulse())
        {
            usleep(1);
        }
    }
    else if(strcmp(argv[1], "pipe") == 0)
    {
        if(strcmp(argv[2], "1")      == 0)
            TestPipe();
        else if(strcmp(argv[2], "2") == 0)
            TestPipe2("test.txt");
        else if(strcmp(argv[2], "3") == 0)
            TestPipe3("test.txt");
    }
    else if(strcmp(argv[1], "ipc") == 0)
    {
        if(strcmp(argv[2], "shm") == 0)
            TestShm();
    }
    else if(strcmp(argv[1], "daemon") == 0)
    {
        TestDaemon(argv[0]);
    }
    else if(strcmp(argv[1], "stl") == 0)
    {
        if(strcmp(argv[2], "iter") == 0)
            TestIter();
    }

    return 0;
}
