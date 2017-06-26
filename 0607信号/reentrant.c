/*************************************************************************
	> File Name: reentrant.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月07日 星期三 18时59分56秒
 ************************************************************************/

#include"../header.h"

//链表结构体
struct list_node
{
    int v;
    struct list_node* next;
};

//全局链表
struct list_node* list;

void listadd(struct list_node *head, struct list_node *node)
{
    node->next = head;
    head = node;
}

void dealsig(int v)//不可重入函数
//访问修改全局变量
//内部定义静态变量
{
    struct list_node *node = malloc(sizeof(struct list_node));
    listadd(list, node);    
}

int main()
{
    signal(SIGINT, dealsig);

    while(1)
    {
        struct list_node *node = malloc(sizeof(struct list_node));
        listadd(list, node);
    }
}
