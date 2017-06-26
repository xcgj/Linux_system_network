/*************************************************************************
  > File Name: Logins.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月16日 星期五 14时22分17秒
 ************************************************************************/

#include"../network.h"

void * threadfunc(void * ptr)
{
    int newfd = (int)(intptr_t)ptr;//intptr_t:根据系统将指针转成4字节或8字节
    //printf("1\n");
    //getchar();
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
        //多线程非APPEND读写需要增加文件锁
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

int main()
{
    //服务器对象
    int server = myServer(9988, "0.0.0.0");
    //保存数据的文件
    close(open("user.data", O_CREAT|O_EXCL, 0777));

    while(1)
    {
        int newfd = myAccept(server, NULL, NULL);
        if(newfd == -1)
        {
            break;
        }

        //创建线程
        pthread_t tid;
        pthread_create(&tid, NULL, threadfunc, (void*)(intptr_t)newfd);
        //(void*)newfd:将newfd的值放在void*这个变量里面？？？
        pthread_detach(tid);
    }
    close(server);
}
