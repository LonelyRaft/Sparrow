/*
Copyright(c) 2021 LonelyRaft. All rights reserved.
*/

#ifndef SPARROW_LIST_H
#define SPARROW_LIST_H

/*
链表节点结构定义
*/
typedef struct SprListNode
{
	struct SprListNode *next;
	struct SprListNode *prev;
	void *data;
}SprListNode;

/*
链表结构定义
*/
typedef struct SprList
{
	SprListNode* head;
	SprListNode* tail;
	unsigned int size; // 链表节点数量
	int(*data_eq)(void *, void *);
	int(*data_gt)(void *, void *);
	int(*data_cpy)(void*, void*);
}SprList;

int spr_lst_space_init(void);
int spr_lst_space_free(void);

// 初始化一个链表
int spr_lst_init(SprList **);
// 设置数据比较大于接口
void spr_lst_set_gt(SprList* _list, int(*_data_gt)(void*, void*));
// 设置数据比较等于接口
void spr_lst_set_eq(SprList* _list, int(*_data_eq)(void*, void*));
// 设置数据拷贝接口，用于深拷贝，可选
void spr_lst_set_cpy(SprList* _list, int(*_data_cpy)(void*, void*));
// 向链表中插入一个节点
int spr_lst_ins(SprList*, void*);
// 向链表中追加一个节点
int spr_lst_apd(SprList*, void*);	
// 在链表中查找一个节点
int spr_lst_find(SprList*, void*);	
// 在链表中更新一个节点
int spr_lst_rpl(SprList*, void*, void*);
// 在链表中删除一个节点
void* spr_lst_del(SprList*, void*);	
// 清空链表
int spr_lst_clr(SprList**);	
// please free all the data before you free the list
int spr_lst_free(SprList**); 

#endif // !SPARROW_LIST_H


