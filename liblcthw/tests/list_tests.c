#include "minunit.h"
#include "lcthw/list.h"
//#include "list.h"
#include <assert.h>

static List *list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";


char *test_create()
{
    list = List_create();
    mu_assert(list != NULL, "Failed to create list.");

    return NULL;
}


char *test_destroy()
{
    //List_clear_destroy(list); //node->value is a string located in static Memory which can not be  used free
    List_destroy(list);
    return NULL;

}


char *test_push_pop()
{
    List_push(list, test1);
    mu_assert(List_last(list) == test1, "Wrong last value.");

    List_push(list, test2);
    mu_assert(List_last(list) == test2, "Wrong last value");

    List_push(list, test3);
    mu_assert(List_last(list) == test3, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on push.");

    char *val = List_pop(list);
    mu_assert(val == test3, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test2, "Wrong value on pop.");

    val = List_pop(list);
    mu_assert(val == test1, "Wrong value on pop.");
    mu_assert(List_count(list) == 0, "Wrong count after pop.");

    return NULL;
}

char *test_unshift()
{
    List_unshift(list, test1);
    mu_assert(List_first(list) == test1, "Wrong first value.");

    List_unshift(list, test2);
    mu_assert(List_first(list) == test2, "Wrong first value");

    List_unshift(list, test3);
    mu_assert(List_first(list) == test3, "Wrong last value.");
    mu_assert(List_count(list) == 3, "Wrong count on unshift.");

    return NULL;
}

char *test_remove()
{
    // we only need to test the middle remove case since push/shift
    // already tests the other cases

    char *val = List_remove(list, list->first->next);
    mu_assert(val == test2, "Wrong removed element.");
    mu_assert(List_count(list) == 2, "Wrong count after remove.");
    mu_assert(List_first(list) == test3, "Wrong first after remove.");
    mu_assert(List_last(list) == test1, "Wrong last after remove.");

    return NULL;
}


char *test_shift()
{
    mu_assert(List_count(list) != 0, "Wrong count before shift.");

    char *val = List_shift(list);
    mu_assert(val == test3, "Wrong value on shift.");

    val = List_shift(list);
    mu_assert(val == test1, "Wrong value on shift.");
    mu_assert(List_count(list) == 0, "Wrong count after shift.");

    return NULL;
}

char *test_concat(){
    List *list1 = List_create();
    List *list2 = List_create();

    List_push(list1,"list1 data1");
    List_push(list1,"list1 data2");
    List_push(list1,"list1 data3");

    List_push(list2,"list2 data1");
    List_push(list2,"list2 data2");

    List_concat(list1,list2);

    mu_assert(List_count(list1) == 5, "Concatenated list has wrong count.");
    mu_assert(List_first(list1) == "list1 data1", "First element of concatenated list is wrong.");
    mu_assert(List_last(list1) == "list2 data2", "Last element of concatenated list is wrong.");

    // Ensure list2 remains unaffected (if you choose not to modify it)
    mu_assert(List_count(list2) == 0, "List2 should be empty.");

//debug
    printf("Before destroying list1: count = %d, first = %p, last = %p\n", List_count(list1), list1->first, list1->last);
    printf("List1 nodes before destroy:\n");

    LIST_FOREACH(list1, first, next, cur) {
    printf("Node: %p, value: %p, prev: %p, next: %p\n", cur, cur->value, cur->prev, cur->next);
}


    List_destroy(list1);
    List_destroy(list2);

    return NULL;

}


char *test_split()
{
    List_push(list, "data1");
    List_push(list, "data2");
    List_push(list, "data3");
    List_push(list, "data4");

    ListNode *node = list->first->next;  
    List *new_list = List_split(list, node);

    mu_assert(List_count(list) == 2, "Original list after split has wrong count.");
    mu_assert(List_count(new_list) == 2, "New list after split has wrong count.");
    mu_assert(List_first(list) == "data1", "Original list first element is wrong after split.");
    mu_assert(List_last(list) == "data2", "Original list last element is wrong after split.");
    mu_assert(List_first(new_list) == "data3", "New list first element is wrong after split.");
    mu_assert(List_last(new_list) == "data4", "New list last element is wrong after split.");

    List_destroy(new_list);

    return NULL;
}


char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_push_pop);
    mu_run_test(test_unshift);
    mu_run_test(test_remove);
    mu_run_test(test_shift);

    mu_run_test(test_concat);
    mu_run_test(test_split);

    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);