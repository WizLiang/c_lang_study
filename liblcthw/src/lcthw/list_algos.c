#include <stdio.h>
#include "list_algos.h"

void print_list(List *list) {
    ListNode *node = list->first;
    printf("List: ");
    while (node != NULL) {
        printf("%d -> ", *(char*)node->value); // 假设存储的是int类型
        node = node->next;
    }
    printf("NULL\n");
}

int List_bubble_sort(List *list, List_compare cmp){

    ListNode *cur,*cur1;
    ListNode *cur_last = NULL;

    if (!list->first) {
        return 0; //empty list
    }
    print_list(list);
    for(cur = list->first;cur->next != NULL;cur = cur->next){
        for(cur1 = cur;cur1->next != cur_last;cur1 = cur1 ->next){
            if(cmp(cur1->value, cur1->next->value) > 0){
                // List_swap_nodes(list, cur1,cur1->next);//exchange the pointer
                // if (cur1->prev) {
                //     cur1 = cur1->prev;
                // }
                void *temp = cur1->value;
                cur1->value = cur1->next->value;
                cur1->next->value = temp;
            }
        }
        print_list(list);
        //cur_last = cur1;
    }
    return 0;
}

// int List_bubble_sort(List *list, List_compare cmp){
//     int swapped;
//     ListNode *cur;
//     ListNode *cur_last = NULL;

//     if (!list->first) {
//         return 0; // 空链表直接返回
//     }

//     do {
//         swapped = 0;
//         cur = list->first;

//         while (cur->next != cur_last) {
//             if (cmp(cur->value, cur->next->value) > 0) {
//                 // 交换节点中的值
//                 void *temp = cur->value;
//                 cur->value = cur->next->value;
//                 cur->next->value = temp;
//                 swapped = 1;
//             }
//             cur = cur->next;
//         }
//         cur_last = cur; // 更新已排序部分的起始位置
//     } while (swapped);

//     return 0;
// }




inline void ListNode_swap(ListNode *a, ListNode *b)
{
    void *temp = a->value;
    a->value = b->value;
    b->value = temp;
}


inline List *List_merge(List *left, List *right, List_compare cmp)
{
    List *result = List_create();
    void *val = NULL;

    while(List_count(left) > 0 || List_count(right) > 0) {
        if(List_count(left) > 0 && List_count(right) > 0) {
            if(cmp(List_first(left), List_first(right)) <= 0) {
                val = List_shift(left);
            } else {
                val = List_shift(right);
            }

            List_push(result, val);
        } else if(List_count(left) > 0) {
            val = List_shift(left);
            List_push(result, val);
        } else if(List_count(right) > 0) {
            val = List_shift(right);
            List_push(result, val);
        }
    }

    return result;
}

List *List_merge_sort(List *list, List_compare cmp)
{
    if(List_count(list) <= 1) {
        return list;
    }

    List *left = List_create();
    List *right = List_create();
    int middle = List_count(list) / 2;

    LIST_FOREACH(list, first, next, cur) {
        if(middle > 0) {
            List_push(left, cur->value);
        } else {
            List_push(right, cur->value);
        }

        middle--;
    }

    List *sort_left = List_merge_sort(left, cmp);
    List *sort_right = List_merge_sort(right, cmp);

    if(sort_left != left) List_destroy(left);
    if(sort_right != right) List_destroy(right);

    return List_merge(sort_left, sort_right, cmp);
}