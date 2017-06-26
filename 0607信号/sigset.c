/*************************************************************************
	> File Name: sigset.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月07日 星期三 16时10分06秒
 ************************************************************************/

#include"../header.h"

#if 0
uint32_t sig_set = 0;//没有任何标记

int main1()
{
    sig_set |= 1 << SIGFPE;//增加
    sig_set ＆= ~(1 << SIGUSR1);//移除
}
#endif

int main()
{
    //定义集合
    sigset_t sigset;
    //填充满集合
    sigfillset(&sigset);//0xffffffff
    //清空集合
    sigemptyset(&sigset);//0
    //增加信号
    sigaddset(&sigset, SIGINT);
    //移除信号
    sigdelset(&sigset, SIGINT);
    //判断是否有信号
    sigismember(&sigset, SIGINT);

    return 0;
}
