#ifndef lcthw_List_h
#define lcthw_List_h

#include <stdlib.h>

struct ListNode;
/*
A structure to represent a node in the doubled linked list.
*/
typedef struct ListNode {
    struct ListNode *next;
    struct ListNode *prev; // pointer to hr previous node in the list
    void *value; // pointer to the value stored in this node 
} ListNode;

typedef struct List {
    int count; // the number of the elements in the list
    ListNode *first;
    ListNode *last;
} List;

List *List_create();
//not important just part of List_clear_destroy
void List_destroy(List *list);
void List_clear(List *list);

/*
* Destroy the list 
*/
void List_clear_destroy(List *list);

#define List_count(A) ((A)->count)
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)


/*
* Add a new vaule to the end of the list
*/
void List_push(List *list, void *value);

/*
* Remove the last node and returns its value
*/
void *List_pop(List *list);

/*
*Add a new value to the start of the list
*/
void List_unshift(List *list, void *value);

/*
*Remove the first node and returns its value
*/
void *List_shift(List *list);


/* 
Remove a specific node from the list and returns its value
*/
void *List_remove(List *list, ListNode *node);


/*
shallow copy
*/
List *List_copy(List *list);


/*
connect list2 to the end of list1
and list2 will set empty

*/
void List_concat(List *list1, List *list2);
/*
spilt the list from the specified node.
returns a list that contains the nodes from the specified node to the end of the origin list

*/
List *List_split(List *list, ListNode *node);

void List_swap_nodes( List *list, ListNode *a,ListNode *b);



#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
    ListNode *V = NULL;\
    for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif