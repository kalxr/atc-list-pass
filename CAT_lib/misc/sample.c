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

	// Iterator it = It_new(l);
	// for (void* i = It_begin(&it); !It_end(&it); i = It_next(&it)) {
	// 	printf("%d\n", i);
	// }

	Node* curr = List_front(l);
	while (curr != NULL) {
		printf("hi there!!");
		printf("element: %d\n", Node_get(*curr));
		curr = Node_next(*curr);
	}

	void** arr = List_to_array(&l);
	for (int i = 0; i < List_size(l); i++) {
		printf("array element: %d\n", arr[i]);
	}
	free(arr);

	return 0;
}