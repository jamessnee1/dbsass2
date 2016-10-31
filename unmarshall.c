#include "marshall.h"

int main(void)
{
	/* allocate the page to hold the data to be read in */
	struct page * page = malloc(PAGESIZE);
	if(!page)
	{
		perror("failed to allocate memory");
		return EXIT_FAILURE;
	}
	/* open the file and read in the page we wrote earlier */
	FILE * fp = fopen(FILENAME, "r");
	if(fread(page, PAGESIZE, 1, fp) < 1)
	{
		perror("failed to read from file.\n");
		free(page);
		fclose(fp);
		return EXIT_FAILURE;
	}
	fclose(fp);
	/* unmarshall the page */
	struct string_vector vec = unmarshall_strings(page);
	unsigned count;
	/* test that there have been no errors in unmarshalling */
	if(vec.num_strings == 0)
	{
		fprintf(stderr, "failed to read strings.\n");
		free(page);
		return EXIT_FAILURE;
	}
	/* print out the data */
	for(count = 0; count < vec.num_strings; ++count)
	{
		printf("%s\n", vec.strings[count]);
		free(vec.strings[count]);
	}

	/* perform cleanup */
	free(vec.strings);
	free(page);
	return EXIT_SUCCESS;
}

struct string_vector unmarshall_strings(struct page * page)
{
	//data structure to hold the strings read in
	struct string_vector vec;
	/* save the pointer to the serialised data */
	char * current = page->data;
	/* zero out our datastructure */
	memset(&vec, 0, sizeof(struct string_vector));
	/* allocate our initial array */
	vec.strings = malloc(MINSIZE * sizeof(char *));
	vec.num_allocated = MINSIZE;
	/* retrieve all the data from the serialised array */
	while(vec.num_strings < page->num_strings)
	{
		/* do we need to resize our array ? */
		if(vec.num_strings == vec.num_allocated)
		{
			/* resize */
			void * new = realloc(vec.strings, vec.num_allocated * 2
					     * sizeof(char*));
			if(!new)
			{
				perror("failed to resize the array");
				free(vec.strings);
				vec.num_strings = 0;
				return vec;
			}
			vec.strings = new;
			vec.num_allocated *=2;
		}
		/* retrieve the length of each string */
		size_t len;
		memcpy(&len, current, sizeof(size_t));
		/* increment the pointer to point to the string */
		current += sizeof(size_t);
		//allocate memory for the string
		vec.strings[vec.num_strings] = malloc(len * sizeof(char));
		/* copy out the data */
		memcpy(vec.strings[vec.num_strings], current, len);
		/* increment the pointer to point to the next length */
		current += len;
		++vec.num_strings;
	}
	return vec;
}
