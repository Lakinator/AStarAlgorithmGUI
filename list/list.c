#include "list.h"

LIST* list_createList() {
    LIST* l = malloc(sizeof(LIST));
    l->len = 0;
    l->start = NULL;
}

void list_destroyList(LIST* list) {
    if (list == NULL)
        return;

    free(list);
    list = NULL;
}

void list_insertFirst(LIST* list, POINT* data) {
    if (list == NULL)
        return;

    if (list->len == 0) {
        list->start = node_createNode(data);
    } else {
        NODE* newNode = node_createNode(data);
        newNode->next = list->start;
        list->start = newNode;
    }

    list->len++;
}

void list_insertLast(LIST* list, POINT* data) {
    if (list == NULL)
        return;

    if (list->len == 0)
        list_insertFirst(list, data);
    else {
        node_insertLast(list->start, data);
        list->len++;
    }
}

int list_deleteNodeAt(LIST* list, int pos) {
    if (list == NULL)
        return 0;

    if (list->len == 0 || pos < 0)
        return 0;
    else if (list->len > 0 && pos == 0) {
        NODE* n = list->start;
        list->start = n->next;
        node_destroyNode(n);
        list->len--;
        return 1;
    } else {
        if (node_deleteNodeAt(list->start, pos)) {
            list->len--;
            return 1;
        } else {
            return 0;
        }
    }
}

NODE* list_getNodeAt(LIST* list, int pos) {
    if (list == NULL)
        return NULL;

    if (list->len == 0 || pos < 0)
        return NULL;
    else {
        return node_getNodeAt(list->start, pos);
    }
}

int list_getNodePos(LIST* list, POINT* p) {
    if (list == NULL)
        return -1;

    if (list->len == 0) {
        return -1;
    } else {
        if (point_compare(list->start->data, p)) {
            return 0;
        } else if (list->len > 1) {
            return node_getNodePosWithPoint(list->start->next, p, 1);
        } else {
            return -1;
        }
    }
}

void list_printList(LIST* list) {
    if (list == NULL)
        return;

    if (list->len == 0)
        printf("Empty list!\n");
    else {
        node_printNode(list->start);
    }
}