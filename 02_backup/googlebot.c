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
    if(DEBUG) printf("Lista inicializada com sucesso.\n");
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

bool emptylist(LIST* l){
	return (l != NULL && l->first == NULL && l->size == 0 ? TRUE : FALSE);
}

bool invalidlist(LIST* l){
	 return (l->size > 9999 || l->size < 0 ? TRUE : FALSE);
}

bool listerror(LIST* l){
	if(emptylist(l)) return 0;
	SITE* aux;
	aux = l->first;
	while(aux->next != NULL) aux = aux->next;
	if(aux == l->last) return 0;
	else fprintf(stderr, "The list is fragmented at some point. Returning error...");
	return 1;
}

int listlength(LIST* l){
	if(l == NULL) return ERROR;
	if(listerror(l)) return ERROR;
	int count = 0;
	SITE* aux = l->first;
	while (aux != NULL){
		count++;
		if(DEBUG) printf("\nCount passando pelo site \"%s\".\n", aux->name);
		aux = aux->next;
	}
	l->size = count;
	if(invalidlist(l)) return ERROR;
	return count;
}

int insertsite(LIST* l, SITE* s, int pos){
	if(invalidlist(l) || l->size == 9999) return ERROR;
	if(listerror(l)) return ERROR;
	SITE* aux = NULL;
	int i = 1;
	if(emptylist(l)){
		l->first = s;
		l->last = s;
		l->size = 1;
		if(DEBUG) printf("Site \"%s\" inserido com sucesso.\n", s->name);
		return (l->first == NULL || l->last == NULL ? FAIL : SUCCESS);
	} else if(pos == 1){
		aux = l->first;
		l->first = s;
		s->next = aux;
		aux = NULL;
		l->size += 1;
		if(DEBUG) printf("Site \"%s\" inserido com sucesso.\n", s->name);
		return (l->first == NULL || s->next == NULL ? FAIL : SUCCESS);
	} else if(pos == (l->size)+1){
		l->last->next = s;
		l->last = s;
		l->size += 1;
		if(DEBUG) printf("Site \"%s\" inserido com sucesso.\n", s->name);
		return (l->last == NULL ? FAIL : SUCCESS);
	} else{
		aux = l->first;
		while(i != pos-1){
			i++;
			aux = aux->next;
		}
		s->next = aux->next->next;
		aux->next = s;
		l->size += 1;
		if(DEBUG) printf("Site \"%s\" inserido com sucesso.\n", s->name);
	}
	return SUCCESS;
}

SITE* searchsite(LIST* l, const char mode, int index, ...){
	if(emptylist(l)) return NULL;
	if(listerror(l)) return NULL;
	SITE* aux = l->first;
	va_list arg;
	va_start(arg, index);
	char str[65];
	char* straux;
	int code_n;
	switch(mode){
		case 'c':
			code_n = va_arg(arg, int);
			while(aux->code != code_n){
				aux = aux->next;
				if(aux == NULL) return NULL;
				if(DEBUG) printf("Busca passando pelo site \"%s\".\n", aux->name);
			}
			if(DEBUG) printf("\nA busca encontrou o site \"%s\".\n\n", aux->name);
			va_end(arg);
			return aux;
		case 'n':
			straux = va_arg(arg, char*);
			strcpy(str, straux);
			while(strcmp(aux->name, str) != 0){
				aux = aux->next;
				if(aux == NULL) return NULL;
				if(DEBUG) printf("Busca passando pelo site \"%s\".\n", aux->name);
			}
			if(DEBUG) printf("\nA busca encontrou o site \"%s\".\n\n", aux->name);
			va_end(arg);
			return aux;
		default: return NULL;
	}
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

int erasesite(LIST* l, const char mode, int index, ...){
	if(emptylist(l)) return FAIL;
	if(listerror(l)) return FAIL;
	SITE* aux1 = l->first;
	SITE* aux2 = l->first->next;
	va_list arg;
	va_start(arg, index);
	char str[65];
	char* straux;
	int code_n;
	switch(mode){
		case 'c':
			code_n = va_arg(arg, int);
			while(aux2->code != code_n){
				aux1 = aux1->next;
				aux2 = aux2->next;
				if(aux2 == NULL){
					printf("O eraser nao encontrou o site na lista.\n");
					va_end(arg);
					return FAIL;
				}
				if(DEBUG) printf("Eraser passando pelo site \"%s\".\n", aux1->name);
			}
			if(DEBUG) printf("\nO eraser encontrou o site \"%s\".\n\n", aux2->name);
			if(aux1 == l->first){
				l->first = aux1->next;
				free(aux1);
			} else {
				if(aux2 == l->last){
					l->last = aux1;
					free(aux2);
					break;
				}
				aux1->next = aux2->next;
				free(aux2);
			}
			break;
		case 'n':
			straux = va_arg(arg, char*);
			strcpy(str, straux);
			while(strcmp(aux2->name, str) != 0){
				aux1 = aux1->next;
				aux2 = aux2->next;
				if(aux2 == NULL){
					printf("O eraser nao encontrou o site na lista.\n");
					va_end(arg);
					return FAIL;
				}
				if(DEBUG) printf("Eraser passando pelo site \"%s\".\n", aux1->name);
			}
			if(DEBUG) printf("\nO eraser encontrou o site \"%s\".\n\n", aux2->name);
			if(aux1 == l->first){
				l->first = aux1->next;
				free(aux1);
			} else {
				if(aux2 == l->last){
					l->last = aux1;
					free(aux2);
					break;
				}
				aux1->next = aux2->next;
				free(aux2);
			}
			break;
		default:
			va_end(arg);
			return FAIL;
	}
	l->size -= 1;
	va_end(arg);
	return SUCCESS;
}

void endlist(LIST* l){
	SITE* aux1, *aux2;
	if(emptylist(l)){
		free(l);
		if(DEBUG) printf("Lista apagada com sucesso.\n");
		return;
	}
	aux1 = l->first->next;
	aux2 = l->first;
	while(aux1 != NULL){
		if(DEBUG) printf("Apagando o site \"%s\" da lista.\n", aux2->name);
		free(aux2);
		aux2 = aux2->next;
		aux1 = aux1->next;
	}
	if(DEBUG) printf("Apagando o site \"%s\" da lista.\n", aux2->name);
	free(aux2);
	if(DEBUG) printf("Lista destruida com sucesso.\n");
	free(l);
}

/*===========================================================*/
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
		casa = separadigito(percorre->relev, digito);
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
		eraselist(baldes[i]);
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
