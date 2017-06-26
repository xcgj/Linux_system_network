/*************************************************************************
  > File Name: threadblock.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月11日 星期日 14时17分10秒
 ************************************************************************/

#include"../header.h"

void setNONBLOCK(int fd)
{
    int flag = fcntl(fd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);
    //fcntl根据fd的下标访问file table entry,修改文件属性
    //因此不用参数不用传指针，只需传递下标
}

void* key(void * ptr)
{
    int fd_key = open("/dev/input/event1", O_RDONLY);
    if (fd_key < 0)
    {
        perror("keyopen");
        return 0;
    }

    setNONBLOCK(fd_key);

    char buf[1024];
    while(1)
    {
        int ret = read(fd_key, buf, sizeof(buf));
        //文件描述符包含NONBLOCK属性时，read不会阻塞，读不到数据返回-1
        if(ret < 0)//resource temporarily unavailable
        /*{
            if(errno == EAGAIN)
            {
                //如果使用非阻塞的read去读取设备数据时，
                //如果设备没有数据，那么返回-1，错误码是EAGAIN
                printf("EAGAIN\n");//读不到数据
            }
            else
            {
                perror("read");//说明遇到严重错误
                break;
            }
        }*/
            continue;
        if(ret > 0)printf("键盘\n");
        //if(ret == 0)
    } 
}

void* mice(void * ptr)
{
    int fd_mice = open("/dev/input/mice", O_RDONLY);
    if (fd_mice < 0)
    {
        perror("miceopen");
        return 0;
    }

    setNONBLOCK(fd_mice);

    char buf[1024];
    while(1)
    {
        int ret = read(fd_mice, buf, sizeof(buf));
        if (ret < 0)
        /*{
            if (errno == EAGAIN)
            {
                printf("EAGAIN\n");
            }
            else
            {
                perror("keyope");
            }
        }*/
            continue;
        if (ret > 0)
        {
            printf("鼠标\n");
        }
    }
}

int main()
{
    pthread_t tkey;
    pthread_t tmice;
    pthread_create(&tkey, NULL, key, NULL);
    pthread_create(&tmice, NULL, mice, NULL);

    pthread_join(tkey, NULL);
    pthread_join(tmice, NULL);

    return 0;
}
