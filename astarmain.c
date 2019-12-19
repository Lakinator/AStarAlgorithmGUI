#include "gui/astargui.h"

int main(int argc, char** argv) {
    gtk_init(&argc, &argv);

    AData adata = {
        .grid = NULL,
        .cellSize = 35,
        .columns = 0,
        .rows = 0,
        .startX = -1,
        .startY = -1,
        .endX = -1,
        .endY = -1,
    };

    MouseData mdata = {
        .pressed = FALSE,
        .x = 0,
        .y = 0,
    };

    ProgramData pdata = {.adata = &adata, .mdata = &mdata, .selectedColor = tile_wall};

    init_gui(&pdata);

    gtk_main();
    return 0;
}