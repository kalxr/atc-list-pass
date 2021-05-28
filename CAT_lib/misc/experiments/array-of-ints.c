#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../CAT.h"

#define LIST_SIZE 10000000
#define REUSE 100
#define ITERS 5

#define TRANSFORM 1
#define ITERATE 1

int main() {

    for (int k = 0; k < ITERS; k++) {

        List l = List_new();

        for (int64_t i = 0; i < LIST_SIZE; i++) {
            int64_t* ptrI = (int64_t*) malloc(sizeof(int64_t));
            *ptrI = rand() % 5;
            List_push_back(&l, ptrI);
        }

        #if ITERATE

        // ARRAY ------

        int64_t counter = 0;

        #if TRANSFORM

        Node* curr = l.front;
        int64_t *array = malloc(LIST_SIZE * sizeof(int64_t));
        int i = 0;
        while (curr != NULL) {
            array[i++] = *(int64_t*) (curr->value);
            curr = curr->next;
        }
        
        int64_t length = List_size(&l);
        for (int64_t j = 0; j < REUSE; j++) {
            for (int i = 0; i < length; i++) {
                counter += array[i];
            }
        }

        #else

        for (int64_t j = 0; j < REUSE; j++) {
            Node* curr = List_front(&l);
            while (curr != NULL) {
                counter += *(int64_t*) (Node_get(curr));
                curr = Node_next(curr);
            }
        }

        #endif


        printf("counter: %ld\n", counter);

        #endif
    }
	
}