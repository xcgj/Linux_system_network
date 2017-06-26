/*************************************************************************
	> File Name: map.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年05月30日 星期二 16时44分12秒
 ************************************************************************/

#include "../header.h"
//#include <sys/mman.h>
int main()
{

    //set parameters
    void *addr = NULL;
    size_t length = 4096;
    int prot = PROT_READ|PROT_WRITE;
    int flags = MAP_SHARED;
    //open a file
    int fd = open("a.tmp", O_RDWR);
    off_t offset = 0;

    //start map
    char * mp = (char *)mmap(
            addr, 
            length, 
            prot, 
            flags, 
            fd, 
            offset);
    close(fd);

    //copy data into disk file
    strcpy(mp, "hello c01");

    //end map
    munmap(mp, length);

    return 0;
}
