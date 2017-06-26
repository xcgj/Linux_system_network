
/*************************************************************************
	> File Name: c.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月17日 星期六 11时14分08秒
 ************************************************************************/

#include"../network.h"

int main()
{
    int fd = myConnect2(9988, "192.168.87.128");

    myWriteBuf(fd, "12345");
    myWriteBuf(fd, "abcde");
    myWriteBuf(fd, "qwert");
    
    close(fd);
    return 0;
}
