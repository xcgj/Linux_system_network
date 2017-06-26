/*************************************************************************
  > File Name: Logins.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月16日 星期五 14时22分17秒
 ************************************************************************/

#include"../network.h"
#include<list>
using namespace std;

//全局链表，文件描述符队列
list<int> socks;
//锁
pthread_mutex_t mutex;
pthread_cond_t cond;

//处理数据详细步骤
void recvData(int newfd)
{
    //接受数据
    // 长度r|name|pass

    //读取内容
    char * buf = myReadBuf(newfd);
    //printf("buf = %s\n", buf);
    //分离内容
    char* saveptr;
    const char* type = strtok_r(buf, "|", &saveptr);
    const char* user = strtok_r(NULL, "|", &saveptr);
    const char* pass = strtok_r(NULL, "|", &saveptr);

    //处理内容
    if(type[0] == 'r')
    {
        //注册, 写入用户名和密码各占一行
        FILE * filefd = fopen("user.data", "a+");
        fprintf(filefd, "%s\n%s\n", user, pass);
        fclose(filefd);
        //回应客户端注册结果 长度r|ok
        myWriteBuf(newfd, "r|ok");            
    }
    else if(type[0] == 'l')
    {
        //登录，遍历文件对比输入
        FILE * fp = fopen("user.data", "r");
        char ubuf[1024];
        char pbuf[1024];
        char * p;
        while(1)
        {
            p = fgets(ubuf, sizeof(ubuf), fp);
            if (p == NULL) break;
            fgets(pbuf, sizeof(pbuf), fp);

            ubuf[strlen(ubuf) - 1] = 0;//去掉\n
            pbuf[strlen(pbuf) - 1] = 0;

            if(strcmp(ubuf, user) == 0 && strcmp(pbuf, pass) == 0)
            {
                myWriteBuf(newfd, "l|ok");
                break;// p!=NULL
            }
        }

        if(p == NULL)//遍历整个文件，都没有匹配项
        {
            myWriteBuf(newfd, "l|err");
        }

        fclose(fp);
    }
    else
    {
        //非法数据
        myWriteBuf(newfd, "unknown cmd");
    }
    free(buf);
    close(newfd);

}

//线程函数取链表中的文件描述符处理数据
void * thread_func(void * ptr)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);//必须要和一把锁配合使用等待条件成立，
        //如果条件不成立，那么程序阻塞在这里，不占用CPU，
        //内部会自动解锁，wait成功后，再自动加锁
        pthread_mutex_unlock(&mutex);
        while(1)//可能在这个线程处理数据的过程中，链表增加了新的fd
        {
            pthread_mutex_lock(&mutex);
            if(socks.size() == 0)//将链表中存储的fd处理完，直到没有fd
            {
                pthread_mutex_unlock(&mutex);
                break;
            }
            int newfd = socks.front();  //读fd
            socks.pop_front();          //删fd
            pthread_mutex_unlock(&mutex);
            //处理fd的数据，因为后面来的fd都存在链表中，可以慢慢处理
            //让主线程可以安心接收新连接而不被打扰
            recvData(newfd);
        }
    }
}

int main()
{
    //服务器对象
    int server = myServer(9988, "0.0.0.0");
    //保存数据的文件
    close(open("user.data", O_CREAT|O_EXCL, 0777));

    //锁
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    //创建线程
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);

    //主线程安心收连接
    while(1)
    {
        int newfd = myAccept(server, NULL, NULL);
        if(newfd > 0)
        {
            //新fd加入链表
            pthread_mutex_lock(&mutex);
            socks.push_back(newfd);
            pthread_mutex_unlock(&mutex);
            //让另外的线程去处理数据
            //发信号--有连接过来了，让之前创建的线程解除等待状态，去处理这个fd
            pthread_cond_signal(&cond);
        }
    }

    close(server);
}
