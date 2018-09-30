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
	int k = 0;
	
	/* Each one of the new site's propertie is read from the standard input and allocated in its node position of the list. */
	int code = fscanf(stdin, "%d", &code);	  
	if (code <= 0 || code > 9999) return ERROR;
	new->site->code = code;
	fflush(stdin);
	
	char name2[100];
	fgets(name2, 100, stdin);
	strcpy(new->site->name, name2);	  
	fflush(stdin);
	
	int relev = fscanf(stdin, "%d", &relev);  
	if (relev < 0 || relev > 1000) return ERROR;
	new->site->relev = relev;
	fflush(stdin);
	
	char link2[100];
	fgets(link2, 100, stdin);
	strcpy(new->site->link, link2);     
	fflush(stdin);
	
	char dump = ' ';
	int j = 0;
	while (dump != '\n'){
		dump = getchar();
		new->site->keyw[j][k] = dump;
		k++;
		if(dump == ','){
			j++;
			k = 0;
		}
	}
	InsertNodeAtPosition(l, new, code);	
	return SUCCESS;
}
