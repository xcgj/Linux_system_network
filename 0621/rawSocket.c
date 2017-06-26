/*************************************************************************
	> File Name: rawSocket.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月22日 星期四 08时30分20秒
 ************************************************************************/

#include"../header.h"
//#include <netpacket/packet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/if_arp.h>


//处理tcp
void handle_tcp(unsigned char * tcp)
{
    printf("---------------tcp报文---------------\n");
    //打印4x5行，20字节的数据
    int i, j;
    for(i = 0; i < 5; ++i)
    {
        for(j = 0; j < 4; ++j)
        {
            unsigned char d = tcp[i*4 + j];//第i行第j个
            printf("%02x", d);
        }
        printf("\n");
    }
/* tcp报文分析
 *  ---------------tcp报文---------------
 *  9f1901bb    9f19：16位源端口号，本机计算机随机分配的临时端口  01bb：16位目标端口号，服务器
 *  36be12f5    tcp传输时滑动窗口的32位序号
 *  57f553d3    tcp传输时滑动窗口的32位确认序号
 *  5010ffff    5：4位首部长度，表示tcp报文长度是4字节＊5个 = 20字节  010-->0000 0001 0000：6位保留是0000 00；6位标识符01 0000，ACK位是1，其余标识符是0  ffff：16位窗口大小
 *  913e0000    913e：16位检验和    0000：16位紧急指针
 *  ---------------tcp报文---------------
 *  01bb9f19
 *  57f553d3
 *  36be12f5
 *  5018faf0
 *  9ece0000
 *  ---------------tcp报文---------------
 *  9f1901bb
 *  36be12f5
 *  57f56163
 *  5010ffff
 *  913e0000
 */  

    //tcp报文下面就是数据内容了
    //根据tcp报文的长度，偏移指针12字节求出数据的首地址
    //====先求出tcp报文的长度
    unsigned char ch = *(tcp + 12);//前4bit包含长度信息
    ch &= 0xf0; //后4bit置零
    ch >>= 4;   //右移4bit
    int tcplen = ch * 4;//tcp长度

    //取出tcp报文后面的数据
    char * data = (char*)(tcp + tcplen);
    printf("data is %s\n", data);
}

//处理udp
void handle_udp(unsigned char * udp)
{
    /*
    printf("---------------udp报文---------------\n");
    //打印4x5行，20字节的数据
    int i, j;
    for(i = 0; i < 5; ++i)
    {
        for(j = 0; j < 4; ++j)
        {
            unsigned char d = udp[i*4 + j];//第i行第j个
            printf("%02x", d);
        }
        printf("\n");
    }
    */
}

//根据handle_ip函数的解析，推导处理TCP/UDP的头
void handle_tcp_udp(unsigned char* ip)
{
    //printf("---------------tcp/udp报文---------------\n");
    //获取ip报文长度
    //ip报文分析中的第一行的第二个数据包含了ip报文的长度信息
    //可以根据这个ip报文长度求出tcp/udp报文的首地址
    unsigned char ch = ip[0];   //ip报文第一个字节，前4bit是版本，后4bit是ip报文
    int len = ch & 0x0f;        //按位与，将前4bit清零，获得后4bit的数据：0x45 & 0x0f == 5
    int iplen = len * 4;            //ip报文长度
    unsigned char* tcp = ip + iplen;// tcp/udp报文的首地址
    
    //判断报文协议类型--tcp／udp
    //第三行第2个字节----ip报文首地址偏移9个字节
    unsigned char protocal = *(ip + 9);
    if(protocal == 0x11)        //udp
    {
        handle_udp(ip+iplen);
    }
    else if(protocal == 0x06)   //tcp
    {
        handle_tcp(ip+iplen);
    }
}

//处理ip报文数据
void handle_ip(unsigned char* ip)
{
#if 0
    printf("---------------ip报文---------------\n");
    //打印4x5行，20字节的数据
    int i, j;
    for(i = 0; i < 5; ++i)
    {
        for(j = 0; j < 4; ++j)
        {
            unsigned char d = ip[i*4 + j];//第i行第j个
            printf("%02x", d);
        }
        printf("\n");
    }
#endif

/* ip报文分析
 * ------------------------------
 *  45000028    4：4位版本 5：4位(每位是4字节)首部长度有5个,一共20字节  00：8位服务类型(TOS)    0028：16位总长度(字节数), 网络序
 *  153a4000    153a：16位标识符(flags) 4--->0100:：前三位是标志    00：和前面0100的最后一个0组合在一起是13位片偏移
 *  400657c0    40：8位生存时间(TTL)    06：8位协议     57c0：16位首部检验和
 *  c0a85780    32位源ip地址：192.168.87.128
 *  5bbd59f0    32位目的ip地址：91.189.89.240
 *  ------------------------------
 *  45000028
 *  236d0000
 *  8006498d
 *  5bbd59f0
 *  c0a85780
 *  ------------------------------
 *  4500003c
 *  35494000
 *  40064ce1
 *  c0a85780
 *  d83ac82e
 *
 *
 */

    //打印tcp/udp报文
    handle_tcp_udp(ip);
}


