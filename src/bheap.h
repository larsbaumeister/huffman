#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <stdbool.h>

typedef struct
{
    /** pointer to the head of the heap */
    void** head;

    /** contains the real size of the heap in memory */
    int allocated_size;

    /** number of elements in the heap */
    int num_elements;
}
BINARY_HEAP;

/**
 * function that compares two values.<br>
 * @return <ul>
 *  <li>a value &lt 0, if the first argument is smaller than the second</li>
 *  <li>a value &gt 0, if the first argument is larger than the second</li>
 *  <li>0, if the first argument is equal to the second</li>
 * </ul>
 */
typedef int (*HEAP_ELEM_COMP) (void*, void*);

/**
 * prints one element of the tree
 */
typedef void (*HEAP_ELEM_PRINT) (void*);

/**
 * Initializes the heap<br>
 * <b>IMPORTANT: call <code>heap_destroy()</code> after use!</b>
 * @param comp the comparator function, to sort the heap
 * @param print the print function, used to print the heap to the console
 */
void heap_init(BINARY_HEAP* heap, HEAP_ELEM_COMP comp, HEAP_ELEM_PRINT print);

/**
 * Frees the heap from memory.<br>
 * No heap operation (except <code>heap_init()</code>) can be used after calling this function.
 */
void heap_destroy(BINARY_HEAP* heap);

/**
 * Inserts an element to the heap.<br>
 * It will automatically reorder the tree.
 * @param element the element to be inserted
 */
void heap_insert(BINARY_HEAP* heap, void* element);

/**
 * Extracts the min values of the heap 
 * @param min_element out parameter, that contains the min heap value if the function returns true
 * @return true if an element was extracted, false if not
 */
bool heap_extract_min(BINARY_HEAP* heap, void** min_element);

/**
 * Prints the heap to the console.<br>
 * Make sure to set a correct <code>HEAP_ELEM_PRINT</code> function.
 */
void heap_print(BINARY_HEAP* heap);

/**
 * Peek at the top element of the heap
 * @param min_element
 * @return if the heap contains elements or not
 */
bool heap_peek(BINARY_HEAP* heap, void** min_element);

#endif
