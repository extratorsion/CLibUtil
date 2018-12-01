/*
	在这个文件中 你只需要 更改两处内容：
	#define TYPE <Node 中 data 域 的类型>
	#define $(Expr) Expr##<固定类型/函数名后面的其他名字>
	如果 你不修改， 那么默认：
		TYPE 被定义为 int, $(Expr) 被定义为 Expr
		也即是说 你 会得到:
				typedef struct Node {
					int data;
					struct Node *pre;
					struct Node *next;
				} Node;
				函数 的 名字 都会是 ListInit, ListDestroy..... 之类


	例 -> 如果你定义了：
		#define TYPE char
		#define $(Expr) Expr##_char
	那么:
		struct $(Node) {
			TYPE data;
			struct $(Node) *pre;
			struct $(Node) *next;
		} $(Node);
		会被替换为:
			struct Node_char {
				char data;
				struct Node_char *pre;
				struct Node_char *next;
			} Node_char;
		inline $(Node) *$(GetNode)() {
			...
		}
		会被替换为:
		inline Node_char *GetNode_char() {
			...
		}
		以此类推

	关于 使用方法 的补充
		如果 你想在 一个.c 文件里 同时 使用 两个 及 以上的 List 和 Node 类型，并 你包含 一种头文件，即是该头文件list.h
		可以 这样写
		#define TYPE int
		#define $(Expr) Expr##Int
		#include "list.h"

		#define TYPE double
		#define $(Expr) Expr##Double
		#include "list.h"

		....

		这样 你 将同时 拥有
			ListInt, NodeInt, ListInitInt, ListGetInt.....
			和
			ListDouble, NodeDouble ListInitDouble, ListGetDouble.....




	关于 链表的索引 支持 两种方式：
	正向 和 反向
		正向：
			0 -> 第1个节点
			1 -> 第2个节点
			...
			n -> 第n+1个节点
		反向:
			-1 -> 倒数第1个节点
			-2 -> 倒数第2个节点
			-n -> 倒数第n个节点

	关于 链表的 插入，删除，索引 都有两种类型：
		直接 对 节点（Node) 进行操作， 这种操作函数  后面都不会 跟上Node这个单词
		对对应 节点 的值域(data 域) 进行操作, 这种操作函数 后面都会 跟上Node这个单词

		在使用时 不建议 使用 List~Node 这样的方法, 这样是对Node 进行直接操作,
		应该尽量避免这样函数的调用, 而使用封装性更好的 函数名 不带 Node的方法。


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

/*---------------------------------------- Node 类型及函数 ------------------------------------*/
// $(Node) will transfer to Node_char after preprocess
typedef struct $(Node) {
	TYPE data;
	struct $(Node) *pre;
	struct $(Node) *next;
} $(Node);

// 得到一个pre和 next 都执行NULL的节点
inline $(Node) *$(GetNode)() {
	$(Node) *node = ($(Node) *)malloc(sizeof($(Node)));
	node->next = node->pre = NULL;
	return node;
}

// 得到一个 pre和 next 都执行NULL, 并且data等于传入参数的节点
inline $(Node) *$(CreateNode)(TYPE data) {
	$(Node) *node = ($(Node) *)malloc(sizeof($(Node)));
	node->data = data;
	node->next = node->pre = NULL;
	return node;
}


/*---------------------------------------- List 类型及函数 ------------------------------------*/
// $(List) will transfer to List_char after preprocess
typedef struct $(List) {
	$(Node) *head;
	$(Node) *tail;
	int length;
} $(List);


/*
 * 对List操作的的所有函数都以 List开头，并且接收的第一个参数都是 List* 类型
 */


/*------------------------------------- List 基本函数 -----------------------------*/
// 初始化链表
void $(ListInit)($(List)* list) {
	list->head = ($(Node)*)malloc(sizeof($(Node)));
	list->head->pre = NULL;
	list->head->next = NULL;
	list->length = 0;
	list->tail = list->head;
}


// 判断链表是否为空 为空返回 1 否则返回 0
inline int $(ListIsEmpty)($(List) *list) {
	return list->length == 0;
}


// 获取链表长度
inline int $(ListGetLength)($(List) *list) {
	return list->length;
}


// 在链表list 中的 next节点之前插入新的节点node
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


// 在链表list 中的 删除cur 结点
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


// 在链表list 中 删除cur 节点, 并返回cur节点
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


// 通过pos索引 找到 节点的首地址, pos >= 0 作为正向索引, pos <= -1作为 反向索引，
// 反向所以从-1开始，-1表示索引最后一个节点，-2表示所以倒数第二个节点，以此类推
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

// 销毁list 链表，free 掉 list链表中的所有节点
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


/* --------------------------------- List 中 插入函数 ---------------------------*/

