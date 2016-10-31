#include "marshall.h"

const char * strings[] = 
{
	"fred", "is", "wearing", "a", "hat"
};

int main(void)
{
	//create a page with the strings serialised
	struct page * page = marshall_strings(strings, 5);
	if(!page)
	{
		fprintf(stderr, "failed to marshall strings.\n");
		return EXIT_FAILURE;
	}
	//open the file and check it opened successfully.
	FILE * fp = fopen(FILENAME, "w");
	if(!fp)
	{
		perror("failed to open file");
		free(page);
		return EXIT_FAILURE;
	}
	//write out a page
	if(fwrite(page, PAGESIZE, 1, fp) != 1)
	{
		perror("failed to write to file");
		free(page);
		return EXIT_FAILURE;
	}
	//clean up
	free(page);
	fclose(fp);
	printf("successfully wrote to file\n");
	return EXIT_SUCCESS;
	
}

struct page * marshall_strings(const char * strings[], size_t num_strings)
{
	struct page * page = malloc(PAGESIZE);
	memset(page, 0, PAGESIZE);
        //grab a pointer to the data part of the page so we can serialise to it
	char * current = page->data;
	size_t count;
	//perform the marshalling. Note that while I am using char arrays here,
	//this approach with memcpy would work with data of any type
	for(count = 0; count < num_strings; ++count)
	{
                //grab the length of the string and add one for the nul terminator
		size_t len = strlen(strings[count])+1;
                //copy the length of the string into the array. Note that if the
		//things we are copying in are all fixed size, we wouldn't need
		//to copy in the length
		memcpy(current, &len, sizeof(size_t));
                //move the pointer along past the thing we just copied in.
		current += sizeof(size_t);
                //copy in the string itself
		memcpy(current, strings[count], len);
                //move the pointer along past the thing we just copied in.
		current += len;
		++page->num_strings;
	}
	return page;
}
