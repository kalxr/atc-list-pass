#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../CAT.h"

int main() {

    for (int k = 0; k < 5; k++) {
        List l = List_new();

        for (int64_t i = 0; i < 10000000; i++) {
            int64_t* ptrI = (int64_t*) malloc(sizeof(int64_t));
            *ptrI = rand() % 100;
            List_push_back(&l, ptrI);
        }

        int64_t counter = 0;
        for (int64_t j = 0; j < 3; j++) {
            Node* curr = List_front(&l);
            while (curr != NULL) {
                counter += *(int64_t*) (Node_get(curr));
                curr = Node_next(curr);
            }
        }
        printf("counter: %ld\n", counter);
    }
	
}