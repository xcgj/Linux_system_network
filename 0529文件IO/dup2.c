#include "../header.h"

int main()
{
    int fd = open("dup2.tmp", O_WRONLY|O_CREAT, 0666);

    //指定新的文件描述符的位置在1号位置
    //如果成功返回参数2的值，失败返回-1
    //如果在1号位置原来就有打开的文件
    //那么dup2会导致1号文件描述符所指的文件自动关闭，然后再拷贝
    int fd2 = dup2(fd, 1);

    printf("fd2 = %d\n", fd2);

    //close函数在程序运行结束时自动调用，可以不写
    return 0;
}
