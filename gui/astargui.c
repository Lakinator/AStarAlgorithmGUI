#include "astargui.h"

void init_gui(ProgramData* pdata) {
    GtkWidget* window;
    GtkWidget *layoutBox, *layoutGrid;

    GtkWidget *labelStart, *labelTiles, *labelSize, *labelPath;
    GtkWidget* gridScale;

    pdata->gdata = malloc(sizeof(GtkData));
    pdata->adata->resultPath = list_createList();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    layoutBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    layoutGrid = gtk_grid_new();
    pdata->gdata->drawingArea = gtk_drawing_area_new();
    pdata->gdata->btnColorEmpty = gtk_button_new_with_label("Erase");
    pdata->gdata->btnColorWall = gtk_button_new_with_label("Wall");
    pdata->gdata->btnColorStart = gtk_button_new_with_label("Start");
    pdata->gdata->btnColorEnd = gtk_button_new_with_label("End");

    pdata->gdata->btnStart = gtk_button_new_with_label("Run");
    labelStart = gtk_label_new("A* Version 1.0");
    labelTiles = gtk_label_new("Choose Tile:");
    labelSize = gtk_label_new("Cell size:");
    labelPath = gtk_label_new("Auto draw path:");
    gridScale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 20, 80, 1);
    pdata->gdata->pathSwitch = gtk_switch_new();

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    gtk_widget_set_sensitive(pdata->gdata->btnStart, FALSE);

    gtk_grid_set_column_spacing(GTK_GRID(layoutGrid), 15);
    gtk_grid_set_row_spacing(GTK_GRID(layoutGrid), 15);

    gtk_widget_set_size_request(GTK_WIDGET(pdata->gdata->drawingArea), 650,
                                550);

    gtk_range_set_value(GTK_RANGE(gridScale), pdata->adata->cellSize);

    gtk_switch_set_active(GTK_SWITCH(pdata->gdata->pathSwitch), FALSE);
    gtk_widget_set_hexpand(pdata->gdata->pathSwitch, FALSE);
    gtk_widget_set_halign(pdata->gdata->pathSwitch, GTK_ALIGN_CENTER);

    // Basic layout containers
    gtk_container_add(GTK_CONTAINER(window), layoutBox);
    gtk_box_pack_start(GTK_BOX(layoutBox), layoutGrid, FALSE, FALSE, 20);
    gtk_box_pack_start(GTK_BOX(layoutBox), pdata->gdata->drawingArea, TRUE,
                       TRUE, 0);
    // Grid components
    gtk_grid_attach(GTK_GRID(layoutGrid), labelStart, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(layoutGrid), pdata->gdata->btnStart, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(layoutGrid), labelTiles, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(layoutGrid), pdata->gdata->btnColorEmpty, 0, 3, 1,
                    1);
    gtk_grid_attach(GTK_GRID(layoutGrid), pdata->gdata->btnColorWall, 0, 4, 1,
                    1);
    gtk_grid_attach(GTK_GRID(layoutGrid), pdata->gdata->btnColorStart, 0, 5, 1,
                    1);
    gtk_grid_attach(GTK_GRID(layoutGrid), pdata->gdata->btnColorEnd, 0, 6, 1,
                    1);
    gtk_grid_attach(GTK_GRID(layoutGrid), labelSize, 0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(layoutGrid), gridScale, 0, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(layoutGrid), labelPath, 0, 9, 1, 1);
    gtk_grid_attach(GTK_GRID(layoutGrid), pdata->gdata->pathSwitch, 0, 10, 1,
                    2);

    gtk_widget_set_events(pdata->gdata->drawingArea,
                          GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK |
                              GDK_BUTTON_RELEASE_MASK);

    g_signal_connect(window, "destroy", G_CALLBACK(on_exit), pdata);
    g_signal_connect(pdata->gdata->drawingArea, "draw", G_CALLBACK(on_draw),
                     pdata);
    g_signal_connect(pdata->gdata->btnStart, "clicked", G_CALLBACK(on_start),
                     pdata);
    g_signal_connect(pdata->gdata->drawingArea, "motion-notify-event",
                     G_CALLBACK(on_mouse_move), pdata);
    g_signal_connect(pdata->gdata->drawingArea, "button-press-event",
                     G_CALLBACK(on_mouse_move), pdata);
    g_signal_connect(pdata->gdata->drawingArea, "button-release-event",
                     G_CALLBACK(on_mouse_move), pdata);
    g_signal_connect(pdata->gdata->btnColorEmpty, "clicked",
                     G_CALLBACK(on_color_switch), pdata);
    g_signal_connect(pdata->gdata->btnColorWall, "clicked",
                     G_CALLBACK(on_color_switch), pdata);
    g_signal_connect(pdata->gdata->btnColorStart, "clicked",
                     G_CALLBACK(on_color_switch), pdata);
    g_signal_connect(pdata->gdata->btnColorEnd, "clicked",
                     G_CALLBACK(on_color_switch), pdata);
    g_signal_connect(gridScale, "value-changed", G_CALLBACK(on_scale_changed),
                     pdata);
    g_signal_connect(pdata->gdata->pathSwitch, "state-set",
                     G_CALLBACK(on_switch_toggled), pdata);

    gtk_widget_show_all(window);
}

