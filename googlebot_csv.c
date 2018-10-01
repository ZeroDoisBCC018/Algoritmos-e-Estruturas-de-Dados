#define ERROR -404
#define SUCCESS 1
#define TRUE 1
#define FALSE 0
#include <stddef.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "googlebot.h"
#define boolean int


int GB_CodeCheck (int code, LIST* l){
	if (GB_CheckInvalidList(l)) return ERROR;
	if (GB_CheckEmptyList(l)) return ERROR;
	if (code <= 0 || code > 9999) return ERROR;
	NODE* aux = l->first; 
	while (aux != NULL){
		if (aux->site->code == code) return ERROR; 
		else aux = aux->next;
	}
	return SUCCESS;
}

CSV GB_OpenCSVread (char* filename){
	if(filename == NULL){
		perror(".csv filename not given.\n");
		exit(EXIT_FAILURE);
	}
	CSV fp;
	fp = fopen(filename, "r");
	printf("PONTEIRO foi aberto: %p\n", fp);
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
	fp = fopen(filename, "w"); 
	if(fp == NULL){
		perror(".csv file could not be opened. Not enough memory/fatal error ocurred while trying to access the disk.\n");
		exit(EXIT_FAILURE);
	}
	return fp;
}

int GB_CloseCSV (CSV fp){
	if(fp == NULL){
		perror("no .csv file opened.");
		return ERROR;
	}
	
	fclose(fp);
	return SUCCESS;
}

int GB_ReadCSV (CSV fp, LIST* l){
	if (GB_CheckInvalidList(l)) return ERROR;
	int i = 0, j = 0, k = 0;
	char dump = ' '; /* Dump is initialized as a blank space so it certainly enters the reading loop on line x. */
	if(fp == NULL){
		perror("File not opened.\n");
		return ERROR;
	}
	fseek(fp, 0, SEEK_SET);
	char ignoreline[200];
	fgets(ignoreline, 200, fp); /* Ignores the header from the CSV file. */
	int count1 = 0;
	int count2 = 0;
	
	while(!feof(fp)){ 	/* This loop counts all the lines of the csv file and rewinds it before performing the reading operations. */
		fgets(ignoreline, 200, fp);
		count1++;
		printf("count1 = %d\n", count1);
	}
	rewind(fp);
	
	NODE* n = NULL;
	n = GB_NewNode(); /* In this loop a number of nodes equivalent to the number of sites counted are created and linked. */
	l->first = n;
	 NODE* aux = NULL;
	aux = n;
	while(count2 != (count1-1)){
		aux->next = GB_NewNode();
		aux = n->next;
		count2++;
		printf("count2 = %d\n", count2);
	}
	printf("saiu do loop count2\n");
	
	aux = l->first; /* Now the node is reset to the first position of the list in order to read the informations. */
	int count3 = 0;
	
	do{
		fgets(ignoreline, 200, fp); /* Ignores again the header. */
		printf("header ignorado wahoo\n");
		fgetc(fp);
		printf("fgetc efetuado com sucesso\n");
		fflush(fp);
		fscanf(fp, "%d", &aux->site->code);
		printf("code lido = %d\n", aux->site->code);
		fscanf(fp, ",%[^,],%d,%[^,],", aux->site->name, &aux->site->relev, aux->site->link);
		printf("nome, relevancia e link lidos\n");
		fgetc(fp); /*ignora aspas iniciais do link*/
		printf("Antes de zerar colunas do keyword\n");
		for(i = 0; i < 10; i++){
			aux->site->keyw[i][0] = '#';
		}
		printf("Colunas de keyword zeradas\n");
		while (dump != '"'){
			dump = fgetc(fp);
			if(dump == '"') break;
			if(dump == ','){
				 j++;
				 k = 0;
				 dump = fgetc(fp);
			}
			else{
				aux->site->keyw[j][k] = dump;
				k++;
			}
		dump = ' ';	
		}	
		if (aux->next == NULL) break;
		aux = n->next;
		l->size++;
		count3++;
		printf("do while realizado %d vezes\n", count3);
	}while(!feof(fp));
	/*GB_Sort(l);*/
	printf("linha 119 csv, linhas do test foram lidas\n");
	return SUCCESS;
}

int GB_UpdateDataBase (CSV fp, char* filename, LIST* l) {
	if (fp == NULL){
		perror("File not opened.\n");
		return ERROR;
	}
	GB_CloseCSV(fp);
	GB_OpenCSVwrite(fp, filename);
	if (fp == NULL){
		perror("An error ocurred while overwriting the file.\n");
		returno ERROR;
	}
	int i = 0, j = 0;
	NODE* aux;
	aux = l->first;
	char c = ' ';
	while (aux->next != l->last){
		fprintf(fp, "%04d,%s,%s", aux->site->code, aux->site->name, aux->site->link);
		do{
			for(i = 0; i < 10; i++){
				fprintf(fp, ",");
				for(j = 0; j < 50; j++){
					fprintf(fp, "%c", aux->site->keyw[i][j]);
				}
			}
		}while (c != '\0');
		aux = aux->next;
	}
	GB_CloseCSV(fp);
	return SUCCESS;
}
