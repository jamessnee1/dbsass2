/*btree_functions.c*/
/*By James Snee s3369721*/
/*Database Systems Assignment 2*/
/*this file includes functions for creating the B+tree, and inserting to it. Writing out to file is contained in write_functions.c*/

#include "master_header.h"

/*function to create new node*/
struct node * make_node(){

	/*allocate memory*/
	struct node *new_node = NULL;
	new_node = malloc(sizeof(struct node));

	if(new_node == NULL){
		perror("Node creation failed!");
		exit(EXIT_FAILURE);
	}

	/*2d keys, where d is the order of the tree*/
	new_node->keys = malloc((ORDER_OF_TREE - 1) * sizeof(int));
	if(new_node->keys == NULL){
		perror("Could not allocate memory for keys!");
		exit(EXIT_FAILURE);
	}

	/*2d+1 pointers to pages, where d is the order of the tree*/
	new_node->page_pointers = malloc(ORDER_OF_TREE * sizeof(struct heap_page) + 1);
	if(new_node->page_pointers == NULL){
		perror("Could not allocate memory for pointers!");
		exit(EXIT_FAILURE);
	}

	/*set everything to NULL or 0*/
	new_node->is_leaf = false;
	new_node->num_keys = 0;
	new_node->parent = NULL;
	new_node->next = NULL;

	return new_node;

}

/*function to make leaf node*/
struct node * make_leaf_node(){

	/*create new leaf node using the other function then set it to a leaf*/
	struct node *leaf = make_node();
	leaf->is_leaf = true;

	return leaf;
}

/*function to insert a page into a node of the tree*/
struct node * insert_into_node(struct node *root, struct heap_page *page, struct heap_header *header){

	struct node *current = NULL;

	/*check if root is occupied*/
	if(root == NULL){
		root = make_leaf_node();
		/*todo: 2d+1 pages in each node*/
		root->page_pointers[0] = page;
		root->parent = NULL;
		root->num_keys++;
		return root;
	}
	/*if root is occupied, create new node and insert as a child of root*/
	else {
		current = make_leaf_node();
		current->page_pointers[0] = page;
		
		/*if root has no children, then make this one the child of root, if not, traverse the tree to find appropriate spot*/
		if(current->parent == NULL){
			current->parent = root;
		}
		else {

			while(current != NULL){

				if(current->next == NULL){
					/*insert here*/


				}

				current = current->next;
			}


		}
		/*check header for how many records per page, and add them to the number of keys in the current node*/
		current->num_keys = header->records_per_page;
		return current;

	}

}

/*function to insert node into B+ tree, 1 if successful, 0 if not*/
int insert_into_tree(struct node *root, struct node *data){

	/*this will most likely just be pseudocode, based on the book*/
	/*struct node *newchildentry = NULL;*/

	/*if root is a non leaf node*/
	if(!root->is_leaf){

		/**/

	}
	else {
		/*root is a leaf node and has space*/
		if(root != NULL){

		}
	}


	return 0;
}

/*free tree data*/
void free_btree(struct node *root){

	int i = 0;

	/*check if the node is a leaf node containing data*/
	if(root->is_leaf){

		for(i = 0; i < root->num_keys; i++){
			free(root->page_pointers[i]);
		}

	}

	free(root->page_pointers);
	free(root->keys);
	free(root);
}