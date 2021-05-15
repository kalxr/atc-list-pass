#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../CAT.h"

int main() {
	List l = List_new();

	for (auto i = 0; i < 1000000; i++) {
		List_push_back(&l, i);
	}

	Node* front = List_front(&l);
	for (auto j = 0; j < 10; j++) {
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