/*************************************************************************
	> File Name: writefifo.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月08日 星期四 17时25分08秒
 ************************************************************************/

#include"../header.h"

int main()
{
    //打开fifo文件
    int fd = open("a.fifo", O_WRONLY);
    if (fd < 0)
    {
        perror("open");
    }

    //向fifo文件写数据
    char buf[1024];
    fgets(buf, sizeof(buf), stdin);

    write(fd, buf, sizeof(buf));

    return 0;
}
