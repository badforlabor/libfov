#include <windows.h>
/*
* Copyright (C) 2006, Greg McIntyre
* All rights reserved. See the file named COPYING in the distribution
* for more details.
*/

#include "map.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <sys/time.h>
//#include <fov/fov.h>
#include "fov.h"
#include "fov_wrapper.h"

#define FOVRADIUS	100

/* Global variables ----------------------------------------------- */
CFovWrapper* GFovInst;
// MAP map(3);
// int pX = 25;
// int pY = 20;
// unsigned radius = 20;
// fov_direction_type direction = FOV_EAST;
// float angle = 130.0f;
// bool beam = false;
// bool apply_to_opaque = true;
// fov_settings_type fov_settings;

/* Callbacks ------------------------------------------------------ */

/**
* Function called by libfov to apply light to a cell.
*
* \param map Pointer to map data structure passed to function such as
*            fov_circle.
* \param x   Absolute x-axis position of cell.
* \param y   Absolute x-axis position of cell.
* \param dx  Offset of cell from source cell on x-axis.
* \param dy  Offset of cell from source cell on y-axis.
* \param src Pointer to source data structure passed to function such
*            as fov_circle.
*/
// void apply(void *map, int x, int y, int dx, int dy, void *src) {
// 	if (((MAP *)map)->onMap(x, y))
// 		((MAP *)map)->setSeen(x, y);
// }


/**
* Function called by libfov to determine whether light can pass
* through a cell. Return zero if light can pass though the cell at
* (x,y), non-zero if it cannot.
*
* \param map Pointer to map data structure passed to function such as
*            fov_circle.
* \param x   Absolute x-axis position of cell.
* \param y   Absolute x-axis position of cell.
*/
// bool opaque(void *map, int x, int y) {
// 	return ((MAP *)map)->blockLOS(x, y);
// }


/* Functions ------------------------------------------------------ */

/**
* Offset player coordinates by (dx,dy). e.g. player_move(1,0) moves
* the player right one cell.
*/
void player_move(int dx, int dy) {
	GFovInst->PlayerMove(dx, dy);
}
void SetDirection(fov_direction_type dir)
{
	GFovInst->SetDirection(dir);
}
void IncreaseView(int delta)
{
	GFovInst->IncreaseView(delta);
}
void IncreaseAngle(int delta)
{
	GFovInst->IncreaseAngle(delta);
}
void ChangeShape(fov_shape_type shape)
{
	//fov_settings_set_shape(&fov_settings, shape);
	GFovInst->ChangeShape(shape);
}


/**
* Redraw the screen. Called in a loop to create the output.
*/
void redraw(void) {
	display_clear();

	/* Cause libfov to mark lit cells using the two callbacks given:
	*  - opaque, which is used to determine information about your
	*    map (a query you can define), and
	*  - apply, which is used to modify your map once libfov
	*    determines that a particular cell is lit (a command you can
	*    define).
	*
	* In this call, the light source is at (pX,pY). We pass NULL for
	* the source pointer parameter because we have no data structure
	* defining our light source in this example. You may want to pass
	* a pointer to a data structure holding your light source if, for
	* example, you wanted different coloured lights. In that case the
	* colour of the cell would depend upon the colour of the source
	* and you would have to extract this colour from your data
	* structure in each call to apply.
	*/

	GFovInst->Draw();

	display_refresh();
}

/**
* Clean up and exit.
*/
void normal_exit(void) {
	display_exit();

	delete GFovInst;
	GFovInst = NULL;

	exit(0);
}


/**
* Print keyboard help.
*/
void print_help(void) {
	printf("----------------------------------------------------\n");
	printf("Keyboard Help\n");
	printf("(left, right, up, down) or the keypad: Move around\n");
	printf("=: Increase radius\n");
	printf("-: Decrease radius\n");
	printf("[: Increase angle (in beam mode)\n");
	printf("]: Decrease angle (in beam mode)\n");
	printf("a: Toggle lighting on opaque tiles\n");
	printf("b: Toggle beam mode\n");
	printf("c: Circle shape\n");
	printf("o: Octogon shape\n");
	printf("p: Precalculated circle shape\n");
	printf("s: Square shape\n");
	printf("h,?: Print this message\n");
	printf("Esc,q: Quit\n");
	printf("----------------------------------------------------\n");
}


