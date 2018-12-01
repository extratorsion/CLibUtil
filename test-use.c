#include <stdio.h>
#include <stdlib.h>
//#include "list-char.h"
//#include "list-int.h"

#define TYPE int
#define $(Expr) Expr##Int
#include "list.h"

#define TYPE char 
#define $(Expr) Expr##_char
#include "list.h"


// typedef $M(Node_) MERG;

int main(void) {

	ListInt li;

	ListInitInt(&li);
	for (int i = 0; i < 10; ++i)
		ListPushBackInt(&li, i);

	for (NodeInt* pre = li.tail; pre != li.head; pre = pre->pre)
		printf("%d\n", pre->data);

	for (int i = 0; i < ListGetLengthInt(&li); ++i) {
		printf("%d ", ListGetInt(&li, i));
	}

	putchar(10);

	ListDestroyInt(&li);

	List_char lc;
	ListInit_char(&lc);
	char buf[] = { "Hello Wahah :) \n" };
	for (char* ptr = buf; *ptr; ++ptr)
		ListPushBack_char(&lc, *ptr);


	int cnt = lc.length;
	for (int i = 0; i < cnt; ++i)
		printf("%c", ListPopFront_char(&lc));

	ListDestroy_char(&lc);

	system("pause");
	return 0;
}