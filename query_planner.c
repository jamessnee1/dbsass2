/*Query Planner*/
/*By James Snee s3369721*/
/*Database Systems Assignment 2*/

#include "master_header.h"

int pagesize = 0;
double t1, t2 = 0;
/*timeval struct*/
struct timeval t;

/*Start timing function*/
double start_timing(){

	/*Start timing*/
	gettimeofday(&t, NULL);
	t1 = t.tv_sec+(t.tv_usec/1000000.0);

	return t1;

}

/*Stop timing function*/
double stop_timing(){

	/*Stop timing*/
	gettimeofday(&t, NULL);
    t2 = t.tv_sec+(t.tv_usec/1000000.0);

    return t2;

}

/*hash function as provided in assignment spec*/
unsigned long sdbm_hash(const byte *data, size_t len){

	unsigned long hash = 0;
	size_t count;

	for(count = 0; count < len; ++count){
		hash = data[count] + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}


/*do a scan of the hash file and print it on screen*/
void scan_hash_file(FILE *fp, int pagesize){

	t1 = start_timing();

	printf("Scanning hash file...\n");

	/*get header from hash file*/
	struct hash_header *header = read_hash_header(fp);
	print_hash_header(header);

	/*loop through file and unmarshall each page*/
	while(!feof(fp)){

		
		struct hash_page *page = read_hash_page(fp, pagesize, header->size_record);
		printf("Page address: %zu\n", page->page_address);
		/*cast the byte data[] to a struct RID*/
		struct rid *rid =(struct rid *) &page->data;
		/*generate hash value from rid*/
		unsigned long result = sdbm_hash(rid->key, rid->size_key);
		/*this hash value is the offset of the page*/
		printf("Heap Page Offset (fseek location): %zu\n ", rid->offset.page_offset);
		printf("Size of key: %zu\n", rid->size_key);
		printf("Result is %lu\n", result);

		free(page);

	}



	t2 = stop_timing();
	printf("Scanning hash index took %.6lf seconds to execute.\n", t2 - t1);

}




int main(int argc, char *argv[]){

	int choice = 0, i = 0, filesize = 0;
	char input;
	char *error = "Error: Not a valid option! Please try again.\n";
	char *filename;
	FILE *fp = NULL;

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

		/*open file*/
		fp = fopen(filename, "rb");

		if (fp == NULL){
			perror("Error");
			return EXIT_FAILURE;
		}
		else {
			/*get file size*/
			fseek(fp, 0, SEEK_END);
			filesize = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			
		}
		
	}

	do {

		printf("Query Planner\n");
		printf("-------------\n\n");

		printf("File %s open. Filesize is %d bytes.\n", filename, filesize);
		printf("Pagesize detected: %d bytes.\n\n", pagesize);

		printf("1) Hash index\n");
		printf("2) Btree index\n");
		printf("3) Sorted file\n");
		printf("4) Exit program\n\n");

		printf("Select your option: ");

		fgets(&input, MENU_INPUT + EXTRA_SPACES, stdin);

		choice = atoi(&input);

		switch(choice){

			case 0:
				printf("%s\n\n", error);
				break;
			case 1:
				printf("Hash index\n");
				scan_hash_file(fp, pagesize);
				/*rewind file to beginning*/
				fseek(fp, 0, SEEK_SET);
				break;
			case 2:
				t1 = start_timing();

				printf("\nBtree index\n");
				printf("This function is not implemented. Returning to menu.\n");
				/*rewind file to beginning*/
				fseek(fp, 0, SEEK_SET);

				t2 = stop_timing();

    			printf("Btree index took %.6lf seconds to execute.\n", t2 - t1);
				break;
			case 3:
				printf("\nSorted heap file\n");
				scan_heap_file(fp, pagesize, t1, t2);
				/*rewind file to beginning*/
				fseek(fp, 0, SEEK_SET);
				printf("\nBinary Search\n");
				binary_search(fp, pagesize, filesize, t1, t2);
				/*rewind file to beginning*/
				fseek(fp, 0, SEEK_SET);
				printf("\nRange Search\n");
				range_search_heap_file(fp, pagesize, t1, t2);
    			/*rewind file to beginning*/
				fseek(fp, 0, SEEK_SET);
				printf("\nEquality Search\n");
				equality_search_heap_file(fp, pagesize, t1, t2);
    			/*rewind file to beginning*/
				fseek(fp, 0, SEEK_SET);
				break;
			case 4:
				fclose(fp);
				return EXIT_SUCCESS;
				break;
			default:
				printf("%s\n\n", error);
				break;					
		}

	}
	while(choice != 4);

	
	return EXIT_SUCCESS;
}