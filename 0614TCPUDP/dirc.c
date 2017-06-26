/*************************************************************************
	> File Name: dirc.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月15日 星期四 14时32分37秒
 ************************************************************************/

#include"../network.h"
//传文件
int sendFile(const char * dst, const char * src, int fd)
{
    //从客户端传文件到服务器
    //先传文件标识
    myWrite(fd, "r", 1);
    //再传文件名大小
    myWriteLen(fd, strlen(dst));
    //再传文件名
    myWrite(fd, dst, strlen(dst));
    //再传内容大小
    myWriteLen(fd, myFileSize(src));
    //最后传内容
    int filefd = open(src, O_RDONLY);
    char buf[4096];
    while(1)
    {
        int ret = read(filefd, buf, sizeof(buf));//普通读取
        if(ret == 0)
        {
            break;//读完了
        }
        myWrite(fd, buf, ret);
    }
    close(filefd);
    return 0;
}

//传目录
int sendDir(const char * dst, const char * src, int fd)
{
    //从客户端发送目录给服务器
    //先传目录标识
    myWrite(fd, "d", 1);
    //传目录名大小
    myWriteLen(fd, strlen(dst));
    //传目录名
    myWrite(fd, dst, strlen(dst));

    //客户端遍历目录项
    char * bufDst = malloc(1024);//存储原目录路径
    char * bufSrc = malloc(1024);//存储目标路径
    DIR * dir = opendir(src);
    struct dirent * entry = NULL;
    while(1)
    {
        //打开目录项
        entry = readdir(dir);
        if(entry == NULL)
            break;

        //读取目录项
        if(entry->d_name[0] == '.')
            continue;//忽略 . .. 隐藏文件

        sprintf(bufDst, "%s/%s", dst, entry->d_name);
        sprintf(bufSrc, "%s/%s", src, entry->d_name);
        if(entry->d_type == DT_REG)
        {
            //文件
            sendFile(bufDst, bufSrc, fd);
        }
        else if(entry->d_type == DT_DIR)
        {
            //目录-->递归调用
            sendDir(bufDst, bufSrc, fd);
        }
    }

    closedir(dir);
    free(bufDst);
    free(bufSrc);
    return 0;
}

//客户端
int main(int argc, char **argv)
{
    const char * src = argv[1];
    const char * dst = argv[2];
    const char * ip = argv[3];
    const char * port = argv[4];

    //连接服务器
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    int ret = myConnect(fd, atoi(port), ip);
    if(ret < 0)
    {
        return -1;
    }

    //传数据
    sendDir(dst, src, fd);
    myWrite(fd, "x", 1);//结束标记

    close(fd);
    return 0;
}
