/*************************************************************************
	> File Name: select.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月11日 星期日 17时07分26秒
 ************************************************************************/

#include"../header.h"

int main()
{
    int fdmic = open("/dev/input/mice", O_RDONLY);
    int fdkey = open("/dev/input/event1", O_RDONLY);

    while(1)
    {
        //初始化select参数，由于select每次调用可能会造成集合中文件描述符抹除
        //并且由于timeout参数每次都会返回改变timeout的值
        //因此每次都要初始化
        int nfds = fdmic>fdkey ? fdmic+1 : fdkey+1;

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(fdmic, &readfds);
        FD_SET(fdkey, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 5;//秒，返回剩余时间
        timeout.tv_usec = 0;//微秒

        //检查键盘和鼠标，哪个文件有数据
        int ret = select(nfds, &readfds, NULL, NULL, &timeout);

        //select运行结果
        if(ret == 0) continue; //超时
        if(ret < 0)//运行失败
        {
            if(ret == -1 && errno == EINTR)continue;//被中断打断
            else break;//严重错误
        }

        char buf[1024];
        if(FD_ISSET(fdkey, &readfds))//运行成功，键盘
        {
            read(fdkey, buf, sizeof(buf));
            printf("键盘\n");
        }
        if(FD_ISSET(fdmic, &readfds))//鼠标
        {
            read(fdmic, buf, sizeof(buf));
            printf("鼠标\n");
        }
    }

    close(fdmic);
    close(fdkey);

    return 0;
}
