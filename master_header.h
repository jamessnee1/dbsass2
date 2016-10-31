#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <ctype.h>
#include <math.h>


#define IDLEN 8
#define CASELEN 8
#define DATELEN sizeof(time_t)
#define IUCRLEN 4
#define BLOCKLEN 38
#define TYPELEN 33
#define DESCLEN 59
#define LOC_DESC_LEN 47
#define ARREST_LEN sizeof(bool)
#define DOMESTIC_LEN sizeof(bool)
#define BEATLEN 10
#define DISTRICTLEN 10
#define WARDLEN 5
#define COMM_AREA_LEN 5
#define FBI_CODE_LEN 4

#define MENU_INPUT 1
#define EXTRA_SPACES 2

# define NUM_FIELDS 15

#define NUM_PAGE_READ 1

/*rename char to byte*/
typedef char byte;

/*record struct*/

struct record {

	char id[IDLEN + 1];
	char case_no[CASELEN + 1];
	time_t date;
	char block[BLOCKLEN + 1];
	char iucr[IUCRLEN + 1];
	char type[TYPELEN + 1];
	char desc[DESCLEN + 1];
	char location_desc[LOC_DESC_LEN + 1];
	bool arrest;
	bool domestic;
	char beat[BEATLEN + 1];
	char district[DISTRICTLEN + 1];
	char ward[WARDLEN + 1];
	char comm_area[COMM_AREA_LEN + 1];
	char fbi_code[FBI_CODE_LEN + 1];	


};

/*enumeration which describes which record we are sorting or indexing with*/

enum record_type {

	R_ID, R_CASE, R_DATE, R_BLOCK, R_IUCR, R_TYPE, R_DESC, R_LOCATION, R_ARREST, 
	R_DOMESTIC, R_BEAT, R_DISTRICT, R_WARD, R_COMM_AREA, R_FBI_CODE, R_INVALID

};

/*Store file offsets so we can jump to them*/
typedef size_t file_offset_t;

/*Index into the page, how many from the top?*/
typedef size_t page_offset_t;


/*Node type in the B+ Tree, used for both leaf nodes and internal nodes*/
/*format is as follows: doubly linked list of heap pages, array of keys,
parent to the node, if we are a leaf node or not, number of keys contained
in node, and a pointer to next node*/
struct node {

	struct heap_page ** page_pointers;
	int * keys;
	struct node * parent;
	bool is_leaf;
	int num_keys;
	struct node * next; 
	/*next pointer for queue*/

};

/*Default order of the tree is 3*/
#define ORDER_OF_TREE 3


/*page struct, with flexible array member*/

struct heap_page {

	size_t num_records;
	struct record records[];	

};

/*header at top of heap file*/

struct heap_header {

	size_t total_pages;
	size_t records_per_page;
	size_t page_size;
	size_t total_records;
	size_t size_record;	
	
};

/*time data structure*/
typedef struct {

	long tv_sec;
	long tv_usec;

} timeval;

/*represents an address of a record in a heap file first by page number and then by record number from top*/

struct heap_offset {

	/*What is the page's address reported with fseek?*/
	file_offset_t file_offset;
	/*What is the index to the data? That is, there are page_offset records before this one. This is complicated*/
	/*by the fact that we had to use a char array for the key and as such, the data has to be manually copied using memcpy*/
	page_offset_t page_offset;

};

struct rid {

	/*Offset into the heap file and into the page within the heap file*/
	struct heap_offset offset;
	/*Provided in case you wish to use the variable length key such as if you wish to play with key compression*/
	size_t size_key;
	/*using byte here is a key can be of any type and this way we dont need a different kind of struct for*/
	/*each field in the data set*/
	byte key[];

};

/*The assumption is that all keys are the same size. Implementing compression makes the following redundant:*/
#define RID_SIZE(key_size) ((key_size) + (sizeof(struct rid)))

/*simplify to avoid pointers as much as we can */

struct hash_page {

	size_t num_rids;
	size_t page_address;
	file_offset_t overflow;
	byte data[];

}; 

inline size_t hash_rids_per_page(size_t pagesize, size_t rid_size){
	size_t usable_space = pagesize - sizeof(struct hash_page);
	double num_rids = usable_space / (double)rid_size;
	return (size_t)floor(num_rids);
}

struct hash_header {

	/*Maximum records per page is fixed as each record will require sizeof(size_t)(for offset) plus sizeof(struct rid) *
	num_offsets + sizeof(key). Given that we opted for fixed length fields when designing the heap page, we have simplicity*/
	/*at the cost of disk space*/

	size_t max_records;

	/*how many records per page?*/
	size_t max_records_per_page;

	/*store search key so we can easily index into each tuple*/
	enum record_type index_key;

	/*fixed length records - how big is each one?*/
	size_t size_record;

	size_t num_pages;

	size_t page_size;

};


/*function prototypes*/
double start_timing();
double stop_timing();
void scan_heap_file(FILE *fp, int pagesize, double t1, double t2);
void binary_search(FILE *fp, int pagesize, int filesize, double t1, double t2);
void range_search_heap_file(FILE *fp, int pagesize, double t1, double t2);
void equality_search_heap_file(FILE *fp, int pagesize, double t1, double t2);
void print_heap_page(struct heap_page *page, struct heap_header *header);
void print_hash_page(struct hash_page *page, struct hash_header *header);
void print_record(struct record *record);
void print_heap_header(struct heap_header *header);
void print_hash_header(struct hash_header *header);
void print_heap_file(struct heap_page *page, int pagesize);
struct heap_header * read_heap_header(FILE *fp);
struct hash_header * read_hash_header(FILE *fp);
struct heap_page * read_heap_page(FILE *fp, int pagesize, int size_record);
struct hash_page * read_hash_page(FILE *fp, int pagesize, int size_record);
struct node * make_node();
struct node * make_leaf_node();
struct node * insert_into_node(struct node *root, struct heap_page *page, struct heap_header *header);
