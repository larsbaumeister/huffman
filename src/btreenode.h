#ifndef _BTREENODE_H
#define _BTREENODE_H

#include <stdbool.h>


typedef struct _BTREE_NODE BTREE_NODE;

typedef void (*DESTROY_DATA_FCT)(void*);

typedef void (*PRINT_DATA_FCT)(void*);


/**
 * Allocates a new BTREE_NODE
 * @param data the data for the node
 * @return the new BTREE_NODE
 */
extern BTREE_NODE *btreenode_new(void *data);

/**
 * Clones a BTREE_NODE
 * @param node
 * @return the new allocated BTREE_NODE
 */
extern BTREE_NODE *btreenode_clone(const BTREE_NODE *node);

/**
 * Compares two tree nodes
 * @param node1
 * @param node2
 * @return if the tree nodes are equal or not
 */
extern bool btreenode_equals(const BTREE_NODE *node1, const BTREE_NODE *node2);

/**
 * destroys a tree node
 * @param node
 * @param destroy_data
 */
extern void btreenode_destroy(BTREE_NODE **node, DESTROY_DATA_FCT destroy_data);

/**
 * @param node
 * @return the data associated to the node
 */
extern void *btreenode_get_data(const BTREE_NODE *node);

/**
 * @param node
 * @return the left child of the node
 */
extern BTREE_NODE *btreenode_get_left(const BTREE_NODE *node);

/**
 * @param node
 * @return the right child of the node
 */
extern BTREE_NODE *btreenode_get_right(const BTREE_NODE *node);

/**
 * @param node
 * @return if the node has children or not
 */
extern bool btreenode_is_leaf(const BTREE_NODE *node);

/**
 * Sets the left child of the given node.
 * The child can only be set if it has not been set before.
 * @param parent_node
 * @param node
 * @return if the child was set
 */
extern bool btreenode_set_left(BTREE_NODE *parent_node, BTREE_NODE *node);

/**
 * Sets the right child of the given node.
 * The child can only be set if it has not been set before.
 * @param parent_node
 * @param node
 * @return if the child was set
 */
extern bool btreenode_set_right(BTREE_NODE *parent_node, BTREE_NODE *node);

/**
 * Prints the node to stdout
 * @param node
 * @param print_data
 */
extern void btreenode_print(const BTREE_NODE *node, PRINT_DATA_FCT print_data);

extern unsigned int btreenode_number_of_children(const BTREE_NODE* node);

#endif
