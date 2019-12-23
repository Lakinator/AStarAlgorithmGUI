#include "astar.h"

void astar(int** grid, int width, int height, int startX, int startY, int endX,
           int endY, LIST* resultPath) {
    POINT start;
    POINT end;

    start.x = startX;
    start.y = startY;
    end.x = endX;
    end.y = endY;

    /* FIRST DRAW */

    draw(grid, width, height);

    /* INIT */

    LIST open = {.len = 0}, closed = {.len = 0};

    int found = 0;
    POINT* s;
    POINT walkableTiles[4];
    int walkableTilesCount = 0;

    l_insertFirst(&open, (POINT){.x = start.x,
                                 .y = start.y,
                                 .scoreF = manhattanDistance(&start, &end),
                                 .scoreG = 0,
                                 .scoreH = manhattanDistance(&start, &end),
                                 .parent = NULL});

    /* START */

    while (open.len != 0 && !found) {

        /* GET LOWEST F SCORE POINT AND ADD IT TO CLOSED LIST */

        s = lowestFScore(&open);
        l_insertLast(&closed, (POINT){.x = s->x,
                                      .y = s->y,
                                      .scoreF = s->scoreF,
                                      .scoreG = s->scoreG,
                                      .scoreH = s->scoreH,
                                      .parent = s->parent});
        l_deleteNodeAt(&open, l_getNodePos(&open, createNode(*s)));
        s = &l_getNodeAt(&closed, closed.len - 1)->data;

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
                        (POINT){.x = s->x, .y = s->y + i};
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
                        (POINT){.x = s->x + i, .y = s->y};
                default:
                    break;
                }
            }
        }

        /* CHECK ADJACENT TILES */

        for (int i = 0; i < walkableTilesCount; i++) {
            int isClosed =
                l_getNodePos(&closed, createNode(walkableTiles[i])) > -1;
            int openPos = l_getNodePos(&open, createNode(walkableTiles[i]));

            if (isClosed) {
                // Do nothing
            } else if (openPos == -1) {
                walkableTiles[i].scoreH =
                    manhattanDistance(&walkableTiles[i], &end);
                walkableTiles[i].scoreG = s->scoreG + 1;
                walkableTiles[i].scoreF =
                    walkableTiles[i].scoreH + walkableTiles[i].scoreG;
                walkableTiles[i].parent = s;
                l_insertLast(&open, walkableTiles[i]);

                grid[walkableTiles[i].x][walkableTiles[i].y] =
                    tile_open; // OPEN TILE
            } else if (openPos > -1) {
                // TODO: test if using the current G score makes the
                // walkableTiles[i].scoreF score lower, if yes update the parent
                // because it means its a better path
            }
        }

        /* CHECK IF END IS REACHED */

        if (manhattanDistance(s, &end) == 0) {
            // Fill path with copies of the points and adjust pathLen
            POINT* p = s;
            while (p != NULL) {
                l_insertLast(resultPath, *p);
                p = p->parent;
            }

            drawFinalPath(grid, width, height, s, &start, &end);
            found = 1;
        }
    }

    if (!found) {
        printf("No path found!");
    }

    /* FREE MEMORY */

    while (open.len > 0) {
        l_deleteNodeAt(&open, open.len - 1);
    }

    while (closed.len > 0) {
        l_deleteNodeAt(&closed, closed.len - 1);
    }

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

void drawFinalPath(int** grid, int width, int height, POINT* lastPoint,
                   POINT* start, POINT* end) {
    LIST path = (LIST){.len = 0};

    POINT* p = lastPoint;

    while (p != NULL) {
        l_insertLast(&path, *p);
        p = p->parent;
    }

    printf("\nSteps: %d\n", path.len - 1);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[j][i] == tile_empty)
                printf("  ");
            else if (grid[j][i] == tile_wall)
                printf("%d ", grid[j][i]);
            else if (l_getNodePos(&path, createNode((POINT){.x = j, .y = i})) >
                     -1) {
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
            p = &list->start->data;
            continue;
        } else {
            if (l_getNodeAt(list, i)->data.scoreF <= p->scoreF) {
                p = &l_getNodeAt(list, i)->data;
            }
        }
    }

    return p;
}