/*************************************************************************
	> File Name: mouseevent.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月11日 星期日 15时48分58秒
 ************************************************************************/

#include"../header.h"

int main()
{
    int fd_mice = open("/dev/input/mice", O_RDONLY);
    int fd_key = open("/dev/input/event1", O_RDONLY);

    char buf[8];
    char key[1024];
    while(1)
    {
        //鼠标阻塞
        int ret = read(fd_mice, buf, sizeof(buf));

        printf("0x%02x, 0x%02x, 0x%02x\n",//十六进制输出，最大宽度为2
                (unsigned int)buf[0],//无符号整型输出，避免char溢出
                (unsigned int)buf[1],
                (unsigned int)buf[2]);
        //键盘阻塞
        ret = read(fd_key, key, sizeof(key));
        printf("key%d\n",ret);
    }

    close(fd_mice);
    close(fd_key);
    return 0;
}
