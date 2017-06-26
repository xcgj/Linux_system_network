/*************************************************************************
	> File Name: relatemmap.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月08日 星期四 18时49分29秒
 ************************************************************************/

#include"../header.h"

int main()
{
    //血缘关系进程内存映射共享，局部变量不共享，堆空间也不共享
    void * ptr = mmap(
            NULL, 
            4096, 
            PROT_READ|PROT_WRITE,
            MAP_SHARED|MAP_ANON,
            -1, 
            0);

    pid_t pid = fork();
    if (pid == 0)
    {
        //子进程修改共享内存
        strcpy((char*)ptr, "xcgj");
        return 0;
    };

    sleep(5);
    //父进程访问共享内存
    printf("%s\n", (char*)ptr);

    wait(NULL);
    munmap(ptr, 4096);
    return 0;
}
