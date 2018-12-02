#include "googlebot.h"                                                                                                       
#include <ctype.h>
/* 0 = sem printf de debug
 * 1 = com printf de debug */
#define DEBUG 0

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

SITE* newsite(void){
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
	return (l->first == NULL && l->size == 0 ? TRUE : FALSE);
}

bool invalidlist(LIST* l){
	 return (l->size > 9999 || l->size < 0 ? TRUE : FALSE);
}

bool listerror(LIST* l){
	
	int i = 1;
	
	if(emptylist(l)) return 0;
	SITE* aux;
	aux = l->first;
	
	while(aux->next != NULL) 
	{
		aux = aux->next;
		i++;
		if (DEBUG) printf("!!!!!!! Ponteiro em aux de listerror é %p, na iteração %d\n", aux, i);
	}
	
	if (DEBUG) printf("!!!!!!! Ponteiro aux PÓS WHILE de listerror é %p, na iteração %d\n", aux, i);
	
	if (DEBUG) printf("TUDO SUAVE NA POSICAO %d\n", i);
	
	if(aux == l->last) return 0;
	else fprintf(stderr, "The list is fragmented at some point. Returning error...\n");
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
		if (DEBUG) printf ("---- insert site no caso de lista vazia ----\n"); 
		l->first = s;
		l->last = s;
		l->last->next = NULL;
		l->size = 1;
		if(DEBUG) printf("Site \"%s\" inserido com sucesso.\n", s->name);
		return (l->first == NULL || l->last == NULL ? FAIL : SUCCESS);
	} else if(pos == 1){
		aux = l->first;
		l->first = s;
		s->next = aux;
		/*aux = NULL;*/
		l->size += 1;
		if(DEBUG) printf("Site \"%s\" inserido com sucesso.\n", s->name);
		return (l->first == NULL || s->next == NULL ? FAIL : SUCCESS);
	} else if(pos == (l->size)+1){
		l->last->next = s;
		l->last = s;
		l->last->next = NULL;
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
	if(DEBUG) printf("tamanho da lista eh %d\n", l->size);
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

void printlist(LIST* l){
	if(emptylist(l)){
		fprintf(stdout, "This list is empty.\n");
		return;
	}
	if(listerror(l)) return;
	SITE* aux;
	aux = l->first;
	while(aux != NULL){
		fprintf(stdout, "\"%s\" (Code %04d):\n", aux->name, aux->code);
		fprintf(stdout, "Link: <%s>\n", aux->link);
		fprintf(stdout, "Current relevance: %04d\n", aux->rel);
		printf("Site tags/keywords:\n");
		int i;
		for(i = 0; i < 10; i++){
			fprintf(stdout, "--");
			printf("%s\n", aux->tag[i]);
		}
		fprintf(stdout, "\n");
		aux = aux->next;
	}
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

/*
int buckpush(LIST* l, SITE* s){
	
	if(invalidlist(l) || l->size == 9999) return ERROR;
	if(emptylist(l)){
		l->first = s;
		l->last = s;
		l->last->next = NULL;
		l->size = 1;
		if(DEBUG) printf("Site %s inserido com sucesso NO PRIMEIRO LUGAR DA LISTA.\n", s->name);
		return SUCCESS;
	}
	else{

		l->last->next = s;
		l->last = s;
		l->last->next = NULL;
		l->size++;
				if(DEBUG) printf("Site %s inserido com sucesso DEPOIS DO PRIMEIRO LUGAR DA LISTA.\n", s->name);

		return SUCCESS;
	}	
}

int separadigito (int recebido, int posicao){
	int digito;
	
	digito = (recebido / (int) pow(10, (posicao-1)) %10);
	
	if(DEBUG) printf("DÍGITO EM SEPARADIGITO É %d\n", digito);
	
	return digito;
}

LIST* concatena(LIST** listor){
	
	int i;
	LIST* concatenada;
	concatenada = newlist();
	SITE* percorre;
	
	for (i = 0; i < 10; i++){
		percorre = listor[i]->first;
		
		while (percorre != NULL){
			insertsite(concatenada, percorre, (concatenada->size)+1);
			percorre = percorre->next;
		}
	}
	free(percorre);
	return concatenada;
}

void bucket(LIST* desordenada, int digito){
	
	if (DEBUG) printf("\n=== A LISTA CHEGA ASSIM ===\n");
	
	printlist(desordenada);
	
	if (DEBUG) printf("ENTRAMOS NO BUCKET\n");
	
	LIST* ordenada;
	LIST** baldes = (LIST**) malloc (sizeof(LIST*) * 10);
	
	if (DEBUG && baldes != NULL) printf ("PONTEIRO DUPLO DE BALDES ALOCADO\n");
	
	int casa, i;
	SITE* percorre;
	
	for (i = 0; i < 10; i++){
		baldes[i] = newlist();
		if(DEBUG) printf ("balde %d inicializado\n", i);
	}
	percorre = desordenada->first;
	
	while (percorre != NULL){
		casa = separadigito(percorre->rel, digito);
		insertsite(baldes[casa], percorre, (baldes[casa]->size)+1);
		
		if(DEBUG) printf("site %s inserido pela rel %d no balde %d\n", percorre->name, percorre->rel, casa);
		
		if(DEBUG) listerror(baldes[casa]); 
		
		if(DEBUG) printf("\n=== TAMANHO DO BUCKET ATUAL É %d ===\n", baldes[casa]->size);
		if(DEBUG) printf("====== percorre %p esta em %s \n", percorre, percorre->name);
		
		percorre = percorre->next;
	}
	if(DEBUG) printf("saiu do while que percorre a lista desordenada e insere no bucket\n");
	ordenada = concatena(baldes);
	printf("tamanho da lista eh %d\n", ordenada->size);
	
	if (DEBUG) printf("\n==== INÍCIO DA IMPRESSÃO DA LISTA ORDENADA ====\n");
	if (DEBUG) printlist(ordenada);
	if (DEBUG) printf("\n---- FIM DA IMPRESSÃO DA LISTA ORDENADA ----\n");
	
	LIST* nosauxiliares;
	nosauxiliares = newlist();
	
	nosauxiliares->first = ordenada->first;
	nosauxiliares->last = ordenada->last;
	
	desordenada->first = nosauxiliares->first;
	desordenada->last = nosauxiliares->last;
	
	free(nosauxiliares);
	
	for (i = 0; i < 10; i++){
		endlist(baldes[i]);
	}
	free(baldes);
}

void radix(LIST* l){
	int i;
	for (i = 1; i <= 4; i++){
		bucket (l, i);
	}
	if (DEBUG) printf("\nFIM DO RADIX\n");
}
*/

LIST* list_insertionsort(LIST* l){
	if(l == NULL || emptylist(l) || listerror(l)) return NULL;
	SITE *aux1, *aux2;
	int tam = listlength(l);
	SITE* vet[tam];
	aux1 = l->first;
	int i = 0, j;
	
	while(aux1 != NULL){
		vet[i] = aux1;
		if(DEBUG) printf("Sort passando pelo site %s. Pos = %d.\n", aux1->name, i);
		i++;
		aux1 = aux1->next;
	}

	for(i = 1; i < tam; i++){
		aux2 = vet[i];
		j = i-1;
		
		while(j >= 0 && vet[j]->rel < aux2->rel){
			vet[j+1] = vet[j];
			j = j-1;
		}
		vet[j+1] = aux2;
	}
	
	LIST* sorted = newlist();
	aux1 = l->first;
	aux2 = l->first->next;
	
	while(aux2 != NULL){
		aux1->next = NULL;
		aux1 = aux2;
		aux2 = aux2->next;
	}
	free(l);
	
	for(i = 0; i < tam; i++){
		insertsite(sorted, vet[i], i+1);
	}
	return sorted;
}

void node_insertionsort(TRIE_N* end){
	
	int tam = 0;
	SITE* aux;
	while(aux != NULL){
		aux = end->results[tam];
		tam += 1;
	}
	
	int i, j;
	for(i = 1; i < tam; i++){
		aux = end->results[i];
		j = i-1;
		
		while(j >= 0 && end->results[j]->rel < aux->rel){
			end->results[j+1] = end->results[j];
			j = j-1;
		}
		end->results[j+1] = aux;
	}
}

int newsite_stdin(LIST* l){
	SITE* new = newsite();
	SITE* aux;
	int ncode, relev;
	scanf("%d", &ncode);
	if(ncode < 0 || ncode > 9999){
		puts("Invalid code! Returning...");
		return FAIL;
	}
	aux = searchsite(l, 'c', ncode);
	if(aux != NULL){
		puts("This code already exists in the database. Returning...");
		return FAIL;
	}
	scanf("%d", &relev);
	if(relev < 0 || relev > 1000){
		puts("Invalid relevance. Returning...");
		return FAIL;
	}
	char name[65];
	fgets(name, 65, stdin);
	char link[1025];
	fgets(link, 1025, stdin);
	int i;
	for(i = 0; i < 10; i++){
		fgets(new->tag[i], 51, stdin);
	}
	strcpy(new->name, name);
	strcpy(new->link, link);
	new->code = ncode;
	new->rel = relev;
	insertsite(l, new, l->size+1);
	return SUCCESS;
}

void printsite(SITE* s){
	printf("%s <%s>\n", s->name, s->link);
	printf("Code: %04d\n", s->code);
	printf("relevance: %04d\n", s->rel);
	int i;
	puts("Keywords/Tags:");
	for(i = 0; i < 10; i++){
		printf("--%s\n", s->tag[i]);
	}
}

void insert_word(char word[], TRIE_N* root, SITE* s, int lsize){
	if(DEBUG) printf("site da palavra sendo inserida: %s\n", s->name);
	TRIE_N* aux = root;
	int i, j;
	if(DEBUG) printf("no aux aponta para root\n");
	int len = strlen(word);
	if(DEBUG) printf("strlen efetuado = %d\n", len);
	int pos = 0;
	for(i = 0; i < len; i++){
		word[i] = tolower(word[i]);
		pos = word[i] - 'a'; /*pos será um número entre 0 e 25 que representa uma letra do alfabeto*/
		if(DEBUG) printf("value of letter '%c' is %d\n", word[i], pos);
		if (aux->son[pos] == NULL) aux->son[pos] = create_node(aux->son[pos], s, lsize);
		if (i == len-1){
			aux->son[pos]->end = 1;
			if(DEBUG) printf("ultima letra da palavra\n");
			for(j = 0; j < (lsize + 1); j++){
				if(aux->son[pos]->results[j] == NULL){
					if(DEBUG) printf("site sendo inserido no vetor: %s, a pos é %d\n", s->name, j);
					aux->son[pos]->results[j] = s;
					if(DEBUG) printf("site inserido: %s, url: %s, code: %d, relev: %d \n", aux->son[pos]->results[j]->name, aux->son[pos]->results[j]->link, aux->son[pos]->results[j]->code, aux->son[pos]->results[j]->rel);
					break;
				}
				if(DEBUG) printf("palavra ja existe em: %s, na pos %d\n", aux->son[pos]->results[j]->name, j);
			} 
			break;
		} 
		aux = aux->son[pos];
	}
}

void search_keyword(char* word, TRIE_N* root, int lsize){
	int i, j, k = 1;
	if(DEBUG) printf("word being searched: %s\n", word);
	TRIE_N* aux = root;
	int len = strlen(word);
	int pos = 0;
	for(i = 0; i < len; i++){
		pos = word[i] - 'a';
		if (aux->son[pos] == NULL){
		    printf("Keyword not found.\n");
		    break;
		}
		else if (i == len-1){
		    if (aux->son[pos]->end != 1) printf("Keyword not found\n");
		    else{
		    	for(j = 0; j < lsize+1; j++){
					if(DEBUG) printf("searching: %s in %dst site\n", word, j+1);
		    		if(aux->son[pos]->results[j] != NULL){
						printf("Result %04d: \"%s\", URL is <%s>.\n", k, aux->son[pos]->results[j]->name, aux->son[pos]->results[j]->link);
						k += 1;
					}
				}
		    break;
			}
		aux = aux->son[pos];
		}
	}
}

void free_trie(TRIE_N* node){
	int i;
	for(i = 0; i < 26; i++){
		if(node->son[i] != NULL) free_trie(node->son[i]);
	}
	free(node);
	return;
}

CSV opencsv(char* filename){
	if(filename == NULL){
		perror(".csv filename not given.\n");
		exit(EXIT_FAILURE);
	}
	CSV fp;
	fp = fopen(filename, "r");
	if (DEBUG) printf("PONTEIRO foi aberto: %p\n", fp);
	if(fp == NULL){
		perror(".csv file could not be opened. Not enough memory/fatal error ocurred while trying to access the disk.\n");
		exit(EXIT_FAILURE);
	}
	return fp;
}

int readcsv (CSV fp, LIST* l, TRIE_N* root){
	if (invalidlist(l)) return ERROR;
	int k = 0;
	char dump = ' '; /* Dump is initialized as a blank space so it certainly enters the reading loop on line x. */
	if(fp == NULL){
		perror("File not opened.\n");
		return ERROR;
	}
	
	rewind(fp);
	
	char ignoreline[200];
	fscanf(fp," %[^\n\r]%*c", ignoreline); /* Ignores the header from the CSV file. */
	int count1 = 0;
	int count2 = 0;
	
	do{ 
		fscanf(fp, " %[^\n\r]%*c", ignoreline);
		count1++;
		if (DEBUG) printf("count1 = %d\n", count1);
	} while(!feof(fp)); /* This loop counts all the lines of the csv file and rewinds it before performing the reading operations. */
	
	rewind(fp);
	
	SITE* n = NULL;
	n = newsite(); /* In this loop a number of nodes equivalent to the number of sites counted are created and linked. */
	l->first = n;
	 SITE* aux = NULL;
	 aux = n;
	 
	while(count2 != (count1-1)){
		aux->next = newsite();
		aux = n->next;
		count2++;
		if(DEBUG) printf("count2 = %d\n", count2);
	}
	printf("saiu do loop count2\n");
	 
	aux = l->first; /* Now the node is reset to the first position of the list in order to read the informations. */
	int count3 = 0;
	
	do{
		fscanf(fp, "%[^\n]", ignoreline); /* Ignores . */
		if(DEBUG) printf("header ignorado: %s wahoo\n", ignoreline);
		
		fscanf(fp, "%d", &aux->code);
		
		if (DEBUG) printf("code lido = %d\n", aux->code);
		fscanf(fp, ",%[^,],%d,%[^,],", aux->name, &aux->rel, aux->link);
		if (DEBUG) printf("nome, relevancia e link lidos: %s ;%d ;%s\n", aux->name, aux->rel, aux->link);
		fgetc(fp); /*ignora aspas iniciais do link*/
		if (DEBUG) printf("Antes de zerar colunas das tags\n");
		
		printf("Colunas de tag zeradas\n");

		while(dump != '"' && k < 10){	
			fscanf(fp, "%[^,\"]", aux->tag[k]);
			if (DEBUG) printf("=!=!=!=!=!=!= tag sendo inserida é %s =!=!=!=!=!=!=\n", aux->tag[k]);
			insert_word(aux->tag[k], root, aux, l->size);
			dump = fgetc(fp);
			if (DEBUG) printf("dump = %c\n", dump);
			k++;
		}
		if (aux->next == NULL) break;
		aux = n->next;
		l->size++;
		count3++;
		if(DEBUG){
			printf("do while realizado %d vezes\n", count3);
			printf("count1: %d\n", count1);
		}
		dump= ' ';
		k = 0;
	}while(feof(fp) == 0 && count3 < count1 - 1);
	
	fclose(fp);
	if(DEBUG) printf("linha final do csv, linhas do test foram lidas\n");
	return SUCCESS;
}

TRIE_N* create_node(TRIE_N* n, SITE* s, int lsize){
	int i, j;
	if(DEBUG) printf("new node being born\n");
	n = (TRIE_N*) malloc(sizeof(TRIE_N));
	n->end = 0;
	n->results = (SITE**) malloc(sizeof(SITE*) * lsize);
	for(i = 0; i < lsize; i++){
		n->results[i] = NULL;
	}
	for(j = 0; j < 26; j++){
		n->son[j] = NULL;
	}
	return n;
}


void update_database (CSV csv, char* name, LIST* list){
	
	int i = 0;
	SITE* percorre;
	
	percorre = list->first;
	
	freopen(name, "w", csv);
	
	fprintf(csv, "\"Index\",\"Site\",\"Relevance\",\"Link\",\"Tags\"");
	fprintf(csv, "\n");
	
	while (percorre != NULL)
	{
	
	fprintf(csv, "%d,%s,%d,%s", percorre->code, percorre->name, percorre->rel, percorre->link);
	fprintf(csv, "\"");
	
		while (percorre->tag[i][0] != '\0' || i <= 10)
		{
			fprintf(csv, "%s", percorre->tag[i]);
			i++;
			
			if(percorre->tag[i+1][0] != '\0') printf (",");
		}
		
		fprintf(csv, "\"");
		fprintf(csv, "\n");
	
	percorre = percorre->next;
	
	}

}
