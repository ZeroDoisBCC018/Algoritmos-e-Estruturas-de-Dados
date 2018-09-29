#define ERROR -404
#define SUCCESS 1
#define TRUE 1
#define FALSE 0
#include <stddef.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "googlebot_lists.h"
#define boolean int


int GB_CodeCheck (int code, LIST* l){
	if (GB_CheckInvalidList(l)) return ERROR;
	if (GB_CheckEmptyList(l)) return ERROR;
	if (code <= 0 || code > 9999) return ERROR;
	NODE* aux = l->first; /*nao é mais array*/
	while (aux != NULL){
		if (aux->site->code == code) return ERROR; /* esse trecho ficou otimo mas coloca else so por garantia*/
		aux = aux->next;
	}
	return SUCCESS;
}

CSV GB_OpenCSVread (CSV fp, char* filename){
	if(filename == NULL){
		perror(".csv filename not given.\n");
		exit(EXIT_FAILURE);
	}
	fp = fopen(filename, "r");
	if(fp == NULL){
		perror(".csv file could not be opened. Not enough memory/fatal error ocurred while trying to access the disk.\n");
		exit(EXIT_FAILURE);
	}
	return fp;
}

CSV GB_OpenCSVwrite (CSV fp, char* filename){
	if(filename == NULL){
		perror(".csv filename not given.\n");
		exit(EXIT_FAILURE);
	}
	fp = fopen(filename, "w"); /* aqui tem que ser "r+" pq "w" sempre cria uma empty file */
	if(fp == NULL){
		perror(".csv file could not be opened. Not enough memory/fatal error ocurred while trying to access the disk.\n");
		exit(EXIT_FAILURE);
	}
	return fp;
}

void GB_CloseCSV (CSV fp){
	if(fp == NULL){
		perror("no .csv file oṕened."); /*nao precisa quitar do programa, so da return na funcao*/
		exit(EXIT_FAILURE);
	}
	
	fclose(fp);
}

int GB_ReadCSV (CSV fp, LIST* l){
	if (GB_CheckInvalidList(l)) return ERROR;
	if (GB_CheckEmptyList(0)) return ERROR; /*Checks if the list isn't empty*/
	
	int i = 0, j = 0, k = 0;
	char dump = ' '; // dump is initialized as blank space so it certainly enters the reading loop on line x
	if(fp == NULL){
		perror("File not opened.\n");
		return ERROR;
	}
	char ignoreline[200];
	fgets(ignoreline, 200, fp); /*Ignores the header from the CSV file*/
	int count1 = 0;
	int count2 = 0;
	fseek(fp, 0, SEEK_SET);
	
	while(dump != EOF){ 	/*This loop counts all the lines of the csv file and rewinds it before performing the reading operations*/
		fgets(ignoreline, 200, fp);
		dump = fgetc(fp);
		count1++;
	}
	rewind(fp);
	
	NODE* n;
	n = GB_NewNode(); /*In this loop a number of nodes equivalent to the number of sites counted are created and linked*/
	while(count2 != count1){
		n->next = GB_NewNode();
		n = n->next;
	}
	n = l->first; /*now the node is reset to the first position of the list in order to  read the informations*/
	
	do{
		fgets(ignoreline, 200, fp); /*Ignores again the header*/
		fscanf(fp, "%d", &n->site->code);
		if (!GB_CodeCheck(n->site->code, l)) return ERROR;
		fscanf(fp, ",%s[^,],%d,%s[^,],", n->site->name, &n->site->relev, n->site->link);
		for(i = 0; i < 10; i++){
			n->site->keyw[i][0] = '+';
		}
		while (dump != '\n'){
			dump = fgetc(fp);
			n->site->keyw[j][k] = dump;
			k++;
			if(dump == ','){
				 j++;
				 k = 0;
			}
		n = n->next;
		l->size++;
		if (n->next == NULL) break;
		}
	}while(!feof(fp));
	GB_Sort(l);
	
	return SUCCESS;
}

int GB_UpdateDataBase (CSV fp, char* filename, LIST* l) {
	int i = 0, j = 0;
	NODE* aux;
	aux = l->first;
	char c = ' ';
	if(fp == NULL){
		perror("File not opened.\n");
		return ERROR;
	}
	fflush(fp);
	GB_CloseCSV(fp);
	GB_OpenCSVwrite(fp, filename);
	while (aux->next != l->last){
		fprintf(fp, "%04d,%s,%s,", aux->site->code, aux->site->name, aux->site->link);
		do{
			for(i = 0; i < 10; i++){
				for(j = 0; j < 50; j++){
					fprintf(fp, "%c", aux->site->keyw[i][j]);
				}
			}
		}while (c != '\n');
		aux = aux->next;
	}
	GB_CloseCSV(fp);
	return SUCCESS;
}
