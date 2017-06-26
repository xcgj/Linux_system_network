/*************************************************************************
	> File Name: filelock.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月08日 星期四 20时40分56秒
 ************************************************************************/

#include"../header.h"

int main()
{
    int fd = open("file", O_RDWR);

    //加锁
    flock(fd, LOCK_EX);
    //文件操作
    write(fd, "xcgj", 4);
    //解锁
    flock(fd, LOCK_UN);

    close(fd);
    return 0;
}

