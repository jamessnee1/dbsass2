/*James Snee s3369721 Database Systems Assignment 2*/
/*dbload.c*/
/*Creates a heap file from an input file*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#pragma pack(1)
#define LONGEST_LINE 1070

struct Page {

	char data[LONGEST_LINE];
	int num_of_records;

};

int main(int argc, char *argv[]){

	/*file pointers*/
	FILE *fp;
	FILE *write_out;
	/*file variables*/
	long fileSize;
	char * fileBuffer;
	char fileLine[LONGEST_LINE];
	char ch;
	size_t result;
	int pageSize = 0, i = 0, num_of_lines = 0;
	int total_count = 0;
	int longest_line = 0;

	/*timing variables*/
	clock_t start, end;
	long elapsed;

	/*check arguments for file name*/
	if (argc != 4){
		fprintf(stderr, "Usage: %s [-p pagesize] datafile\n", argv[0]);
		printf("Entered: ");
		for (i = 0; i < argc; i++){
			printf("%s ", argv[i]);
		}
		printf("\n");
		return EXIT_FAILURE;

	}

	/*check for -p parameter*/
	if (strcmp(argv[1], "-p") != 0){
		fprintf(stderr, "Error: Incorrect or malformed -p option!\n");
		return EXIT_FAILURE;
	}

	/*page size variable to be used when allocating enough memory for all the records*/
	pageSize = atoi(argv[2]);

	/*page size as a string for file extension*/
	char fileExtension[10];
	strcpy(fileExtension, argv[2]);

	/*new file name*/
	char new_file_name[40];
	strcpy(new_file_name, "relation.");
	strcat(new_file_name, fileExtension);

	if (pageSize == 0){
		fprintf(stderr, "Error: Incorrect or malformed page size entered!\n");
		return EXIT_FAILURE;
	}
	
	fp = fopen(argv[3], "rb");
	if (fp == NULL){
		fprintf(stderr, "Error: File could not be opened!\n");
		return EXIT_FAILURE;
	}

	/*Open new file to write to*/
	write_out = fopen(new_file_name, "wb+");
	if (write_out == NULL){
		fprintf(stderr, "Error: File could not be created!\n");
		return EXIT_FAILURE;
	}

	printf("Working...\n");

	/*get number of lines in the file*/
	while(!feof(fp)){
		
		ch = fgetc(fp);
		
		if (ch == '\n'){
			num_of_lines++;
		}

	}

	/*get file size*/
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	rewind(fp);
	
	/*allocate memory for file*/
	fileBuffer = malloc(sizeof(char) * fileSize);

	/*copy the file into the buffer*/
	result = fread(fileBuffer, 1, fileSize, fp);
	if (result != fileSize){
		fprintf(stderr, "Error: File could not be read into memory buffer! Exiting....\n");
		free(fp);
		free(fileBuffer);
		return EXIT_FAILURE;
	}

	printf("File %s open. File is %li bytes.\n", argv[3], fileSize);

	/*Page struct*/
	struct Page page;

	/*get each line of file and save to a Page*/
	rewind(fp);

	/*start timing*/
	start = clock();

	while(fgets(fileLine, LONGEST_LINE, fp) != NULL){
		/*printf("%s\n", fileLine);*/

		/*set page to null*/
		memset(page.data, 0, LONGEST_LINE);

		/*copy file line into page struct*/
		strcpy(page.data, fileLine);

		/*write page to file*/
		fwrite((const void*)&page, sizeof(page), 1, write_out);

		int charCount = 0;

		/*get number of chars in each line*/
		for (i = 0; fileLine[i]; i++){

			if (fileLine[i] != ' '){
				charCount++;

			}
		
		}

		/*printf("Number of characters in line: %i\n", charCount);*/

		if (charCount > longest_line){

			longest_line = charCount;

		}

		total_count += charCount;

	}

	/*finish timing*/
	end = clock();

	elapsed = ((double)end - start) / CLOCKS_PER_SEC * 1000;


	/*variables for pages*/
	int num_of_records_per_page = pageSize / longest_line;
	int num_of_pages = num_of_lines / num_of_records_per_page;
	int total_number_of_chars_page = longest_line * num_of_records_per_page;
	

	printf("The total number of characters in the file is %i\n", total_count);
	printf("The longest line in the file is %i characters.\n", longest_line);
	printf("Number of records loaded: %i\n", num_of_lines);
	printf("Number of records per page: %i\n", num_of_records_per_page);
	printf("Number of pages used: %i\n", num_of_pages);
	printf("Number of characters per page file needed: %i\n", total_number_of_chars_page);



	fclose(write_out);
	fclose(fp);
	free(fileBuffer);

	printf("Number of milliseconds to create the heap file: %ld ms.\n", elapsed);

	return EXIT_SUCCESS;
}