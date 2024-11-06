#ifndef A6_H
#define A6_H

#include <stdio.h>
#include <stdbool.h>

//structure declarations

//structure for tree node
//B: block node
//H: horizontal orientation node
//V: vertical orientation node
typedef struct tree_node{
    int label;
    int height;
    int width;
    int x;
    int y;
    char orientation;
    struct tree_node *left;
    struct tree_node *right;
} tree_node;

//structure for tree
typedef struct tree{
    tree_node *root;
} tree;

//structure for stack node
typedef struct stack_node {
    tree_node *tree_node;
    struct stack_node *next;
} stack_node;

//structure for stack
typedef struct stack {
    stack_node *top;
} stack;


//fuction declarations
tree_node* create_block_node(int label, int width, int height);
tree_node* create_orientation_node(char orientation, tree_node *left, tree_node *right);
void push(stack *stack, tree_node *node);
tree_node* pop(stack *stack);
void free_tree(tree_node *node);
void free_stack(stack *stack);
tree_node* build_tree(FILE *input_file);
void calculate_coordinate(tree_node *node, int x, int y);
void print_pre_order(tree_node *node, FILE *output_file);
void print_post_order(tree_node *node, FILE *output_file);
void print_coordinates(tree_node *node, FILE *output_file);

#endif