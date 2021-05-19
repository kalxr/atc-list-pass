#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../CAT.h"

int main() {
	List l = List_new();

	for (auto i = 0; i < 5; i++) {
		List_push_back(&l, i);
	}

	printf("%d\n", List_size(&l));

    Node* curr = List_front(&l);
    while (curr != NULL) {
        printf("element: %d\n", Node_get(curr));
        curr = Node_next(curr);
    }

    curr = List_front(&l);
    int counter = 0;
    while (curr != NULL) {
        counter += Node_get(curr);
        curr = Node_next(curr);
    }
    printf("counter: %d", counter);

	return 0;
}