/*************************************************************************
	> File Name: sigmask.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月07日 星期三 16时38分55秒
 ************************************************************************/

#include"../header.h"

//设置掩码
void setmask(sigset_t *mask)
{
    sigprocmask(
            SIG_BLOCK,
            mask,
            NULL
            );
}

//解除掩码
void unsetmask(sigset_t *set)
{
    sigprocmask(
            SIG_UNBLOCK,
            set,
            NULL);
}

//实时信号处理函数
void handle_sigint(int v)
{
    if (v == SIGINT)
        printf("rt\n");
    else if(v == 34)
        printf("34\n");
}

int main()
{
    signal(SIGINT, handle_sigint);
    signal(34, handle_sigint);

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, 34);

    setmask(&mask);

    kill(getpid(), SIGINT);
    kill(getpid(), SIGINT);//信号只执行一次
    kill(getpid(), 34);
    kill(getpid(), 34);//两次信号都会被执行

    unsetmask(&mask);//掩码取消后，处理SIGINT

    return 0;
}
