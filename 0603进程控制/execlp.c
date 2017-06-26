/*************************************************************************
	> File Name: execlp.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月03日 星期六 20时02分21秒
 ************************************************************************/

#include "../header.h"
int main()
{
    //execl命令需要提供完整路径才能执行可执行程序
    //execl("/bin/ls", "/bin/ls", NULL);//正确
    //execl("ls", "ls", NULL);//错误

    //execlp只需提供程序名，会在$PATH路径查找
    //execlp("/bin/ls", "/bin/ls", NULL);
    execlp("ls", "ls", NULL);
    //execlp("wf.out", "wf.out", NULL);//错误

    printf("error signal\n");
}
