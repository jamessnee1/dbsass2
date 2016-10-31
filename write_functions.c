/*write_functions.c*/
/*By James Snee s3369721*/
/*Database Systems Assignment 2*/
/*functions for writing data structures out to file*/

#include "master_header.h"


/*function to write individual node to file*/
void write_node_to_file(struct node *node, FILE *fp){

	/*fwrite(ptr to write, size in bytes of each element, number of elements, file pointer)*/
	fwrite(node, sizeof(node), 1, fp);

}

/*function to write btree out to file*/
void write_btree_to_file(struct node *root){

	struct node *current = NULL;
	FILE *fp;
	fp = fopen("output.bin", "wb");


	/*set root of tree to current*/
	current = root;

	/*iterate through the tree, find the leaf nodes and write them to the file*/
	while(current != NULL){

		/*only write out data if current node is a leaf node, that is, it contains data*/
		if(current->is_leaf == true){
			write_node_to_file(current, fp);
		}

		/*iterate through tree*/
		current = current->next;
	}

	fclose(fp);
	printf("File output.bin saved and closed.\n");


}

