/*************************************************************************
	> File Name: varargs.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月03日 星期六 22时11分15秒
 ************************************************************************/

#include<stdio.h>
#include<stdarg.h>

int add(int count, int a, int b, ...)
    //count 表示不定参数的个数
{
    //表示后面的不定参数，是一个指针
    va_list ap;

    //ap指向b的后面的参数
    va_start(ap, b);

    int sum = a + b; 
    int i = 0;
    for(; i < count; ++i)
    {
        //取出数组中的元素
        //指定了参数类型是int
        //每次循环，ap指针走一步
        int arg = va_arg(ap, int);
        sum += arg;
    }

    //清理重置ap
    va_end(ap);

    return sum;
}


int main()
{
    int a = add(2,1,2,3,4);
    printf("%d\n", a);

    return 0;
}
