#ifndef A_STAR_
#define A_STAR_

#include "../list/list.h"
#include <math.h>
#include <string.h>

#define tile_empty 0
#define tile_wall 1
#define tile_start 2
#define tile_end 3
#define tile_open 7
#define tile_closed 9

void astar(int** grid, int width, int height, int startX, int startY, int endX, int endY);
void draw(int** grid, int width, int height);
void drawFinalPath(int** grid, int width, int height, POINT* lastPoint,
                   POINT* start, POINT* end);
int manhattanDistance(POINT* p1, POINT* p2);
POINT* lowestFScore(LIST* list);

#endif