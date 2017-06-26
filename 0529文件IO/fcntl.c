#include "../header.h"

int main()
{
    int fd = open("a.temp", O_WRONLY);
    int fd2 = open("a.temp", O_WRONLY);
    int fd3 = dup(fd2);

    //给fd2增加O_APPEND属性
    int flag;
    //先和旧的属性叠加，避免覆盖
    flag = fcntl(fd2, F_GETFL);
    printf("flag is %d\n", flag);
    printf("wronly = %d \n", O_WRONLY);
    flag |= O_APPEND;//增加属性
    printf("or append flag=%d \n", flag);
    printf("append=%d \n", O_APPEND);
    printf("wr or append=%d\n", O_WRONLY | O_APPEND);
    //再设置属性
    fcntl(fd2, F_SETFL, flag);

    //验证扩展了O_APPEND属性
    write(fd, "hello", 5);
    write(fd2, "c01", 3);//如果没有扩展，会覆盖

    //验证fd2属性改变是否影响fd3
    write(fd3, "xcgj", 4);
    //验证方式2
    {
        int flag;
        flag = fcntl(fd3, F_GETFL);
        if (flag & O_APPEND)//如果flag没有append属性，与出来是0
        {
            printf("has append\n");
        }
    }
    return 0;
}
