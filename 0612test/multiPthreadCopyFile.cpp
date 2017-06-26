/*************************************************************************
  > File Name: multiPthreadCopyFile.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月12日 星期一 11时37分24秒
 ************************************************************************/
#include <iostream>
#include"../header.h"
#include <pthread.h>
#include <list>
using namespace std;

list<int> listPos;
pthread_mutex_t mutex;

typedef struct parameters
{
    char * sourceFile;
    char * targetFile;
    //int pos;
    int part;
}PM;

void * copy(void * m)
{
    PM * p = (PM *)m;
    //提取参数
    char * sourceFile = p->sourceFile ;
    char * targetFile  = p->targetFile;
    int part = p->part;
    //alarm(0);

    //printf("%d,%d,\n", pos, part);

    //打开文件
    int sfd = open(sourceFile, O_RDONLY);
    int tfd = open(targetFile, O_WRONLY);

    //获取读写位置
        //链表加锁
    pthread_mutex_lock(&mutex);
    int pos = listPos.front();
    listPos.pop_front();
    pthread_mutex_unlock(&mutex);

    lseek(sfd, pos, SEEK_SET);
    lseek(tfd, pos, SEEK_SET);

    //开始拷贝
    char buf[4096] = {0};
    while(1)
    {
        int convertByte = 0;
        if (part < sizeof(buf))
        {
            convertByte = part;
        }
        else
        {
            convertByte = sizeof(buf);
        }

        int readByte = read(sfd, buf, convertByte);
        write(tfd, buf, readByte);

        part -= convertByte;
        if (part <= 0)
        {
            break;
        }
    }
}

// ./a.out n a.c b.c
int main(int argc, char * argv[])
{
    if (argc != 4)
    {
        return -1;
    }

    int pthreadNumber = atoi(argv[1]);
    char * sourceFile = argv[2];
    char * targetFile = argv[3];

    if (pthreadNumber < 1)
    {
        pthreadNumber = 1;
    }

    //获取源文件信息
    struct stat buf;
    int ret = stat(sourceFile, &buf);
    if(ret == -1)
    {
        printf("文件不存在\n");
        return -2;
    }
    int fileLength = buf.st_size;//文件长度

    //创建目标文件
    unlink(targetFile);
    int fd = open(targetFile, O_WRONLY|O_CREAT, 0777);
    close(fd);

    //初始化锁
    pthread_mutex_init(&mutex, NULL);

    //产生子线程
    int i = 0;
    //pthread_t a[1024];
    for(; i < pthreadNumber; ++i)
    {
        //初始化线程函数参数
        PM p;
        p.sourceFile = sourceFile;
        p.targetFile = targetFile; 

        //定位读写位置
        int part = fileLength;
        if (pthreadNumber != 1)
        {
            part = fileLength/(pthreadNumber - 1);
        }

        p.part = part;
        int pos = i * part;

        //链表加锁---
        pthread_mutex_lock(&mutex);
        listPos.push_front(pos);
        pthread_mutex_unlock(&mutex);

        pthread_t tid;
        pthread_create(&tid, NULL, copy, &p);
        pthread_detach(tid);//线程分离
        //sleep(1);
        //pause();
    }

    /*回收
    for(i = 0; i < pthreadNumber; ++i)
    {
        pthread_join(a[i], NULL);
    }*/

    return 0;
}
