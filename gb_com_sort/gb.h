#ifndef _GOOGLEBOT_H_
#define _GOOGLEBOT_H_
#include <stdio.h>
#include <stdlib.h>
#define SUCCESS 1
#define FAIL 0
#define ERROR -32000
#define TRUE 1
#define FALSE 0
#define CSV FILE*
#ifndef bool
#define bool short
#endif

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
	SITE* results[];
}TRIE_N;

LIST* newlist(void);

SITE* newsite (void);

bool invalidlist(LIST* l);

bool emptylist(LIST* l);

int listlength(LIST* l);

int insertsite(LIST* l, SITE* s, int pos);

int buckpush (LIST* l, SITE* s);

int separadigito (int recebido, int posicao);

LIST* concatena (LIST** listor);

void bucket (LIST* desordenada, int digito);

void radix (LIST* l);

#endif
