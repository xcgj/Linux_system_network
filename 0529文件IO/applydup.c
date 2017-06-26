/*************************************************************************
	> File Name: applydup.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年05月29日 星期一 15时17分47秒
 ************************************************************************/

#include "../header.h"

int main()
{
    int fd = open("appdup.tmp", O_WRONLY|O_CREAT, 0666);

    //保存标准输出的文件
    int stdfd = dup(1);
    printf("第一次打印\n");

    //将fd放到标准输出的位置
    dup2(fd, 1);
    printf("第二次打印，到文件\n");

    //将标准输出文件放回描述符为1的位置
    dup2(stdfd, 1);
    printf("第三次打印\n");

    //实现基础：printf函数write(1, "...", ...);实现
    return 0;
}
