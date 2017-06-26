/*************************************************************************
	> File Name: detach1.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月10日 星期六 16时51分08秒
 ************************************************************************/

#include"../header.h"

void * func(void * ptr)
{
    sleep(1);
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, func, NULL);

    //设置线程分离属性
    pthread_detach(tid);

    int ret = pthread_join(tid, NULL);
    if (ret == EINVAL)
        printf("ret = %d\n", ret);

    getchar();
    return 0;
}
