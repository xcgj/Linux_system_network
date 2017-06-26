/*************************************************************************
	> File Name: forkFile.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月03日 星期六 15时08分15秒
 ************************************************************************/

#include "../header.h"

int main()
{
    //fork之前先创建一个文件
    int fd = open("a.txt", O_WRONLY|O_CREAT, 0777);

    pid_t pid = fork();
    //子进程也含有3号文件描述符

    //子进程dup了父进程的文件描述符
    //共享了一个文件结构体，文件offset变量是同一个
    //因此内容不会覆盖
    write(fd, "xcgj\n", 5);

    //测试子进程改变复制过来的文件描述符父进程会不会变化————会
    if (pid == 0)
    {
        //先获得原来文件的属性
        int flag = fcntl(fd, F_GETFL);
        //子进程增加属性
        flag |= O_APPEND;
        fcntl(fd, F_SETFL, flag);
    }

    else
    {
        // 父进程先休眠1秒，让子进程先执行文件属性的更改
        sleep(1);

        //获得文件的flag属性
        int flag = fcntl(fd, F_GETFL);

        //父进程检查属性是否增加
        if (flag & O_APPEND)//只有2个都为1，结果才是1
        {
            //子进程修改文件描述符属性，会影响父进程对应的文件描述符
            printf("APPEND\n");
        }
        else
        {
            printf("NONE\n");
        }
    }
    return 0;
}
