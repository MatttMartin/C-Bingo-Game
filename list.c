/*Source: list.c */
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

list *initList(list *l, int maxSize) {
	l = malloc(sizeof(*l) + maxSize * sizeof(int));
	l->maxSize = maxSize;
	l->currentSize = 0;
	return l;
}
void addToList(list *l, int x) {
	printf("%d\n", l->currentSize);
	l->array[l->currentSize] = x;
	l->currentSize++;
	return;
}
void printList(list *l) {
	int i;
	for (i = 0; i < l->currentSize; i++) {
		printf("%d ", l->array[i]);
	}
	printf("\n");
}
void printCallList(list *l) {
	int i;
	char c;
	int val;
        for (i = 0; i < l->currentSize; i++) {
		val = l->array[i];

		if (val <= 15) c = 'L';
		else if (val <= 30) c = 'I';
		else if (val <= 45) c = 'N';
		else if (val <= 60) c = 'U';
		else if (val <= 75) c = 'X';
                printf("%c%d ", c, val);
        }
        printf("\n");
}
int listContains(list *l, int x) {
	int i;
	for (i=0; i < l->currentSize; i++) {
		if (l->array[i] == x) {
			return 1; //true
		}
	}
	return 0;
}
