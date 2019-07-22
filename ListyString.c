 // Andrew Gomes - COP 3502 FALL 2017
// Sean Szumlanski
// NID: an289864
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ListyString.h"

int processInputFile(char *filename) {
	
	FILE *file = fopen(filename, "r");
	char *firstLine = malloc(sizeof(char) * 1024);
	char *commandString = malloc(sizeof(char) * 1026);
	ListyString *linkedString;

	if (file == NULL) {
		return 1;
	}
  
	fscanf(file, "%s", firstLine);
	linkedString = createListyString(firstLine);

	while (fgets(commandString, 1026, file) != NULL) {
		switch (commandString[0]) {
			case '@':
				replaceChar(linkedString, *(commandString + 2), (commandString + 4));
			break;
			case '+':
				listyCat(linkedString, (commandString + 2));
			break;
			case '-':
				replaceChar(linkedString, *(commandString + 2), NULL);
			break;
			case '~':
				reverseListyString(linkedString);
			break;
			case '?':
				printf("%i\n", listyLength(linkedString));
			break;
			case '!':
				printListyString(linkedString);
			break;
		}
	}

	free(commandString);
	free(firstLine);
	fclose(file);
}

ListyString *createListyString(char *str) {

	int i;
	int length;
	ListyString *string = malloc(sizeof(struct ListyString));
	ListyNode *currentNode = malloc(sizeof(struct ListyNode));	
	ListyNode *nextNode = malloc(sizeof(struct ListyNode));

	if (string == NULL) {
		return NULL;
	}
 
  // Argument was null, return an empty string
  if (str == NULL || strlen(str) <= 0) {
    string->head = NULL;
    string->length = 0;
    return string;
  }
 
  length = strlen(str);

	// Set up the string's length and entry point
	string->length = length;
	string->head = currentNode;

	for (i = 0; i < length; i++) {	
		// Set the char as the node's data
		currentNode->data = str[i];

		// Set next to point to the next node, or to point to null if it's the last node.
		if (i == length - 1) {
			currentNode->next = NULL;
			free(nextNode);
		} else {
			// For the next iteration, our current node becomes the next node to use,
			// and the node after that is malloced.
			currentNode->next = nextNode;
			currentNode = nextNode;
			nextNode = malloc(sizeof(struct ListyNode));
		}
	}

	return string;
}

void reverseListyString(ListyString *listy) {

  int i = 0;
	ListyNode *currentNode;
	ListyNode *previousNode;
	ListyNode *newNextNode;
	
	if (listy == NULL || listy->head == NULL) {
		return;
	}

	currentNode = listy->head;

	while (currentNode != NULL) {
    		i++;
		newNextNode = currentNode->next;
		currentNode->next = previousNode;
		previousNode = currentNode;
		currentNode = newNextNode;
    		if (i == 1) {
      		// Ensure proper termination
      			previousNode->next = NULL;
    		}
	}

	// New head has now been established
	listy->head = previousNode;
 
}

void printListyString(ListyString *listy) {

	int i;
	ListyNode *currentNode;;

	if (listy == NULL || listy->length <= 0) {
		printf("(empty string)\n");
		return;
	}
 
  currentNode = listy->head;

	for (i = 0; i < listy->length; i++) {
		printf("%c", currentNode->data);
		fflush(stdout);
		currentNode = currentNode->next;
	}

	printf("\n");
}

int listyCmp(ListyString *listy1, ListyString *listy2) {

	int i;
	ListyNode *currentListy1Node;
	ListyNode *currentListy2Node;

	// Considered equal if BOTH are null
	if (listy1 == NULL && listy2 == NULL) {
		return 0;
	}

	// Considered NOT equal if one is null and the other is non-null but doesn't have a head.
	if ((listy1 == NULL && listy2 != NULL && listy2->head == NULL) || (listy1 != NULL && listy1->head == NULL && listy2 == NULL)) {
		return -1;
	}

	// Can't be equal if different lengths!!
	// Two empty lists are however considered equal
	if (listy1->length != listy2->length) {
		return -1;
	} else if (listy1->length == 0 && listy2->length == 0) {
		return 0;
	}

	// Compare the linked lists if they're the same size
	currentListy1Node = listy1->head;
	currentListy2Node = listy2->head;
	for (i = 0; i < listy1->length; i++) {
		
		// Found an unequal node, exit early.
		if (currentListy1Node->data != currentListy2Node->data) {
			return -1;
		}
		
		currentListy1Node = currentListy1Node->next;
		currentListy2Node = currentListy2Node->next;
	}

	// Loop completed without finding an mismatch
	return 0;
}

ListyString *destroyListyString(ListyString *listy) {

	int i;
	ListyNode *node;

	if (listy == NULL || listy->length <= 0) {
		return NULL;
	}
	
	if (listy->head != NULL) {
		while (listy->head != NULL) {
       			node = listy->head;
       			listy->head = listy->head->next;
       			free(node);
		}
	}			

	free(listy);
	return listy = NULL;
}

