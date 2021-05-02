#ifndef CAT_H
#define CAT_H

#include <stdint.h>

// typedef struct CATData CATData;

typedef struct List {
	int64_t length;
	struct NodeData* front;
	struct NodeData* back;
} List;

typedef struct Iterator {
	struct List list;
	struct NodeData* curr;
} Iterator;

typedef struct NodeData {
	void*	value;
	struct NodeData* next;
    struct NodeData* previous;
} Node;

List List_new();

Node* List_front(List l);
Node* List_back(List l);

void List_push_front(List* l, void* value);
void List_push_back(List* l, void* value);

void* List_pop_front(List* l);
void* List_pop_back(List* l);

Node* Node_next(Node n);
Node* Node_prev(Node n);
void* Node_get(Node n);

int64_t List_size(List l);
int List_empty(List l);

///////////////////////////////////////////

Iterator It_new(List l);
void* It_begin(Iterator* it);
int It_end(Iterator* it);
void* It_next(Iterator* it);

///////////////////////////////////////////

void* List_to_array(List* l);


// CATData List_new (void* value, CATData* next, CATData* prev);

// void CAT_set_prev (CATData* node, CATData* prev);
// void CAT_set_next (CATData* node, CATData* next);

// const void* CAT_get (const CATData* v);

// CATData* CAT_next(CATData* node);

// CATData* CAT_previous(CATData* node);

// void CAT_set (CATData* v, void* value);

///////////////////

// CATData* CAT_index(CATData* head, int64_t i);

// void CAT_add (CATData result, const CATData v1, const CATData v2);

// void CAT_sub (CATData result, const CATData v1, const CATData v2);

// const int64_t CAT_invocations (void);

#endif
