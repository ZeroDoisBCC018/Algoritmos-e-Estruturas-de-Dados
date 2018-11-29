#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
	int code, rel;
	char link[1025], name[65];
	char tag[10][51];
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

TRIE_N* create_node(TRIE_N* n, SITE* s, int lsize){
	printf("new node being born\n");
	n = (TRIE_N*) malloc(sizeof(TRIE_N));
	n->end = 0;
	n->results = (SITE**) malloc(sizeof(SITE*) * lsize);
	for(int i = 0; i < lsize; i++){
		n->results[i] = NULL;
	}
	for(int j = 0; j < 26; j++){ /*Atribui todos os nós 'filhos' do nó criado para NULO*/
		n->son[j] = NULL;
	}
	return n;
}

void insert_word(char word[], TRIE_N* root, SITE* s, int lsize){
	TRIE_N* aux = root;
	short flag = 0;
	printf("no aux aponta para root\n");
	int len = strlen(word);
	printf("strlen efetuado = %d\n", len);
	int pos = 0;
	for(int i = 0; i < len; i++){
		pos = word[i] - 'a'; /*pos será um número entre 0 e 25 que representa uma letra do alfabeto*/
		//printf("value of letter '%c' is %d\n", word[i], pos);
		if (aux->son[pos] == NULL) aux->son[pos] = create_node(aux->son[pos], s, lsize);
		if (i == len-1){
			aux->son[pos]->end = 1;
			for(int j = 0; j < lsize; j++){
				if(aux->son[pos]->results[j] == NULL){
					 aux->son[pos]->results[j] = s;
					 break;
				}	
			} 
			break;
		} 
		aux = aux->son[pos];
	}
}

void search_keyword(char* word, TRIE_N* root, int lsize){
	printf("word being searched: %s\n", word);
	TRIE_N* aux = root;
	int len = strlen(word);
	int pos = 0;
	for(int i = 0; i < len; i++){
		pos = word[i] - 'a';
		if (aux->son[pos] == NULL){
		    printf("keyword not found\n");
		    break;
		}
		else if (i == len-1){
		    if (aux->son[pos]->end != 1) printf("keyword not found\n");
		    else {
		    	for(int j = 0; j < lsize; j++){
				//printf("searching: %s in %dst site\n", word, j+1);
		    		if(aux->son[pos]->results[j] != NULL) printf("keyword was found in %s, url is %s\n", aux->son[pos]->results[j]->name, aux->son[pos]->results[j]->link); 
		   	}
		    }
		    break;
		}
		aux = aux->son[pos];
	}
}

void free_trie(TRIE_N* node){
	for(int i = 0; i < 26; i++){
		if(node->son[i] != NULL) free_trie(node->son[i]);
	}
	free(node);
	return;
}

/*void print_trie(TRIE_N* node){
	char c;
	for(int i = 0; i < 26; i++){
		if(node->son[i] != NULL){
			c = (char) (i + 'a');
			printf("%c", c);
			if (node->end == 1) printf("\n");
			print_trie(node->son[i]);	
		}
	}
	return;
}*/

/*int readcsv (CSV fp, LIST* l, TRIE_N* root){
	if (invalidlist(l)) return ERROR;
	int i = 0, j = 0, k = 0;
	char dump = ' '; // Dump is initialized as a blank space so it certainly enters the reading loop on line x.
	if(fp == NULL){
		perror("File not opened.\n");
		return ERROR;
	}
	
	rewind(fp);
	
	char ignoreline[200];
	fscanf(" %[^\n\r]%*c", ignoreline, fp); // Ignores the header from the CSV file.
	int count1 = 0;
	int count2 = 0;
	
	do{ 
		fscanf(" %[^\n\r]%*c", ignoreline, fp);
		count1++;
		if (DEBUG) printf("count1 = %d\n", count1);
	} while(!feof(fp)) // This loop counts all the lines of the csv file and rewinds it before performing the reading operations.
	
	rewind(fp);
	
	SITE* n = NULL;
	n = newsite(); // In this loop a number of nodes equivalent to the number of sites counted are created and linked.
	l->first = n;
	 SITE* aux = NULL;
	 aux = n;
	 
	while(count2 != (count1-1)){
		aux->next = GB_NewNode();
		aux = n->next;
		count2++;
		if(DEBUG) printf("count2 = %d\n", count2);
	}
	printf("saiu do loop count2\n");
	 
	aux = l->first; // Now the node is reset to the first position of the list in order to read the informations.
	int count3 = 0;
	
	do{
		fscanf(" %[^\n\r]%*c", ignoreline, fp); // Ignores again the header.
		if(DEBUG) printf("header ignorado wahoo\n");
		
		fscanf(fp, "%d", &aux->code);
		
		if (DEBUG) printf("code lido = %d\n", aux->site->code);
		fscanf(fp, ",%[^,],%d,%[^,],", aux->name, &aux->rel, aux->link);
		if (DEBUG) printf("nome, relevancia e link lidos\n");
		fgetc(fp); //ignora aspas iniciais do link
		if (DEBUG) printf("Antes de zerar colunas do keyword\n");
		
		
		//========================
		for(i = 0; i < 10; i++){
			aux->keyw[i][0] = '#';
		}
		printf("Colunas de keyword zeradas\n");
		while (dump != '"'){
			dump = fgetc(fp);
			if(dump == '"'){
				insert_word(aux->site->keyw[j], root, aux->site);
				break;
			}
			if(dump == ','){
				insert_word(aux->site->keyw[j], root, aux->site);
				j++;
				k = 0;
				dump = fgetc(fp);
			}
			else{
				aux->site->keyw[j][k] = dump;
				k++;
			}
		}	
		if (aux->next == NULL) break;
		aux = n->next;
		l->size++;
		count3++;
		printf("do while realizado %d vezes\n", count3);
	}while(!feof(fp));
	
	return SUCCESS;
}*/

int main(int argc, char* argv[]){
	int lsize = 50;
	SITE* teste = (SITE*) malloc(sizeof(SITE));
	teste->code = 6345;
	teste->rel = 9981;
	strcpy(teste->link, "boss.com");
	strcpy(teste->name, "boos");
	strcpy(teste->tag[1], "bomdia1");
	strcpy(teste->tag[2], "boanoite1");
	strcpy(teste->tag[3], "boamanha1");
	printf("site teste criado\n");
	
	SITE* teste2 = (SITE*) malloc(sizeof(SITE));
	teste2->code = 0345;
	teste2->rel = 9987;
	strcpy(teste2->link, "baus2.com");
	strcpy(teste2->name, "n word");
	strcpy(teste2->tag[1], "bomdia2");
	strcpy(teste2->tag[2], "boanoite2");
	strcpy(teste2->tag[3], "boamanha2");
	teste2->next = NULL;
	printf("site teste2 criado\n");
	
	teste->next = teste2;

	TRIE_N* root = NULL;
	root = create_node(root, teste, 5);
	printf("no raiz da trie criado\n");

	insert_word("black", root, teste, lsize);
	insert_word("black", root, teste2, lsize);
	insert_word("brutal", root, teste, lsize);
	insert_word("boss", root, teste, lsize);
	insert_word("hes", root, teste, lsize);
	insert_word("the", root, teste, lsize);
	insert_word("boss", root, teste2, lsize);
	insert_word("bossniba", root, teste, lsize);

	search_keyword("white", root, lsize);
	search_keyword("brute", root, lsize);
	search_keyword("black", root, lsize);
	search_keyword("boss", root, lsize);

	free_trie(root);

	return 0;
}
