/*************************************************************************
	> File Name: mouse.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年05月28日 星期日 20时24分28秒
 ************************************************************************/

#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>

int main()
{
    //打开鼠标文件
    int fd = open("/dev/input/mouse0", O_RDONLY);
    if (fd < 0)
    {
        //errno是一个全局变量
        printf("errno = %d\n", errno);
        printf("%s\n", strerror(errno));
        perror("open");
        return;
    }

    while(1)
    {
        char buf[8];
        //读取鼠标移动或者点击信息
        int ret = read(fd, buf, sizeof(buf));

        printf("ret is %d\n", ret);
    }

    return 0;
}
