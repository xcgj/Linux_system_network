/*************************************************************************
	> File Name: zoombie.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月02日 星期五 20时12分03秒
 ************************************************************************/

#include "../header.h"

int main()
{
    if(fork() > 0)
    {
        //让子进程等待
        getchar();
    }
    return 0;
}
