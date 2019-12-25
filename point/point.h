#ifndef POINT_H
#define POINT_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _POINT {
    int x;
    int y;
    int scoreF;
    int scoreG;
    int scoreH;
    struct _POINT* parent;
} POINT;

// Call point_deletePoint if the point is not needed anymore
POINT* point_createPoint(int x, int y);
// Does not destroy parent
void point_destroyPoint(POINT* point);
// @return: 1 = success, 0 = no success
int point_compare(POINT* p1, POINT* p2);
POINT* point_copyPoint(POINT* p);
void point_print(POINT* p);

#endif