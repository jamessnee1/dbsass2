/*heap_functions.c*/
/*By James Snee s3369721*/
/*Database Systems Assignment 2*/
/*this file contains all the functions to operate on heap files.*/
#include "master_header.h"


/*do a scan of the heap file and find result of query*/
void scan_heap_file(FILE *fp, int pagesize, double t1, double t2){

	t1 = start_timing();

	int i = 0, count = 0;

	printf("Scanning heap file...\n");

	/*first, get header from the heap file*/
	struct heap_header *header = read_heap_header(fp);
	print_heap_header(header);

	
	/*scan the file a page size at a time, and print to screen. We know this information from the heap header now.*/

	while(!feof(fp)){

		/*fread(pointer to read into, size of each element, number of elements, input file stream)*/
		struct heap_page *page = read_heap_page(fp, pagesize, header->size_record);

		/*iterate through page*/
		for(i = 0; i < header->records_per_page; i++){

			/*Store things to check here*/
			char *temp_desc = NULL;
			char *temp_loc_desc = NULL;

			temp_desc = malloc(DESCLEN + 1);
			temp_loc_desc = malloc(LOC_DESC_LEN + 1);

			if(!temp_desc && !temp_loc_desc){
				perror("page memory allocation failed!");
				exit(EXIT_FAILURE);
			}


			/*copy temp results*/
			strcpy(temp_desc, page->records[i].desc);
			strcpy(temp_loc_desc, page->records[i].location_desc);

			if(strcmp(temp_loc_desc, "RESIDENCE") == 0 && strstr(temp_desc, "HANDGUN") && temp_desc != NULL && temp_loc_desc != NULL){
				/*If we have found a record containing HANDGUN in the desc and RESIDENCE in location desc, increase count*/
				/*print_page(page, header);*/
				count++;
			}

			/*free temp variables*/
			free(temp_desc);
			free(temp_loc_desc);

		}

		/*free memory*/
		free(page);
		
		

	}

	printf("\n\n");
	printf("File scanned.\n");
	printf("\n\n");

	printf("Query: SELECT COUNT(Description) FROM CRIMES WHERE Description LIKE \"%%HANDGUN%%\" and Location Description = \"%%RESIDENCE%%\";\n");
	printf("The result of the query is: %d\n", count);


	t2 = stop_timing();
	printf("Scanning sorted heap file took %.6lf seconds to execute.\n", t2 - t1);

	free(header);
	printf("\n\n");

}

/*Binary search for sorted file. Designed for files sorted on Description*/
void binary_search(FILE *fp, int pagesize, int filesize, double t1, double t2){

	t1 = start_timing();

	int i = 0, count = 0;

	struct heap_header *header = read_heap_header(fp);
	print_heap_header(header);

	/*calculate where the middle page of the file resides in memory*/
	int first_page = 0;
	int last_page = header->total_pages - 1;
	int middle_page = (first_page + last_page) / 2;
	long int middle_in_bytes = header->page_size * middle_page;

	/*Store things to check here*/
	int result = 9012667;
	int result_to_check = 0;

	/*query for binary search is to find an int value*/
	printf("Query: SELECT * FROM CRIMES WHERE ID = 9012667;\n");

	while(first_page <= last_page){

		/*seek to middle of the file*/
		fseek(fp, middle_in_bytes, SEEK_CUR);
		/*read in the middle page of the file*/	
		struct heap_page *page = read_heap_page(fp, pagesize, header->size_record);
		/*print_heap_page(page, header);*/
		
		/*loop through page that was read in*/
		for(i = 0; i < header->records_per_page; i++){

			/*save result to temp variable*/
			result_to_check = atoi(page->records[i].id);
			

			/*search for values*/
			if(result_to_check < result){
				first_page = middle_page + 1;
			}
			else if(result_to_check == result){
				/*print_record(&page->records[i]);*/
				count++;

			}
			else {
				last_page = middle_page - 1;
			}

		}

		/*free current page*/
		free(page);
		/*move position of file pointer to new middle*/
		middle_page = (first_page + last_page) / 2;
		middle_in_bytes = header->page_size * middle_page;
	}

	printf("Found %d records.\n", count);
	
	t2 = stop_timing();
	printf("Binary search on sorted heap file took %.6lf seconds to execute.\n", t2 - t1);

	
	free(header);

	printf("\n\n");
}

/*Range Search for Heap file. We will do a range search based on a sorted heap file, looking for IDs between two values.*/
void range_search_heap_file(FILE *fp, int pagesize, double t1, double t2){

	t1 = start_timing();

	int i = 0, count = 0, temp_id = 0;
	int result1 = 10365347, result2 = 10452284;

	printf("Performing range search on heap file...\n");
	printf("SELECT ID FROM CRIMES WHERE ID BETWEEN 10365347 AND 10452284;\n");

	/*first, get header from the heap file*/
	struct heap_header *header = read_heap_header(fp);
	print_heap_header(header);

	

	while(!feof(fp)){

		/*fread(pointer to read into, size of each element, number of elements, input file stream)*/
		struct heap_page *page = read_heap_page(fp, pagesize, header->size_record);

		/*iterate through page*/
		for(i = 0; i < header->records_per_page; i++){

			/*Store things to check here*/
			char *temp_char_id = NULL;
			temp_char_id = malloc(IDLEN + 1);

			if(!temp_char_id){
				perror("temporary memory allocation failed!");
				exit(EXIT_FAILURE);
			}

			strcpy(temp_char_id, page->records[i].id);
			
			/*save ID temporarily for comparison*/
			temp_id = atoi(temp_char_id);
			/*if page contains a record between the two results, print them out on screen*/
			if(temp_id >= result1 && temp_id <= result2){
				/*print_record(&page->records[i]);*/
				count++;
			}

			free(temp_char_id);

		}

		free(page);

	}

	printf("Found %d records.\n", count);

	t2 = stop_timing();
	printf("Range search on sorted heap file took %.6lf seconds to execute.\n", t2 - t1);
	printf("\n\n");

}


/*Equality Search for Heap file. We will do an equality search based on a sorted heap file, looking a specific ID.*/
void equality_search_heap_file(FILE *fp, int pagesize, double t1, double t2){

	t1 = start_timing();

	int i = 0, count = 0;
	int result = 10441580;

	printf("Performing equality search on heap file...\n");
	printf("SELECT ID FROM CRIMES WHERE ID = ""10441580"";\n");

	/*first, get header from the heap file*/
	struct heap_header *header = read_heap_header(fp);
	print_heap_header(header);


	while(!feof(fp)){

		/*fread(pointer to read into, size of each element, number of elements, input file stream)*/
		struct heap_page *page = read_heap_page(fp, pagesize, header->size_record);

		/*iterate through page*/
		for(i = 0; i < header->records_per_page; i++){

			/*save description temporarily for comparison*/
			char *temp_char_id = NULL;

			temp_char_id = malloc(IDLEN + 1);

			if(!temp_char_id){
				perror("temporary memory allocation failed!");
				exit(EXIT_FAILURE);
			}
			strcpy(temp_char_id, page->records[i].id);
			/*convert to int*/

			int temp_id = atoi(temp_char_id);
			/*if page contains a record between the two results, print them out on screen*/
			if(temp_id == result){
				print_record(&page->records[i]);
				count++;
			}

			/*free temp variables*/
			free(temp_char_id);

		}

		free(page);
	}

	printf("Found %d records.\n", count);

	t2 = stop_timing();
	printf("Equality search on sorted heap file took %.6lf seconds to execute.\n", t2 - t1);
	printf("\n\n");


}
