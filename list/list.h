#ifndef LIST_H
#define LIST_H

#include "node.h"

typedef struct _LIST {
    int len;
    struct _NODE* start;
} LIST;

// Call list_deleteList if the list is not needed anymore
LIST* list_createList();
// Does not destroy the nodes
void list_destroyList(LIST* list);
void list_insertFirst(LIST* list, POINT* data);
void list_insertLast(LIST* list, POINT* data);
// @return: 1 = success, 0 = no success
int list_deleteNodeAt(LIST* list, int pos);
NODE* list_getNodeAt(LIST* list, int pos);
int list_getNodePos(LIST* list, POINT* p);
void list_printList(LIST* list);

#endif