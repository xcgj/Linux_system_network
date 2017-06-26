
/*多线程调用accept函数提高运行效率*/
#include"../header.h"
//程序问题：accept没有进入阻塞状态
int server;
//子进程处理函数
void* thread_fun(void* ptr)
{
    //int server = *(int*)ptr;
    printf("server = %d\n", server);
    while(1)//让5个线程不退出
    {
        int newfd = accept(server, NULL, NULL);//5个线程抢任务
        printf("%d\n", newfd);
        close(newfd);
    }
}

int main()
{
    server = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");

    int ret = bind(server, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0) 
    {
        perror("bind");
        return -1;
    }

    listen(server, 200);
    printf("等待连接: server = %d\n", server);

#if 1
    int threadNumber = 5;
    int i;
    for(i=0; i<threadNumber; ++i)
    {
        pthread_t tid;
        printf("tid = %d\n", (int)tid);
        pthread_create(&tid, NULL, thread_fun, NULL);
        pthread_detach(tid);//不阻塞，会死循环
    }
#endif

#if 0
    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1, NULL, thread_fun, NULL);
    pthread_create(&tid2, NULL, thread_fun, NULL);
    pthread_create(&tid3, NULL, thread_fun, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
#endif

    close(server);
    sleep(1000);
    return 0;
}
