#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../CAT.h"

int main() {
	// List l = List_new();

	// for (auto i = 0; i < 1000000; i++) {
	// 	List_push_back(&l, i);
	// }

	
	// int64_t counter = 0;
	// for (auto j = 0; j < 1000; j++) {
	// 	Node* curr = List_front(&l);
	// 	while (curr != NULL) {
	// 		counter += Node_get(curr);
	// 		curr = Node_next(curr);
	// 	}
	// }
	// printf("counter: %d\n", counter);

	int array[1000];
	for (int i =0; i < 1000; i++) {
		array[i] = i;
	}
	int counter = 0;
	for (int i =0; i < 1000; i++) {
		// printf("%d\n", array[i]);
		counter++;
	}
	printf("%d\n", counter);

	return 0;
}