// #ifndef _LCTHW_STACK_H
// #define _LCTHW_STACK_H

// #include <stdlib.h>  // For malloc and free functions

// // Define the node structure for the stack
// typedef struct StackNode {
//     struct StackNode *next;  // Pointer to the next node in the stack
//     void *value;             // Pointer to the value stored in the node
// } StackNode;

// // Define the stack structure
// typedef struct Stack {
//     int count;        // Number of elements in the stack
//     StackNode *top;   // Pointer to the top node of the stack
// } Stack;

// // Macro to get the current count of the stack
// #define Stack_count(A) ((A)->count)


// //Creates a new stack and initializes its members.

// static inline Stack *Stack_create()
// {
//     Stack *stack = (Stack *)malloc(sizeof(Stack));
//     if (stack == NULL) return NULL;  // Allocation failed
//     stack->count = 0;
//     stack->top = NULL;
//     return stack;
// }

// /**
//  * Destroys the stack and frees all allocated memory.
//  */
// static inline void Stack_destroy(Stack *stack)
// {
//     if (stack) {
//         StackNode *current = stack->top;
//         StackNode *next = NULL;

//         // Iterate over each node and free it
//         while (current) {
//             next = current->next;
//             free(current);
//             current = next;
//         }

//         // Free the stack structure itself
//         free(stack);
//     }
// }

// /**
//  * Pushes a new value onto the top of the stack.
//  *
//  * @param stack The stack to operate on.
//  * @param value The value to push onto the stack.
//  */
// static inline void Stack_push(Stack *stack, void *value)
// {
//     // Create a new node
//     StackNode *node = (StackNode *)malloc(sizeof(StackNode));
//     if (node == NULL) return;  // Allocation failed

//     node->value = value;
//     node->next = stack->top;  // Point to the previous top node
//     stack->top = node;        // Update the top to the new node
//     stack->count++;           // Increment the count
// }

// /**
//  * Pops the top value off the stack and returns it.
//  */
// static inline void *Stack_pop(Stack *stack)
// {
//     if (stack->top == NULL) return NULL;  // Stack is empty

//     StackNode *node = stack->top;
//     void *value = node->value;

//     stack->top = node->next;  // Move the top pointer to the next node
//     free(node);               // Free the old top node
//     stack->count--;           // Decrement the count

//     return value;
// }

// /**
//  * Peeks at the top value of the stack without removing it.
//  */
// static inline void *Stack_peek(Stack *stack)
// {
//     if (stack->top == NULL) return NULL;  // Stack is empty
//     return stack->top->value;             // Return the top value
// }

// /**
//  * Macro to iterate over each node in the stack.
//  *s
//  * @param S The stack to iterate over.
//  * @param V The variable name to use for the node in the loop.
//  */
// #define STACK_FOREACH(S, V) \
//     for (StackNode *V = (S)->top; V != NULL; V = V->next)

// #endif 


#ifndef _LCTHW_STACK_H
#define _LCTHW_STACK_H

#include <stdlib.h>
#include <assert.h>

/** DArray implementation **/

// Define the dynamic array structure
typedef struct DArray {
    int end;              // Index of the next empty slot
    int max;              // Maximum capacity of the array
    size_t element_size;  // Size of each element
    size_t expand_rate;   // Rate at which the array expands
    void **contents;      // Pointer to the array of elements
} DArray;

#define DEFAULT_EXPAND_RATE 50  // Default rate for expanding the array

/**
 * Creates a new dynamic array.
 *
 * @param element_size The size of each element in the array.
 * @param initial_max The initial maximum capacity of the array.
 * @return A pointer to the newly created dynamic array.
 */
static inline DArray *DArray_create(size_t element_size, size_t initial_max)
{
    DArray *array = malloc(sizeof(DArray));
    if (!array) return NULL;

    array->max = initial_max;
    array->end = 0;
    array->element_size = element_size;
    array->expand_rate = DEFAULT_EXPAND_RATE;

    array->contents = calloc(initial_max, sizeof(void *));
    if (!array->contents) {
        free(array);
        return NULL;
    }

    return array;
}

/**
 * Destroys the dynamic array and frees allocated memory.
 *
 * @param array The dynamic array to destroy.
 */
static inline void DArray_destroy(DArray *array)
{
    if (array) {
        if (array->contents) free(array->contents);
        free(array);
    }
}

/**
 * Expands the dynamic array when capacity is reached.
 *
 * @param array The dynamic array to expand.
 * @return 0 on success, -1 on failure.
 */
