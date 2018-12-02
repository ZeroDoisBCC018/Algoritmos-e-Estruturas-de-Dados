#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define DEBUG 1
#define SUCCESS 1
#define FAIL 0
#define ERROR -32000
#define TRUE 1
#define FALSE 0
#define CSV FILE*
#define bool short

typedef struct node{
	int code, rel;
	char link[1025], name[65];
	char** tag;
	struct node* next;
}SITE;

typedef struct list{
	SITE* first;
	SITE* last;
	int size;
}LIST;

typedef struct trienode{
	short end;
	struct trienode* son[26];
	SITE** results;
}TRIE_N;

bool emptylist(LIST* l){
	return (l->first == NULL && l->size == 0 ? TRUE : FALSE);
}

bool invalidlist(LIST* l){
	 return (l->size > 9999 || l->size < 0 ? TRUE : FALSE);
}

TRIE_N* create_node(TRIE_N* n, int lsize){
	int i, j;
	printf("new node being born\n");
	n = (TRIE_N*) malloc(sizeof(TRIE_N));
	n->end = 0;
	n->results = (SITE**) malloc(sizeof(SITE*) * lsize);
	for(i = 0; i < lsize; i++){
		n->results[i] = NULL;
	}
	for(j = 0; j < 26; j++){ /*Atribui todos os nós 'filhos' do nó criado para NULO*/
		n->son[j] = NULL;
	}
	return n;
}

SITE* newsite (void){
	SITE* new = (SITE*) malloc(sizeof(SITE));
	/*if(new == NULL){
		fprintf(stderr, "Fatal error: not enough memory.\n");
		exit(EXIT_FAILURE);
	}*/
	new->next = NULL;
	if(DEBUG) printf("Site alocado.\n");
	new->link[1024] = '\0';
	if(DEBUG) printf("Inicializacao do link feita com sucesso.\n");
	new->name[64] = '\0';
	if(DEBUG) printf("Inicializacao do nome feita com sucesso.\n");
	int i;
	new->tag = (char**) malloc(sizeof(char*) * 10);
	for(i = 0; i < 10; i++){
		new->tag[i] = (char*) malloc(sizeof(char) * 51);
	}
	return new;
}

