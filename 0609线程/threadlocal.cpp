/*************************************************************************
	> File Name: threadlocal.cpp
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月10日 星期六 19时07分47秒
 ************************************************************************/

#include"../header.h"
#include<iostream>
using namespace std;

thread_local int a = 1000;

void* func(void * ptr)
{
    a = 100;
    printf("thread a = %d\n", a);
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, func, NULL);

    pthread_join(tid, NULL);
    printf("main a = %d\n", a);
    return 0;
}
