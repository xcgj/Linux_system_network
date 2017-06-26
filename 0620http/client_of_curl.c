/*************************************************************************
	> File Name: dynamichtml_libcurl.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月20日 星期二 20时45分35秒
 ************************************************************************/

#include"../header.h"
#include<curl/curl.h>

/* 业务：
 * 使用curl实现客户端
    //1 创建curl对象
    //2 设置curl对象参数
    //3 通过curl对象向服务器发起请求
    //4 读服务器的响应数据
    //5 回调write_callback函数，将响应数据保存起来
 * */

struct Data
{
    char * buf;
    int size;//buf总长度
    int datasize;//有用的数据长度
};

//写的回调函数，如果数据没拷完，每次拷贝接着上次的数据继续拷贝
size_t write_callback(
                        char *ptr,           //需要被接收数据
                        size_t size,          //ptr单元大小
                        size_t nmemb,   //单元的数量
                        void *userdata) //需要手动设置
{
    struct Data * data = (struct Data *) userdata;
    size_t dSize = size * nmemb;
    memcpy(data->buf + data->datasize, ptr, dSize);
    data->datasize += dSize;
    return dSize;//返回值告诉curl对象这些数据已经被取走，无效了
}

int main()
{
    //1 创建curl对象
    //2 设置curl对象参数
    //3 通过curl对象向服务器发起请求
    //4 读服务器的响应数据

    //多线程时使用：
    //curl_global_init();//对应 curl_global_cleanup();

    //1
    CURL * curl = curl_easy_init();

    //2 请求URL地址
    CURLcode code = curl_easy_setopt(curl, CURLOPT_URL, "htttp://127.0.0.1/xcgj.html");
    if(code != CURLE_OK)
    {
        printf("errno\n");
        return 0;
    }

    //保存响应数据
    struct Data data;
    data.buf = malloc(8192);
    data.size = 8192;
    data.datasize = 0;
    code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);//设置write_callback的参数4为NULL

    code = curl_easy_perform(curl);//阻塞，收到服务器相应后才继续往下执行
    if(code != CURLE_OK)
    {
        printf("errno\n");
        return 0;
    }
                                            //这里会打包报文和解析报文，并且会回调函数write_callback
    //当响应数据被回调函数write_callback处理完毕，需要处理C风格字符串
    data.buf[data.datasize] = 0;
    printf("%s\n", data.buf);

    free(data.buf);
    curl_easy_cleanup(curl);
    return 0;
}
