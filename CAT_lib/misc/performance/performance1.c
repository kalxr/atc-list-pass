#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../CAT.h"

int main() {
	int64_t counter = 0;

	List l = List_new();

	for (int i = 0; i < 1000; i++) {
		List_push_back(&l, i);
	}

	for (int j = 0; j < 1000; j++) {
		Node* curr = List_front(&l);
		while (curr != NULL) {
			counter += Node_get(curr);
			curr = Node_next(curr);
		}
	}

	// int array[1000];
	// for (int i =0; i < 1000; i++) {
	// 	array[i] = i;
	// }
	// int counter = 0;
	// for (int i =0; i < 1000; i++) {
	// 	counter += array[i];
	// }


	printf("counter: %ld\n", counter);
	return 0;
}