/*************************************************************************
  > File Name: canccel.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月10日 星期六 19时48分27秒
 ************************************************************************/

#include"../header.h"

void* func1(void * ptr)
{
    int i = 0;
    while(1)
    {
        //设置cancel命令无效
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        i++;//sleep(1);
        //恢复cancel命令的效果
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

        //检查cancel状态，如果检测到外部对本线程调用了cencel命令，则本线程退出
        pthread_testcancel();
    }
    //即使设置回复了cancel，但是程序还是无法退出
    //原因在于没有遇到退出点函数
}

void* func2(void * ptr)
{
    while(1);
    //没有退出点，线程只是接收到信号该退出了，但是却无法退出
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, func2, NULL);

    //调用异常退出函数干掉线程
    pthread_cancel(tid);

    void * ret;
    pthread_join(tid, &ret);

    printf("done\n");

    return 0;
}
