/*************************************************************************
	> File Name: strtokr.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月07日 星期三 19时36分22秒
 ************************************************************************/

#include"../header.h"

#if 0
//strtok伪代码
char * mystrtok(char * str, char * delim)
{
    static char * saveptr;
    if (str)
    {
        saveptr = str;
    }

    return split(saveptr, char * delim);
}
#endif

#if 0
//strtok_r伪代码
char * mystrtok_r(char * str, char * delim, char ** saveptr)
{
    if (str)
    {
        *saveptr = str;
    }

    return split(saveptr, delim);
}
#endif

int main()
{
    char buf[] = "aaa bbb ccc";
    char * saveptr;
    //拆分字符串
}
