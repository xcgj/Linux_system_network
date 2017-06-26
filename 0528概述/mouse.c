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

int main()
{
    //打开鼠标文件
    int fd = open("/dev/input/mice", O_RDONLY);

    while(1)
    {
        char buf[8];
        //读取鼠标移动或者点击信息
        int ret = read(fd, buf, sizeof(buf));

        printf("ret is %d\n", ret);
    }

    return 0;
}
