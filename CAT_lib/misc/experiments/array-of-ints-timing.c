#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "../CAT.h"

#define LIST_SIZE 10000000
#define REUSE 100
#define CLOCK_ITER 2
#define TRANSFORM 1

int main() {
    setlocale(LC_NUMERIC, "");

    List l = List_new();

    for (int64_t i = 0; i < LIST_SIZE; i++) {
        int64_t* ptrI = (int64_t*) malloc(sizeof(int64_t));
        *ptrI = rand() % 100;
        List_push_back(&l, ptrI);
    }

    clock_t begin;
    clock_t end;
    double total = 0;
    int64_t counter = 0;
    for (int k = 0; k < CLOCK_ITER; k++) {
        begin = clock();
        
        #if TRANSFORM

        // Manual transformation to avoid hoisting        
        Node* curr = l.front;
        int64_t* array = malloc(LIST_SIZE * sizeof(int64_t));
        int i = 0;
        while (curr != NULL) {
            array[i++] = *(int64_t*)curr->value;
            curr = curr->next;
        }
        
        int64_t length = List_size(&l);
        for (int64_t j = 0; j < REUSE; j++) {
            for (int64_t i = 0; i < length; i++) {
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
        
        end = clock();

        
        
        total += (double)(end - begin);
    }
    printf("counter: %ld", counter);

    double total_time_spent = total / CLOCKS_PER_SEC;
    
    printf("\n\nTotal iteration time for %'d clock runs with transformation %s: %f\n\n", CLOCK_ITER, (TRANSFORM ? "enabled" : "disabled"), total_time_spent);
	
}
