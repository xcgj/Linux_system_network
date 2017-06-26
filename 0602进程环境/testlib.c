/*************************************************************************
	> File Name: testlib.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月02日 星期五 21时14分53秒
 ************************************************************************/

#include<stdio.h>

extern a;
extern void func(void);

int main()
{
    func();
    a = 1;
    return 0;
}
