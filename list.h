/*Source: list.h */
typedef struct {
        int currentSize;
        int maxSize;
        int array[];
} list;
list *initList(list *l, int maxSize);
void addToList(list *l, int x);
void printList(list *l);
void printCallList(list *l);
int listContains(list *l, int x);
