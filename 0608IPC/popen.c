/*************************************************************************
	> File Name: popen.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月08日 星期四 17时04分19秒
 ************************************************************************/

#include"../header.h"

int main()
{
    //popen 调用了fork和exec
    FILE * fp = popen("ifconfig | grep inet | grep -v inet6 | awk '{print $2}' | awk -F \":\" '{print $2}'", "r");

    char buf[1024];

    while(fgets(buf, sizeof(buf), fp))
    {
        printf("buf is %s", buf);
    }

    pclose(fp);
    return 0;
}
