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
#define tile_path tile_closed

// resultPath contains after calculation startpoint, endpoint and the path in
// between -> steps = resultPath->len - 1 (going onto endpoint is also
// considered a step)
void astar(int** grid, int width, int height, int startX, int startY, int endX,
           int endY, LIST* resultPath);
void draw(int** grid, int width, int height);
void drawFinalPath(int** grid, int width, int height, LIST* path,
                   POINT* start, POINT* end);
int manhattanDistance(POINT* p1, POINT* p2);
POINT* lowestFScore(LIST* list);
void destroyListWithData(LIST* list);

#endif