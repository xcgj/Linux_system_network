/*************************************************************************
  > File Name: myls.c
  > Author: 陈琳耀 || C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月01日 星期四 16时00分45秒
 ************************************************************************/

#include "header.h"
#include <dirent.h>// directory entry 目录项
int main()
{
    //dir中保存了一个指针
    DIR * dir = opendir(".");//打开当前目录

    //读取当前所指目录信息，返回当前目录的目录项指针
    //每次readdir会使dir指针向下偏移
    while(1)
    {
        struct dirent * entry = readdir(dir);
        if (entry == NULL)
        {
            break;
        }
        //打印目录项信息
        //printf("inode = %d, name = %s\n", (int)entry->d_ino, entry->d_name);
        if (entry->d_name[0] == '.')
        {
            continue;//不打印 .
        }
        printf("%s  ", entry->d_name);
    }
    printf("\n");

    closedir(dir);
}
