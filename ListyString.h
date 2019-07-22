#ifndef __LISTY_STRING_H
#define __LISTY_STRING_H

typedef struct ListyNode
{
	// Each node holds a single character.
	char data;

	// Pointer to next node in linked list.
	struct ListyNode *next;
} ListyNode;


typedef struct ListyString
{
	// Head of the linked list representing this string.
	ListyNode *head;

	// Length of this linked list.
	int length;
} ListyString;


// The following line is modified automatically by the test-all.sh script to
// enable and disable unit testing.

#define main main


// Functional Prototypes

int processInputFile(char *filename);

ListyString *createListyString(char *str);

ListyString *destroyListyString(ListyString *listy);

ListyString *cloneListyString(ListyString *listy);

void replaceChar(ListyString *listy, char key, char *str);

void reverseListyString(ListyString *listy);

ListyString *listyCat(ListyString *listy, char *str);

int listyCmp(ListyString *listy1, ListyString *listy2);

int listyLength(ListyString *listy);

void printListyString(ListyString *listy);

double difficultyRating(void);

double hoursSpent(void);


#endif