void on_exit(GtkWidget* widget, gpointer data) {
    if (data == NULL)
        return;

    ProgramData* d = (ProgramData*)data;

    remove_path_from_grid(d->adata);

    list_destroyList(d->adata->resultPath);

    if (d->adata->grid != NULL) {
        for (int i = 0; i < d->adata->columns; i++)
            free(d->adata->grid[i]);
        free(d->adata->grid);
    }

    free(d->gdata);

    gtk_main_quit();
}

gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data) {
    if (data == NULL)
        return FALSE;

    ProgramData* d = (ProgramData*)data;

    draw_grid(cr, d, 5, 5, gtk_widget_get_allocated_width(widget),
              gtk_widget_get_allocated_height(widget));

    return FALSE;
}

void on_start(GtkWidget* widget, gpointer data) {
    if (data == NULL)
        return;

    ProgramData* d = (ProgramData*)data;

    if (d->adata->grid == NULL)
        return;

    run_algorithm(d);
}

gboolean on_mouse_move(GtkWidget* widget, GdkEvent* event, gpointer data) {
    if (data == NULL)
        return FALSE;

    ProgramData* d = (ProgramData*)data;

    if (event->type == GDK_MOTION_NOTIFY) {
        GdkEventMotion* e = (GdkEventMotion*)event;
        d->mdata->x = (guint)e->x;
        d->mdata->y = (guint)e->y;

        if (update_grid(d))
            gtk_widget_queue_draw(d->gdata->drawingArea); // Force redraw
    } else if (event->type == GDK_BUTTON_PRESS) {
        d->mdata->pressed = TRUE;
        if (update_grid(d))
            gtk_widget_queue_draw(d->gdata->drawingArea); // Force redraw
    } else if (event->type == GDK_BUTTON_RELEASE) {
        d->mdata->pressed = FALSE;
    }

    return FALSE;
}

void on_color_switch(GtkWidget* widget, gpointer data) {
    if (data == NULL)
        return;

    ProgramData* d = (ProgramData*)data;
    if (widget == d->gdata->btnColorEmpty) {
        d->selectedColor = tile_empty;
    } else if (widget == d->gdata->btnColorWall) {
        d->selectedColor = tile_wall;
    } else if (widget == d->gdata->btnColorStart) {
        d->selectedColor = tile_start;
    } else if (widget == d->gdata->btnColorEnd) {
        d->selectedColor = tile_end;
    }
}

void on_scale_changed(GtkRange* range, gpointer data) {
    if (data == NULL)
        return;

    ProgramData* d = (ProgramData*)data;
    d->adata->cellSize = (int)gtk_range_get_value(range);
    gtk_widget_queue_draw(d->gdata->drawingArea); // Force redraw
}

gboolean on_switch_toggled(GtkSwitch* widget, gboolean state, gpointer data) {
    if (data == NULL)
        return FALSE;

    ProgramData* d = (ProgramData*)data;

    // Disable/Enable run button if auto run is enabled/disabled
    gtk_widget_set_sensitive(d->gdata->btnStart, !state);

    if (state) {
        run_algorithm(d);
    }

    return FALSE;
}

