#include "lcthw/list.h"
#include "lcthw/dbg.h"
#include <assert.h>

List *List_create()
{
    return calloc(1, sizeof(List));
}

void List_destroy(List *list)
{
    assert(list != NULL);
    LIST_FOREACH(list, first, next, cur) {
        if(cur->prev) {
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);
}


void List_clear(List *list)
{
    assert(list != NULL);
    LIST_FOREACH(list, first, next, cur) {
        printf("Freeing value at: %p\n", cur->value);
        if (cur->value) {
            free(cur->value);
            printf("Successfully freed value at: %p\n", cur->value);
        }
    }
}



void List_clear_destroy(List *list)
{
    assert(list != NULL);
    List_clear(list);
    // LIST_FOREACH(list,first,next,cur){
    //     if(cur->value) free(cur->value);
    //     if(cur)free(cur);
    // }

    List_destroy(list);
}


void List_push(List *list, void *value)
{
    assert(list != NULL);
    assert(value != NULL);
    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if(list->last == NULL) {
        list->first = node;
        list->last = node;
    } else {
        list->last->next = node;
        node->prev = list->last;
        list->last = node;
    }

    list->count++;

    //agure check
    assert(list->count >= 0);
    assert(list->first != NULL && list->last != NULL);

error:
    return;
}

void *List_pop(List *list)
{
    assert(list != NULL);
    assert(value != NULL);

    ListNode *node = list->last;
    return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List *list, void *value)
{
    assert(list != NULL);
    assert(value != NULL);

    ListNode *node = calloc(1, sizeof(ListNode));
    check_mem(node);

    node->value = value;

    if(list->first == NULL) {
        list->first = node;
        list->last = node;
    } else {
        node->next = list->first;
        list->first->prev = node;
        list->first = node;
    }

    list->count++;

error:
    return;
}

void *List_shift(List *list)
{
    assert(list != NULL);

    ListNode *node = list->first;
    return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List *list, ListNode *node)
{
    assert(list != NULL);

    void *result = NULL;

    check(list->first && list->last, "List is empty.");
    check(node, "node can't be NULL");

    if(node == list->first && node == list->last) {
        list->first = NULL;
        list->last = NULL;
    } else if(node == list->first) {
        list->first = node->next;
        check(list->first != NULL, "Invalid list, somehow got a first that is NULL.");
        list->first->prev = NULL;
    } else if (node == list->last) {
        list->last = node->prev;
        check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
        list->last->next = NULL;
    } else {
        ListNode *after = node->next;
        ListNode *before = node->prev;
        after->prev = before;
        before->next = after;
    }

    list->count--;
    //agure check
    assert(list->count >= 0);
    assert(list->first != NULL && list->last != NULL);

    result = node->value;
    free(node);

error:
    return result;
}

List *List_copy(List *list){
    
    assert(list != NULL);
    List *copy = List_create();
    LIST_FOREACH(list,first,next,cur){
        // if(cur->value != NULL) {
        //     char *new_value = malloc(strlen((char *)cur->value)+1);
        //     assert(new_value != NULL);

        //     strcpy(new_value,(char *)cur->value);

        //     List_push(copy,new_value);
        // }else{
        //     List_push(copy,NULL);
        // }
        List_push(copy, cur->value);  // shallow copy
    }

    return copy;

}

void List_concat(List *list1, List *list2){

    assert(list1 != NULL && list2!= NULL);
    if(List_count(list2)==0)return;
    if(List_count(list1)==0){
        list1->first = list2->first;
        list1->last = list2->last;
        list1->count = list2->count;
        return;
    }
    check(list2->first != NULL, "Invalid list2, somehow got a first that is NULL.");
    list1->last->next = list2->first;
    check(list1->last != NULL, "Invalid list1, somehow got a last that is NULL.");
    list2->first->prev = list1->last;

    list1->last = list2->last;
    list1->count = list1->count + list2->count;
    

    //turn list2 to empty
    list2->first = NULL;
    list2->last =NULL;
    list2->count = 0;
error:
    return;
}

List *List_split(List *list, ListNode *node){
    assert(list != NULL && node != NULL);
    int new_count = 0;

    List *new_list = List_create();

    if(node == list->last){
        return new_list;
    }
    
    new_list->first = node->next;
    new_list->last = list->last;

    node->next->prev = NULL;
    node->next = NULL;

    //new_list->count = List_count(new_list);
    // Calculate the count for new_list
    ListNode *current = new_list->first;
    while (current != NULL) {
        new_count++;
        current = current->next;
    }
    new_list->count = new_count;


    list->last = node;
    list->count -= new_count;

    printf("Original list count: %d\n", list->count);
    printf("New list count: %d\n", new_list->count);


    return new_list;

}


void List_swap_nodes( List *list, ListNode *a,ListNode *b){
    if(a == b) return;

    //tmp
    ListNode *prevA = a->prev;
    ListNode *nextA = a->next;
    ListNode *prevB = b->prev;
    ListNode *nextB = b->next;

    //swap
    if (nextA == b){
        a->next = nextB;
        a->prev = b;
        b->next = a;
        b->prev = prevA;
        if (nextB) nextB->prev = a;
        if (prevA) prevA->next = b;
    }
    else if(nextB == a){
        a->next = b;
        a->prev = prevB;
        b->next = nextA;
        b->prev = a;
        if (nextA) nextA->prev = b;
        if (prevB) prevB->next = a;
    }
    else{
        if (prevA) prevA->next = b;
        if (nextA) nextA->prev = b;
        b->prev = prevA;
        b->next = nextA;

        if (prevB) prevB->next = a;
        if (nextB) nextB->prev = a;
        a->prev = prevB;
        a->next = nextB;
    }
    // check and update the first and the last pointer
    if (list->first == a) list->first = b;
    else if (list->first == b) list->first = a;
    if (list->last == a) list->last = b;
    else if (list->last == b) list->last = a;

}