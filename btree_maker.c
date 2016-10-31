/*Btree Maker*/
/*By James Snee s3369721*/

#include "master_header.h"

/*Timing structs*/
struct timeval t;


int main(int argc, char *argv[]){

	int i = 0;
	FILE *fp;
	int filesize = 0, pagesize = 0;
	char *filename;

	/*Start timing*/
	gettimeofday(&t, NULL);
	double t1 = t.tv_sec+(t.tv_usec/1000000.0);

	/*check arguments for file name*/
	if (argc != 2){
		fprintf(stderr, "Usage: %s datafile\n", argv[0]);
		printf("Entered: ");
		for (i = 0; i < argc; i++){
			printf("%s ", argv[i]);
		}
		printf("\n");
		return EXIT_FAILURE;

	}
	else {
		/*store filename for use*/
		filename = argv[1];
		int ext = 0;
		/*get the page size from file name*/
		char fileext[5];
		/*check if our file extension is of length 4 or 5*/
		for(i = 0; i < strlen(filename); i++){

			if(isdigit(filename[i])){
				 fileext[ext] = filename[i];
				 ext++;
			}
		}

		/*convert file extension to an int for pagesize*/
		pagesize = atoi(fileext);

	}

	printf("B+Tree Maker\n");

	/*open input file*/
	fp = fopen(argv[1], "rb");

	/*error checking*/
	if (fp == NULL){
		perror("Error reading file.");
		return EXIT_FAILURE;
	}
	else {
		/*get file size*/
		fseek(fp, 0, SEEK_END);
		filesize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		printf("File %s open. Filesize is %d bytes.\n", argv[1], filesize);
	}

	printf("Pagesize detected: %d bytes.\n", pagesize);

	/*first, turn off OS buffering*/
	/*int setvbuf(FILE * stream, char * buffer, int mode, size_t size)*/
	/*modes: _IOFBF = full buffering, _IOLBF = line buffering, _IONBF = no buffering*/
	setvbuf(fp, NULL, _IOFBF, pagesize);

	
	printf("Creating tree from input file...\n");

	/*create a new node for the root of the tree*/
	/*struct node *root = NULL;*/

	/*first, get header from the heap file*/
	struct heap_header *header = read_heap_header(fp);
	print_heap_header(header);


	/*loop through file, a page at a time*/
	while(!feof(fp)){

		/*struct heap_page *page = read_heap_page(fp, pagesize, header->size_record);*/
		/*insert into node*/
		/*This is all discussion as the insert function has not been completed.*/
		/*this is what I would have done: 
		B+Tree is always Balanced. 
		Each node contains m entries, where d <= m <= 2d, leaf nodes are right at the bottom of tree.
		Internal nodes direct the search to the node we need to insert into.
		Leaves are joined by a doubly linked list for easy traversal, using the next pointer.
		Non leaf nodes contain m+1 offsets (pointers) to children separated by search key values.
		Node to left of key points to values less than key value, and right is greater than. 
		To insert, find correct leaf L, insert data if there is enough space. If not,
		split L into L and L2. Spread out keys evenly, copy up one level. Splitting a node
		grows the tree.
		Number of disk I/O's during a search is height of tree + num of leaf pages with our matches.
		Finding the correct leaf page is faster than Binary Search.
		*/
		/*print_page(page, header);*/

	}


	/*free memory*/
	free(header);
	
	
	/*Stop timing*/
	gettimeofday(&t, NULL);
    double t2 = t.tv_sec+(t.tv_usec/1000000.0);

    fclose(fp);

	printf("B+Tree Maker took %.6lf seconds to execute.\n", t2 - t1);

	return EXIT_SUCCESS;
}