void draw_grid(cairo_t* cr, ProgramData* pdata, int x, int y, int width,
               int height) {
    int offset = pdata->adata->cellSize / 20;

    int columns_new = (width - x) / pdata->adata->cellSize;
    int rows_new = (height - y) / pdata->adata->cellSize;

    realloc_grid(pdata, columns_new, rows_new);

    // Background
    cairo_set_source_rgb(cr, 0, 0, 200);
    for (int i = 0; i < pdata->adata->columns; i++) {
        for (int j = 0; j < pdata->adata->rows; j++) {
            cairo_rectangle(cr, i * pdata->adata->cellSize + x,
                            j * pdata->adata->cellSize + y,
                            pdata->adata->cellSize, pdata->adata->cellSize);
            cairo_fill(cr);
        }
    }

    // Actual Squares
    for (int i = 0; i < pdata->adata->columns; i++) {
        for (int j = 0; j < pdata->adata->rows; j++) {
            switch (pdata->adata->grid[i][j]) {
            case tile_wall:
                cairo_set_source_rgb(cr, 0, 0, 0);
                break;
            case tile_start:
                cairo_set_source_rgb(cr, 255, 0, 0);
                break;
            case tile_end:
                cairo_set_source_rgb(cr, 0, 255, 0);
                break;
            case tile_path:
                cairo_set_source_rgb(cr, 255, 255, 0);
                break;
            default:
                cairo_set_source_rgb(cr, 255, 255, 255);
                break;
            }
            cairo_rectangle(cr, i * pdata->adata->cellSize + offset + x,
                            j * pdata->adata->cellSize + offset + y,
                            pdata->adata->cellSize - (offset * 2),
                            pdata->adata->cellSize - (offset * 2));
            cairo_fill(cr);
        }
    }
}

// @return: true if drawing area should be redrawn
gboolean update_grid(ProgramData* pdata) {
    if (pdata->mdata->pressed) {
        int p_x = pdata->mdata->x / pdata->adata->cellSize;
        int p_y = pdata->mdata->y / pdata->adata->cellSize;

        // Only update the cell if the color actually changed
        if (p_x < pdata->adata->columns && p_y < pdata->adata->rows &&
            pdata->adata->grid[p_x][p_y] != pdata->selectedColor) {

            // Change cell color
            pdata->adata->grid[p_x][p_y] = pdata->selectedColor;

            // Only one start/end at a time -> remove last start/end by
            // setting it to empty
            if (pdata->selectedColor == tile_start) {
                if (pdata->adata->startX >= 0 && pdata->adata->startY >= 0) {
                    pdata->adata
                        ->grid[pdata->adata->startX][pdata->adata->startY] =
                        tile_empty;
                }
                pdata->adata->startX = p_x;
                pdata->adata->startY = p_y;

                // Remove end if it was overdrawn by start
                if (p_x == pdata->adata->endX && p_y == pdata->adata->endY) {
                    pdata->adata->endX = -1;
                    pdata->adata->endY = -1;
                }
            } else if (pdata->selectedColor == tile_end) {
                if (pdata->adata->endX >= 0 && pdata->adata->endY >= 0) {
                    pdata->adata->grid[pdata->adata->endX][pdata->adata->endY] =
                        tile_empty;
                }
                pdata->adata->endX = p_x;
                pdata->adata->endY = p_y;

                // Remove start if it was overdrawn by end
                if (p_x == pdata->adata->startX &&
                    p_y == pdata->adata->startY) {
                    pdata->adata->startX = -1;
                    pdata->adata->startY = -1;
                }
            } else {
                // Clear previous start/end coordinates if they were overdrawn
                if (p_x == pdata->adata->startX &&
                    p_y == pdata->adata->startY) {
                    pdata->adata->startX = -1;
                    pdata->adata->startY = -1;
                } else if (p_x == pdata->adata->endX &&
                           p_y == pdata->adata->endY) {
                    pdata->adata->endX = -1;
                    pdata->adata->endY = -1;
                }
            }

            // Remove old path
            if (pdata->selectedColor == tile_empty ||
                pdata->selectedColor == tile_wall ||
                pdata->selectedColor == tile_start ||
                pdata->selectedColor == tile_end) {

                remove_path_from_grid(pdata->adata);
            }

            // Option to enable/disable instant calculation of the path
            if (gtk_switch_get_active(GTK_SWITCH(pdata->gdata->pathSwitch)))
                run_algorithm(pdata);
            else {
                // Toggle run button
                gboolean active =
                    (pdata->adata->startX >= 0 && pdata->adata->startY >= 0 &&
                     pdata->adata->endX >= 0 && pdata->adata->endY >= 0);

                gtk_widget_set_sensitive(pdata->gdata->btnStart, active);
            }
        }

        return TRUE;
    }

    return FALSE;
}

