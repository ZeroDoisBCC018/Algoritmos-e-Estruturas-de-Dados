#include <stddef.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "googlebot_lists.h"
#include "googlebot_ds.c"
#define CSV FILE*

LIST* l;

void menu(CSV fp, char* fn);

void std_divide(void){
	fprintf(stdout, "=#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==#=\n");
}

void print_first(void){
	std_divide();
	fprintf(stdout, "Modify the database: you can change the properties of a given site directly.\n");
	fprintf(stdout, "Choose one of the following options:\n");
	fprintf(stdout, "\t1 - Insert a new site;\n\t2 - Remove a site from the database;\n\t3 - Update the relevance of a specific site;\n");
	fprintf(stdout, "\t4 - Insert a new keyword to a site;\n\t5 - Print this menu section again;\n\t6 - Return to the main menu.\n");
}

void new_site(void){
	
	int a;
	fprintf(stdout, "You're about to insert a new site on the database.\n");
	fprintf(stdout, "To cancel this operation, insert code 0. To continue please insert code 1.\n");
	fscanf(stdin, "%d", &a);
	if (a == 0) return;
	while(a != 1){
		fprintf(stdout, "Unknown code. To cancel this operation, insert code 0. To continue please insert code 1.\n");
		fflush(stdin);
		fscanf(stdin, "%d", &a);
		if (a == 0) return;
	}
	std_divide();
	fprintf(stdout, "To insert your new site, please type these properties on the following order:\n");
	fprintf(stdout, "(Press enter after each typed propertie)\n");
	fprintf(stdout, "\t1 - Code (1 up to 9999);\n\t2 - Name (up to 50 characters);\n\t3 - Relevance (0 up to 1000);\n\t4 - Link (up to 100 characters);\n\t5 - Site tags/keywords (up to 10 tags, each with a max of 50 characters).\n");
	GB_GetSiteStdin(l);
}

void remove_site(void){
	
	int b;
	fprintf(stdout, "You're about to insert a new site on the database.\n");
	fprintf(stdout, "To cancel this operation, insert code 0. To continue please insert code 1.\n");
	fscanf(stdin, "%d", &b);
	if (b == 0) return;
	while(b != 1){
		fprintf(stdout, "Unknown code. To cancel this operation, insert code 0. To continue please insert code 1.\n");
		fflush(stdin);
		fscanf(stdin, "%d", &b);
		if (b == 0) return;
	}
	std_divide();
	fprintf(stdout, "To remove a site, insert its code below:\n");
	int code = fscanf(stdin, "%d", &code);
	fprintf(stdin, "Are you sure? Insert code 1 to continue.\n");
	int d = fscanf(stdin, "%d", &d);
	if (d != 1){
		fprintf(stdout, "Operation canceled. Returning to the Modify Database menu section.\n");
		std_divide();
		return;
	}
	if (GB_RemoveSite(l, code) != 1){
		fprintf(stdout, "No such code on the database. Returning to the Modify Database menu section.\n");
		std_divide();
		return;
	} else {
		fprintf(stdout, "Site removed from the database successfully.\n");
	}
}

void update_rel(void){
	
	int c;
	
	fprintf(stdout, "You're about to update the relevance of a specific site on the database.\n");
	fprintf(stdout, "To cancel this operation, insert code 0. To continue please insert code 1.\n");
	fscanf(stdin, "%d", &c);
	if (c == 0) return;
	while(c != 1){
		fprintf(stdout, "Unknown code. To cancel this operation, insert code 0. To continue please insert code 1.\n");
		fflush(stdin);
		fscanf(stdin, "%d", &c);
		if (c == 0) return;
	}
	std_divide();
	
	fprintf(stdout, "Insert the site's code:\n");
	int code = fscanf(stdin, "%d", &code);
	fprintf(stdout, "Insert the new relevance:\n");
	int rel = fscanf(stdin, "%d", &rel);
	
	if (GB_UpdateRelevance(l, code, rel) != 1){
		fprintf(stdout, "Relevance could not be updated. Return to the Modify Database menu section.\n");
		return;
	} else {
		fprintf(stdout, "Relevance updated succesfully.\n");
	}
}

void new_keyw(void){
	
	int d;
	fprintf(stdout, "You're about to insert new tags into a specific site on the database.\n");
	fprintf(stdout, "To cancel this operation, insert code 0. To continue please insert code 1.\n");
	fscanf(stdin, "%d", &d);
	if (d == 0) return;
	while (d != 1){
		fprintf(stdout, "Unknown code. To cancel this operation, insert code 0. To continue please insert code 1.\n");
		fflush(stdin);
		fscanf(stdin, "%d", &d);
		if (d == 0) return;
	}
	std_divide();
	fprintf(stdout,"Please insert the side code:\n");
	int code = fscanf(stdin, "%d", &code);
	fprintf(stdout, "Now insert the new tags: (max of 10 keywords, each up to 50 characters)\n");
	
	if (GB_InsertKeyword(l, code) != 1){
		fprintf(stdout, "New keywords could not be inserted. Returning to the Modify Database menu section.\n");
		return;
	} else {
		fprintf(stdout, "New keywords inserted succesfully.\n");
	}
}

