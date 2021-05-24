#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../CAT.h"

int main() {
	printf("START \n");
	List l = List_new();

	for (int64_t i = 0; i < 1000000; i++) {
		int64_t* ptrI = (int64_t*) malloc(sizeof(int64_t));
		*ptrI = i;
		List_push_back(&l, ptrI);
	}

	
	int64_t counter = 0;
	for (int64_t j = 0; j < 100; j++) {
		Node* curr = List_front(&l);
		while (curr != NULL) {
			counter += *(int64_t*) (Node_get(curr));
			curr = Node_next(curr);
		}
		printf("counter: %d\n", counter);
	}
	printf("counter: %d\n", counter);

	// ARRAY ------

	Node* curr = l.front;
	int array[1000000];
	int i = 0;
	while (curr != NULL) {
		int64_t* numPtr = (int64_t*) (curr->value);
		array[i++] = *numPtr;
		curr = curr->next;
	}

	printf("herer hererh erhehre: \n");

	int64_t counter = 0;
	for (int64_t j = 0; j < 100; j++) {
		for (int i =0; i < 1000000; i++) {
			counter += array[i];
		}
	}
	printf("counter: %d\n", counter);

	// END ----

	// int array[1000];
	// for (int i =0; i < 1000; i++) {
	// 	array[i] = i;
	// }
	// int counter = 0;
	// for (int i =0; i < 1000; i++) {
	// 	// printf("%d\n", array[i]);
	// 	counter++;
	// }
	// printf("%d\n", counter);

	// int array[1000];
	// for (int i =0; i < 1000; i++) {
	// 	array[i] = i;
	// }
	// int counter = 0;
	// for (int i =0; i < 1000; i++) {
	// 	counter += array[i];
	// }

	return 0;
}