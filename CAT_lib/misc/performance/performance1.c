#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../CAT.h"

int main() {
	printf("START \n");
	List l = List_new();

	for (int64_t i = 0; i < 10000000; i++) {
		int64_t* ptrI = (int64_t*) malloc(sizeof(int64_t));
		*ptrI = i;
		List_push_back(&l, ptrI);
	}

	clock_t begin = clock();
	
	int64_t counter = 0;
	for (int64_t j = 0; j < 100; j++) {
		Node* curr = List_front(&l);
		while (curr != NULL) {
			counter += *(int64_t*) (Node_get(curr));
			curr = Node_next(curr);
		}
	}
	

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time: %f\n", time_spent);

	printf("counter: %d\n", counter);

	// ARRAY ------

	clock_t begin2 = clock();

	Node* curr = l.front;
	int64_t *array = malloc(10000000 * sizeof(int64_t));
	int i = 0;
	while (curr != NULL) {
		// int64_t* numPtr = (int64_t*) (curr->value);
		array[i++] = *(int64_t*) (curr->value);
		curr = curr->next;
	}

	printf("herer hererh erhehre: \n");

		int64_t counter2 = 0;
	for (int64_t j = 0; j < 100; j++) {
		for (int i =0; i < 10000000; i++) {
			counter2 += array[i];
		}
	}
	

	clock_t end2 = clock();
	double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
	printf("time: %f\n", time_spent2);

	printf("counter: %d\n", counter2);

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