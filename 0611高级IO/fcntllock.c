/*************************************************************************
	> File Name: fcntllock.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月11日 星期日 22时11分41秒
 ************************************************************************/

#include"../header.h"

int main()
{
    int fd = open("a.txt", O_RDWR);

    //描述文件锁的基本信息，比如文件区间，文件长度
    struct flock lock;
    lock.l_type = F_WRLCK;//写锁
    lock.l_whence = SEEK_SET;//从哪里开始锁
    lock.l_start = 0;//偏移量为0，说明从文件开始加锁
    lock.l_len = 1024;//加锁的区间大小，1024个字节

    //文件加锁
    //F_SETLK：不等待加锁，遇到锁返回-1，加锁成功返回0
    //F_SETLKW：等待加锁，遇到锁等待锁的任务完成再加锁
    fcntl(fd, F_SETLKW, &lock);

    printf("lock\n");
    getchar();

    //文件解锁
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    close(fd);

    return 0;
}
