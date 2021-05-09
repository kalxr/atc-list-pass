#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CAT.h"

#define VALID_STRING 		"p6pbbUlpLo0BL1bM2k8K"
#define VALID_STRING_SIZE	20

static int64_t invocations = 0;

// int main() {
// 	return 0;
// }

// TODO? maybe make this not a pointer
struct List List_new() {
	struct List *l = (struct List*) malloc(sizeof(struct List));

  	invocations++;

	l->length = 0;
	l->front = NULL;
	l->back = NULL;

	return (List) *l;
}

Node* List_front(List* l) {
	return l->front;
}

Node* List_back(List l) {
	return l.back;
}

void List_push_front(List* l, void* value) {
	Node* d = (Node*) malloc(sizeof(Node));
	d->value = value;
	d->next = l->front;
	d->previous = NULL;

	if (l->front != NULL) {
		l->front->previous = d;
	}
	l->front = d;

	if (l->back == NULL) {
		l->back = d;
	}
	l->length++;
}

void List_push_back(List* l, void* value) {
	Node* d = (Node*) malloc(sizeof(Node));
	d->value = value;
	d->previous = l->back;
	d->next = NULL;

	if (l->back != NULL) {
		l->back->next = d;
	}
	l->back = d;

	if (l->back->previous == NULL) {
		l->front = d;
	}

	l->length++;
}

void* List_pop_front(List* l) {
	if (l->front != NULL) {
		Node*  front = l->front;

		void* val = l->front->value;
		l->front = l->front->next;

		if (l->front != NULL) {
			l->front->previous = NULL;
		} else {
			l->back = NULL;
		}
		l->length--;

		if (l->length == 1) {
			l->back = l->front;
		}

		free(front);
		return val;
	}

	return NULL;
}

void* List_pop_back(List* l) {
	if (l->back != NULL) {
		Node*  back = l->back;

		void* val = l->back->value;
		l->back = l->back->previous;

		if (l->back != NULL) {
			l->back->next = NULL;
		} else {
			l->front = NULL;
		}

		l->length--;

		if (l->length == 1) {
			l->front = l->back;
		}

		free(back);
		return val;
	}

	return NULL;
}

Node* Node_next(Node n) {
	return n.next;
}

Node* Node_prev(Node n) {
	return n.previous;
}

void* Node_get(Node n) {
	return n.value;
}

int64_t List_size(List* l) {
	return l->length;
}

int List_empty(List l) {
	return !l.length;
}

Iterator It_new(List l) {
	Iterator *it= (Iterator*) malloc(sizeof(Iterator));

	it->list = l;
	it->curr = NULL;

	return (Iterator) *it;
}

void* It_begin(Iterator* it) {
	it->curr = it->list.front;
	return it->curr->value;
}

int It_end(Iterator* it) {
	return it->curr == NULL;
}

void* It_next(Iterator* it) {
	it->curr = it->curr->next;

	if (it->curr != NULL) {
		return it->curr->value;
	} else {
		return NULL;
	}
}

void** List_to_array(List* l) {
	Node* curr = l->front;
	int i = 0;
	void** arr = malloc(l->length * sizeof(void*));
	while (curr != NULL) {
		arr[i++] = curr->value;
		curr = Node_next(*curr);
	}
	return arr;
}

// const void* CAT_get (const CATData* v) {
//     invocations++;
// 	return v->value;
// }

// CATData* CAT_next(CATData* node) {
// 	invocations++;
// 	return node->next;
// }

// CATData* CAT_previous(CATData* node) {
// 	invocations++;
// 	return node->prev;
// }

// void CAT_set(CATData* v, void* value) {
// 	invocations++;
// 	v->value = value;
// }

// void CAT_set_prev (CATData* node, CATData* prev) {
// 	invocations++;
// 	node->prev = prev;
// }

// void CAT_set_next (CATData* node, CATData* next) {
// 	invocations++;
// 	node->next = next;
// }

// const void* CAT_invocations (void){
//   return invocations;
// }

// int main() {
// 	return 0;
// }


// void CAT_sub (CATData result, const CATData v1, const CATData v2){
// 	internal_data_t		*d1;
// 	internal_data_t		*d2;
// 	internal_data_t		*dresult;
//   invocations++;

//   if (v1 == NULL || v2 == NULL) return;

// 	d1				= internal_check_data(v1);
// 	d2				= internal_check_data(v2);
// 	dresult			= internal_check_data(result);

// 	dresult->value	= d1->value - d2->value;

// 	return ;
// }

// void CAT_add (CATData result, const CATData v1, const CATData v2){
// 	internal_data_t		*d1;
// 	internal_data_t		*d2;
// 	internal_data_t		*dresult;
//   invocations++;

//   if (v1 == NULL || v2 == NULL) return;
// 	d1				= internal_check_data(v1);
// 	d2				= internal_check_data(v2);
// 	dresult			= internal_check_data(result);

// 	dresult->value	= d1->value + d2->value;

// 	return ;
// }

// static internal_data_t * internal_check_data (CATData v){
// 	internal_data_t	*d;

// 	if (v == NULL){
// 		fprintf(stderr, "libCAT: ERROR = input is NULL\n");
// 		abort();
// 	}
// 	d	= (internal_data_t *) v;

// 	if ( (strncmp(d->begin_validation_string, VALID_STRING, VALID_STRING_SIZE) != 0)		||
// 			 (strncmp(d->end_validation_string, VALID_STRING, VALID_STRING_SIZE) != 0)		  ){
// 		fprintf(stderr, "libCAT: ERROR = data has been corrupted\n");
// 		abort();
// 	}

// 	return d;
// }
