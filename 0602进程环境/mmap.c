/*************************************************************************
	> File Name: mmap.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月02日 星期五 22时34分29秒
 ************************************************************************/

#include "../header.h"

int main()
{
    void * ptr = mmap(
            NULL,
            4096,
            PROT_READ|PROT_WRITE,
            MAP_PRIVATE|MAP_ANON,//匿名映射
            -1,
            0);
    strcpy(ptr, "xcgj");
    printf("%s\n", (char *)ptr);
    munmap(ptr, 4096);

    return 0;
}
