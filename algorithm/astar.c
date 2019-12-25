#include "astar.h"

void astar(int** grid, int width, int height, int startX, int startY, int endX,
           int endY, LIST* resultPath) {
    POINT* start;
    start = point_createPoint(startX, startY);
    POINT* end;
    end = point_createPoint(endX, endY);

    /* FIRST DRAW */

    draw(grid, width, height);

    /* INIT */

    LIST *open, *closed;
    open = list_createList();
    closed = list_createList();

    int found = 0;
    POINT* s = NULL;
    POINT* walkableTiles[4];
    for (int i = 0; i < 4; i++)
        walkableTiles[i] = NULL;

    int walkableTilesCount = 0;

    start->scoreF = manhattanDistance(start, end);
    start->scoreG = 0;
    start->scoreH = manhattanDistance(start, end);

    list_insertFirst(open, start);

    /* START */

    while (open->len != 0 && !found) {

        /* GET LOWEST F SCORE POINT AND ADD IT TO CLOSED LIST */

        s = lowestFScore(open);
        list_insertLast(closed, s);
        list_deleteNodeAt(open, list_getNodePos(open, s));

        grid[s->x][s->y] = tile_closed; // CLOSED TILE

        /* GET ADJACENT TILES */

        walkableTilesCount = 0;

        for (int i = -1; i <= 1; i += 2) {
            // Top + Bottom
            if (s->y + i >= 0 && s->y + i < height) {
                switch (grid[s->x][s->y + i]) {
                case tile_empty:
                case tile_closed:
                case tile_open:
                case tile_end:
                    walkableTiles[walkableTilesCount++] =
                        point_createPoint(s->x, s->y + i);
                default:
                    break;
                }
            }

            // Left + Right
            if (s->x + i >= 0 && s->x + i < width) {
                switch (grid[s->x + i][s->y]) {
                case tile_empty:
                case tile_closed:
                case tile_open:
                case tile_end:
                    walkableTiles[walkableTilesCount++] =
                        point_createPoint(s->x + i, s->y);
                default:
                    break;
                }
            }
        }

        /* CHECK ADJACENT TILES */

        for (int i = 0; i < walkableTilesCount; i++) {
            int isClosed = list_getNodePos(closed, walkableTiles[i]) > -1;
            int openPos = list_getNodePos(open, walkableTiles[i]);

            if (isClosed) {
                // Do nothing
                point_destroyPoint(walkableTiles[i]);
            } else if (openPos == -1) {
                walkableTiles[i]->scoreH =
                    manhattanDistance(walkableTiles[i], end);
                walkableTiles[i]->scoreG = s->scoreG + 1;
                walkableTiles[i]->scoreF =
                    walkableTiles[i]->scoreH + walkableTiles[i]->scoreG;
                walkableTiles[i]->parent = s;
                list_insertLast(open, walkableTiles[i]);

                grid[walkableTiles[i]->x][walkableTiles[i]->y] =
                    tile_open; // OPEN TILE
            } else if (openPos > -1) {
                // TODO: test if using the current G score makes the
                // walkableTiles[i].scoreF score lower, if yes update the parent
                // because it means its a better path
                point_destroyPoint(walkableTiles[i]);
            }
        }

        /* CHECK IF END IS REACHED */

        if (manhattanDistance(s, end) == 0) {
            // Fill path with copies of the points and adjust pathLen
            POINT* p = s;
            while (p != NULL) {
                list_insertLast(resultPath, point_copyPoint(p));
                p = p->parent;
            }

            drawFinalPath(grid, width, height, resultPath, start, end);
            found = 1;
        }
    }

    if (!found) {
        printf("No path found!");
    }

    /* FREE MEMORY */

    destroyListWithData(open);
    destroyListWithData(closed);

    printf("\n");
}

void draw(int** grid, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[j][i] == tile_empty)
                printf("  ");
            else
                printf("%d ", grid[j][i]);
        }
        printf("\n");
    }
}

void drawFinalPath(int** grid, int width, int height, LIST* path, POINT* start,
                   POINT* end) {
    POINT pTemp;

    printf("\nSteps: %d\n", path->len - 1);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pTemp.x = j;
            pTemp.y = i;

            if (grid[j][i] == tile_empty)
                printf("  ");
            else if (grid[j][i] == tile_wall)
                printf("%d ", grid[j][i]);
            else if (list_getNodePos(path, &pTemp) > -1) {
                if (start->x == j && start->y == i) {
                    printf("%d ", tile_start);
                    grid[j][i] = tile_start;
                } else if (end->x == j && end->y == i) {
                    printf("%d ", tile_end);
                    grid[j][i] = tile_end;
                } else
                    printf("%d ", grid[j][i]);
            } else {
                printf("  ");
                grid[j][i] = tile_empty;
            }
        }
        printf("\n");
    }
}

int manhattanDistance(POINT* p1, POINT* p2) {
    return (int)(fabs(p1->x - p2->x) + fabs(p1->y - p2->y));
}

POINT* lowestFScore(LIST* list) {

    POINT* p = NULL;

    for (int i = 0; i < list->len; i++) {
        if (i == 0) {
            p = list->start->data;
            continue;
        } else {
            if (list_getNodeAt(list, i)->data->scoreF <= p->scoreF) {
                p = list_getNodeAt(list, i)->data;
            }
        }
    }

    return p;
}

void destroyListWithData(LIST* list) {
    int index = list->len - 1;
    POINT* destroyer = NULL;
    while (index >= 0) {
        destroyer = list_getNodeAt(list, index)->data;
        if (destroyer != NULL)
            point_destroyPoint(destroyer);
        list_deleteNodeAt(list, index);
        index = list->len - 1;
    }
    list_destroyList(list);
}