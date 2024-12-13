#include<stdio.h>
#include<stdlib.h>

struct node
{
    int data;
    char name[15];
    struct node *left_child;
    struct node *right_child;
};

void insert(node ** binary_tree, int value) {
node *tmp = NULL;
if(!(*binary_tree)) {
tmp = (node *)malloc(sizeof(node));
tmp->left = tmp->right = NULL;
tmp->data = value;
*binary_tree = tmp;
return;
}
if(value < (*binary_tree)->data) {
insert(&(*binary_tree)->left, value);
} else if(value > (*binary_tree)->data) {
insert(&(*binary_tree)->right, value);
}
}

node *search(node ** binary_tree, int value) {
if(!(*binary_tree)) {
return NULL;
}
if(value == (*binary_tree)->data) {
return *binary_tree;
} else if(value < (*binary_tree)->data) {
search(&((*binary_tree)->left), value);
} else if(value > (*binary_tree)->data){
search(&((*binary_tree)->right), value);
}
}

void delete_tree(node * binary_tree) {
if (binary_tree) {
delete_tree(binary_tree->left);
delete_tree(binary_tree->right);
free(binary_tree);
}
}
void display_preorder(node * binary_tree) {
if (binary_tree) {
printf("%d\n",binary_tree->data);
display_preorder(binary_tree->left);
display_preorder(binary_tree->right);
}
}
void display_inorder(node * binary_tree) {
if (binary_tree) {
display_inorder(binary_tree->left);
printf("%d\n",binary_tree->data);
display_inorder(binary_tree->right);
}
}
void display_postorder(node * binary_tree) {
if (binary_tree) {
display_postorder(binary_tree->left);
display_postorder(binary_tree->right);
printf("%d\n",binary_tree->data);
}
}