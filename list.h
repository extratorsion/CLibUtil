/*
	������ļ��� ��ֻ��Ҫ �����������ݣ�
	#define TYPE <Node �� data �� ������>
	#define $(Expr) Expr##<�̶�����/�������������������>
	��� �㲻�޸ģ� ��ôĬ�ϣ�
		TYPE ������Ϊ int, $(Expr) ������Ϊ Expr
		Ҳ����˵ �� ��õ�:
				typedef struct Node {
					int data;
					struct Node *pre;
					struct Node *next;
				} Node;
				���� �� ���� ������ ListInit, ListDestroy..... ֮��


	�� -> ����㶨���ˣ�
		#define TYPE char
		#define $(Expr) Expr##_char
	��ô:
		struct $(Node) {
			TYPE data;
			struct $(Node) *pre;
			struct $(Node) *next;
		} $(Node);
		�ᱻ�滻Ϊ:
			struct Node_char {
				char data;
				struct Node_char *pre;
				struct Node_char *next;
			} Node_char;
		inline $(Node) *$(GetNode)() {
			...
		}
		�ᱻ�滻Ϊ:
		inline Node_char *GetNode_char() {
			...
		}
		�Դ�����

	���� ʹ�÷��� �Ĳ���
		��� ������ һ��.c �ļ��� ͬʱ ʹ�� ���� �� ���ϵ� List �� Node ���ͣ��� ����� һ��ͷ�ļ������Ǹ�ͷ�ļ�list.h
		���� ����д
		#define TYPE int
		#define $(Expr) Expr##Int
		#include "list.h"

		#define TYPE double
		#define $(Expr) Expr##Double
		#include "list.h"

		....

		���� �� ��ͬʱ ӵ��
			ListInt, NodeInt, ListInitInt, ListGetInt.....
			��
			ListDouble, NodeDouble ListInitDouble, ListGetDouble.....




	���� ��������� ֧�� ���ַ�ʽ��
	���� �� ����
		����
			0 -> ��1���ڵ�
			1 -> ��2���ڵ�
			...
			n -> ��n+1���ڵ�
		����:
			-1 -> ������1���ڵ�
			-2 -> ������2���ڵ�
			-n -> ������n���ڵ�

	���� ����� ���룬ɾ�������� �����������ͣ�
		ֱ�� �� �ڵ㣨Node) ���в����� ���ֲ�������  ���涼���� ����Node�������
		�Զ�Ӧ �ڵ� ��ֵ��(data ��) ���в���, ���ֲ������� ���涼�� ����Node�������

		��ʹ��ʱ ������ ʹ�� List~Node �����ķ���, �����Ƕ�Node ����ֱ�Ӳ���,
		Ӧ�þ����������������ĵ���, ��ʹ�÷�װ�Ը��õ� ������ ���� Node�ķ�����


	enjoy ......... :)

*/

#include <malloc.h>
#include <stdio.h>

#ifndef NULL
#define NULL 0
#endif // !NULL


#ifndef TYPE
#define TYPE  int
#endif // !TYPE


#ifndef $
#define $(Expr) Expr
#endif

/*---------------------------------------- Node ���ͼ����� ------------------------------------*/
// $(Node) will transfer to Node_char after preprocess
typedef struct $(Node) {
	TYPE data;
	struct $(Node) *pre;
	struct $(Node) *next;
} $(Node);

// �õ�һ��pre�� next ��ִ��NULL�Ľڵ�
inline $(Node) *$(GetNode)() {
	$(Node) *node = ($(Node) *)malloc(sizeof($(Node)));
	node->next = node->pre = NULL;
	return node;
}

// �õ�һ�� pre�� next ��ִ��NULL, ����data���ڴ�������Ľڵ�
inline $(Node) *$(CreateNode)(TYPE data) {
	$(Node) *node = ($(Node) *)malloc(sizeof($(Node)));
	node->data = data;
	node->next = node->pre = NULL;
	return node;
}


/*---------------------------------------- List ���ͼ����� ------------------------------------*/
// $(List) will transfer to List_char after preprocess
typedef struct $(List) {
	$(Node) *head;
	$(Node) *tail;
	int length;
} $(List);


/*
 * ��List�����ĵ����к������� List��ͷ�����ҽ��յĵ�һ���������� List* ����
 */


/*------------------------------------- List �������� -----------------------------*/
// ��ʼ������
void $(ListInit)($(List)* list) {
	list->head = ($(Node)*)malloc(sizeof($(Node)));
	list->head->pre = NULL;
	list->head->next = NULL;
	list->length = 0;
	list->tail = list->head;
}


// �ж������Ƿ�Ϊ�� Ϊ�շ��� 1 ���򷵻� 0
inline int $(ListIsEmpty)($(List) *list) {
	return list->length == 0;
}


// ��ȡ������
inline int $(ListGetLength)($(List) *list) {
	return list->length;
}


