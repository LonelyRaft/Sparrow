/*
Copyright(c) 2021 LonelyRaft. All rights reserved.
*/

#include <stdlib.h>
#include "spr_list.h"

#define SIZE_SPACE_LIST_DEFAULT 16

typedef struct SprListSpace
{
	SprListNode *buf;			// �ڵ㻺����
	unsigned int size;			// ��������С���������ɽڵ����
	unsigned int lidx;			// ������
	unsigned int ridx;			// ������
	unsigned char full;		// ����������
	unsigned char empty;	// �������ѿ�
	unsigned char inited;	// �Ƿ񱻳�ʼ����
}SprListSpace;

SprListSpace node_space;

int spr_lst_space_init(void)
{
	node_space.buf = (SprListNode*)malloc(
		sizeof(SprListNode) * SIZE_SPACE_LIST_DEFAULT);
	node_space.size = SIZE_SPACE_LIST_DEFAULT;
	node_space.ridx = 0;
	node_space.lidx = 0;
	node_space.full = 0;
	node_space.empty = 1;
	node_space.inited = 1;
	return 0;
}

int spr_lst_space_ext(void)
{
	SprListNode *space;
	void *tmp;
	unsigned int idx;
	unsigned int prev_idx;
	unsigned int next_idx;
	if (!node_space.inited)
		return -1; // ���󣬿ռ��δ����ʼ��
	if (!node_space.full)
		return -1; // ���󣬿ռ��δ��
	// �����еĿռ���������
	space = (SprListNode*)malloc(
		sizeof(SprListNode) * node_space.size * 2);
	if (0 == space)
		return -1; // �����ڴ�ռ����ʧ��
	// ���ɿռ��е����ݸ��ƹ������ڵ�ָ��Ҫ���¼���
	for (idx = 0; idx < node_space.size; idx++)
	{
		// ���ƽڵ�����
		(space + idx)->data = (node_space.buf + idx)->data;
		// ���ƽڵ� prev ָ��
		if ((node_space.buf + idx)->prev->prev)
		{
			prev_idx = (node_space.buf + idx)->prev - node_space.buf;
			(space + idx)->prev = space + prev_idx; // ���㸴�ƺ�Ľڵ�� prev
		}
		else
		{
			tmp = (node_space.buf + idx)->prev; // �ƽڵ�
			(space + idx)->prev = tmp;
			// ���������ƽڵ�ָ��
			((SprListNode*)tmp)->next = (space + idx);
			tmp = (char*)tmp - sizeof(SprList); // �ƽڵ��Ӧ������
			// ���¸������β�ڵ�ָ��
			((SprList*)tmp)->tail = space + (((SprList*)tmp)->tail - node_space.buf);
		}
		// ���ƽڵ� next ָ��
		if ((node_space.buf + idx)->next)
		{
			next_idx = (node_space.buf + idx)->next - node_space.buf;
			(space + idx)->next = space + next_idx; // ���㸴�ƺ�Ľڵ�� next
		}
		else
			(space + idx)->next = 0; // β�ڵ�
	}
	// �ͷžɿռ�
	free(node_space.buf);
	// �����������
	node_space.buf = space;
	node_space.ridx = node_space.size;
	node_space.size += node_space.size;
	node_space.full = 0;
	return 0;
}

SprListNode* spr_lst_node_alloc(void)
{
	SprListNode *node;
	if (!node_space.inited)
		return 0; // ���󣬿ռ��δ��ʼ��
	if (node_space.full)
		spr_lst_space_ext(); // �ռ䲻�㣬�Կռ�������չ
	node = node_space.buf + node_space.ridx;
	node_space.ridx++;
	if (node_space.ridx == node_space.size)
		node_space.ridx = 0;
	if (node_space.lidx == node_space.ridx)
		node_space.full = 1;
	node_space.empty = 0;
	return node;
}

int spr_lst_node_free(SprListNode *_node)
{
	SprListNode *tmp;
	if (!node_space.inited)
		return -1; // ���󣬿ռ��δ��ʼ��
	if (node_space.empty)
		return 0;
	// ��λ�����һ���ڵ�
	tmp = node_space.buf + node_space.lidx;
	node_space.lidx = (node_space.lidx + 1) % node_space.size;
	if (node_space.lidx == node_space.ridx)
		node_space.empty = 1;
	// �����һ���ڵ��뱻ɾ���Ľڵ㽻��λ��
	*(_node) = *(tmp);
	tmp->prev->next = _node;
	node_space.full = 0;
	return 0;
}