// Custom function (finds the tail of the list)
ListyNode *findTail(ListyString *listy) {

  if (listy->head == NULL) {
    return NULL;
  }

	int i;
	ListyNode *thisPointer = listy->head;
	ListyNode *nextPointer = listy->head->next;
	
	for (i = 0; i < listy->length; i++) {
		if (nextPointer == NULL) {
			return thisPointer;
		}
		thisPointer = thisPointer->next;
		nextPointer = thisPointer->next;
	}

	return NULL;
}

ListyString *listyCat(ListyString *listy, char *str) {
	
	int i;
	int len;
	ListyNode *tail;
	
	if (listy == NULL && str == NULL) {
		return NULL;
	}

	// Unchanged listy
	if (str == NULL || strlen(str) < 1) {
		return listy;
	}
 
  	len = strlen(str);
	
	// Listy was null so we create a new Listy using the string argument that was passed in,
	// as long as the string is a valid one
	if ((listy == NULL || listy->head == NULL) && len > 0) {
     	if (listy != NULL) {
       		destroyListyString(listy);
    	}
     
    	listy = createListyString(str);
    	return listy;
	}
	
	// Initialize to zero if str is non-null but empty
	if (listy == NULL && str != NULL && len < 1) {
		listy->head = NULL;
		listy->length = 0;
		return listy;
	}

	// Actually concatenate the two strings if we passed all the checks
	for (i = 0; i < len; i++) { // -1 ?
		
		tail = findTail(listy);
		if (tail != NULL) {
			tail->next = malloc(sizeof(struct ListyNode));
			tail->next->data = str[i];
      			tail->next->next = NULL;
      			listy->length++;
		}
   
	}

	return listy;
}

void replaceChar(ListyString *listy, char key, char *str) {

	int i = 0;
	ListyNode *previous;
	ListyNode *check;

	int flag = 0;
	ListyNode *copyNode;
	int index = 0;
	int length = 0;
	char *resultString = malloc(sizeof(char) * 1024);

	if (listy == NULL || listy->head == NULL) {
		return;
	}

	// Begin with the head
	check = listy->head;
	
	// Remove all instances of the key from the list (deletion)
	if (str == NULL || strlen(str) < 1) {
		for (i = 0; i < listy->length + 1; i++) {

			if (check == NULL) {
				break;
			}

			// Found a match, simply delete the key and update the link
			if (check->data == key) {
				if (check->next != NULL) {
					// Multiple letters to replace next to eachother
					while (check != NULL && check->data == key) {
						check = check->next;
						listy->length--;
					}

					if (i == 0) {
						listy->head = check;
					}
					
				} else if (check->next == NULL) {
					listy->length--;
					previous->next = NULL;
				}

				if (previous != NULL && check->next != NULL) {	
					previous->next = check;		
				}
			}

			// Advance the pointers
			previous = check;
			check = check->next;
	
		}

		free(resultString);
		return;
	}

	length = strlen(str); //-1

	// Copy all the entries from the old listy to the new listy (beginning from the head node)
	// ... but don't add the entries that match the key (instead, we'll add str as a replacement)
	copyNode = listy->head;
	while (copyNode != NULL) {
		if (copyNode->data == key) {
   
     if (length == 0) {
       	resultString[index++] = str[0];
     } else {
			// Add the str instead of the key
			for (i = 0; i < length; i++) {
				resultString[index++] = str[i];
			}
     }
			
			flag++;			
	
		} else {
			// Otherwise, add the character
			resultString[index++] = copyNode->data;
		}
		copyNode = copyNode->next;
	}
 
  	//printf("str: [%s]\n", resultString);
 
	destroyListyString(listy);
	listy = createListyString(resultString);

	//if (flag < 1) {
		//listy->length--;
	//}

  	//free(resultString);

}

ListyString *cloneListyString(ListyString *listy) {

	int i;
	ListyString *newString;
	ListyNode *copyNode;
	char *copiedString = malloc(sizeof(char) * 1024);

	if (listy == NULL) {
		return NULL;
	}

	// If the head is null, create a blank list instead.
	if (listy->head == NULL) {
		newString = malloc(sizeof(ListyString));
		newString->head = NULL;
		newString->length = 0;
		return newString;
	}
	
	// Copy all the entries from the old listy to the new listy (beginning from the head node)
	copyNode = listy->head;
	while (copyNode != NULL) {
		copiedString[i] = copyNode->data;
		copyNode = copyNode->next;
		i++;
	}
	
	newString = createListyString(copiedString);
	free(copiedString);

	return newString;
}	

int listyLength(ListyString *listy) {
	return listy == NULL ? -1 : listy->head == NULL ? 0 : listy->length;
}

double hoursSpent(void) {
	return 16.0;
}

double difficultyRating(void) {
	return 4.25;
}

int main(int argc, char **argv) {
	processInputFile(argv[1]);
  	//unit_test(argc, argv);
	return 0;
}