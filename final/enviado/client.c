#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "googlebot.h"
#define DEBUG 1
#define CSV FILE*

LIST* l;
TRIE_N* root;

void menu(CSV fp, char* fn);

void std_divide(void){
	puts("=#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#=");
}

void print_first(void){
	std_divide();
	puts("Modify the database: you can change the properties of a given site directly.");
	puts("Choose one of the following options:");
	fprintf(stdout, "\t1 - Insert a new site;\n\t2 - Remove a site from the database;\n\t3 - Update the relevance of a specific site;\n");
	fprintf(stdout, "\t4 - Insert new keywords to a site;\n\t5 - Print this menu section again;\n\t6 - Return to the main menu.\n");
}

void new_site(void){
	
	int a;
	puts("You're about to insert a new site on the database.");
	puts("To cancel this operation, insert code 0. To continue please insert code 1.");
	fscanf(stdin, "%d", &a);
	if(DEBUG) printf("opcao de confirmacao para insercao de site e = %d\n", a);
	if (a == 0) return;
	while(a != 1){
		puts("Unknown code. To cancel this operation, insert code 0. To continue please insert code 1.");
		fscanf(stdin, "%d", &a);
		if (a == 0) return;
	}
	std_divide();
	puts("To insert your new site, please type these properties on the following order:");
	puts("(Press enter after each typed propertie)");
	fprintf(stdout, "\t1 - Code (1 up to 9999);\n\t2 - Name (up to 50 characters);\n\t3 - Relevance (0 up to 1000);\n\t4 - Link (up to 100 characters);\n\t5 - Site tags/keywords (up to 10 tags, each with a max of 50 characters).\n");
	newsite_stdin(l);
}

void remove_site(void){
	
	int b;
	puts("You're about to insert a new site on the database.");
	puts("To cancel this operation, insert code 0. To continue please insert code 1.");
	fscanf(stdin, "%d", &b);
	if (b == 0) return;
	while(b != 1){
		puts("Unknown code. To cancel this operation, insert code 0. To continue please insert code 1.");
		fscanf(stdin, "%d", &b);
		if (b == 0) return;
	}
	std_divide();
	puts("To remove a site, insert its code below:");
	int code = fscanf(stdin, "%d", &code);
	puts("Are you sure? Insert code 1 to continue.");
	int d = fscanf(stdin, "%d", &d);
	if (d != 1){
		puts("Operation canceled. Returning to the Modify Database menu section.");
		std_divide();
		return;
	}
	if (erasesite(l, 'c', 1, code) != 1){
		puts("No such code on the database. Returning to the Modify Database menu section.");
		std_divide();
		return;
	} else {
		puts("Site removed from the database successfully.");
	}
}

void update_rel(void){
	
	int c;
	
	puts("You're about to update the relevance of a specific site on the database.");
	puts("To cancel this operation, insert code 0. To continue please insert code 1.");
	fscanf(stdin, "%d", &c);
	if (c == 0) return;
	while(c != 1){
		puts("Unknown code. To cancel this operation, insert code 0. To continue please insert code 1.");
		fscanf(stdin, "%d", &c);
		if (c == 0) return;
	}
	std_divide();
	
	printf("Insert the site's code: ");
	int code = fscanf(stdin, "%d", &code);
	printf("\nInsert the new relevance: ");
	int rel = fscanf(stdin, "%d", &rel);
	
	if (site_newrelevance(searchsite(l, 'c', 1, code), rel) != 1){
		puts("Relevance could not be updated. Return to the Modify Database menu section.");
		return;
	} else {
		puts("Relevance updated succesfully.");
	}
}

void new_keyw(void){
	
	int d;
	puts("You're about to insert new tags into a specific site on the database.");
	puts("To cancel this operation, insert code 0. To continue please insert code 1.");
	
	fscanf(stdin, "%d", &d);
	if (d == 0) return;
	while (d != 1){
		puts("Unknown code. To cancel this operation, insert code 0. To continue please insert code 1.");
		fscanf(stdin, "%d", &d);
		if (d == 0) return;
	}
	
	std_divide();
	puts("Please insert the side code:");
	int code = fscanf(stdin, "%d", &code);
	
	SITE* target = searchsite(l, 'c', 1, code);
	puts("Now insert the new tags: (max of 10 keywords, each up to 50 characters)");
	
	int i;
	for(i = 0; i < 10; i++){
		scanf("%50[^\n]%*c", target->tag[i]);
	}
	puts("New tags inserted succesfully.");
}

void first(void){
	
	int op1 = 101;
	std_divide();
	print_first();
	
	while(op1 != 6){
		fscanf(stdin, "%d", &op1);
		if(DEBUG) printf("opcao de first recebida foi %d\n", op1);
		switch(op1){
			case 1: new_site(); break;
			case 2: remove_site(); break;
			case 3: update_rel(); break;
			case 4: new_keyw(); break;
			case 5: 
				std_divide();
				print_first(); 
				break;
			case 6:
				puts("Returning to the main menu.");
				std_divide(); 
				break;
			default:
				puts("Unknown Code! Please insert one of the option codes above.");
		}
		if (op1 != 6) puts("Insert code 5 to print this menu section again.");
	}
}

