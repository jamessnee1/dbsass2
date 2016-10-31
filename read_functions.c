/*read_functions.c*/
/*By James Snee s3369721*/
/*Database Systems Assignment 2*/

/*This file contains all the read functions of the assignment*/

#include "master_header.h"

/*function to read a header from heap file*/
struct heap_header * read_heap_header(FILE *fp){

	struct heap_header *header = NULL;
	header = malloc(sizeof(struct heap_header));
	fread(header, sizeof(struct heap_header), 1, fp);
	return header;
}

/*function to read a header from hash file*/
struct hash_header * read_hash_header(FILE *fp){

	struct hash_header *header = NULL;
	header = malloc(sizeof(struct hash_header));
	fread(header, sizeof(struct hash_header), 1, fp);
	return header;

}

/*function to read a heap page from file*/
struct heap_page * read_heap_page(FILE *fp, int pagesize, int size_record){

	struct heap_page *page = NULL;
	/*allocate enough memory for the pagesize*/
	page = malloc(pagesize);

	if(!page){
		perror("page memory allocation failed!");
		exit(EXIT_FAILURE);
	}

	fread(page, pagesize, NUM_PAGE_READ, fp);
	return page;

}

/*function to read a hash page from file*/
struct hash_page * read_hash_page(FILE *fp, int pagesize, int size_record){

	struct hash_page *page = NULL;
	/*allocate enough memory for the pagesize*/
	page = malloc(pagesize);

	if(!page){
		perror("page memory allocation failed!");
		exit(EXIT_FAILURE);
	}

	fread(page, pagesize, NUM_PAGE_READ, fp);
	return page;

}

