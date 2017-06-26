/*************************************************************************
  > File Name: Logins.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月16日 星期五 14时22分17秒
 ************************************************************************/

#include"../network.h"

//定义原始监听集合，每次select都用这个集合初始化
fd_set saveSet;
int maxfd = 0;

//处理对接客户端的socket接收到的数据
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
    //关闭文件描述符需要将文件描述符从原始集合中清除
    FD_CLR(newfd, &saveSet);
    //如果这个描述符刚好是最大的那一个，需要更新maxfd
    if(maxfd == newfd)
        maxfd--;
}



int main()
{
    //服务器对象
    int server = myServer(9988, "0.0.0.0");
    //保存数据的文件
    close(open("user.data", O_CREAT|O_EXCL, 0777));

    //select
    maxfd = server;    //最大可用文件描述符
    FD_ZERO(&saveSet);      //初始化原始监听集合
    FD_SET(server, &saveSet);
    while(1)
    {
        fd_set set;         //可读集合
        struct timeval tv;  //等待时间

        //初始化set tv
        memcpy(&set, &saveSet, sizeof(set));

        tv.tv_sec = 5;
        tv.tv_usec = 0;     //这个微秒的作用:usec和sec结合使用，等待值是他们的和

        //返回值是有动静的描述符的个数
        int ret = select(maxfd+1, &set, NULL, NULL, &tv);
        if(ret > 0)
        {
            //判断是否有新的客户端发连接请求过来 server
            if(FD_ISSET(server, &set))
            {
                //创建新的连接socket
                int newfd = myAccept(server, NULL, NULL);
                //将这个newfd也加入原始监听集合
                FD_SET(newfd, &saveSet);
                if(maxfd < newfd) 
                    maxfd = newfd;  //更新最大可用描述符 
            }
            //判断与客户端通信的socket有没有动静
            int i;
            for(i = server+1; i <= maxfd; ++i)
            {
                if(FD_ISSET(i, &set))
                {
                    //处理数据
                    recvData(i);
                }
            }
        }
        else if(ret == 0)
        {
            //集合没有动静
        }
        else if(ret < 0 && errno == EINTR)
        {
            //被信号打断
        }
        else if(ret <0)
        {
            //严重错误
            printf("errno = %d\n", errno);
            break;
        }
    }
    close(server);
    return 0;
}
