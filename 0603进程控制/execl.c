/*************************************************************************
	> File Name: execl.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月03日 星期六 18时34分59秒
 ************************************************************************/

#include "../header.h"
//extern char ** environ;

int main()
{
    //当前目录下执行./bx.out程序
    int ret = execl("./bx.out", "./bx.out", NULL);//./bx.out替换了本程序
    //如果程序执行失败，才会往下走
    printf("执行失败\n");

    //如果程序执行成功，PCB被占，原程序被替换，不会再执行下面的代码
    /*
    if (ret == -1)
    {
        printf("执行失败\n");
    }
    else
    {
        printf("执行成功, %d\n", ret);
    }
    */
    return 0;
}