void insert_word(char word[], TRIE_N* root, SITE* s, int lsize){
	printf("site da palavra sendo inserida: %s\n", s->name);
	TRIE_N* aux = root;
	int i = 0, j = 0;
	int len = strlen(word);
	int pos = 0;
	for(i = 0; i < len; i++){
		word[i] = tolower(word[i]);
		pos = word[i] - 'a'; /*pos será um número entre 0 e 25 que representa uma letra do alfabeto*/
		printf("value of letter '%c' is %d\n", word[i], pos);
		if (aux->son[pos] == NULL) aux->son[pos] = create_node(aux->son[pos], lsize);
		if (i == len-1){
			aux->son[pos]->end = 1;
			//printf("ultima letra da palavra\n");
			for(j = 0; j < lsize; j++){
				if(aux->son[pos]->results[j] == NULL){
					printf("site sendo inserido no vetor: %s, a pos é %d\n", s->name, j);
					aux->son[pos]->results[j] = s;
					printf("site inserido: %s, url: %s, code: %d, relev: %d \n", aux->son[pos]->results[j]->name, aux->son[pos]->results[j]->link, aux->son[pos]->results[j]->code, aux->son[pos]->results[j]->rel);
					break;
				}
				else printf("palavra ja existe em: %s, na pos %d\n", aux->son[pos]->results[j]->name, j);
			} 
			break;
		} 
		aux = aux->son[pos];
	}
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


void search_keyword(char* word, TRIE_N* root, int lsize, int flag){
	int i, j;
	printf("word being searched: %s\n", word);
	TRIE_N* aux = root;
	int len = strlen(word);
	int pos = 0;
	for(i = 0; i < len; i++){
		pos = word[i] - 'a';
		if (aux->son[pos] == NULL){
		    printf("keyword not found\n");
		    break;
		}
		else if (i == len-1){
		    if (aux->son[pos]->end != 1 && flag != 1) printf("keyword not found\n");
		    else {
		   	
		    	for(j = 0; j < lsize; j++){
				//printf("searching: %s in %dst site\n", word, j+1);
		    		if(aux->son[pos]->results[j] != NULL){
				       	if (flag != 1) printf("POS = %d keyword was found in %s, url is %s\n", j, aux->son[pos]->results[j]->name, aux->son[pos]->results[j]->link);
					else {
						node_insertionsort(aux->son[pos]);
						printf("\t uma recomendação é %s, url:%s", aux->son[pos]->results[j]->name, aux->son[pos]->results[j]->link);
					}
				}	
				else break;
		   	}
		    }
		    break;
		}
		aux = aux->son[pos];
	}
}

void recommendation(char* word, TRIE_N* root, int lsize){
	int i, j, k;
	printf("word being searched: %s\n", word);
	TRIE_N* aux = root;
	int len = strlen(word);
	int pos = 0;
	for(i = 0; i < len; i++){
		pos = word[i] - 'a';
		if (aux->son[pos] == NULL){
		    printf("keyword not found\n");
		    break;
		}
		else if (i == len-1){
		    if (aux->son[pos]->end != 1) printf("keyword not found\n");
		    else {
		    	for(j = 0; j < lsize; j++){
				//printf("searching: %s in %dst site\n", word, j+1);
		    		if(aux->son[pos]->results[j] != NULL) printf("POS = %d keyword was found in %s, url is %s\n", j, aux->son[pos]->results[j]->name, aux->son[pos]->results[j]->link); 
				else break;
				printf("As palavras chave do site %s (que compartilha a palavra %s) são:\n", aux->son[pos]->results[j]->name, word);
				for(k = 0; k < 10; k++){
					printf("%s\n", aux->son[pos]->results[j]->tag[k]);
					if (aux->son[pos]->results[j]->tag[k][0] != '\0') search_keyword(aux->son[pos]->results[j]->tag[k], root, lsize, 1);
				}
		   	}
		    }
		    break;
		}
		aux = aux->son[pos];
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

/*int readcsv2(CSV data, LIST* l, TRIE_N* root){
	if(data == NULL || root == NULL || l == NULL) return FAIL;
	fseek(data, 0, SEEK_SET);
	// dump no header (1a linha)
	char c = 'a';
	int offset = 0, line = 0;

   	do{
        	c = fgetc(data);
		offset += 1;
    	}while(c != '\n');

	// contando numero de sites
	do{
		c = fgetc(data);
		if(c == '\n') line += 1;
		// existe um \n antes do EOF
	}while(c != EOF);

	c = 'a';
	// volta pra linha do 1o site
	fseek(data, offset, SEEK_SET);
	SITE* node = NULL;
	char tagbuffer[51]
	if(DEBUG) printf("CSV tem %d linhas.\n", line);
	int i, j = 0;
	for(i = 0; i < line; i++){
		node = newsite();
		insertsite(l, node, l->size+1);
		fscanf(data, "%4d[^,]%*c", node->code);
		fscanf(data, "%64[^,]%*c", node->name);
		if(DEBUG) printf("Site \"%s\":\n", node->name);
		fscanf(data, "%4d[^,]%*c", node->relevance);
		fscanf(data, "%1024[^,]%*c", node->link);
		c = fgetc(data); // pega as primeiras aspas
		do{
			fscanf(data, "%51[^,|\"]", tagbuffer);
			if(DEBUG) printf("Tag adquirida = \"%s\".\n", tagbuffer);
			strcpy(node->tag[j], tagbuffer);
			insert_word(root, node, l->size);
			j += 1;
			c = fgetc(data);
		}while(c != '"');
		// pega o \n
		c = fgetc(data);
	}
	return SUCCESS;
}*/

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
	l->size = count1;	
 
	while(count2 < (count1-1)){
		aux->next = newsite();
		aux = aux->next;
		count2++;
		if(DEBUG) printf("count2 = %d\n", count2);
	}
	l->last = aux;
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
		
		while(dump != '"' && k < 10){	
			fscanf(fp, "%[^,\"]", aux->tag[k]);
			//if (DEBUG) printf("=!=!=!=!=!=!= tag sendo inserida é %s =!=!=!=!=!=!=\n", aux->tag[k]);
			insert_word(aux->tag[k], root, aux, l->size);
			dump = fgetc(fp);
			if (DEBUG) printf("dump = %c\n", dump);
			k++;
		}
		if (aux->next == NULL) break;
		aux = n->next;
		l->size++;
		count3++;
		//printf("do while realizado %d vezes\n", count3);
		//printf("count1: %d\n", count1);
		dump= ' ';
		k = 0;
	}while(feof(fp) == 0 && count3 < count1 - 1);
	
	fclose(fp);
	printf("linha final do csv, linhas do test foram lidas\n");
	return SUCCESS;
}

/*=================================================================================================*/

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

int main(void){
	LIST* l = newlist();
	TRIE_N* root = NULL;
	root = create_node(root, 31); 
	CSV fp = opencsv("csv.csv");
	readcsv(fp, l, root);
	
	search_keyword("youtubr", root, l->size, 0);
	search_keyword("socia", root, l->size, 0);
	recommendation("puppy", root, l->size);
	recommendation("dogger", root, l->size);
	recommendation("youtube", root, l->size);
	recommendation("reddit", root, l->size);
	recommendation("social", root, l->size);
	recommendation("googlebot", root, l->size);
	recommendation("people", root, l->size);
	search_keyword("googlebob", root, l->size, 0);
	search_keyword("social", root, l->size, 0);
	search_keyword("boss", root, l->size, 0);
	search_keyword("niba", root, l->size, 0);
	search_keyword("black", root, l->size, 0);
	
	return 0;
}
