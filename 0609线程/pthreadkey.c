/*************************************************************************
	> File Name: pthreadkey.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月10日 星期六 19时14分03秒
 ************************************************************************/

#include"../header.h"

//定义线程私有变量
pthread_key_t a;

void* foo()
{
    void * s = pthread_getspecific(a);
    printf("%s\n", (char *)s);
}

void* func(void * ptr)
{
    //子线程设置私有变量
    char * p = malloc(100);//内存申请格式要一致
    strcpy(p, "thread");
    pthread_setspecific(a, p);
    foo();
}

int main()
{
    //创建私有变量
    //如果是简单数据类型(如单个指针)，参数二填NULL，表示默认调用清理函数
    //如果setspecific用的是复杂数据类型（如结构题里面套指针）
    //那么需要自己写内存清理函数，参数二写函数名
    pthread_key_create(&a, NULL);

    pthread_t tid;
    pthread_create(&tid, NULL, func, NULL);

    //主线程设置使用私有变量
    char * p = malloc(100);
    strcpy(p, "main");
    pthread_setspecific(a, p);

    pthread_join(tid, NULL);

    foo();

    return 0;
}
