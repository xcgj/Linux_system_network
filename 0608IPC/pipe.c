/*************************************************************************
  > File Name: pipe.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月08日 星期四 14时53分24秒
 ************************************************************************/

#include"../header.h"

int main()
{
    //管道文件描述符
    //fd[0]:读取管道的数据
    //fd[1]:从外部写文件到管道
    int fd[2];
    int ret = pipe(fd);

    //读数据和写数据都是针对管道这个对象而言的
    //管道大小由系统分配

    pid_t pid = fork();
    //子进程写，父进程读
    if (pid == 0)
    {
        close(fd[0]);
        sleep(1);//父进程会等待管道中的数据，阻塞
        write(fd[1], "xcgj", 4);
    }
    else
    {
        close(fd[1]);
        char buf[1024];
        read(fd[0], buf, sizeof(buf)); 
        printf("%s\n", buf);
    }
}

