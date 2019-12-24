#ifndef LIST_H
#define LIST_H

#include "node.h"

typedef struct _LIST {
    int len;
    struct _NODE* start;
} LIST;

LIST* list_createList(); // Call list_deleteList if the list is not needed
                         // anymore
void list_destroyList(LIST* list); // This does not destroy the nodes
void list_insertFirst(LIST* list, POINT* data);
void list_insertLast(LIST* list, POINT* data);
int list_deleteNodeAt(LIST* list, int pos); // 1 if success, 0 if not
NODE* list_getNodeAt(LIST* list, int pos);
int list_getNodePos(LIST* list, POINT* p);
void list_printList(LIST* list);

#endif