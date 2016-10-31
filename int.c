#include <stdio.h>
#include <stdlib.h>

void print_int(){

	int one;

	printf("Please enter an integer: ");
	scanf("%d", &one);

	
	printf("Integer in Decimal: %d\n", one);
	printf("Integer in Hexadecimal: %x\n", one);
	printf("Integer in Octal: %o\n", one);


}

void write_to_fwrite(){
	
	int intarray[5];
	
	printf("Please input the first integer: ");
	scanf("%d", &intarray[0]);
	
	printf("Please input the second integer: ");
	scanf("%d", &intarray[1]);
	
	printf("Please input the third integer: ");
	scanf("%d", &intarray[2]);
	
	printf("Please input the fourth integer: ");
	scanf("%d", &intarray[3]);
	
	printf("Please input the fifth integer: ");
	scanf("%d", &intarray[4]);
	
	/*Open file and write ints to file using fwrite*/
	
	FILE *fp;
	
	fp = fopen("binaryfile.bin", "w");
	
	/*fwrite(ptr to write, size in bytes of each element, number of elements, file pointer)*/
	fwrite(intarray, 4, sizeof(int) * 5, fp);
	fclose(fp);
	
	printf("File saved and closed.\n");

}

void write_to_printf(){

	int one, two, three, four, five;
	
	printf("Please input the first integer: ");
	scanf("%d", &one);
	
	printf("Please input the second integer: ");
	scanf("%d", &two);
	
	printf("Please input the third integer: ");
	scanf("%d", &three);
	
	printf("Please input the fourth integer: ");
	scanf("%d", &four);
	
	printf("Please input the fifth integer: ");
	scanf("%d", &five);
	
	/*Open file and write ints to file using fprintf*/
	
	FILE * fp;
	
	fp = fopen("file.txt", "w");
	fprintf(fp, "%d %d %d %d %d", one, two, three, four, five);
	
	fclose(fp);

	printf("File saved and closed.\n");

}

void display_binary_file(){

	int intarray[100];
	FILE * fp;
	fp = fopen("binaryfile.bin", "r");
	
	/*Seek to beginning*/
	fseek(fp, SEEK_SET, 0);
	
	/*read into buffer and display*/
	/*fread(ptr to read, size of each element, number of elements, stream (fp))*/

	fread(intarray, sizeof(int), 5, fp);
	printf("%d, %d, %d, %d, %d", intarray[0], intarray[1], intarray[2], intarray[3], intarray[4]);
	fclose(fp);
	

}

int main(){

	int choice;
	char menuOption[1 + 2];
	char error[] = "Error: Not a valid option! Please try again.\n";

	do{
		
		printf("\nMenu\n");
		printf("-----\n");
		printf("1) Print entered integer in Decimal, Hexadecimal, and Octal\n");
		printf("2) Write integers to file using fprintf\n");
		printf("3) Write integers to file using fwrite\n");
		printf("4) Display binary file\n");
		printf("5) Quit\n\n");
		printf("Enter your option: ");
		
		/*prompt to take in menu option*/
		fgets(menuOption, 1 + 2, stdin);
		
		choice = atoi(menuOption);
		
		switch(choice){
			
			case 0:
				printf(error);
				break;
			case 1:
				print_int();
				break;
			case 2:
				write_to_printf();
				break;
			case 3:
				write_to_fwrite();
				break;
			case 4:
				display_binary_file();
				break;
			case 5:
				return EXIT_SUCCESS;
				break;
			default:
				printf(error);
				break;
		
		
		}
		
	}
	while(choice != 5);

	write_to_fwrite();
	
	return EXIT_SUCCESS;
}
