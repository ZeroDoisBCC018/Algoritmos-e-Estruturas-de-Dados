#define ERROR -404
#define SUCCESS 1
#define TRUE 1
#define FALSE 0
#include "googlebot_lists.h"
#include <stddef.h> 
#include <stdlib.h>
#include <stdio.h>


#define boolean int



NODE* GB_NewNode (void) {
	NODE* new = (NODE*) malloc(sizeof (NODE));
	new->next = NULL;
	new->site = NULL;
	return new;
}

LIST* GB_NewList (void) {
    LIST* l = (LIST*) malloc(sizeof (LIST));
    l->first = NULL;
    l->last = NULL;
    l->size = 0;
    return l;
}

boolean GB_CheckEmptyList (LIST* l) {
    return (l->size == 0 ? TRUE : FALSE);
}

boolean GB_CheckInvalidList (LIST* l) {
    return ((l->size > 9999 || l-> size < 0) ? TRUE : FALSE);
}

int GB_CountListSize (LIST* l) { 
	if (l == NULL) return ERROR;
	NODE* aux = l->first;
	int count = 0;
	while (aux != NULL){
		count++;
	}
	l->size = count;
	if (GB_CheckInvalidList(l)) return ERROR;
	return count;
}

int GB_InsertNodeFirstPos (LIST* l, NODE* n) {
    if (GB_CheckInvalidList(l)) return ERROR;
    if (GB_CheckEmptyList(l)) {
        l->first = n;
        return SUCCESS;
    } else {
        NODE* aux;
        aux = l->first;
        l->first = n;
        n->next = aux;
        aux = NULL;
        (l->size)++;
    }
    if (GB_CheckEmptyList(l)) return ERROR;
    else if (GB_CheckInvalidList(l)) return ERROR;
    return SUCCESS;
}

int GB_InsertNodeLastPos (LIST* l, NODE* n) {
    if (GB_CheckInvalidList(l)) return ERROR;
    else if (GB_CheckEmptyList(l)) {
        l->first = n;
        l->last = n;
        (l->size)++;
    } else {
        l->last->next = n;
        l->last = n;
        n->next = NULL;
        (l->size)++;
        if (GB_CheckEmptyList(l)) return ERROR;
        else if (GB_CheckInvalidList(l)) return ERROR;
        return SUCCESS;
    }
    return SUCCESS;
}

NODE* GB_SearchCode (LIST* l, int code) {
	NODE* aux = l->first;
	if (GB_CheckEmptyList(l)) perror("The list is empty\n");
	else if (GB_CheckInvalidList(l)) perror("The list is invalid\n");
	else {
		int c = aux->site->code;
		while (c != code || aux != l->last->next) {
			aux = aux->next;
			c = aux->site->code;
		}
	}
	if (aux == l->last->next){
		perror("Error 404 - Site was not found\n"); 
		return NULL;
		
	} 

	else return aux;
}

/* REVER */
int GB_UpdateRelevance (LIST* l, int code, int newrel) {
	if (GB_CheckEmptyList(l)) return ERROR;
	else if (GB_CheckInvalidList(l)) return ERROR;
	else if (code < 0 || code > 1000) return ERROR;
	NODE* aux;
	aux = GB_SearchCode (l, code);	
	aux->site->code = newrel;
	/*int check = 0;
	int i = 0;
	int listsize = GB_CountListSize(l);
	for (i = 0; i < listsize; i++) {
		if (aux->site->code == code) {
			check = 1;
			break;
		}
	}*/
	return SUCCESS;
}

int GB_EraseList (LIST* l) {
	NODE* aux1;
	NODE* aux2;
	aux1 = l->first;
    while (aux1 != NULL) {
		aux2 = aux1->next;
		free(aux1);
		aux1 = aux2;
    }
	aux1 = NULL;
	aux2 = NULL;
	free(l);
	return SUCCESS;
}

void GB_Sort(LIST* l) {
	int i = 0;
	LIST* auxfirst = NULL;
	LIST* auxsecond = NULL;
	LIST* auxfinal = NULL;

	int listsize = GB_CountListSize(l);
	if (listsize == 0 || listsize == 1) return;
	else {
		
		NODE* auxnode = l->first;
	
		auxfirst = GB_NewList();
		auxsecond = GB_NewList();
		
		while (i < (listsize/2)) {
			GB_InsertNodeLastPos (auxfirst, auxnode);
			auxnode = auxnode->next;
			i++;
		}
		while (i <= listsize) {
			GB_InsertNodeLastPos (auxsecond, auxnode);
			auxnode = auxnode->next;
			i++;
		}
		GB_Sort(auxfirst);
		GB_Sort(auxsecond);
		
		auxfinal = GB_Merge(auxfirst, auxsecond);
	}
	
	int auxlistsize;
	auxlistsize = GB_CountListSize(l);
	
	if (auxlistsize == listsize)
	{
		NODE* naux1;
		NODE* naux2;
		naux1 = l->first;
		while (naux1 != NULL){
			naux2 = naux1->next;
			free (naux1);
			naux1 = naux2;
		}
		naux1 = NULL;
		naux2 = NULL;
		
		l->first = auxfinal->first;
		l->last = auxfinal->last;
		free (auxfinal);
	}
	
}

LIST* GB_Merge (LIST* lfirst, LIST* lsecond) {
	LIST* aux = GB_NewList();
	NODE* auxnode1 = lfirst->first;
	NODE* auxnode2 = lsecond->first;
	
	while (auxnode1 != NULL || auxnode2 != NULL) {
		if (auxnode1->site->code < auxnode2->site->code) {
			GB_InsertNodeLastPos (aux, auxnode1);
			auxnode1 = auxnode1->next;
		} else {
			GB_InsertNodeLastPos (aux, auxnode2);
			auxnode2 = auxnode2->next;
		}
	}
	
	if (auxnode1 == NULL) {
		while (auxnode2 != NULL) {
			GB_InsertNodeLastPos (aux, auxnode2);
			auxnode2 = auxnode2->next;
		}	
	} else {
		while (auxnode1 != NULL) {
			GB_InsertNodeLastPos (aux, auxnode1);
			auxnode1 = auxnode1->next;
		}	
	}
	
	return aux;
}

int InsertNodeAtPosition (LIST* l, NODE* n, int code){
	if (GB_CheckInvalidList(l)) return ERROR;
	if (n == NULL) n = GB_NewNode();
	if (GB_CheckEmptyList(l)) {
		l->first = n;
		l->last = n;
		l->size = 1;
		return SUCCESS;
	}
	NODE* aux1;
	NODE* aux2;
	aux1 = l->first;
	aux2 = l->first;
	int i, count = 0;
	while (aux1->next != NULL || aux1->next->site->code < code) {
		aux1 = aux1->next;
		count++;
	}
	for(i = 0; i < (count-1); i++){
		aux2 = aux2->next;
	}
	aux2->next = n;
	n->next = aux1;
	aux1 = NULL;
	aux2 = NULL;
	if (GB_CheckInvalidList(l)) return ERROR;
	return SUCCESS;
}
