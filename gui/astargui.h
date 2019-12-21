#ifndef _ASTAR_GUI_H_
#define _ASTAR_GUI_H_

#include "../algorithm/astar.h"
#include <gtk/gtk.h>

typedef struct _AData {
    int** grid;
    int cellSize;
    int columns;
    int rows;
    int startX, startY;
    int endX, endY;
} AData;

typedef struct _MouseData {
    gboolean pressed;
    guint x;
    guint y;
} MouseData;

typedef struct _GtkData {
    GtkWidget* drawingArea;
    GtkWidget* btnColorEmpty;
    GtkWidget* btnColorWall;
    GtkWidget* btnColorStart;
    GtkWidget* btnColorEnd;
} GtkData;

typedef struct _ProgramData {
    AData* adata;
    MouseData* mdata;
    GtkData* gdata;
    int selectedColor;
} ProgramData;

void init_gui(ProgramData* pdata);

void on_exit(GtkWidget* widget, gpointer data);
gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data);
void on_start(GtkWidget* widget, gpointer data);
gboolean on_mouse_move(GtkWidget* widget, GdkEvent* event, gpointer data);
void on_color_switch(GtkWidget* widget, gpointer data);
void on_scale_changed(GtkRange* range, gpointer data);
void draw_grid(cairo_t* cr, AData* data, int x, int y, int width, int height);
// @return: true -> redraw drawing area
gboolean update_grid(ProgramData* pdata);
// @return: successful
gboolean realloc_grid(AData* adata, int new_columns, int new_rows);

#endif