void first(void){
	
	int op1 = 101;
	std_divide();
	print_first();
	
	while(op1 != 6){
		fflush(stdin);
		fscanf(stdin, "%d", &op1);
		switch(op1){
			case 1: new_site(); break;
			case 2: remove_site(); break;
			case 3: update_rel(); break;
			case 4: new_keyw(); break;
			case 5: print_first(); break;
			case 6:
				fprintf(stdout, "Returning to the main menu.\n");
				std_divide(); 
				break;
			default:
				fprintf(stdout, "Unknown Code! Please insert one of the option codes above.\n");
		}
		if (op1 != 6) fprintf(stdout, "Insert code 5 to print this menu section again.\n");
	}
}

void second(CSV fp, char* fn){
	
	int op2 = 101;
	
	std_divide();
	fprintf(stdout, "Update the database: this option will update your .csv file and print\n");
	fprintf(stdout, "the current recorded site list. Be sure to make a backup before continuing.\n");
	fprintf(stdout, "Insert code 1 to update the database, 0 to return to the main menu: ");
	op2 = fscanf(stdin, "%d", &op2);
	if (op2 == 1){
		if (GB_UpdateDataBase(fp, fn, l)) fprintf(stdout, "Database updated successfully. Returning to the main menu.\n");
		else {
			perror("An error ocurred during the database updating proccess.\n");
			exit(EXIT_FAILURE);
		}
	} else {
		fprintf(stdout, "Returning to the main menu.\n");
	}
}

void menu_options(void){	
	fprintf(stdout, "\t1 - Modify the database;\n\t2 - Update the database;\n\t3 - Print the menu options on the screen again;\n\t4 - Exit the Googlebot.\n");
	std_divide();
}

void welcome(CSV fp, char* fn){
	fflush(stdin);
	fprintf(stdout, "Googlebot SCC0202-2018 Project. Developed by Joao Villaca, Bruno Gazoni and Bruno Baldissera.\n");
	fprintf(stdout, "ICMC/USP - Brazil, SP, Sao Carlos. This software is distributed as a free software under the\n");
	fprintf(stdout, "\"Unlicense\". Check the official Googlebot documentation or access <http://www.unlicense.org>\n");
	fprintf(stdout, "for more information.\n");
	std_divide();
	fprintf(stdout, "Welcome. This is Googlebot alpha version 1.0. Choose one of the following options:\n");
	menu(fp, fn);	
}

void menu(CSV fp, char* fn){
	
	int op0 = 101;
	menu_options();
	std_divide();
	
	while(op0 != 4){
		fflush(stdin);
		fscanf(stdin, "%d", &op0);
		switch(op0){
			case 1: first(); break;
			case 2: second(fp, fn); break;
			case 3: menu_options(); break;
			case 4:	break; 
			default:
				fprintf(stdout, "Unknown Code! Please insert one of the option codes above.\n");
		}
		if (op0 != 4) fprintf(stdout, "Insert code 3 to print the menu options again.\n");
	}	
}

void error_no_name(void){
	fprintf(stdout, "No filename given!\n");
	fprintf(stdout, "Instructions on how to use GBSiteRelevance:\n");
	fprintf(stdout, "On console command line, type: ");
	fprintf(stdout, "./GBSiteRelevance <csv_file>\n");
	fprintf(stdout, "If the filename input is a nonexistent file\n");
	fprintf(stdout, "an empty new one with this name will be created.\n");
}

int main(int argc, char* argv[]){
	
	if (argv[1] == NULL){ 
		error_no_name();
		perror("Input error: missing filename.\n");
		exit(EXIT_FAILURE);
	} 
	
	char* fn = (char*)malloc(50*sizeof(char));
	strcpy(fn, argv[1]);
	
	CSV fp = GB_OpenCSVread(fp, fn);
	l = GB_NewList();
	GB_ReadCSV(fp, l);
	
	if (fp == NULL){
		fprintf(stdout, "Are you sure you typed the filename correctly?\n");
		perror("Memory allocation error/file can't be opened.\n");
		exit(EXIT_FAILURE);
	}
	
	welcome(fp, fn);
	free(fn);
	
	return EXIT_SUCCESS;
}
