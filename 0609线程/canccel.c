/*************************************************************************
	> File Name: canccel.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月10日 星期六 19时48分27秒
 ************************************************************************/

#include"../header.h"

void* func(void * ptr)
{
    //设置cancel命令无效
    //只对当前线程有效
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    while(1)
    {
        sleep(2);
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, func, NULL);

    //调用异常退出函数干掉线程
    pthread_cancel(tid);

    void * ret;
    pthread_join(tid, &ret);//可以join，但是ret的值是无效的，线程异常退出没有返回值

    printf("done\n");

    return 0;
}
