/*************************************************************************
	> File Name: enviroment.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月02日 星期五 19时24分17秒
 ************************************************************************/

#include "../header.h"

//glibc中定义的全局变量，保存所有的环境变量
extern char ** environ;

int main()
{
    //增加环境变量
    setenv("xcgj", "10000", 1);

    int i = 0;
    for (;;++i)
    {
        if(environ[i] == NULL)
        {
            break;
        }
        printf("%s\n", environ[i]);
    }
    return 0;
}