void second(CSV fp, char* fn){
	
	int op2 = 101;
	
	std_divide();
	puts("Update the database: this option will update your .csv file and print");
	puts("the current recorded site list. Be sure to make a backup before continuing.");
	fprintf(stdout, "Insert code 1 to update the database, 0 to return to the main menu: ");
	op2 = fscanf(stdin, "%d", &op2);
	if (op2 == 1){
		if (1/*updatedatabase*/) puts("Database updated successfully. Returning to the main menu.");
		else {
			perror("An error ocurred during the database updating proccess.\n");
			exit(EXIT_FAILURE);
		}
	} else {
		puts("Returning to the main menu.");
	}
}

void menu_options(void){	
	fprintf(stdout, "\t1 - Modify the database;\n\t2 - Update the database;\n3 - Print the database;\n\t4 - Print the menu options on the screen again;\n\t5 - Browse with Googlebot\n\t6 - Exit the Googlebot.\n");
	std_divide();
}

void welcome(CSV fp, char* fn){
	std_divide();
	puts("Googlebot SCC0202-2018 Project. Developed by Joao Villaca, Bruno Gazoni and Bruno Baldissera.");
	puts("ICMC/USP - Brazil, SP, Sao Carlos. This software is distributed as a free software under the");
	fprintf(stdout, "\"Unlicense\". Check the official Googlebot documentation or access <http://www.unlicense.org>\n");
	puts("for more information.");
	std_divide();
	puts("Welcome. This is Googlebot alpha version 1.0. Choose one of the following options:");
	menu(fp, fn);	
}

void third(void){
	puts("Do you want to sort the dabatase by relevance? y/n");
	char op;
	op = fgetc(stdin);
	switch(op){
		case 'y':
			list_insertionsort(l);
			printlist(l);
			break;
		case 'n':
			printlist(l);
			break;
		default:
			puts("Returning...");
			break;
	}
	return;
}

void browse(){
	puts("Time to browse with the Googlebot!");
	puts("You can search for sites by name, code or keyword.");
	printf("Insert 1 for name, 2 for code, 3 for tag, else to return: ");
	int op, code;
	char name[65], tag[51];
	SITE* target = NULL;
	scanf("%d", &op);
	switch(op){
		case 1:
			fgets(name, 65, stdin);
			target = searchsite(l, 'n', 1, name);
			printsite(target);
			break;
		case 2:
			scanf("%d", &code);
			target = searchsite(l, 'c', 1, code);
			printsite(target);
			break;
		case 3:
			fgets(tag, 51, stdin);
			search_keyword(tag, root, l->size);
			printsite(target);
			break;
		default: return;
	}
}

void menu(CSV fp, char* fn){
	
	int op0 = 101;
	menu_options();
	std_divide();
	
	while(op0 != 6){
		fscanf(stdin, "%d", &op0);
		switch(op0){
			case 1: first(); break;
			case 2: second(fp, fn); break;
			case 3: third(); break;
			case 4: menu_options(); break;
			case 5: browse(); break;
			case 6:	break; 
			default:
				puts("Unknown Code! Please insert one of the option codes above.");
		}
		if (op0 != 6) puts("Insert code 3 to print the menu options again.");
	}	
}

void error_no_name(void){
	puts("No filename given!");
	puts("Instructions on how to use GBSiteRelevance:");
	puts("On console command line, type: ./Googlebot");
	puts("*Welcoming messages, menu, instructions and*");
	puts("<.csv filename here> (FILENAME MUST BE UP TO 50 CHARACTERS)");
	puts("If the filename input is a nonexistent file");
	puts("an empty new one with this name will be created.");
}

int main(int argc, char* argv[]){
	
	std_divide();
	puts("Starting Googlebot... You need to configure your database first. Beware:");
	puts("The standard filetype for the database is a .csv with the site properties");
	fprintf(stdout, "as mentioned in the official documentation. Be sure to read the README.");
	fprintf(stdout, "Please insert the name of your .csv file: ");
	
	char* fn = NULL;
	scanf("%m[^\n]", &fn);
	if(DEBUG) printf("file name is %s \n", fn);
	
	if (fn == NULL){
		perror("Input error: missing filename.\n");
		exit(EXIT_FAILURE);
	}
	
	l = newlist();
	root = create_node(root, NULL, l->size);
	CSV fp = opencsv(fn);
	readcsv(fp, l, root);
	
	if (fp == NULL){
		puts("Are you sure you typed the filename correctly?");
		perror("Memory allocation error/file can't be opened.\n");
		exit(EXIT_FAILURE);
	}
	
	welcome(fp, fn);
	free(fn);
	
	return EXIT_SUCCESS;
}