// ������list �е� next�ڵ�֮ǰ�����µĽڵ�node
inline void $(ListInsertHandler)($(List) *list,  $(Node) *next, $(Node) *node) {
	$(Node) *pre = NULL;
	if (next)
		pre = next->pre;
	else
		pre = list->tail;
	pre->next = node;
	node->pre = pre;
	node->next = next;
	if (next)
		next->pre = node;
	else
		list->tail = node;
	++list->length;
}


// ������list �е� ɾ��cur ���
inline void $(ListDeleteHandler)($(List) *list, $(Node) *cur) {
	$(Node) *pre = cur->pre;
	if (cur->next) {
		pre->next = cur->next;
		cur->next->pre = pre;
	}
	else {
		pre->next = NULL;
		list->tail = pre;
	}
	free(cur);
	--list->length;
}


// ������list �� ɾ��cur �ڵ�, ������cur�ڵ�
inline $(Node) *$(ListWithdraw)($(List) *list, $(Node) *cur) {
	if (cur == NULL || cur == list->head)
		return NULL;
	$(Node) *pre = cur->pre;
	if (cur->next) {
		pre->next = cur->next;
		cur->next->pre = pre;
	}
	else {
		pre->next = NULL;
		list->tail = pre;
	}
	--list->length;
	return(cur);
}


// ͨ��pos���� �ҵ� �ڵ���׵�ַ, pos >= 0 ��Ϊ��������, pos <= -1��Ϊ ����������
// �������Դ�-1��ʼ��-1��ʾ�������һ���ڵ㣬-2��ʾ���Ե����ڶ����ڵ㣬�Դ�����
$(Node) *$(ListGetNode)($(List) *list, int pos) {
	if (pos >= 0 && pos <= list->length) {
		$(Node) *cur = list->head->next;
		for (int i = 0; i < pos; ++i)
			cur = cur->next;
		return cur;
	}
	else if (pos < 0 && -pos <= list->length) {
		$(Node) *cur = list->tail;
		for (int i = -1; i > pos; --i)
			cur = cur->pre;
		return cur;
	}
	else {
		perror("pos out of range " __FUNCTION__ "\n");
		return NULL;
	}
}

// ����list ����free �� list�����е����нڵ�
void $(ListDestroy)($(List) *list) {
	$(Node) *tmp = NULL;
	$(Node) *cur = list->head->next;
	while (cur) {
		tmp = cur;
		cur = cur->next;
		--list->length;
		free(tmp);
	}
	free(list->head);
}


/* --------------------------------- List �� ���뺯�� ---------------------------*/

// ������list ��β�������µĽڵ�node
void $(ListPushBackNode)($(List) *list, $(Node) *node) {
	node->next = list->tail->next;
	node->pre = list->tail;
	list->tail->next = node;
	list->tail = node;
	++list->length;
}

// ������list ��β�������µĽڵ㣬�ýڵ�� data��Ϊ����Ĳ��� data
void $(ListPushBack)($(List) *list, TYPE data) {
	$(Node) *node = $(CreateNode)(data);
	$(ListPushBackNode)(list, node);
}

// ������list ��ͷ�������µĽڵ�node
void $(ListPushFrontNode)($(List) *list, $(Node) *node) {
	node->next = list->head->next;
	node->pre = list->head;
	list->head->next = node;

	if (list->head == list->tail)
		list->tail = node;

	++list->length;
}

// ������list ��ͷ�������µĽڵ㣬�ýڵ�� data��Ϊ����Ĳ��� data
void $(ListPushFront)($(List) *list, TYPE data) {
	$(Node) *node = $(CreateNode)(data);
	$(ListPushFrontNode)(list, node);
}

// ��list �ĵ�pos λ�ô� ����ڵ�node
void $(ListInsertNode)($(List) *list, int pos, $(Node) *node) {
	if (!(pos >= 0 && pos <= list->length || pos < 0 && -pos <=list->length) ) {
		perror("pos out of range on " __FUNCTION__ "\n");
		return;
	}

	$(ListInsertHandler)(list, $(ListGetNode)(list, pos), node);
}

// ������list ��pos�������µĽڵ㣬�ýڵ��data��Ϊ ����Ĳ���data
void $(ListInsert)($(List) *list, int pos, TYPE data) {
	if (!(pos >= 0 && pos <= list->length || pos < 0 && -pos <=list->length) ) {
		perror("pos out of range on " __FUNCTION__ "\n");
		return;
	}
	$(Node) *node = $(CreateNode)(data);

	$(ListInsertHandler)(list, $(ListGetNode)(list, pos), node);
}


