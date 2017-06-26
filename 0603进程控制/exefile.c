/*************************************************************************
	> File Name: exefile.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月03日 星期六 19时21分05秒
 ************************************************************************/

#include "../header.h"

int main()
{
    int fp = open("b.txt", O_WRONLY);

    int ret = execl("./wf.out", "./wf.out", NULL);
    printf("写入失败\n");

    return 0;
}