//打印以太网帧头，6+6+2字节，目的地址+源地址+类型
void print_eth_header(unsigned char * eth_frame)
{
    //目的地网卡地址：00:00:00:00:00:00
    //源网卡地址：00:00:00:00:00:00
    //类型：0800 0806 8005
    unsigned char * p = eth_frame;
    printf("dst:%02x:%02x:%02x:%02x:%02x:%02x\n",
            p[0], p[1], p[2], p[3], p[4], p[5]);
    printf("src:%02x:%02x:%02x:%02x:%02x:%02x\n",
            p[6], p[7], p[8], p[9], p[10], p[11]);
    printf("type:%04x\n", ntohs(*(unsigned short*)(p+12)));
}

int main()
{
    //SOCK_RAW:包含以太网帧的头和尾
    //SOCK_DGRAM:只包含数据
    //ETH_P_ALL:设置网卡抓包，网络序
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0)
    {
        printf("0");
        close(sock);
        return 0;
    }
    
    //设置原始套接字属性
    struct ifreq ifstruct;
    strcpy(ifstruct.ifr_ifrn.ifrn_name, "eth0");//添加网卡
    if (ioctl(sock, SIOCGIFINDEX, &ifstruct) == -1)
    {
        printf("1");
        close(sock);
        return 0;
    }

    //设置地址属性
    struct sockaddr_ll addr;//low level
    addr.sll_family = AF_PACKET;
    addr.sll_ifindex = ifstruct.ifr_ifindex;
    addr.sll_protocol = htons(ETH_P_ALL);
    addr.sll_halen = ARPHRD_ETHER;  //以太网// ha == hardware硬件
    addr.sll_pkttype = PACKET_OTHERHOST;//抓包 外面进来的
    addr.sll_halen = ETH_ALEN;      //mac地址长度：6 == 硬件地址长度
    addr.sll_addr[6] = 0;           //mac地址6个字节，将8字节的剩下2个字节置零
    addr.sll_addr[7] = 0;

    //原始套接字获取mac地址
    if(ioctl(sock, SIOCGIFHWADDR, &ifstruct) == -1)
    {
        printf("2\n");
        close(sock);
        return 0;
    }

    //设置混杂模式，数据从网卡进入，出网卡时有多条分支
    //抓包必须
    //SIOCGIFFLAGS, SIOCSIFFLAGS读取 或 设置 设备的 活动标志字
    if(ioctl(sock, SIOCGIFFLAGS, &ifstruct) == -1)
    {
        printf("3\n");
        close(sock);
        return 0;
    }

    ifstruct.ifr_ifru.ifru_flags |= IFF_PROMISC;
    //SIOCGIFFLAGS, SIOCSIFFLAGS读取 或 设置 设备的 活动标志字
    if(ioctl(sock, SIOCSIFFLAGS, &ifstruct) == -1)
    {
        printf("4\n");
        close(sock);
        return 0;
    }

    //绑定地址
    if(bind(sock,(struct sockaddr*)&addr, sizeof(struct sockaddr_ll)) == -1)
    {
        printf("5\n");
        close(sock);
        return 0;
    }

    ioctl(sock, SIOCGIFHWADDR, &ifstruct);

    char buf[1518];//以太网帧最大长度
    while(1)
    {
        //读取抓包抓到的以太网帧数据
        read(sock, buf, sizeof(buf));
        //printf("recv data\n");
        
        //打印以太网帧头，6+6+2，目的地址+源地址+类型
        //print_eth_header((unsigned char*)buf);

        //跳过以太网帧数据，打印ip报文
        unsigned short eth_type = *(unsigned short*)(buf+12);
        eth_type = ntohs(eth_type);     //类型
        if(eth_type == 0x800)           //IP packet
        {
            //处理ip报文
            handle_ip((unsigned char*)buf+14);
        }
    }
    close(sock);
    return 0;
}
