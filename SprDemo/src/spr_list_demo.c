#include "sparrow.h"

typedef struct point
{
	int x;
	int y;
	int z;
}point;

int point_eq(void * _left, void *_right)
{
	return ((point*)_left)->x == ((point*)_right)->x;
}

int point_gt(void *_left, void *_right)
{
	return ((point*)_left)->x > ((point*)_right)->x;
}

int point_cpy(void *_left, void *_right)
{
	((point*)_left)->x = ((point*)_right)->x;
	((point*)_left)->y = ((point*)_right)->y;
	((point*)_left)->z = ((point*)_right)->z;
	return 0;
}

point ps[20];

int spr_list_demo(void)
{
	SprList *list;
	SprListNode *node;
	unsigned int idx;
	unsigned int size;
	spr_lst_space_init(); // once is okay in all project
	// 初始化链表
	spr_lst_init(&list);
	spr_lst_set_eq(list, point_eq);
	spr_lst_set_gt(list, point_gt);
	spr_lst_set_cpy(list, point_cpy);
	// 插入10个节点
	size = 10;
	for (idx = 0; idx < size; idx++)
	{
		ps[idx].x = ps[idx].y = idx;
		spr_lst_ins(list, ps + idx);
	}
	node = list->head->next;
	// 追加10个节点
	for (idx = 0; idx < size; idx++)
	{
		ps[idx + size].x = ps[idx + size].y = idx;
		spr_lst_apd(list, ps + idx + size);
	}
	if (spr_lst_find(list, ps))
		spr_lst_rpl(list, ps + 2, ps + 7);
	spr_lst_del(list, ps + 4);
	spr_lst_clr(&list);
	spr_lst_free(&list);
	spr_lst_space_free(); // once is okay in all project
	return 0;
}
