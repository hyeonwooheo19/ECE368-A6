#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "a6.h"

//create block node of the tree. Contains label, height, width, x, y and orientation.
tree_node* create_block_node(int label, int width, int height){
    tree_node *new_node = malloc(sizeof(tree_node));
    new_node->label = label;
    new_node->height = height;
    new_node->width = width;
    new_node->x = 0;
    new_node->y = 0;
    new_node->orientation = 'B';
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

//create orientation node of the tree. Contains orientation, left and right subtree.
tree_node* create_orientation_node(char orientation, tree_node *left, tree_node *right){
    tree_node *new_node = malloc(sizeof(tree_node));
    new_node->orientation = orientation;
    new_node->left = left;
    new_node->right = right;

    if (orientation == 'H') {
        new_node->width = (left->width > right->width) ? left->width : right->width;
        new_node->height = left->height + right->height;
    } else if (orientation == 'V') {
        new_node->width = left->width + right->width;
        new_node->height = (left->height > right->height) ? left->height : right->height;
    }

    return new_node;
}

//push node in stack
void push(stack *stack, tree_node *node){
    stack_node *new_node = malloc(sizeof(stack_node));
    new_node->tree_node = node;
    new_node->next = stack->top;
    stack->top = new_node;
}

//pop node from stack
tree_node* pop(stack *stack){
    if(stack->top == NULL){
        return NULL;
    }

    stack_node *temp = stack->top;
    stack->top = stack->top->next;
    tree_node *node = temp->tree_node;
    free(temp);
    return node;
}

//build tree from input file.
//reads line by line and creates block node if line starts with digit.
//pushes node in stack.
//creates orientation node if line starts with H or V.
//pop two nodes from stack and create orientation node.
//push orientation node in stack.
//return root of the tree.
tree_node* build_tree(FILE *input_file){
    stack *stack = malloc(sizeof(stack));
    stack->top = NULL;

    tree_node *root = NULL;

    char line[100];
    while(fgets(line, 100, input_file) != NULL){
        if(isdigit(line[0])){
            int label;
            int height;
            int width;
            sscanf(line, "%d(%d,%d)", &label, &width, &height);
            tree_node *node = create_block_node(label, width, height);
            push(stack, node);
        } else if(line[0] == 'H' || line[0] == 'V'){
            tree_node *right = pop(stack);
            tree_node *left = pop(stack);
            tree_node *node = create_orientation_node(line[0], left, right);
            push(stack, node);
        }
    }

    root = pop(stack);
    free_stack(stack);
    return root;
}

//free tree
void free_tree(tree_node *node){
    if(node == NULL){
        return;
    }

    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

//free stack
void free_stack(stack *stack) {
    while (stack->top != NULL) {
        stack_node *temp = stack->top;
        stack->top = stack->top->next;
        free(temp);
    }
    free(stack);
}

//print tree in pre order
void print_pre_order(tree_node *node, FILE *output_file){
    if(node == NULL){
        return;
    }

    if(node->orientation == 'B'){
        fprintf(output_file, "%d(%d,%d)\n", node->label, node->width, node->height);
    } else {
        fprintf(output_file, "%c\n", node->orientation);
    }

    print_pre_order(node->left, output_file);
    print_pre_order(node->right, output_file);
}

//print tree in post order with height width of the orientation node.
void print_post_order(tree_node *node, FILE *output_file){
    if(node == NULL){
        return;
    }

    print_post_order(node->left, output_file);
    print_post_order(node->right, output_file);

    if(node->orientation == 'B'){
        fprintf(output_file, "%d(%d,%d)\n", node->label, node->width, node->height);
    } else {
        fprintf(output_file, "%c(%d,%d)\n", node->orientation, node ->width, node->height);
    }
}

//calculate x and y coordinate of the block node.
void calculate_coordinate(tree_node *node, int x, int y) {
    if (node == NULL) return;

    if (node->orientation == 'B') {
        node->x = x;
        node->y = y;
    } else if (node->orientation == 'H') {
        calculate_coordinate(node->left, x, y + node->right->height);
        calculate_coordinate(node->right, x, y);
    } else if (node->orientation == 'V') {
        calculate_coordinate(node->left, x, y);
        calculate_coordinate(node->right, x + node->left->width, y);
    }
}

//print coordinates of the block node.
void print_coordinates(tree_node *node, FILE *output_file) {
    if (node == NULL) return;

    if (node->orientation == 'B') {
        fprintf(output_file, "%d((%d,%d)(%d,%d))\n", node->label, node->width, node->height, node->x, node->y);
    } else {
        print_coordinates(node->left, output_file);
        print_coordinates(node->right, output_file);
    }
}

//main function
int main(int argc, char *argv[]) {
    if(argc != 5){
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");

    if(input_file == NULL){
        return 1;
    }

    tree_node *tree = build_tree(input_file);
    
    FILE *output_file1 = fopen(argv[2], "w");
    FILE *output_file2 = fopen(argv[3], "w");
    FILE *output_file3 = fopen(argv[4], "w");

    print_pre_order(tree, output_file1);
    print_post_order(tree, output_file2);
    calculate_coordinate(tree, 0, 0);
    print_coordinates(tree, output_file3);

    free_tree(tree);
    fclose(input_file);
    fclose(output_file1);
    fclose(output_file2);
    fclose(output_file3);
    return 0;
}