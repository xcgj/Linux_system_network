/*************************************************************************
	> File Name: dup.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年05月29日 星期一 14时32分38秒
 ************************************************************************/

#include "../header.h"

int main()
{
    int fd = open("a.tmp", O_WRONLY|O_CREAT, 0777);
    if (fd < 0)
    {
        perror("open");
        return 0;
    }

    //将fd复制到fd2，fd2也是一个文件描述符，和fd共享一个文件结构体
    int fd2 = dup(fd);

    write(fd, "hello", 5);
    write(fd2, "c01", 3);

    close(fd);
    close(fd2);

    return 0;
}
