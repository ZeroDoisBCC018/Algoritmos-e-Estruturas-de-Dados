#ifndef _GOOGLEBOT_H_
#define _GOOGLEBOT_H_

#include <stddef.h> 
#include <stdlib.h>
#include <stdio.h>
#define boolean int

typedef FILE* CSV;

typedef struct site_ SITE;
typedef struct node_ NODE;
typedef struct list_ LIST;

/* A NODE struct contains a SITE pointer and a NODE pointer for the next node in the list. */
struct node_ {
    SITE* site;
    struct node_ * next;
};

/* A LIST struct contains an integer for the size, one NODE pointer for the first node 
 * and one for the last node on the list. */
typedef struct list_ {
    NODE* first;
    NODE* last;
    int size;
}LIST;

/* Data structure for storing site properties, gathered from the csv file. */
struct site_ {
	int code;
	char* name;
	int relev;
	char* link;
	char keyw[10][50];
};

NODE* GB_NewNode(void);

LIST* GB_NewList(void);

boolean GB_CheckEmptyList(LIST* l);

/* Invalid size: less than 0 or greater than 9999. */
boolean GB_CheckInvalidList(LIST* l);

int GB_EraseList(LIST* l);

/* Insert node at the first position of the list. */
int GB_InsertNodeFirstPos(LIST* l, NODE* n);

/* Insert node at the last position of the list. */
int GB_InsertNodeLastPos(LIST* l, NODE* n);

int GB_CountListSize(LIST* l);

/* Search a site on the list by its code. */
NODE* GB_SearchCode(LIST* l, int code);

/* These 2 functions are part of the list sorting algorithm. It is a mergesort-like algorithm. */
void GB_Sort(LIST* l);
LIST* GB_Merge(LIST* lfirst, LIST* lsecond);

/* Update the relevance of a choosen site. */
int GB_UpdateRelevance(LIST* l, int code, int newrel);

/* Insert node at an X specific position of the list. */
int InsertNodeAtPosition (LIST* l, NODE* n, int code);

/* Insert keyword at X specific code of the list). */
int GB_InsertKeyWord(LIST* l, int code);

/* Reads site information from standard input. */
int GB_GetSiteStdin(LIST* l);

/* Inserts site information in structure pointed to by a given list position. */
int GB_InsertSite(LIST* l, int pos);

/* Removes a site from list according to its code. */
int GB_RemoveSite(LIST* l, int code);

/* Checks validity for codes of new site entries. */
int GB_CodeCheck (int code, LIST* l);

/* Opens CSV file with read permission. */
CSV GB_OpenCSVread (char* filename);

/* Opens CSV file and erases its original content, so it can be rewritten with updated data from the site list. */
CSV GB_OpenCSVWrite (CSV fp, char* filename);

/* Closes opened CSV file. */
int GB_CloseCSV();

/* Reads CSV file and stores its data in site data structures. */
int GB_ReadCSV (CSV fp, LIST* s);

/* Prints updated struct data into original CSV file*/
int GB_UpdateDataBase (CSV fp, char* filename, LIST* l);

#endif
