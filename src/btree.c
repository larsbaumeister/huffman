//
// Created by Lars on 20.12.2018.
//
#include "btree.h"
#include "btreenode.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>

struct _BTREE
{
    BTREE_NODE* head;
    DESTROY_DATA_FCT destroy_function;
    PRINT_DATA_FCT  print_function;
};


BTREE* btree_new(void *data, DESTROY_DATA_FCT destroy_data, PRINT_DATA_FCT print_data)
{
    BTREE* tree = hf_malloc(sizeof(BTREE));

    tree->head = btreenode_new(data);
    tree->print_function = print_data;
    tree->destroy_function = destroy_data;
    return tree;
}

BTREE* btree_clone(BTREE* tree)
{
    BTREE* clone = hf_malloc(sizeof(BTREE));

    clone->print_function = tree->print_function;
    clone->destroy_function = tree->destroy_function;
    clone->head = btreenode_clone(tree->head);
    return clone;
}

void btree_destroy(BTREE** tree, bool destroy_data)
{
    btreenode_destroy( &((*tree)->head), destroy_data ? (*tree)->destroy_function : NULL );
    free(*tree);
    *tree = NULL;
}

bool btree_equals(BTREE* tree1, BTREE* tree2)
{
    return btreenode_equals(tree1->head, tree2->head);
}

BTREE_NODE* btree_get_root(const BTREE *tree)
{
    return tree->head;
}

BTREE *btree_merge(BTREE* left, BTREE* right, void* data)
{
    BTREE* merged_tree = btree_new(data, left->destroy_function, left->print_function);
    btreenode_set_left(merged_tree->head, left->head);
    btreenode_set_right(merged_tree->head, right->head);

    free(left);
    free(right);
    return merged_tree;
}

void btree_print(BTREE* tree)
{
    btreenode_print(tree->head, tree->print_function);
    printf("\n");
}

unsigned int btree_number_of_elements(BTREE* tree)
{
    BTREE_NODE* root = btree_get_root(tree);
    return btreenode_number_of_children(root) + 1;
}