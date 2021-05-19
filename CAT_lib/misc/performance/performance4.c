#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../CAT.h"

int main() {
	List l = List_new();

	for (auto i = 0; i < 1000000; i++) {
		List_push_back(&l, i);
	}

	int64_t counter = 0;
	for (auto j = 0; j < 3; j++) {
		Node* front = List_front(&l);
		Node* curr = front;
		while (curr != NULL) {
			counter += Node_get(curr);
			curr = Node_next(curr);
		}
	}
	printf("counter: %d\n", counter);

	return 0;
}