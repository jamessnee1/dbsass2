James Snee s3369721
Database Systems Assignment 2

Files included: Makefile, master_header.h, btree_functions.c, query_planner.c, 
read_functions.c, write_functions.c, print_functions.c, btree_maker.c, heap_functions.c

Run query planner with ./query_planner <file>

run btree maker with ./btree_maker <file>

Incomplete functionality:
Btree maker parses heap file, but does not add pages to the Btree. There is untested code that will write Btree nodes out to file.
Query maker parses heap file and runs all types of search on it, but partially parses a hash file, but not a Btree index.