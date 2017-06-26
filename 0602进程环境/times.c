/*************************************************************************
	> File Name: times.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月02日 星期五 20时43分15秒
 ************************************************************************/

#include <sys/times.h>
#include "../header.h"

int main()
{
    //第一次调用
    struct tms buf1;
    clock_t a = times(&buf1);

    int i = 0, sum = 0;
    for (; i < 50000; ++i)
    {
        printf("%d\n", sum += i);
    }

    //第二次调用
    struct tms buf2;
    clock_t b = times(&buf2);

    printf("user cost %d, sys cost %d\n", 
            (int)(buf2.tms_utime - buf1.tms_utime),
            (int)(buf2.tms_stime - buf1.tms_stime));
    printf("b - a = %d\n", (int)(b - a));
    return 0;
}
