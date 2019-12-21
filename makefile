all:
	gcc -mwindows `pkg-config --cflags gtk+-3.0` astarmain.c algorithm/astar.c gui/astargui.c point/point.c list/list.c list/node.c -o astar `pkg-config --libs gtk+-3.0`
	./astar.exe