#include "node.h"

NODE* node_createNode(POINT* data) {
    NODE* newNode = malloc(sizeof(NODE));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void node_destroyNode(NODE* node) {
    if (node == NULL)
        return;

    free(node);
    node == NULL;
}

int node_compare(NODE* n1, NODE* n2) {
    return point_compare(n1->data, n2->data);
}

void node_insertLast(NODE* node, POINT* data) {
    if (node == NULL)
        return;

    if (node->next == NULL) {
        NODE* newNode = node_createNode(data);
        node->next = newNode;
    } else {
        node_insertLast(node->next, data);
    }
}

int node_deleteNodeAt(NODE* node, int pos) {
    if (node == NULL)
        return 0;

    if (pos == 1) {
        NODE* n = node->next;
        node->next = n->next;
        node_destroyNode(n);
        return 1;
    } else {
        return node_deleteNodeAt(node->next, --pos);
    }
}

NODE* node_getNodeAt(NODE* node, int pos) {
    if (node == NULL)
        return NULL;

    if (pos == 0)
        return node;
    else
        return node_getNodeAt(node->next, --pos);
}

int node_getNodePosWithPoint(NODE* node, POINT* p, int pos) {
    if (node == NULL)
        return -1;

    if (point_compare(node->data, p)) {
        return pos;
    } else {
        if (node->next != NULL)
            return node_getNodePosWithPoint(node->next, p, ++pos);
        else {
            return -1;
        }
    }
}

void node_printNode(NODE* node) {
    if (node == NULL)
        return;

    point_print(node->data);

    if (node->next != NULL) {
        node_printNode(node->next);
    } else {
        printf("End of list reached!\n");
    }
}
