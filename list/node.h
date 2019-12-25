#ifndef NODE_H
#define NODE_H

#include "../point/point.h"

typedef struct _NODE {
    POINT* data;
    struct _NODE* next;
} NODE;

// Call node_deleteNode if the node is not needed anymore
NODE* node_createNode(POINT* data);
// Does not destroy data
void node_destroyNode(NODE* node);
// @return: 1 = success, 0 = no success
int node_compare(NODE* n1, NODE* n2);
void node_insertLast(NODE* node, POINT* data);
// @return: 1 = success, 0 = no success
int node_deleteNodeAt(NODE* node, int pos);
NODE* node_getNodeAt(NODE* node, int pos);
int node_getNodePosWithPoint(NODE* node, POINT* p, int pos);
void node_printNode(NODE* node);

#endif