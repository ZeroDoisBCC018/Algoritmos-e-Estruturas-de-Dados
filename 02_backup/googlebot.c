#include "googlebot.h"                                                                                                       

/* 0 = sem printf de debug
 * 1 = com printf de debug */
#define DEBUG 1

LIST* newlist(void) {
    LIST* l = (LIST*) malloc(sizeof (LIST));
    if (l == NULL){
		fprintf(stderr, "Fatal error: not enough memory.\n");
		exit(EXIT_FAILURE);
	} 
    l->first = NULL;
    l->last = NULL;
    l->size = 0;
    if(DEBUG) printf("List inicializada com sucesso.\n");
    return l;
}

SITE* newsite (void){
	SITE* new = (SITE*) malloc(sizeof(SITE));
	if(new == NULL){
		fprintf(stderr, "Fatal error: not enough memory.\n");
		exit(EXIT_FAILURE);
	}
	new->next = NULL;
	if(DEBUG) printf("Site alocado.\n");
	new->link[1024] = '\0';
	if(DEBUG) printf("Inicializacao do link feita com sucesso.\n");
	new->name[64] = '\0';
	if(DEBUG) printf("Inicializacao do nome feita com sucesso.\n");
	int i;
	for(i = 0; i < 10; i++){
		new->tag[i][50] = '\0';
		if(DEBUG) printf("Inicialização da tag %d feita com sucesso.\n", i);
	}
	return new;
}

bool invalidlist(LIST* l){
	 return ((l->size > 9999 || l->size < 0) ? TRUE : FALSE);
}

bool emptylist(LIST* l){
	return (l->size == 0 ? TRUE : FALSE);
}

int listlength(LIST* l){
	if(l == NULL) return ERROR;
	int count = 0;
	SITE* aux = l->first;
	while (aux != NULL){
		count++;
		if(DEBUG) printf("Count passando pelo site %s.\n", aux->name);
		aux = aux->next;
	}
	l->size = count;
	if(invalidlist(l)) return ERROR;
	return count;
}

int insertsite(LIST* l, SITE* s, int pos){
	if(invalidlist(l) || l->size == 9999) return ERROR;
	SITE* aux = NULL;
	if(emptylist(l)){
		l->first = s;
		l->last = s;
		l->size = 1;
		if(DEBUG) printf("Site %s inserido com sucesso.\n", s->name);
		return (l->first == NULL || l->last == NULL ? FAIL : SUCCESS);
	} else if(pos == 1){
		aux = l->first;
		l->first = s;
		s->next = aux;
		aux = NULL;
		l->size += 1;
		if(DEBUG) printf("Site %s inserido com sucesso.\n", s->name);
		return (l->first == NULL || s->next == NULL ? FAIL : SUCCESS);
	} else if(pos == (l->size)+1){
		l->last->next = s;
		l->last = s;
		l->size += 1;
		if(DEBUG) printf("Site %s inserido com sucesso.\n", s->name);
		return (l->last == NULL ? FAIL : SUCCESS);
	} else{
		/*---------------------------------------------------------*/
		if(DEBUG) printf("Site %s inserido com sucesso.\n", s->name);
		
	}
	return SUCCESS;
}

SITE* searchsite(LIST* l, const char mode, ...){
	if(emptylist(l)) return NULL;
	if(l->first == NULL || l->last == NULL) return NULL;
	SITE* aux = l->first;
	va_list arg;
	va_start(arg, 1);
	char str[] = NULL;
	int code_n;
	switch(mode){
		case 'c':
			code_n = va_arg(arg, int);
			while(aux->code != code_n){
				aux = aux->next;
				if(aux == NULL) return NULL;
				if(DEBUG) printf("Busca passando pelo site %s.\n", aux->name);
			}
			if(DEBUG) printf("A busca encontrou o site %s.\n" aux->name);
			return aux;
		case 'n':
			str = va_arg(arg, char*);
			while(strcmp(aux->name, str) != 0){
				aux = aux->next;
				if(aux == NULL) return NULL;
				if(DEBUG) printf("Busca passando pelo site %s.\n", aux->name);
			}
			if(DEBUG) printf("A busca encontrou o site %s.\n" aux->name);
			return aux;
		default: return NULL;
	}
	va_end(arg);
	return SUCCESS;
}

int site_newrelevance(SITE* s, int newrel){
		if(s == NULL) return FAIL;
		if(newrel < 0 || newrel > 9999){
			fprintf(stderr, "An error ocurred: new requested relevance is invalid. The value should be between 0 and 9999.\n");
			return FAIL;
		}
		s->rel = newrel;
		return SUCCESS;
}

void eraselist(LIST* l){
	SITE* aux1, aux2;
	if(emptylist(l)){
		free(l);
		return;
	}
	aux1 = l->first->next;
	aux2 = l->first;
	while(aux1 != NULL){
		free(aux2);
		aux2 = aux1;
		if(DEBUG) printf("Apagando o site %s da lista.\n", aux2->name);
		aux1 = aux1->next;
	}
	free(l);
}
