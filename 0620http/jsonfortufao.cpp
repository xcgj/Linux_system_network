
#include "../header.h"
#include "cJSON.h"
#include <curl/curl.h>

/*用json和curl给服务器发送信息，方法是post*/
 //gcc jsonfortufao.c cJSON.c -lcurl -lm

int main()
{
    CURL *curl = curl_easy_init();

    //创建json
    char * jsonbuf;
    cJSON * root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "cmd", "register");
    cJSON_AddStringToObject(root, "uname", "xxx");
    cJSON_AddStringToObject(root, "passwd", "yyy");

    //写json
    jsonbuf = cJSON_Print(root);
    cJSON_Delete(root);

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:9988");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonbuf);

    //post发送
    curl_easy_perform(curl);

    free(jsonbuf);
    return 0;
}
