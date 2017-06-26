/*************************************************************************
	> File Name: Jason.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月21日 星期三 18时30分59秒
 ************************************************************************/
#include<inttypes.h>
#include"../header.h"
#include"cJSON.h"

/* 业务：练习Json语法
 * 嵌套json对象
 * gcc Jason.c cJSON.c -lm
 * */

struct person
{
    int age;
    char name[12];
    int  sexual;
};

int main()
{
    //初始化结构体对象
    struct person p;
    p.age = 19;
    strcpy(p.name, "xcgj");
    p.sexual = 1;

    //打包成Jason
    //创建JSON被嵌套对象
    cJSON * phone = cJSON_CreateObject();
    cJSON_AddStringToObject(phone, "band", "nokia");
    cJSON_AddNumberToObject(phone, "version", 3);
    //创建JSON对象
    cJSON * root = cJSON_CreateObject();
    //增加键值对
    cJSON_AddStringToObject(root, "name", p.name);
    cJSON_AddNumberToObject(root, "age", p.age);
    cJSON_AddBoolToObject(root, "sexual", p.sexual);
    cJSON_AddItemToObject(root, "mobiphone", phone);

    //root输出到jsonbuf，两个是不一样的
    char * jsonbuf = cJSON_Print(root);

    //printf("jsonbuf:\n%s\n", jsonbuf);
    //解析字符串
    {
        cJSON* root = cJSON_Parse(jsonbuf);
        cJSON * name = cJSON_GetObjectItem(root, "name");//抽取键值对
        printf("%s=%s\n", name->string, name->valuestring);
        cJSON * age = cJSON_GetObjectItem(root, "age");
        printf("%s=%d, %s=%g\n", age->string, age->valueint, age->string, age->valuedouble);

        cJSON * phone = cJSON_GetObjectItem(root, "mobiphone");
        cJSON * ver = cJSON_GetObjectItem(phone, "band");
        printf("%s=%s\n", ver->string, ver->valuestring);

        cJSON_Delete(root);
    }

    cJSON_Delete(root);//只需要删除根
    free(jsonbuf);
    return 0;
}
