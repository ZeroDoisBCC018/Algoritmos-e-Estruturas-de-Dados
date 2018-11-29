#include "googlebot.h"
#include <math.h>                                                                                                 

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
	SITE* aux;
	if(emptylist(l)){
		l->first = s;
		l->last = s;
		if(DEBUG) printf("Site %s inserido com sucesso.\n", s->name);
		return SUCCESS;
	} else if(pos == 1){
		aux = l->first;
		l->first = s;
		s->next = aux;
		aux = NULL;
		l->size += 1;
		if(DEBUG) printf("Site %s inserido com sucesso.\n", s->name);
	} else if(pos == l->size){
		if(DEBUG) printf("Site %s inserido com sucesso.\n", s->name);
		
	} else{
		if(DEBUG) printf("Site %s inserido com sucesso.\n", s->name);
		
	}
	return SUCCESS;
}

int buckpush (LIST* l, SITE* s){
	if(invalidlist(l) || l->size == 9999) return ERROR;
	
	if(emptylist(l)){
		l->first = s;
		l->last = s;
		if(DEBUG) printf("Site %s inserido com sucesso.\n", s->name);
		return SUCCESS;
	}
	
	else {

		l->last->next = s;
		l->last = s;
		l->last->next = NULL;
		l->size++;
		return SUCCESS;
	}
		
}


int separadigito (int recebido, int posicao)
{
	int digito;
	
	digito = (recebido / (int) pow(10, (posicao-1)) %10);
		
	return digito;
}

LIST* concatena (LIST** listor)
{
	int i;
	LIST* concatenada;
	concatenada = newlist();
	
	SITE* percorre;
	
	for (i = 0; i < 10; i++)
	{
		percorre = listor[i]->first;
		
		while (percorre != NULL)
		{
			buckpush(concatenada, percorre->site);
			percorre = percorre->next;
		}
	}
	
	free(percorre);
	
	return concatenada;
}

void bucket (LIST* desordenada, int digito)
{
	LIST* ordenada;
	LIST** baldes = (LIST**) malloc (sizeof(LIST*) * 10);
	int casa, i;
	SITE* percorre;
	
	for (i = 0; i < 10; i++)
	{
		baldes[i] = newlist();
	}
	
	percorre = desordenada->first;
	
	while (percorre != NULL)
	{
		casa = separadigito(percorre->rel, digito);
		buckpush(baldes[casa], percorre);
		
		percorre = percorre->next;
	}
	
	ordenada = concatena(baldes);
	
	LIST* nosauxiliares;
	nosauxiliares = newlist();
	
	nosauxiliares->first = ordenada->first;
	nosauxiliares->last = ordenada->last;
	
	desordenada->first = nosauxiliares->first;
	desordenada->last = nosauxiliares->last;
	
	free(nosauxiliares);
	
	for (i = 0; i < 10; i++)
	{
		endlist(baldes[i]);
	}
	
	free(baldes);
}


void radix (LIST* l)
{
	int i;
	
	for (i = 1; i <= 4; i++)
	{
		bucket (l, i);
	}
}