/* --------------------------------- List �� ɾ������ ---------------------------*/
// ɾ��list ��ֵΪ data �Ľڵ�, ������ɾ���Ľڵ����
int $(ListRemove)($(List) *list, TYPE data) {
	int cnt = 0;
	$(Node) *pre = list->head, *cur = list->head->next;
	$(Node) *tmp = NULL;
	for (; cur;) {
		if (cur->data == data) {
			tmp = cur;
			cur = cur->next;
			pre->next = cur;

			if (cur)
				cur->pre = pre;
			if (tmp == list->tail)
				list->tail = pre;

			free(tmp);
			--list->length;
			++cnt;
		}
		else
			pre = cur, cur = cur->next;
	}
	return cnt;
}


// ɾ��list �е�һ��ֵΪ data �Ľڵ�, ������ɾ���Ľڵ���� 0 ���� 1
int $(ListRemoveFirst)($(List) *list, TYPE data) {
	int cnt = 0;
	$(Node) *pre = list->head, *cur = list->head->next;
	$(Node) *tmp = NULL;
	for (; cur;) {
		if (cur->data == data) {
			tmp = cur;
			cur = cur->next;
			pre->next = cur;

			if (cur)
				cur->pre = pre;

			if (tmp == list->tail)
				list->tail = pre;

			free(tmp);
			--list->length;
			break;
		}
		else
			pre = cur, cur = cur->next;
	}
	return cnt;
}


// ժȡlist �е�pos ���Ľڵ�
$(Node) *$(ListIsoateNode)($(List) *list, int pos) {
	if (!(pos >= 0 && pos < list->length || pos < 0 && -pos <=list->length) ) {
		perror("pos out of range on " __FUNCTION__ "\n");
		return NULL;
	}

	return $(ListWithdraw)(list, $(ListGetNode)(list, pos));
}


// ɾ��list �е�pos ���Ľڵ㣬������0 ��ʼ
void $(ListDelete)($(List) *list, int pos) {
	if (!(pos >= 0 && pos < list->length || pos < 0 && -pos <=list->length) ) {
		perror("pos out of range on " __FUNCTION__ "\n");
		return;
	}

	$(ListDeleteHandler)(list, $(ListGetNode)(list, pos));
}


// ����β�ڵ���׵�ַ��������β�ڵ�� list ��ɾ��
$(Node) *$(ListPopBackNode)($(List) *list) {
	return $(ListWithdraw)(list, list->tail);
}

// ����β�ڵ��data�򣬲�����β�ڵ�� list ��ɾ��
TYPE $(ListPopBack)($(List) *list) {
	if (list->head->next == NULL) {
		perror("No more node in list :" __FUNCTION__ "\n");
		return;
	}

	$(Node) *tailNode = $(ListPopBackNode)(list);
	TYPE tailData = tailNode->data;
	free(tailNode);

	return tailData;
}


// ���ص�һ���ڵ���׵�ַ��������β�ڵ�� list ��ɾ��
$(Node) *$(ListPopFrontNode)($(List) *list) {
	return $(ListWithdraw)(list, list->head->next);
}


// ���ص�һ���ڵ��data�򣬲�����β�ڵ�� list ��ɾ��
TYPE $(ListPopFront)($(List) *list) {
	if (list->head->next == NULL) {
		perror("No more node in list :" __FUNCTION__ "\n");
		return;
	}

	$(Node) *firstNode = $(ListPopFrontNode)(list);
	TYPE firstData = firstNode->data;
	free(firstNode);

	return firstData;
}




/* --------------------------------- List �� ���Һ��� ---------------------------*/

// ��list �в��ҵ�һ�� ֵΪ data �Ľڵ��������û�з���-1
int $(ListIndex)($(List) *list, TYPE data) {
	$(Node) *cur = list->head->next;
	int index = 0;
	for (; cur; cur = cur->next) {
		if (cur->data == data)
			return index;
		++index;
	}
	return -1;
}


// ��list �еõ��� pos���ڵ��ֵ(data ��)
TYPE $(ListGet)($(List) *list, int pos) {
	if (!(pos >= 0 && pos < list->length || pos < 0 && -pos <=list->length) ) {
		perror("pos out of range on " __FUNCTION__ "\n");
		return;
	}

	return $(ListGetNode)(list, pos)->data;
}


/*------------------ ���� ���� -----------------------*/
/*
 * �������� ������� : for(Node* node = ListBegin(&list); node != ListEnd(&list); node = node->next) {}
 */

 // ������������������׽ڵ�ָ��
$(Node) *$(ListBegin)($(List) *list) {
	return list->head->next;
}

// ��������������������һ���ڵ����һ���ڵ��ָ��
$(Node) *$(ListEnd)($(List) *list) {
	return NULL;
}

/*
 * �������� ������� : for(Node* node = ListRBegin(&list); node != ListREnd(&list); node = node->next) {}
 */

 // ����������������׽ڵ�ָ��
$(Node) *$(ListRBegin)($(List) *list) {
	return list->tail;
}

// ������������������һ���ڵ����һ���ڵ��ָ��
$(Node) *$(ListRend)($(List) *list) {
	return list->head;
}

#undef TYPE
#undef $
