/*print_functions.c*/
/*By James Snee s3369721*/
/*Database Systems Assignment 2*/

/*This file contains all the print functions of the assignment. These are mainly for testing*/

#include "master_header.h"

/*function to print individual heap pages, for testing*/
void print_heap_page(struct heap_page *page, struct heap_header *header){

	int i = 0;
	for(i = 0; i < header->records_per_page; i++){

		printf("%s, %s, %li, %s, %s, %s, %s, %s, %d, %d, %s, %s, %s, %s, %s\n", page->records[i].id, page->records[i].case_no, 
		page->records[i].date, page->records[i].block, page->records[i].iucr, page->records[i].type, page->records[i].desc,
		page->records[i].location_desc, (int)page->records[i].arrest, (int)page->records[i].domestic, page->records[i].beat, 
		page->records[i].district, page->records[i].ward, page->records[i].comm_area, page->records[i].fbi_code);

	}

	printf("\n");

}

/*function to print individual hash pages, for testing*/
void print_hash_page(struct hash_page *page, struct hash_header *header){

	int i = 0;
	/*iterate through*/
	for(i = 0; i < header->max_records_per_page; i++){

		printf("%hhd", page->data[i]);
		
	}

	printf("\n");

}

/*function to print individual page, for testing*/
void print_record(struct record *record){

	printf("%s, %s, %li, %s, %s, %s, %s, %s, %d, %d, %s, %s, %s, %s, %s\n", record->id, record->case_no, 
		record->date, record->block, record->iucr, record->type, record->desc,
		record->location_desc, (int)record->arrest, (int)record->domestic, record->beat, 
		record->district, record->ward, record->comm_area, record->fbi_code);

}

/*function to print heap header*/
void print_heap_header(struct heap_header *header){

	printf("Heap header: Total pages: %zu, Records per page: %zu, Page size: %zu, Total records: %zu, Size of records: %zu\n", 
		header->total_pages, header->records_per_page, header->page_size, header->total_records, header->size_record);

}

/*function to print hash header*/
void print_hash_header(struct hash_header *header){

	printf("Hash header: Max records: %zu, Max records per page: %zu, Index key: %d, Size of records: %zu, Number of pages: %zu, Page size: %zu\n", 
		header->max_records, header->max_records_per_page, header->index_key, header->size_record, header->num_pages, header->page_size);

}


