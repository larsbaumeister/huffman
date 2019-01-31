//
// Created by Lars on 20.12.2018.
//
#include "btreenode.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct _BTREE_NODE
{
    BTREE_NODE* left;
    BTREE_NODE* right;
    void* value;
};

BTREE_NODE* btreenode_new(void *data)
{
    BTREE_NODE* node = hf_malloc(sizeof(BTREE_NODE));
    node->value = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

BTREE_NODE* btreenode_clone(const BTREE_NODE *node)
{
    BTREE_NODE* clone = btreenode_new(node->value);
    clone->right = node->right == NULL
                        ? NULL
                        : btreenode_clone(node->right);

    clone->left = node->left == NULL
                        ? NULL
                        : btreenode_clone(node->left);

    return clone;
}

bool btreenode_equals(const BTREE_NODE *node1, const BTREE_NODE *node2)
{
    bool equals;

    if(node1 == NULL && node2 == NULL)
    {
        equals = true;
    }
    else if(node1 == NULL || node2 == NULL)
    {
        equals = false;
    }
    else
    {
        equals = (node1->value == node2->value)
                        && (btreenode_equals(node1->left, node2->left))
                        && (btreenode_equals(node1->right, node2->right));
    }
    return equals;
}

void btreenode_destroy(BTREE_NODE** node, DESTROY_DATA_FCT destroy_data)
{
    if(destroy_data != NULL)
        destroy_data((*node)->value);

    BTREE_NODE* left_node = (*node)->left;
    BTREE_NODE* right_node = (*node)->right;

    if(left_node != NULL)
        btreenode_destroy(&left_node, destroy_data);
    if(right_node != NULL)
        btreenode_destroy(&right_node, destroy_data);

    free(*node);
    *node = NULL;
}

void* btreenode_get_data(const BTREE_NODE *node)
{
    return node->value;
}

BTREE_NODE* btreenode_get_left(const BTREE_NODE *node)
{
    return node->left;
}

BTREE_NODE* btreenode_get_right(const BTREE_NODE *node)
{
    return node->right;
}

bool btreenode_is_leaf(const BTREE_NODE *node)
{
    return node->left == NULL && node->right == NULL;
}

bool btreenode_set_left(BTREE_NODE *parent_node, BTREE_NODE *node)
{
    if(parent_node->left != NULL) return false;

    parent_node->left = node;
    return true;
}

bool btreenode_set_right(BTREE_NODE *parent_node, BTREE_NODE *node)
{
    if(parent_node->right != NULL) return false;

    parent_node->right = node;
    return true;
}

void _btreenode_print(const BTREE_NODE *node, PRINT_DATA_FCT print_data, int num_tabs)
{
    if(node == NULL || node->value == NULL) return;

    for(int i = 0; i < num_tabs; i++)
        printf("\t");

    print_data(node->value);
    printf("\n");

    _btreenode_print(node->left, print_data, num_tabs + 1);
    _btreenode_print(node->right, print_data, num_tabs + 1);
}

void btreenode_print(const BTREE_NODE *node, PRINT_DATA_FCT print_data)
{
    _btreenode_print(node, print_data, 0);
}

unsigned int btreenode_number_of_children(const BTREE_NODE* node)
{
    BTREE_NODE* left_node = btreenode_get_left(node);
    BTREE_NODE* right_node = btreenode_get_right(node);

    unsigned int num = 0;
    if(left_node != NULL)
    {
        num += btreenode_number_of_children(left_node) + 1;
    }

    if(right_node != NULL)
    {
        num += btreenode_number_of_children(right_node) + 1;
    }

    return num;
}