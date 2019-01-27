#ifndef _BTREE_H
#define _BTREE_H

#include "btreenode.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _BTREE BTREE;

/**
 * Allocates a new BTREE.<br>
 * Call btree_destroy after use.
 * @param data the data associated to the root node
 * @param destroy_data the function called to destroy the data
 * @param print_data the function called to print the data
 * @return the new allocated BTREE
 */
extern BTREE *btree_new(void *data, DESTROY_DATA_FCT destroy_data, PRINT_DATA_FCT print_data);

/**
 * Allocates a new BTREE and initialises values base on the given object
 * @param tree the tree to clone
 * @return the new tree
 */
extern BTREE *btree_clone(BTREE *tree);

/**
 * Destroys a BTREE and the tree data, if the param destroy_data is set
 * @param tree the tree to destroy
 * @param destroy_data if the data should be destroyed or not
 */
extern void btree_destroy(BTREE **tree, bool destroy_data);

/**
 * Compares two trees
 * @param tree1
 * @param tree2
 * @return if the given trees are equal
 */
extern bool btree_equals(BTREE *tree1, BTREE *tree2);

/**
 * @param tree
 * @return the root node of the given tree
 */
extern BTREE_NODE *btree_get_root(const BTREE *tree);

/**
 * Merges two trees
 * @param left the tree that goes to the left node
 * @param right the tree that goes to the right node
 * @param data the data for the new root node
 * @return the merged tree
 */
extern BTREE *btree_merge(BTREE *left, BTREE *right, void *data);

/**
 * prints a tree to the stdout
 * @param tree
 */
extern void btree_print(BTREE *tree);

#endif
