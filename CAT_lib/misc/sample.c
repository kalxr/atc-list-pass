#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CAT.h"

int main() {
	List l = List_new();

	List_push_back(&l, 8);
	List_push_back(&l, 10);
	List_push_back(&l, 9);

	printf("%d\n", List_size(l));

	Node* curr = List_front(&l);
	while (curr != NULL) {
		printf("element: %d\n", Node_get(*curr));
		curr = Node_next(*curr);
	}

	return 0;
}