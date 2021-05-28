#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../CAT.h"

#define LIST_SIZE 10000000
#define REUSE 100
#define ITERS 5

#define ITERATE 1

int main() {

    for (int k = 0; k < ITERS; k++) {

        List l = List_new();

        for (int64_t i = 0; i < LIST_SIZE; i++) {
            int64_t* ptrI = (int64_t*) malloc(sizeof(int64_t));
            *ptrI = rand() % 100;
            List_push_back(&l, ptrI);
        }

        #if ITERATE

        int64_t counter = 0;
        for (int64_t j = 0; j < REUSE; j++) {
            Node* curr = List_front(&l);
            while (curr != NULL) {
                counter += *(int64_t*) (Node_get(curr));
                curr = Node_next(curr);
            }
        }

        printf("counter: %ld\n", counter);

        #endif
    }
	
}