gboolean realloc_grid(ProgramData* pdata, int columns_new, int rows_new) {
    // Realloc grid only if size changed
    if (pdata->adata->columns != columns_new ||
        pdata->adata->rows != rows_new) {

        // Columns
        int** temp = pdata->adata->grid;
        pdata->adata->grid =
            realloc(pdata->adata->grid, columns_new * sizeof(int*));

        if (pdata->adata->grid == NULL) {
            printf("Realloc column failed\n");
            pdata->adata->grid = temp;
            return FALSE;
        }

        // Rows
        for (int i = 0; i < columns_new; i++) {

            // Check for completely new added columns, which need to be set
            // to NULL for successful realloc
            if (i >= pdata->adata->columns)
                pdata->adata->grid[i] = NULL;

            int* t = pdata->adata->grid[i];
            pdata->adata->grid[i] =
                realloc(pdata->adata->grid[i], rows_new * sizeof(int));

            if (pdata->adata->grid[i] == NULL) {
                printf("Realloc row failed\n");
                pdata->adata->grid[i] = t;
                return FALSE;
            }

            // Init new int's to 0
            for (int j = pdata->adata->rows; j < rows_new; j++) {
                pdata->adata->grid[i][j] = tile_empty;
            }

            // Init new int rows to 0
            if (i >= pdata->adata->columns) {
                for (int j = 0; j < rows_new; j++) {
                    pdata->adata->grid[i][j] = tile_empty;
                }
            }
        }

        pdata->adata->columns = columns_new;
        pdata->adata->rows = rows_new;

        // Check if start/end were removed
        if (pdata->adata->startX >= pdata->adata->columns ||
            pdata->adata->startY >= pdata->adata->rows) {
            pdata->adata->startX = -1;
            pdata->adata->startY = -1;
            gtk_widget_set_sensitive(pdata->gdata->btnStart, FALSE);
            remove_path_from_grid(pdata->adata);
        } else if (pdata->adata->endX >= pdata->adata->columns ||
                   pdata->adata->endY >= pdata->adata->rows) {
            pdata->adata->endX = -1;
            pdata->adata->endY = -1;
            gtk_widget_set_sensitive(pdata->gdata->btnStart, FALSE);
            remove_path_from_grid(pdata->adata);
        }
    }
}

void remove_path_from_grid(AStarData* adata) {
    // Remove old path
    if (adata->resultPath->len > 0) {
        // Removing each cell from the path by deleting the
        // first cell etc. (start at index 0 in the list)
        NODE* n = NULL;
        while (adata->resultPath->len > 0) {
            n = list_getNodeAt(adata->resultPath, 0);
            // Making sure only path tiles
            // get erased from the grid
            if (n->data->x < adata->columns && n->data->y < adata->rows) {
                if (adata->grid[n->data->x][n->data->y] == tile_path)
                    adata->grid[n->data->x][n->data->y] = tile_empty;
            }
            point_destroyPoint(n->data);
            list_deleteNodeAt(adata->resultPath, 0);
        }
    }
}

void run_algorithm(ProgramData* pdata) {
    // Call astar main method if start and end exist
    if (pdata->adata->startX >= 0 && pdata->adata->startY >= 0 &&
        pdata->adata->endX >= 0 && pdata->adata->endY >= 0) {
        remove_path_from_grid(pdata->adata);
        astar(pdata->adata->grid, pdata->adata->columns, pdata->adata->rows,
              pdata->adata->startX, pdata->adata->startY, pdata->adata->endX,
              pdata->adata->endY, pdata->adata->resultPath);
        gtk_widget_queue_draw(pdata->gdata->drawingArea); // Force redraw
    } else {
        printf("Error: Start- and Endcell need to be specified!\n");
    }
}
