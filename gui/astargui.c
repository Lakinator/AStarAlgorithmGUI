#include "astargui.h"

void init_gui(ProgramData* pdata) {
    GtkWidget* window;
    GtkWidget *layoutBox, *layoutGrid;

    GtkWidget* btnStart;
    GtkWidget *labelStart, *labelTiles;
    GtkWidget* gridScale;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    layoutBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    layoutGrid = gtk_grid_new();
    pdata->drawingArea = gtk_drawing_area_new();
    pdata->btnColorEmpty = gtk_button_new_with_label("Erase");
    pdata->btnColorWall = gtk_button_new_with_label("Wall");
    pdata->btnColorStart = gtk_button_new_with_label("Start");
    pdata->btnColorEnd = gtk_button_new_with_label("End");

    btnStart = gtk_button_new_with_label("Start");
    labelStart = gtk_label_new("A* Version 1.0");
    labelTiles = gtk_label_new("Choose Tile:");
    gridScale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 20, 80, 1);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    // gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    gtk_grid_set_column_spacing(GTK_GRID(layoutGrid), 15);
    gtk_grid_set_row_spacing(GTK_GRID(layoutGrid), 15);

    gtk_widget_set_size_request(GTK_WIDGET(pdata->drawingArea), 650, 450);

    gtk_range_set_value(GTK_RANGE(gridScale), pdata->adata->cellSize);

    // Basic layout containers
    gtk_container_add(GTK_CONTAINER(window), layoutBox);
    gtk_box_pack_start(GTK_BOX(layoutBox), layoutGrid, FALSE, FALSE, 20);
    gtk_box_pack_start(GTK_BOX(layoutBox), pdata->drawingArea, TRUE, TRUE, 0);
    // Grid components
    gtk_grid_attach(GTK_GRID(layoutGrid), labelStart, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(layoutGrid), btnStart, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(layoutGrid), labelTiles, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(layoutGrid), pdata->btnColorEmpty, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(layoutGrid), pdata->btnColorWall, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(layoutGrid), pdata->btnColorStart, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(layoutGrid), pdata->btnColorEnd, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(layoutGrid), gridScale, 0, 7, 2, 1);

    gtk_widget_set_events(pdata->drawingArea, GDK_POINTER_MOTION_MASK |
                                                  GDK_BUTTON_PRESS_MASK |
                                                  GDK_BUTTON_RELEASE_MASK);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(pdata->drawingArea, "draw", G_CALLBACK(on_draw),
                     pdata->adata);
    g_signal_connect(btnStart, "clicked", G_CALLBACK(on_start), pdata);
    g_signal_connect(pdata->drawingArea, "motion-notify-event",
                     G_CALLBACK(on_mouse_move), pdata);
    g_signal_connect(pdata->drawingArea, "button-press-event",
                     G_CALLBACK(on_mouse_move), pdata);
    g_signal_connect(pdata->drawingArea, "button-release-event",
                     G_CALLBACK(on_mouse_move), pdata);
    g_signal_connect(pdata->btnColorEmpty, "clicked",
                     G_CALLBACK(on_color_switch), pdata);
    g_signal_connect(pdata->btnColorWall, "clicked",
                     G_CALLBACK(on_color_switch), pdata);
    g_signal_connect(pdata->btnColorStart, "clicked",
                     G_CALLBACK(on_color_switch), pdata);
    g_signal_connect(pdata->btnColorEnd, "clicked", G_CALLBACK(on_color_switch),
                     pdata);
    g_signal_connect(gridScale, "value-changed", G_CALLBACK(on_scale_changed),
                     pdata);

    gtk_widget_show_all(window);
}

void on_exit(GtkWidget* widget, gpointer data) {
    if (data == NULL)
        return;

    AData* d = (AData*)data;

    if (d->grid == NULL)
        return;

    for (int i = 0; i < d->columns; i++)
        free(d->grid[i]);
    free(d->grid);
}

gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data) {
    if (data == NULL)
        return FALSE;

    AData* d = (AData*)data;

    draw_grid(cr, data, 5, 5, gtk_widget_get_allocated_width(widget),
              gtk_widget_get_allocated_height(widget));

    return FALSE;
}

void on_start(GtkWidget* widget, gpointer data) {
    if (data == NULL)
        return;

    ProgramData* d = (ProgramData*)data;

    if (d->adata->grid == NULL)
        return;

    // Call astar main method if start/end exist
    if (d->adata->startX >= 0 && d->adata->startY >= 0 && d->adata->endX >= 0 &&
        d->adata->endY >= 0) {
        astar(d->adata->grid, d->adata->columns, d->adata->rows,
              d->adata->startX, d->adata->startY, d->adata->endX,
              d->adata->endY);
        gtk_widget_queue_draw(d->drawingArea); // Force redraw
    } else {
        printf("Error: No start/end cell exists!\n");
    }
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
            gtk_widget_queue_draw(d->drawingArea); // Force redraw
    } else if (event->type == GDK_BUTTON_PRESS) {
        d->mdata->pressed = TRUE;
        if (update_grid(d))
            gtk_widget_queue_draw(d->drawingArea); // Force redraw
    } else if (event->type == GDK_BUTTON_RELEASE) {
        d->mdata->pressed = FALSE;
    }

    return FALSE;
}

