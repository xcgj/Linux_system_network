/*************************************************************************
	> File Name: todeamon.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月08日 星期四 22时54分20秒
 ************************************************************************/

#include"../header.h"

int main(int argc, char * argv[])
{
    pid_t pid = fork();
    if (pid == 0)
    {
        setsid();
        setcwd();
        chroot();
        execlp(argv[1], argv[1], NULL);
        return 0;
    }
}
