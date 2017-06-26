/*************************************************************************
	> File Name: map.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年05月30日 星期二 14时50分17秒
 ************************************************************************/

#include "../header.h"

int main()
{
    int fd = open("a.tmp", O_RDONLY);

    getchar();//等待，删除a.tmp文件

    char buf[4096];
    read(fd, buf, sizeof(buf));
    printf("%s\n", buf);//删除以后依然能打印出来

    return 0;
}