int spr_lst_space_free(void)
{
	free(node_space.buf);
	node_space.buf = 0;
	node_space.size = 0;
	node_space.ridx = 0;
	node_space.lidx = 0;
	node_space.full = 0;
	node_space.empty = 0;
	node_space.inited = 0;
	return 0;
}

int spr_lst_init(SprList** _list)
{
	void *tmp;
	if (0 == _list)
		return -1; // ���󣬷��ʿ�ָ��
	tmp = malloc(sizeof(SprList) + sizeof(SprListNode));
	(*_list) = (SprList*)tmp;
	tmp = (char*)tmp + sizeof(SprList);
	(*_list)->head = (SprListNode*)tmp;
	(*_list)->tail = (*_list)->head;
	(*_list)->size = 0;
	(*_list)->head->prev = 0;
	(*_list)->head->next = 0;
	(*_list)->head->data = 0;
	return 0;
}

void spr_lst_set_eq(SprList* _list, int(*_data_eq)(void*, void*))
{
	_list->data_eq = _data_eq;
}

void spr_lst_set_gt(SprList* _list, int(*_data_gt)(void*, void*))
{
	_list->data_gt = _data_gt;
}

void spr_lst_set_cpy(SprList* _list, int(*_data_cpy)(void*, void*))
{
	_list->data_cpy = _data_cpy;
}

int spr_lst_ins(SprList* _list, void *_data)
{
	SprListNode *node;
	SprListNode *tmp;
	node = spr_lst_node_alloc();
	node->prev = _list->head;
	node->data = _data;
	if (0 == _list->head->next)
	{
		_list->head->next = node;
		_list->tail = node;
		node->next = 0;
	}
	else
	{
		tmp = _list->head->next;
		tmp->prev = node;
		_list->head->next = node;
		node->next = tmp;
	}
	_list->size++;
	return 0;
}

int spr_lst_apd(SprList* _list, void *_data)
{
	SprListNode *node;
	node = spr_lst_node_alloc();
	node->prev = _list->tail;
	node->next = 0;
	node->data = _data;
	_list->tail->next = node;
	_list->tail = node;
	_list->size++;
	return 0;
}

int spr_lst_find(SprList* _list, void *_data)
{
	SprListNode *head;
	unsigned int cnt;
	if (0 == _list->data_eq)
		return -1; // ����δ������Ƚӿ�
	cnt = 0;
	head = _list->head->next;
	while (0 != head)
	{
		if (_list->data_eq(head->data, _data))
			break;
		head = head->next;
		cnt++;
	}
	return cnt < _list->size;
}

int spr_lst_rpl(SprList* _list, void *_old, void *_new)
{
	SprListNode *head;
	if (0 == _list->data_eq)
		return -1; // ����δ������Ƚӿ�
	head = _list->head->next;
	while (0 != head)
	{
		if (_list->data_eq(head->data, _old))
			break;
		head = head->next;
	}
	if (0 == _list->data_cpy)
		head->data = _new;
	else
		_list->data_cpy(head->data, _new);
	return 0;
}

void* spr_lst_del(SprList* _list, void *_data)
{
	SprListNode *head;
	SprListNode *tmp;
	void *data;
	if (0 == _list->data_eq) 
		return 0;
	head = _list->head;
	while (0 != head->next)
	{
		if (_list->data_eq(head->next->data, _data))
			break;
		head = head->next;
	}
	tmp = head->next;
	head->next = head->next->next;
	if (0 != head->next)
		head->next->prev = head;
	else
		_list->tail = head;
	data = tmp->data;
	spr_lst_node_free(tmp);
	_list->size--;
	return data;
}

int spr_lst_clr(SprList** _list)
{
	SprListNode *head;
	SprListNode *tmp;
	head = (*_list)->head;
	while (0 != head->next)
	{
		tmp = head->next;
		head->next = head->next->next;
		if (0 != head->next)
			head->next->prev = head;
		spr_lst_node_free(tmp);
	}
	(*_list)->size = 0;
	(*_list)->tail = (*_list)->head;
	(*_list)->head->next = 0;
	return 0;
}

int spr_lst_free(SprList** _list)
{
	SprListNode *head;
	SprListNode *tmp;
	head = (*_list)->head->next;
	while (0 != head)
	{
		tmp = head;
		head = head->next;
		spr_lst_node_free(tmp);
	}
	free(*_list);
	*_list = 0;
	return 0;
}



