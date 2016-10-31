# Database System Assignment 2 Makefile
# By James Snee s3369721

all: btree_maker query_planner
	@echo Making all....
	
query_planner: query_planner.o print_functions.o read_functions.o heap_functions.o
	@echo Making query_planner...
	@gcc -Wall -o query_planner query_planner.o print_functions.o read_functions.o heap_functions.o

query_planner.o: query_planner.c master_header.h
	@echo making query_planner.o
	@gcc -Wall -c -g -std=c99 query_planner.c

btree_maker: btree_maker.o print_functions.o read_functions.o btree_functions.o write_functions.o
	@echo Making btree_maker...
	@gcc -Wall -o btree_maker -lm btree_maker.o print_functions.o read_functions.o btree_functions.o write_functions.o

btree_maker.o: btree_maker.c master_header.h
	@echo making btree_maker.o
	@gcc -Wall -c -g -std=c99 btree_maker.c

print_functions.o: print_functions.c
	@echo making print_functions.o
	@gcc -Wall -c -g -std=c99 print_functions.c	

read_functions.o: read_functions.c
	@echo making read_functions.o
	@gcc -Wall -c -g -std=c99 read_functions.c

write_functions.o: write_functions.c
	@echo making write_functions.o
	@gcc -Wall -c -g -std=c99 write_functions.c	

btree_functions.o: btree_functions.c
	@echo making btree_functions.o
	@gcc -Wall -c -g -std=c99 btree_functions.c

heap_functions.o: heap_functions.c
	@echo making heap_functions.o
	@gcc -Wall -c -g -std=c99 heap_functions.c					

clean:
	rm -f *.o main