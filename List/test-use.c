#include <stdio.h>
#include <stdlib.h>

#define TYPE int
#define $(Expr) Expr##Int
#include "list.h"

#define TYPE char 
#define $(Expr) Expr##_char
#include "list.h"


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
  
 // 如果是 VS　环境下，需要 调用 pause 命令，是程序输出为结果后不直接退出
 #ifdef _WIN32 
	system("pause");
 #endif
	return 0;
}