// 在链表list 的尾部插入新的节点node
void $(ListPushBackNode)($(List) *list, $(Node) *node) {
	node->next = list->tail->next;
	node->pre = list->tail;
	list->tail->next = node;
	list->tail = node;
	++list->length;
}

// 在链表list 的尾部插入新的节点，该节点的 data域为传入的参数 data
void $(ListPushBack)($(List) *list, TYPE data) {
	$(Node) *node = $(CreateNode)(data);
	$(ListPushBackNode)(list, node);
}

// 在链表list 的头部插入新的节点node
void $(ListPushFrontNode)($(List) *list, $(Node) *node) {
	node->next = list->head->next;
	node->pre = list->head;
	list->head->next = node;

	if (list->head == list->tail)
		list->tail = node;

	++list->length;
}

// 在链表list 的头部插入新的节点，该节点的 data域为传入的参数 data
void $(ListPushFront)($(List) *list, TYPE data) {
	$(Node) *node = $(CreateNode)(data);
	$(ListPushFrontNode)(list, node);
}

// 在list 的第pos 位置处 插入节点node
void $(ListInsertNode)($(List) *list, int pos, $(Node) *node) {
	if (!(pos >= 0 && pos <= list->length || pos < 0 && -pos <=list->length) ) {
		perror("pos out of range on " __FUNCTION__ "\n");
		return;
	}

	$(ListInsertHandler)(list, $(ListGetNode)(list, pos), node);
}

// 在链表list 的pos处插入新的节点，该节点的data域为 传入的参数data
void $(ListInsert)($(List) *list, int pos, TYPE data) {
	if (!(pos >= 0 && pos <= list->length || pos < 0 && -pos <=list->length) ) {
		perror("pos out of range on " __FUNCTION__ "\n");
		return;
	}
	$(Node) *node = $(CreateNode)(data);

	$(ListInsertHandler)(list, $(ListGetNode)(list, pos), node);
}


/* --------------------------------- List 中 删除函数 ---------------------------*/
// 删除list 中值为 data 的节点, 并返回删除的节点个数
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


// 删除list 中第一个值为 data 的节点, 并返回删除的节点个数 0 或者 1
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


// 摘取list 中第pos 个的节点
$(Node) *$(ListIsoateNode)($(List) *list, int pos) {
	if (!(pos >= 0 && pos < list->length || pos < 0 && -pos <=list->length) ) {
		perror("pos out of range on " __FUNCTION__ "\n");
		return NULL;
	}

	return $(ListWithdraw)(list, $(ListGetNode)(list, pos));
}


// 删除list 中第pos 个的节点，索引从0 开始
void $(ListDelete)($(List) *list, int pos) {
	if (!(pos >= 0 && pos < list->length || pos < 0 && -pos <=list->length) ) {
		perror("pos out of range on " __FUNCTION__ "\n");
		return;
	}

	$(ListDeleteHandler)(list, $(ListGetNode)(list, pos));
}


// 返回尾节点的首地址，并将该尾节点从 list 中删除
$(Node) *$(ListPopBackNode)($(List) *list) {
	return $(ListWithdraw)(list, list->tail);
}

// 返回尾节点的data域，并将该尾节点从 list 中删除
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


// 返回第一个节点的首地址，并将该尾节点从 list 中删除
$(Node) *$(ListPopFrontNode)($(List) *list) {
	return $(ListWithdraw)(list, list->head->next);
}


// 返回第一个节点的data域，并将该尾节点从 list 中删除
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




/* --------------------------------- List 中 查找函数 ---------------------------*/

// 在list 中查找第一个 值为 data 的节点的索引，没有返回-1
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


// 在list 中得到第 pos个节点的值(data 域)
TYPE $(ListGet)($(List) *list, int pos) {
	if (!(pos >= 0 && pos < list->length || pos < 0 && -pos <=list->length) ) {
		perror("pos out of range on " __FUNCTION__ "\n");
		return;
	}

	return $(ListGetNode)(list, pos)->data;
}


/*------------------ 迭代 函数 -----------------------*/
/*
 * 迭代函数 正向遍历 : for(Node* node = ListBegin(&list); node != ListEnd(&list); node = node->next) {}
 */

 // 返回链表正向遍历的首节点指针
$(Node) *$(ListBegin)($(List) *list) {
	return list->head->next;
}

// 返回链表正向遍历的最后一个节点的下一个节点的指针
$(Node) *$(ListEnd)($(List) *list) {
	return NULL;
}

/*
 * 迭代函数 反向遍历 : for(Node* node = ListRBegin(&list); node != ListREnd(&list); node = node->next) {}
 */

 // 返回链表反向遍历的首节点指针
$(Node) *$(ListRBegin)($(List) *list) {
	return list->tail;
}

// 返回链表反向遍历的最后一个节点的下一个节点的指针
$(Node) *$(ListRend)($(List) *list) {
	return list->head;
}

#undef TYPE
#undef $
