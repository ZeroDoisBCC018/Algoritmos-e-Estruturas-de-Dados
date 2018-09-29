#ifndef _GOOGLEBOTLIST_H_
#define _GOOGLEBOTLIST_H_

#include <stddef.h> 
#include <stdlib.h>
#include <stdio.h>


#define boolean int
#define CSV FILE*


typedef struct site_ SITE;
typedef struct node_ NODE;

/*A NODE struct contains a SITE* and a NODE* for the next node in the list.
typedef struct node_ NODE;
A LIST struct contains an int for the size, one NODE* for the first node 
 and one for the last node on the list
typedef struct list_ LIST;*/
struct node_ {
    SITE* site;
    struct node_ * next;
};

typedef struct list_ {
    NODE* first;
    NODE* last;
    int size;
}LIST;
/*data structure for storing site information, gathered from the csv file
typedef struct site_ SITE;*/

struct site_ {
	int code;
	const char* name;
	int relev;
	const char* link;
	const char keyw[10][50];
};



NODE* GB_NewNode(void);

LIST* GB_NewList(void);

boolean GB_CheckEmptyList(LIST* l);

/* Invalid size: less than 0 or greater than 9999. */
boolean GB_CheckInvalidList(LIST* l);

int GB_EraseList(LIST* l);

/*Insert node at the first position of the list.*/
int GB_InsertNodeFirstPos(LIST* l, NODE* n);

/*Insert node at the last position of the list.*/
int GB_InsertNodeLastPos(LIST* l, NODE* n);

int GB_CountListSize(LIST* l);

/*Search a site on the list by its code.*/
NODE* GB_SearchCode(LIST* l, int code);

/*Sort the list by site codes using a merge sort algorithm. This is the sort function.*/
void GB_Sort(LIST* l);

/*Merge function for the merge sort algorithm.*/
LIST* GB_Merge(LIST* lfirst, LIST* lsecond);

/*Update the relevance of a choosen site.*/
int GB_UpdateRelevance(LIST* l, int code, int newrel);

/*Insert node at an X specific position of the list.*/
int InsertNodeAtPosition (LIST* l, NODE* n, int code);

/*Insert keyword at X specific code of the list)*/
int InsertKeyWord(LIST* l, int code);




/*reads site information from standard input*/
int GB_GetSiteStdin(LIST* l);

/*inserts site information in structure pointed to by a given list position*/
int GB_InsertSite(LIST* l, int pos);

/*removes a site from list according to its code*/
int GB_RemoveSite(LIST* l, int code);



/*checks validity for codes of new site entries*/
int GB_CodeCheck (int code, LIST* l);

/*opens CSV file with read permission*/
CSV GB_OpenCSVread (CSV fp, char* filename);

/*opens CSV file and erases its original content, so it can be rewritten with updated data from structs*/
CSV GB_OpenCSVWrite (CSV fp, char* filename);

/*closes opened CSV file*/
void GB_CloseCSV();

/*reads CSV file and stores its data in site structs*/
int GB_ReadCSV (CSV fp, LIST* s);

/*prints updated struct data into original CSV file*/
int GB_UpdateDataBase (CSV fp, char* filename, LIST* l);


#endif
