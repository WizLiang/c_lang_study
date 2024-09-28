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

int List_bubble_sort(List *list, List_compare cmp) {
    if (!list->first) {
        return 0; // empty list
    }
    
    ListNode *cur, *cur_next;
    int swap_flag;

    do {
        swap_flag = 0; // 初始化交换标志
        for (cur = list->first; cur->next != NULL; cur = cur->next) {
            cur_next = cur->next; // 获取当前节点的下一个节点
            
            if (cmp(cur->value, cur_next->value) > 0) {
                // 交换节点的值
                void *temp = cur->value;
                cur->value = cur_next->value;
                cur_next->value = temp;
                swap_flag = 1; // 发生交换
            }
        }
        print_list(list); // 打印列表状态
    } while (swap_flag); // 如果发生交换，继续循环

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


// inline List *List_merge(List *left, List *right, List_compare cmp)
// {
//     List *result = List_create();
//     void *val = NULL;

//     while(List_count(left) > 0 || List_count(right) > 0) {
//         if(List_count(left) > 0 && List_count(right) > 0) {
//             if(cmp(List_first(left), List_first(right)) <= 0) {
//                 val = List_shift(left);
//             } else {
//                 val = List_shift(right);
//             }

//             List_push(result, val);
//         } else if(List_count(left) > 0) {
//             val = List_shift(left);
//             List_push(result, val);
//         } else if(List_count(right) > 0) {
//             val = List_shift(right);
//             List_push(result, val);
//         }
//     }

//     return result;
// }


// List *List_merge_sort(List *list, List_compare cmp)
// {
//     if(List_count(list) <= 1) {
//         return list;
//     }

//     List *left = List_create();
//     List *right = List_create();
//     int middle = List_count(list) / 2;

//     LIST_FOREACH(list, first, next, cur) {
//         if(middle > 0) {
//             List_push(left, cur->value);
//         } else {
//             List_push(right, cur->value);
//         }

//         middle--;
//     }

//     List *sort_left = List_merge_sort(left, cmp);
//     List *sort_right = List_merge_sort(right, cmp);

//     if(sort_left != left) List_destroy(left);
//     if(sort_right != right) List_destroy(right);

//     return List_merge(sort_left, sort_right, cmp);
// }

// 合并两个已排序的链表
ListNode *List_merge(ListNode *left, ListNode *right, List_compare cmp) {
    ListNode *result = NULL;

    // 基本情况
    if (!left)
        return right;
    if (!right)
        return left;

    // 选择较小的节点
    if (cmp(left->value, right->value) <= 0) {
        result = left;
        result->next = List_merge(left->next, right, cmp);
        if (result->next)
            result->next->prev = result;
        result->prev = NULL; // 确保头节点的 prev 为空
    } else {
        result = right;
        result->next = List_merge(left, right->next, cmp);
        if (result->next)
            result->next->prev = result;
        result->prev = NULL; // 确保头节点的 prev 为空
    }
    return result;
}

// 拆分链表为两个子链表
void List_split_merge(ListNode *source, ListNode **frontRef, ListNode **backRef) {
    ListNode *fast;
    ListNode *slow;
    slow = source;
    fast = source->next;

    // 使用快慢指针找到中间节点
    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    // 分割链表
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
    if (*backRef)
        (*backRef)->prev = NULL;
}

// 递归排序函数
ListNode *List_merge_sort_recursive(ListNode *head, List_compare cmp) {
    if (!head || !head->next)
        return head;

    ListNode *left;
    ListNode *right;

    // 拆分链表
    List_split_merge(head, &left, &right);

    // 递归排序左右子链表
    left = List_merge_sort_recursive(left, cmp);
    right = List_merge_sort_recursive(right, cmp);

    // 合并已排序的子链表
    return List_merge(left, right, cmp);
}

// 主排序函数
List *List_merge_sort(List *list, List_compare cmp) {
    if (!list || list->count <= 1)
        return list;

    // 对链表进行排序
    list->first = List_merge_sort_recursive(list->first, cmp);

    // 重新设置 last 指针和 count
    ListNode *current = list->first;
    list->count = 0;
    ListNode *prev = NULL;
    while (current) {
        current->prev = prev;
        prev = current;
        current = current->next;
        list->count++;
    }
    list->last = prev;

    return list;
}

