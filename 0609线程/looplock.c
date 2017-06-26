/*************************************************************************
	> File Name: relock.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月09日 星期五 20时19分27秒
 ************************************************************************/

#include"../header.h"

pthread_mutex_t mutex;

//被调函数有锁，造成重复加锁、
void foo()
{
    pthread_mutex_lock(&mutex);
    printf("thread = %d\n", (int)pthread_self());
    pthread_mutex_unlock(&mutex);
}

void * func(void* ptr)
{
    //加锁
    pthread_mutex_lock(&mutex);
    //调函数
    foo();
    //解锁
    pthread_mutex_unlock(&mutex);
}

int main()
{
    //给锁增加属性，避免循环锁的问题
    pthread_mutexattr_t attr;//定义属性
    pthread_mutexattr_init(&attr);//初始化属性
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);//设置锁的属性为循环锁的属性
    
    //锁初始化
    pthread_mutex_init(&mutex, &attr);//添加锁属性

    //创建子线程
    pthread_t tid;
    pthread_create(&tid, NULL, func, NULL);

    //线程回收
    void* ret;
    pthread_join(tid, &ret);

    return 0;
}
