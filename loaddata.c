#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "tokenize.h"
#include "btree_header.h"


int main(int argc, char *argv[]) {

	FILE * fp;
	fp = fopen("crimessampledata.csv", "r");
	int filesize = 0;
	
	if (fp == NULL){
		perror("Error");
		return EXIT_FAILURE;
	}
	else {
		/*get file size*/
		fseek(fp, 0, SEEK_END);
		filesize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		printf("File open. Filesize is %d bytes.\n", filesize);
	}

	/*set pagesize*/
	int pagesize = 1024;
	
	/*malloc a record*/
	struct record *record = malloc(sizeof(struct record));
	
	/*set buffer size*/
	/*int setvbuf(FILE * stream, char * buffer, int mode, size_t size)*/
	/*modes: _IOFBF = full buffering, _IOLBF = line buffering, _IONBF = no buffering*/
	setvbuf(fp, NULL, _IOFBF, 1024);
	
	printf("Size of a record is %lu bytes.\n", sizeof(struct record));
	printf("The page size is %lu bytes. We can get %d records into a page.\n", pagesize, pagesize/sizeof(struct record));
	printf("Filesize is %d bytes.\n", filesize);
	
	/*read file into a buffer*/
	char *buffer = malloc(filesize);
	/*fread(ptr to read, size of each element, number of elements, stream (fp))*/
	fread(buffer, sizeof(struct record), filesize/sizeof(struct record), fp);

	/*get num of records in file*/
	int i = 0, count = 1;

	for(i = 0; i < filesize; i++){
		if (buffer[i] == '\n'){
			count++;
		}
	}
	
	printf("There are %d records in this file.\n", count);
	
	/*tokenize the file into an array*/
	struct token_vector *tok = malloc(sizeof(struct record) * count);

	tokenize(buffer, '\n');

	print_vector(tok);



	free_vector(tok);
	free(record);
	free(buffer);
	fclose(fp);
	printf("File closed.\n");

	return EXIT_SUCCESS;
}

