/*************************************************************************
	> File Name: unrelatemmap.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月08日 星期四 19时15分18秒
 ************************************************************************/

#include"../header.h"

//写
int main()
{
    int fd = open("share", O_RDWR);
    void * ptr = mmap(
            NULL,
            4096, 
            PROT_READ|PROT_WRITE,
            MAP_SHARED,     //映射了文件，不匿名
            fd,
            0);

    strcpy((char*)ptr, "xcgj");
    printf("write done\n");

    close(fd);
    munmap(ptr, 4096);
    return 0;
}
