/*************************************************************************
  > File Name: Logins.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月16日 星期五 14时22分17秒
 ************************************************************************/

#include"../network.h"

void dealwait(int v)
{
    if(v == SIGCHLD)
    {
        while(1)
        {
            //进程挂掉，返回进程pid
            //进程没有挂掉，父进程不会阻塞，直接返回0
            //出现错误，返回-1
            pid_t pid = waitpid(-1, NULL, WNOHANG);
            if(pid <= 0)
            {
                break;
            }
        }
    }
}

int main()
{
    //多进程收尸
    signal(SIGCHLD, dealwait);

    //服务器对象
    int server = myServer(9988, "0.0.0.0");
    //保存数据的文件
    close(open("user.data", O_CREAT|O_EXCL, 0777));
    int newfd;

    while(1)
    {
        newfd = myAccept(server, NULL, NULL);
        if(newfd == -1)
        {
            close(server);
            return -1;
        }

        //新连接建立后就创建子进程
        pid_t pid = fork();
        if (pid == 0)
        {
            close(server);
            break;
        }
        close(newfd);//必须关闭父进程newfd，解除占用
    }

    //子进程
    //接受数据
    // 长度r|name|pass

    //读取内容
    char * buf = myReadBuf(newfd);
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

    return 0;
}
