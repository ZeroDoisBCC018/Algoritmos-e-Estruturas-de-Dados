#ifndef _GOOGLEBOT_H_
#define _GOOGLEBOT_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
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

SITE* newsite(void);

bool emptylist(LIST* l);

/* Returns TRUE if the size is greater than 9999 or lesser than 0. */
bool invalidlist(LIST* l);

/* Returns 1 if there is any break between the pointers of the list. */
bool listerror(LIST* l);

/* 
 * Lists length starts at 1. 0 is returned if the list is empty.
 * The list will always have the size updated before returning the final count.
 */
int listlength(LIST* l);


/* First site = 1, end of list = (l->size) + 1. */
int insertsite(LIST* l, SITE* s, int pos);

/*
 * Mode 'c' = search by code;
 * Mode 'n' = search by name;
 * Index must be 1;
 * Any other character will return NULL.
 * In the extra argument field: the matching code/string.
 * Example: 600 for code mode. "Google" for name mode.
 */
SITE* searchsite(LIST* l, const char mode, int index, ...);

int site_newrelevance(SITE* s, int newrel);

/*
 * Mode 'c' = search by code;
 * Mode 'n' = search by name;
 * Index must be 1;
 * Any other character will return FAIL.
 * In the extra argument field: the matching code/string.
 * Works like the searchsite function to erase a site in the middle of the list.
 */
int erasesite(LIST* l, const char mode, int index, ...);

void endlist(LIST* l);

int buckpush(LIST* l, SITE* s);

int separadigito(int recebido, int posicao);

LIST* concatena (LIST** listor);

void bucket (LIST* desordenada, int digito);

void radix (LIST* l);

#endif