void on_color_switch(GtkWidget* widget, gpointer data) {
    if (data == NULL)
        return;

    ProgramData* d = (ProgramData*)data;
    if (widget == d->btnColorEmpty) {
        d->selectedColor = tile_empty;
    } else if (widget == d->btnColorWall) {
        d->selectedColor = tile_wall;
    } else if (widget == d->btnColorStart) {
        d->selectedColor = tile_start;
    } else if (widget == d->btnColorEnd) {
        d->selectedColor = tile_end;
    }
}

void on_scale_changed(GtkRange* range, gpointer data) {
    if (data == NULL)
        return;

    ProgramData* d = (ProgramData*)data;
    d->adata->cellSize = (int)gtk_range_get_value(range);
    gtk_widget_queue_draw(d->drawingArea); // Force redraw
}

void draw_grid(cairo_t* cr, AData* data, int x, int y, int width, int height) {
    int offset = data->cellSize / 20;

    int columns_new = (width - x) / data->cellSize;
    int rows_new = (height - y) / data->cellSize;

    realloc_grid(data, columns_new, rows_new);

    // Background
    cairo_set_source_rgb(cr, 0, 0, 200);
    for (int i = 0; i < data->columns; i++) {
        for (int j = 0; j < data->rows; j++) {
            cairo_rectangle(cr, i * data->cellSize + x, j * data->cellSize + y,
                            data->cellSize, data->cellSize);
            cairo_fill(cr);
        }
    }

    // Actual Squares
    for (int i = 0; i < data->columns; i++) {
        for (int j = 0; j < data->rows; j++) {
            switch (data->grid[i][j]) {
            case tile_wall:
                cairo_set_source_rgb(cr, 0, 0, 0);
                break;
            case tile_start:
                cairo_set_source_rgb(cr, 255, 0, 0);
                break;
            case tile_end:
                cairo_set_source_rgb(cr, 0, 255, 0);
                break;
            case tile_closed:
                cairo_set_source_rgb(cr, 255, 255, 0);
                break;
            default:
                cairo_set_source_rgb(cr, 255, 255, 255);
                break;
            }
            cairo_rectangle(cr, i * data->cellSize + offset + x,
                            j * data->cellSize + offset + y,
                            data->cellSize - (offset * 2),
                            data->cellSize - (offset * 2));
            cairo_fill(cr);
        }
    }
}

// @return: true if drawing area should be redrawn
gboolean update_grid(ProgramData* pdata) {
    if (pdata->mdata->pressed) {
        int p_x = pdata->mdata->x / pdata->adata->cellSize;
        int p_y = pdata->mdata->y / pdata->adata->cellSize;

        // Set grid if button is pressed
        if (p_x < pdata->adata->columns && p_y < pdata->adata->rows &&
            pdata->adata->grid[p_x][p_y] != pdata->selectedColor) {
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
            } else if (pdata->selectedColor == tile_end) {
                if (pdata->adata->endX >= 0 && pdata->adata->endY >= 0) {
                    pdata->adata->grid[pdata->adata->endX][pdata->adata->endY] =
                        tile_empty;
                }
                pdata->adata->endX = p_x;
                pdata->adata->endY = p_y;
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
        }

        return TRUE;
    }

    return FALSE;
}

gboolean realloc_grid(AData* adata, int columns_new, int rows_new) {
    // Realloc grid only if size changed
    if (adata->columns != columns_new || adata->rows != rows_new) {

        // Columns
        int** temp = adata->grid;
        adata->grid = realloc(adata->grid, columns_new * sizeof(int*));

        if (adata->grid == NULL) {
            printf("Realloc column failed\n");
            adata->grid = temp;
            return FALSE;
        }

        // Rows
        for (int i = 0; i < columns_new; i++) {

            // Check for completely new added columns, which need to be set
            // to NULL for successful realloc
            if (i >= adata->columns)
                adata->grid[i] = NULL;

            int* t = adata->grid[i];
            adata->grid[i] = realloc(adata->grid[i], rows_new * sizeof(int));

            if (adata->grid[i] == NULL) {
                printf("Realloc row failed\n");
                adata->grid[i] = t;
                return FALSE;
            }

            // Init new int's to 0
            for (int j = adata->rows; j < rows_new; j++) {
                adata->grid[i][j] = tile_empty;
            }

            // Init new int rows to 0
            if (i >= adata->columns) {
                for (int j = 0; j < rows_new; j++) {
                    adata->grid[i][j] = tile_empty;
                }
            }
        }

        adata->columns = columns_new;
        adata->rows = rows_new;
    }
}
