#include "point.h"

POINT* point_createPoint(int x, int y) {
    POINT* p = malloc(sizeof(POINT));
    p->x = x;
    p->y = y;
    p->parent = NULL;
    return p;
}
void point_destroyPoint(POINT* point) {
    if (point == NULL)
        return;

    free(point);
    point = NULL;
}

int point_compare(POINT* p1, POINT* p2) {
    return (p1->x == p2->x && p1->y == p2->y);
}

POINT* point_copyPoint(POINT* p) {
    POINT* copy = point_createPoint(p->x, p->y);
    copy->scoreF = p->scoreF;
    copy->scoreG = p->scoreG;
    copy->scoreH = p->scoreH;
    copy->parent = NULL;
}

void point_print(POINT* p) {
    printf("POINT{x = %d, y = %d, scoreF = %d, scoreG = %d, scoreH = %d}\n",
           p->x, p->y, p->scoreF, p->scoreG, p->scoreH);
}