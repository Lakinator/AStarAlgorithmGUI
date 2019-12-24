#ifndef NODE_H
#define NODE_H

#include "../point/point.h"

typedef struct _NODE {
    POINT* data;
    struct _NODE* next;
} NODE;

NODE* node_createNode(
    POINT* data); // Call node_deleteNode if the node is not needed anymore
void node_destroyNode(NODE* node); // This does not destroy the data
int node_compare(NODE* n1, NODE* n2);
void node_insertLast(NODE* node, POINT* data);
int node_deleteNodeAt(NODE* node, int pos); // 1 if success, 0 if not
NODE* node_getNodeAt(NODE* node, int pos);
int node_getNodePosWithPoint(NODE* node, POINT* p, int pos);
void node_printNode(NODE* node);

#endif