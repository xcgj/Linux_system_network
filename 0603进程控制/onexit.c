/*************************************************************************
	> File Name: onexit.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月03日 星期六 16时33分57秒
 ************************************************************************/

#include "../header.h"

//用来被on_exit()函数调用的函数
void func(int ret, void * ptr)
{
    printf("ret is %d\n", ret);
}

void myexit()
{
    exit(100);
}

int main()
{
    //先注册函数
    on_exit(func, NULL);

    //程序退出
    myexit();
    //abort();
    //getchar();//ctrl+c异常退出，未能捕获
    
    return 5;
}
