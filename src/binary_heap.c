#include "binary_heap.h"
#include "common.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_SIZE 256
#define INCREASE_SIZE 128

/**
 * The parent of the given node
 */
#define PARENT(idx) (((idx) - 1) / 2)

/**
 * The left child of the given node
 */
#define LEFT_CHILD(idx) ((2 * (idx)) + 1)

/**
 * The right child of the given node
 */
#define RIGHT_CHILD(idx) ((2 * (idx)) + 2)

/**
 * Swaps two elements in an array. The array must contain pointers (type doesn't matter)
 */
#define SWAP(arr, i, j) {\
                        void* cache = (void*) arr[i];\
                        arr[i] = arr[j];\
                        arr[j] = cache;\
                        }

static HEAP_ELEM_COMP comparator;
static HEAP_ELEM_PRINT print_function;


/**
 * @param i the index of the parent node
 * @return the index of the min child
 */
int min_child(BINARY_HEAP* heap, int i)
{
    int min_idx;
    int left_idx = LEFT_CHILD(i);
    int right_idx = RIGHT_CHILD(i);
    
    if(left_idx < heap->num_elements && right_idx < heap->num_elements)
    {
        // if both children exists, compare which one is smalled
        min_idx = comparator((heap->head)[left_idx], (heap->head)[right_idx]) > 0
                    ? right_idx 
                    : left_idx;
    }
    else if(left_idx < (heap->num_elements) && right_idx >= (heap->num_elements))
    {
        // if only the left child exists, it must be the min child
        min_idx = left_idx;
    }
    // it is not possible that only the right child exists, so no check is required
    else
    {
        // in this case the parent node has no children
        min_idx = -1;
    }
    return min_idx;
}

void heap_print_(BINARY_HEAP* heap, int idx, int num_tabs)
{
    void* parent_elem = idx <= heap->num_elements ? (heap->head)[idx] : NULL;
    void* left_elem = LEFT_CHILD(idx) < heap->num_elements ? (heap->head)[LEFT_CHILD(idx)] : NULL;
    void* right_elem = RIGHT_CHILD(idx) < heap->num_elements ? (heap->head)[RIGHT_CHILD(idx)] : NULL;
        
    if(parent_elem != NULL)
    {
        printf("╚══ ");
        print_function(parent_elem);
        printf("\n");
    }
    if(left_elem != NULL)
    {
        for(int i = 0; i < num_tabs; i++) 
        {
            printf("\t");
        }
        heap_print_(heap, LEFT_CHILD(idx), num_tabs + 1);
    }
    if(right_elem != NULL)
    {
        for(int i = 0; i < num_tabs; i++) 
        {
            printf("\t");
        }
        heap_print_(heap, RIGHT_CHILD(idx), num_tabs + 1);
    }
}

void heap_print(BINARY_HEAP* heap)
{
    printf("\n");
    heap_print_(heap, 0, 1);
    printf("\n");
}

void heap_init(BINARY_HEAP* heap, HEAP_ELEM_COMP comp, HEAP_ELEM_PRINT print)
{
    // allocate memory for the heap
    heap->allocated_size = DEFAULT_SIZE;
    heap->head = hf_malloc(sizeof (void*) * heap->allocated_size);

    comparator = comp;
    print_function = print;
}

void heap_destroy(BINARY_HEAP* heap)
{
    free(heap->head);
    
    // reset default values for later use
    heap->head = NULL;
    heap->allocated_size = 0;
    heap->num_elements = 0;
}

void heap_insert(BINARY_HEAP* heap, void* element)
{
    // check if the heap is large enough
    if (heap->num_elements + 2 > heap->allocated_size)
    {
        heap->allocated_size += INCREASE_SIZE;
        heap->head = hf_realloc(heap->head, heap->allocated_size * sizeof (void*));
    }

    // insert element at the last position
    int idx = (heap->num_elements)++;
    (heap->head)[idx] = element;

    // recreate a valid heap (aka bubble up)
    for(int parent_idx = PARENT(idx);
        idx != 0 && comparator((heap->head)[parent_idx], (heap->head)[idx]) > 0; // as long as the parent is larger than the child
        parent_idx = PARENT(idx))
    {
        SWAP(heap->head, idx, parent_idx);
        idx = parent_idx;
    }
}

bool heap_extract_min(BINARY_HEAP* heap, void **min_element)
{
    if(heap->num_elements <= 0)
    {
        // the heap is empty, there is no min element
        *min_element = NULL;
        return false;
    }
    else if(heap->num_elements == 1)
    {
        // the heap contains only one element, we don't need to recreate a valid heap
        *min_element = (heap->head)[0];
        (heap->num_elements)--;
        return true;
    }
    
    // set the parameter to the min value
    *min_element = (heap->head)[0];
    
    // make the last element the new top element
    (heap->num_elements)--;
    (heap->head)[0] = (heap->head)[heap->num_elements];
    (heap->head)[heap->num_elements] = NULL;
    
    // now the heap may be invalid, we need to decrease the top node till
    // it's in the correct position (aka bubble down)
    int parent_idx = 0;
    int min_child_idx;
    while( ((min_child_idx = min_child(heap, parent_idx)) != -1) &&                 // while the parent node has children
           (comparator((heap->head)[parent_idx], (heap->head)[min_child_idx]) > 0)) // and the min child is larger than the parent
    {
        SWAP(heap->head, parent_idx, min_child_idx);
        parent_idx = min_child_idx;
    }
    
    return true;
}

bool heap_peek(BINARY_HEAP* heap, void** min_element)
{
    if(heap->num_elements <= 0)
    {
        *min_element = NULL;
        return false;
    }
    else
    {
        *min_element = (heap->head)[0];
        return true;
    }
}
