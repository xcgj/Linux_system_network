/*************************************************************************
  > File Name: shmwrite.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月08日 星期四 19时51分30秒
 ************************************************************************/

#include"../header.h"

int main()
{
    int fd = shm_open("/share", O_RDWR|O_CREAT|O_EXCL, 0777);
    if (fd < 0)//文件存在
    {
        fd = shm_open("/share", O_RDWR, 0777);
    }
    else//文件不存在，并被成功创建，修改文件大小
    {
        ftruncate(fd, 4096);
    }

    void * ptr = mmap(
            NULL, 
            4096,
            PROT_READ|PROT_WRITE,
            MAP_SHARED,
            fd,
            0);
    
    //写
    strcpy((char*)ptr, "xcgj");
    printf("write done\n");
    getchar();

    close(fd);
    shm_unlink("/share");//解除映射
    munmap(ptr, 4096);
    return 0;
}
