#define ERROR -404
#define SUCCESS 1
#define TRUE 1
#define FALSE 0
#include <stddef.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "googlebot.h"

int GB_RemoveSite(LIST* l, int code){
	if (GB_CheckInvalidList(l)) return ERROR; 
	if (GB_CheckEmptyList(l)) return ERROR;
	NODE* aux1 = l->first; 
	NODE* aux2 = aux1->next;	
	if (aux1->site->code == code){ 		  /* Checks if the first node is to be erased, which will have 2 different cases. */
		int size = GB_CountListSize(l);
		if (size == 1) free(aux1);        /* 1 (one): the list only has this node, so we only need to free it. */
		else{				  /* 2: the list has more elements. */
			l->first = aux2;
			aux1->next = NULL;
			free(aux1);
			return SUCCESS;
		}
	}
	while (aux2->site->code != code && aux2 != NULL){ /* Runs through the list until it ends or until the code is found. */
		aux1 = aux1->next;
		aux2 = aux2->next;
	}
	if (aux2 == NULL) return ERROR;            /* If the code was not found, error is returned. */
	if (aux2->site->code == code){		       /* If the code was found, we start removing the node, but only after. */
		aux1->next = aux2->next;               /* Linking correctly the nodes after and before the one to be removed. */
		aux2->next = NULL;
		free(aux2);
		return SUCCESS;
	}
	return ERROR; /* If this function haven't return SUCCES yet, it has failed somewhere in the middle of its way. */
}					           

int GB_InsertKeyWord(LIST* l, int code){
	if (GB_CheckInvalidList(l)) return ERROR; 
	if (GB_CheckEmptyList(l)) return ERROR;
	NODE* n = GB_SearchCode(l, code);
	char dump = ' ';
	int count = 0;
	int k = 0;
	while (n->site->keyw[count][0] != '#' || count > 10){
		count++;
	}
	if (count > 10){
		perror("No more space for keywords\n");
		return ERROR;
	}
	dump = ' '; /* Resets the dump character. */
	while (dump != '\n'){
		dump = getchar();
		n->site->keyw[count][k] = dump;
		k++;
	}
	return SUCCESS;
}

int GB_GetSiteStdin(LIST* l) {
	
	if (l == NULL) return ERROR;
	if (GB_CheckInvalidList(l)) return ERROR;
	NODE* new = GB_NewNode(); /* A new node is created. */
	if (new == NULL) printf("erro de alocação em cria nó, chamado em getsite\n");
	
	/*fflush(stdin);*/
	/* Each one of the new site's properties is read from the standard input and allocated in its node position of the list. */
	int code = 0;
	fscanf(stdin, " %d", &code);	  
	printf("O codigo (getstdin) inserido foi %d\n", code);  
	if (code <= 0 || code > 9999) return ERROR;
	new->site->code = code;
	/*fflush(stdin);*/
	
	char name2[100];
	fscanf(stdin, "%s", name2);
	printf("O nome inserido (getstdin) foi %s\n", name2);
	strcpy(new->site->name, name2);	  
	/*fflush(stdin);*/
	
	int relev = 0;
	fscanf(stdin, " %d", &relev);  
	if (relev < 0 || relev > 1000) return ERROR;
	new->site->relev = relev;
	printf("relevancia (getstdin) inserida %d\n", relev);
	/*fflush(stdin);*/
	
	char link2[100];
	fscanf(stdin, "%s", link2);
	strcpy(new->site->link, link2);
	printf("link (getstdin) inserida %s\n", link2);    
	
	char kw[50];
	int j = 0, len = 0, cl = 0, m = 0;
	while (cl < 50){
		scanf(" %[^\n]", kw);
		if (strcmp(kw, "exit") == 0) break;
		len = strlen(kw);	
		for(m = 0; m < len; m++){
			new->site->keyw[j][m] = kw[m];
		}
		j++;
		cl++;
	}
	InsertNodeAtPosition(l, new, code);	
	return SUCCESS;
}
