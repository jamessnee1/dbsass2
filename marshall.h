#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef char byte;

struct page
{
	size_t num_strings;
	byte data[];
};

struct string_vector
{
	size_t num_strings;
	size_t num_allocated;
	char ** strings;
};

#define PAGESIZE 1024
#define FILENAME "strings.bin"
#define MINSIZE 16

struct page * marshall_strings(const char * strings[], size_t num_strings);
struct string_vector unmarshall_strings(struct page *);
