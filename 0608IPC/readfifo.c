/*************************************************************************
	> File Name: readfifo.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月08日 星期四 17时22分15秒
 ************************************************************************/

#include"../header.h"

int main()
{
    //打开fifo文件
    int fd = open("a.fifo", O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return 0;
    }

    //读取fifo文件中的数据
    char buf[1024];
    int ret = read(fd, buf, sizeof(buf));

    printf("buf: %s\n", buf);

    return 0;
}
