/*************************************************************************
	> File Name: test.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年05月29日 星期一 16时48分21秒
 ************************************************************************/

#include "../header.h"

int main()
{
    char path[256];

    //指定随机文件产生的目录及产生的文件名模板
    //现在指定在当前目录产生
    strcpy(path, "./s_XXXXXX");
    //strcpy(path, "/tmp/daaa");

    //mktemp(path);//失败
    //mkstemps(path, 2);//失败
    mkstemp(path);
    

    printf("%s\n", path);

    return 0;
}
