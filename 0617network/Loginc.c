/*************************************************************************
	> File Name: Loginc.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月16日 星期五 15时27分37秒
 ************************************************************************/

#include"../network.h"
//短连接：装一次逼就跑--网页浏览，每次都需要重新建立连接
//长连接：服务器可以和客户端通信
int main()
{
    char cmd[16];
    char user[32];
    char pass[32];
    while(1)
    {
        //客户端socket
        int fd = myConnect2(9988, "127.0.0.1");
        printf("Login or Register: ");
    
        //用户输入
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd)-1] = 0;
        if(strcmp(cmd, "r") == 0 || strcmp(cmd, "l") == 0)
        {
            printf("user name: ");
            fgets(user, sizeof(user), stdin);
            user[strlen(user)-1] = 0;

            printf("passwords: ");
            fgets(pass, sizeof(pass), stdin);
            pass[strlen(pass)-1] = 0;

            char packet[2048];
            sprintf(packet, "%s|%s|%s", cmd, user, pass);
            printf("%s\n", packet);

            myWriteBuf(fd, packet);//写入数据

            char * ret = myReadBuf(fd);//读 写入结果
            printf("r|l is: %s\n", ret);
            free(ret);
        }
        else continue;
        close(fd);
    }
}
