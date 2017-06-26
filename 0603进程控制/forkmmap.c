/*************************************************************************
	> File Name: forkmmap.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月03日 星期六 15时49分10秒
 ************************************************************************/

#include "../header.h"

int main()
{
    //映射内存，内存共享
    void * ptr = mmap(
            NULL, 
            4096, 
            PROT_READ|PROT_WRITE,
            MAP_SHARED|MAP_ANON,
            -1,
            0);

    //堆内存，内存不共享
    char * str = malloc(100);
    strcpy(str, "c01");

    pid_t pid = fork();
    if (pid == 0)//son process
    {
        strcpy((char*)ptr, "xcgj");
        strcpy(str, "rbmw");
    }
    else//father process
    {
        sleep(1);
        printf("mmap = %s\n", (char*)ptr);//xcgj
        printf("malloc = %s\n", str);//c01
    }

    free(str);
    return 0;
}