static inline int DArray_expand(DArray *array)
{
    size_t old_max = array->max;
    array->max += array->expand_rate;

    //void *realloc(void *ptr, size_t new_size); resize the previous allocate block
    void **contents = realloc(array->contents, array->max * sizeof(void *));
    if (!contents) return -1;

    array->contents = contents;
    memset(array->contents + old_max, 0, array->expand_rate * sizeof(void *));

    return 0;
}

/**
 * Contracts the dynamic array to free unused space.
 */
static inline int DArray_contract(DArray *array)
{
    int new_size = ( array->end < array->expand_rate) ? array->expand_rate : array->end;

    if (new_size < array->max) {
        void **contents = realloc(array->contents, new_size * sizeof(void *));
        if (!contents) return -1;

        array->contents = contents;
        array->max = new_size;
    }

    return 0;
}

/**
 * Adds an element to the end of the dynamic array.
 */
static inline int DArray_push(DArray *array, void *el)
{
    array->contents[array->end] = el;
    array->end++;

    if (array->end >= array->max) {
        return DArray_expand(array);
    } else {
        return 0;
    }
}

/**
 * Removes and returns the last element of the dynamic array.
 */
static inline void *DArray_pop(DArray *array)
{
    if (array->end == 0) {
        return NULL;
    } else {
        void *el = array->contents[array->end - 1];
        array->contents[array->end - 1] = NULL;
        array->end--;
    // If the array has fewer elements than the expand_rate, contracting doesn't provide significant memory savings and may introduce overhead
    // free rate is expand rate
        if (array->end > array->expand_rate && array->end % array->expand_rate) {
            DArray_contract(array);
        }

        return el;
    }
}

// Macros for convenience
#define DArray_last(A) ((A)->contents[(A)->end - 1])
#define DArray_end(A)  ((A)->end)
#define DArray_count(A) DArray_end(A)
#define DArray_max(A)  ((A)->max)

/**
 * Retrieves the element at the specified index.
 */
static inline void *DArray_get(DArray *array, int i)
{
    if (i >= array->end) return NULL;
    return array->contents[i];
}

/** Stack implementation using DArray **/

// Define the stack node structure
typedef struct StackNode {
    void *value;  // Pointer to the value stored in the node
} StackNode;

// Define the stack structure
typedef struct Stack {
    DArray *contents;  // Dynamic array to store stack nodes
} Stack;

// Macro to get the current count of the stack
#define Stack_count(A) DArray_count((A)->contents)

/**
 * Creates a new stack.
 */
static inline Stack *Stack_create()
{
    Stack *stack = malloc(sizeof(Stack));
    if (!stack) return NULL;

    stack->contents = DArray_create(sizeof(StackNode *), 50);
    if (!stack->contents) {
        free(stack);
        return NULL;
    }

    return stack;
}

static inline void Stack_destroy(Stack *stack)
{
    if (stack) {
        if (stack->contents) {
            // Free each stack node
            int i;
            for (i = 0; i < DArray_count(stack->contents); i++) {
                StackNode *node = DArray_get(stack->contents, i);
                if (node) free(node);
            }
            DArray_destroy(stack->contents);
        }
        free(stack);
    }
}

/**
 * Pushes a new value onto the top of the stack.
 */
static inline void Stack_push(Stack *stack, void *value)
{
    // Create a new stack node
    StackNode *node = malloc(sizeof(StackNode));
    if (!node) return;

    node->value = value;

    // Add the node to the dynamic array
    DArray_push(stack->contents, node);
}

/**
 * Pops the top value off the stack and returns it.
 */
static inline void *Stack_pop(Stack *stack)
{
    StackNode *node = DArray_pop(stack->contents);
    if (node) {
        void *value = node->value;
        free(node);
        return value;
    } else {
        return NULL;
    }
}

/**
 * Peeks at the top value of the stack without removing it.
 */
static inline void *Stack_peek(Stack *stack)
{
    StackNode *node = DArray_last(stack->contents);
    if (node) {
        return node->value;
    } else {
        return NULL;
    }
}

/**
 * Macro to iterate over each node in the stack.
 */
#define STACK_FOREACH(S, V) \
    for (size_t i = DArray_count((S)->contents); i > 0;) \
        for (StackNode *(V) = DArray_get((S)->contents, --i); (V); (V) = NULL)
        
#endif  // _LCTHW_STACK_H
