#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../CAT.h"

int main() {

    for (int k = 0; k < 1; k++) {
        int64_t length = 10000000;
        int64_t reuse = 100;
        int64_t length = List_size(&l);

        List l = List_new();

        for (int64_t i = 0; i < length; i++) {
            int64_t* ptrI = (int64_t*) malloc(sizeof(int64_t));
            *ptrI = rand() % 5;
            List_push_back(&l, ptrI);
        }

        // int64_t counter = 0;
        // for (int64_t j = 0; j < reuse; j++) {
        //     Node* curr = List_front(&l);
        //     while (curr != NULL) {
        //         counter += *(int64_t*) (Node_get(curr));
        //         curr = Node_next(curr);
        //     }
        // }
        // printf("counter: %ld\n", counter);



        // ARRAY ------

        Node* curr = l.front;
        int64_t *array = malloc(length * sizeof(int64_t));
        int i = 0;
        while (curr != NULL) {
            // int64_t* numPtr = (int64_t*) (curr->value);
            array[i++] = *(int64_t*) (curr->value);
            curr = curr->next;
        }

        int64_t counter = 0;
        for (int64_t j = 0; j < reuse; j++) {
            for (int i =0; i < length; i++) {
                counter += array[i];
            }
        }
        printf("counter: %ld\n", counter);
    }
	
}