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

    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < 10; i++) {
            Node* curr = List_front(&l);
            while (curr != NULL) {
                printf("element: %d\n", Node_get(curr));
                curr = Node_next(curr);
            }
            printf("hi there");
        }
        List_push_back(&l, 99);
    }   
}