#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CAT.h"

int main() {
	List l = List_new();

	List_push_back(&l, 8);
	List_push_back(&l, 10);
	List_push_back(&l, 9);

	for (auto i = 0; i < 1000000; i++) {
		List_push_back(&l, i);
	}

	Node* front = List_front(&l);
	for (auto j = 0; j < 1000; j++) {
		int64_t counter = 0;
		Node* curr = front;
		while (curr != NULL) {
			counter += Node_get(*curr);
			curr = Node_next(*curr);
		}
		// printf("counter: %d\n", counter);
	}

	return 0;
}