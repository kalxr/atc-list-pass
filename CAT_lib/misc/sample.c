#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CAT.h"

int main() {
	List l = List_new();

	for (auto i = 0; i < 10; i++) {
		List_push_back(&l, i);
	}

	printf("%d\n", List_size(l));

	// Node* curr = List_front(&l);
	// while (curr != NULL) {
	// 	printf("element: %d\n", Node_get(*curr));
	// 	curr = Node_next(*curr);
	// }

	
	// for (int k = 0; k < 10; k ++) {

		
	// 	for (int i = 0; i < 100; i++) {

	// 		int counter;			

	// 		for (int j = 0; j < List_size(&l); j++) {
	// 			void* val = array[j];
	// 			counter += val;
	// 		}
		
	// 	}

	// 	// List_push_back(&l, i);
	// }

	array = List_to_array(l);

	for ...

	for ...
	
	for ...


	

	return 0;
}