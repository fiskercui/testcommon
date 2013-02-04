#ifndef TESTFILE_H
#define TESTFILE_H

#include "apue.h"

int TestFile()
{
    const char* pFile = "abc.txt";
    FILE* fp = NULL;
    fp = fopen(pFile, "r");
    if(fp == NULL)
        err_sys("open file error");
    else
        printf("open file ok ");
//    return 0;
    int n = 0;
    char line[MAXLINE];
    while(fgets(line, MAXLINE, fp) != NULL)
    {
        n = strlen(line);
        //
    }
    return 0;
}

int TestCopyFile(const char* srcFile, const char* dstFile)
{
    if(srcFile == NULL || dstFile == NULL)
        return 0;
    FILE * fpin, * fpout;
    if( (fpin = fopen(srcFile, "r")) == NULL)
        err_sys("read file error, %s", srcFile);
    if( (fpout = fopen(dstFile, "w")) == NULL)
        err_sys("open file error, %s", dstFile);

    char line[MAXLINE];
    int nread = 0;
    while( (nread = fread( line, sizeof(char), MAXLINE, fpin)) != 0)
    {
        if(fwrite(line, sizeof(char), nread, fpout)  != nread )
            err_sys("write file error");
        memset(line, 0, MAXLINE);
    }
    return 0;
}



#endif
