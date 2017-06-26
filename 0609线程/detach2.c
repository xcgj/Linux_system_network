/*************************************************************************
	> File Name: detach2.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月10日 星期六 18时07分40秒
 ************************************************************************/

#include"../header.h"

void * func(void * ptr)
{
    sleep(1);
}

int main()
{
    //attr属性增加detach
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_t tid;
    pthread_create(&tid, &attr, func, NULL);

    int ret = pthread_join(tid, NULL);
    if (ret == EINVAL)
    {
        printf("EINVAL\n");
    }
    return 0;
}