/**
* Handle a keypress. Callback used by display_event_loop.
*/
void keypressed(int key, int shift) {
	switch (key) {
	case KEY_UP:
	case KEY_KP8:
		//if (!beam || direction == FOV_NORTH)
			player_move(0, -1);
		SetDirection(FOV_NORTH);
		break;
	case KEY_KP2:
	case KEY_DOWN:
		//if (!beam || direction == FOV_SOUTH)
			player_move(0, 1);
		SetDirection(FOV_SOUTH);
		break;
	case KEY_KP4:
	case KEY_LEFT:
		//if (!beam || direction == FOV_WEST)
			player_move(-1, 0);
		SetDirection(FOV_WEST);
		break;
	case KEY_KP6:
	case KEY_RIGHT:
		//if (!beam || direction == FOV_EAST)
			player_move(1, 0);
		SetDirection(FOV_EAST);
		break;
	case KEY_KP7:
		//if (!beam || direction == FOV_NORTHWEST)
			player_move(-1, -1);
		SetDirection(FOV_NORTHWEST);
		break;
	case KEY_KP9:
		//if (!beam || direction == FOV_NORTHEAST)
			player_move(1, -1);
		SetDirection(FOV_NORTHEAST);
		break;
	case KEY_KP1:
		//if (!beam || direction == FOV_SOUTHWEST)
			player_move(-1, 1);
		SetDirection(FOV_SOUTHWEST);
		break;
	case KEY_KP3:
		//if (!beam || direction == FOV_SOUTHEAST)
			player_move(1, 1);
		SetDirection(FOV_SOUTHEAST);
		break;
	case KEY_EQUALS:
		//radius++;
		IncreaseView(1);
		//printf("Increased radius to %d\n", radius);
		break;
	case KEY_MINUS:
// 		radius--;
// 		if (radius <= 0)
// 			radius = 1;
		IncreaseView(-1);
		//printf("Decreased radius to %d\n", radius);
		break;
	case KEY_RIGHTBRACKET:
// 		angle += 5.0;
// 		if (angle >= 360.0f)
// 			angle = 360.0f;
		IncreaseAngle(5);
		//printf("Increased angle to %0.1f\n", angle);
		break;
	case KEY_LEFTBRACKET:
// 		angle -= 5.0;
// 		if (angle <= 0.0f)
// 			angle = 0.0f;
		IncreaseAngle(-5);
		//printf("Decreased angle to %0.1f\n", angle);
		break;
	case KEY_p:
		ChangeShape(FOV_SHAPE_CIRCLE_PRECALCULATE);
		printf("Precalculated circular limit\n");
		break;
	case KEY_c:
		ChangeShape(FOV_SHAPE_CIRCLE);
		printf("Circular limit\n");
		break;
	case KEY_s:
		ChangeShape(FOV_SHAPE_SQUARE);
		printf("Square limit\n");
		break;
	case KEY_o:
		ChangeShape(FOV_SHAPE_OCTAGON);
		printf("Octagonal limit\n");
		break;
	case KEY_a:
		//fov_settings_set_opaque_apply(&fov_settings, (fov_opaque_apply_type)(!fov_settings.opaque_apply));
		//printf("Toggling applying to opaque tiles (%d)\n", fov_settings.opaque_apply);
		break;
	case KEY_b:
		//beam = !beam;
		GFovInst->ToggleBeam();
		//printf("Toggling beam (%d)\n", beam);
		break;
	case KEY_h:
	case KEY_SLASH:
		print_help();
		break;
	case KEY_q:
	case KEY_ESCAPE:
		normal_exit();
		break;
	default:
		break;
	}
	redraw();
}


/**
* Program entry point.
*/
int main1()
{
	GFovInst = CreateFovInstance();
	
	display_init();
	redraw();
	display_event_loop(keypressed);
	normal_exit();
	return 0;
}


int _stdcall
WinMain (struct HINSTANCE__ *hInstance,
struct HINSTANCE__ *hPrevInstance,
	char               *lpszCmdLine,
	int                 nCmdShow)
{
	return main1();
}