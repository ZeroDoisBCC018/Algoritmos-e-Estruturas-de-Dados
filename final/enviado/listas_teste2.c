/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "googlebot.h"

#define DEBUG 1

int main(void){
	LIST* l;
	l = newlist();
	SITE* site1;
	site1 = newsite();
	strcpy(site1->name, "youtube");
	site1->code = 465;
	site1->rel = 487;
	strcpy(site1->link, "youtube.com/pewdiepie");
	strcpy(site1->tag[0], "videos");
	strcpy(site1->tag[1], "seila");
	
	SITE* site2;
	site2 = newsite();
	strcpy(site2->name, "google");
	site2->code = 545;
	site2->rel =356;
	strcpy(site2->link, "google.com/tradutor");
	strcpy(site2->tag[0], "pesquisa");
	strcpy(site2->tag[1], "naosei");
	
	SITE* site3;
	site3 = newsite();
	strcpy(site3->name, "governo");
	site3->code = 12;
	site3->rel = 77;
	strcpy(site3->link, "sp.gov.br");
	strcpy(site3->tag[0], "corrupcao");
	strcpy(site3->tag[1], "fodase");
	
	SITE* site4;
	site4 = newsite();
	strcpy(site4->name, "facebook");
	site4->code = 999;
	site4->rel = 999;
	strcpy(site4->link,  "facebook.com/user");
	strcpy(site4->tag[0], "redesocial");
	strcpy(site4->tag[1], "markzuck");
	
	SITE* site5;
	site5 = newsite();
	strcpy(site5->name, "leninja");
	site5->code = 1;
	site5->rel = 1;
	strcpy(site5->link, "leninja.blogspot.com");
	strcpy(site5->tag[0], "memes");
	strcpy(site5->tag[1], "jooj");
	
	SITE* site6;
	site6 = newsite();
	strcpy(site6->name, "uol");
	site6->code = 88;
	site6->rel = 234;
	strcpy(site6->link, "uol.com.br");
	strcpy(site6->tag[0], "informatica");
	strcpy(site6->tag[1], "forum");
	
	insertsite(l, site1, (l->size)+1);
	printf("tamanho da lista eh %d\n", l->size);
	insertsite(l, site2, (l->size)+1);
	printf("tamanho da lista eh %d\n", l->size);
	insertsite(l, site3, (l->size)+1);
	printf("tamanho da lista eh %d\n", l->size);
	insertsite(l, site4, (l->size)+1);
	printf("tamanho da lista eh %d\n", l->size);
	insertsite(l, site6, (l->size)+1);
	printf("tamanho da lista eh %d\n", l->size);
	insertsite(l, site5, (l->size)+1);
	printf("tamanho da lista eh %d\n", l->size);
	
	if(!listerror(l)) printf("Lista aparenta nao ter erros.\n");
	
	if(DEBUG) printlist(l);

	l = list_insertionsort(l);
	
	printlist(l);
	
	SITE *aux1, *aux2;
	aux1 = searchsite(l, 'n', 1, "facebook");
	puts(aux1->link);
	aux2 = searchsite(l, 'c', 1, 12);
	puts(aux2->link);
	int k;
	k = listlength(l);
	printf("\nLista tem %d sites.\n\n", k);
	erasesite(l, 'n', 1, site6->name);
	aux2->rel = 200;
	site_newrelevance(aux2, 900);
	printf("\nRelevancia nova do site \"%s\" eh %d.\n\n", aux2->name, aux2->rel);
	endlist(l);
	l = newlist();
	if(emptylist(l)) printf("\nLista vazia.\n");
	endlist(l);
	return 0;
}
*/
