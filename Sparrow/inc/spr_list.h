/*
Copyright(c) 2021 LonelyRaft. All rights reserved.
*/

#ifndef SPARROW_LIST_H
#define SPARROW_LIST_H

/*
����ڵ�ṹ����
*/
typedef struct SprListNode
{
	struct SprListNode *next;
	struct SprListNode *prev;
	void *data;
}SprListNode;

/*
����ṹ����
*/
typedef struct SprList
{
	SprListNode* head;
	SprListNode* tail;
	unsigned int size; // ����ڵ�����
	int(*data_eq)(void *, void *);
	int(*data_gt)(void *, void *);
	int(*data_cpy)(void*, void*);
}SprList;

int spr_lst_space_init(void);
int spr_lst_space_free(void);

// ��ʼ��һ������
int spr_lst_init(SprList **);
// �������ݱȽϴ��ڽӿ�
void spr_lst_set_gt(SprList* _list, int(*_data_gt)(void*, void*));
// �������ݱȽϵ��ڽӿ�
void spr_lst_set_eq(SprList* _list, int(*_data_eq)(void*, void*));
// �������ݿ����ӿڣ������������ѡ
void spr_lst_set_cpy(SprList* _list, int(*_data_cpy)(void*, void*));
// �������в���һ���ڵ�
int spr_lst_ins(SprList*, void*);
// ��������׷��һ���ڵ�
int spr_lst_apd(SprList*, void*);	
// �������в���һ���ڵ�
int spr_lst_find(SprList*, void*);	
// �������и���һ���ڵ�
int spr_lst_rpl(SprList*, void*, void*);
// ��������ɾ��һ���ڵ�
void* spr_lst_del(SprList*, void*);	
// �������
int spr_lst_clr(SprList**);	
// please free all the data before you free the list
int spr_lst_free(SprList**); 

#endif // !SPARROW_LIST_H


