/*************************************************************************
	> File Name: myprintf.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月03日 星期六 22时30分41秒
 ************************************************************************/

#include<stdio.h>
#include<stdarg.h>

//int printf(const char *format, ...);
void myprint(const char* filename, int line, const char* format, ...)
{
    //把format和不定参数放到缓存里面
    char buf[4096];

    va_list ap;
    va_start(ap, format);
    //格式化输入缓存
    //int vsprintf(char *str, const char *format, va_list ap);
    vsprintf(buf, format, ap);
    va_end(ap);

    printf("[%s:%d]:%s", filename, line, buf);
}

//不定参数宏
#define print(str, ...) myprint(__FILE__, __LINE__, str, ##__VA_ARGS__)

int main()
{
    int a = 1, b = 2;
    print("信息%d, %d\n", a, b);
    return 0;
}
