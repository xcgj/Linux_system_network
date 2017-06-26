/*************************************************************************
	> File Name: deamon.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月08日 星期四 22时40分55秒
 ************************************************************************/

#include"../header.h"

int main()
{
    pid_t pid1 = fork();
    if(pid1 == 0)
    {
        pid_t pid2 = fork();
        if(pid2 == 0)
        {
            setsid();//会话组长
            setcwd();//改变当前目录
            chroot();//改变根目录

            //重定向标准输出
            int fd = open("/dev/null", OWRONLY);
            dup2(fd, 1);
            close(fd);
        }
        
    }
}
