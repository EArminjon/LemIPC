/*
** EPITECH PROJECT, 2018
** mdr
** File description:
** mdr
*/

#include "lemipc.h"

void init_window()
{
	SDL_Init(SDL_INIT_VIDEO);
	atexit(SDL_Quit);
	SDL_WM_SetCaption("SDL GL Application", NULL);
	SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_OPENGL);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(70,(double)WIDTH/HEIGHT,1,1000);
	glEnable(GL_DEPTH_TEST);
	gluLookAt(10 , ARENA_SIZE, ARENA_SIZE / 2, 0, 0, 0, 0, 0, 1);
}

void draw_arena()
{
	glBegin(GL_QUADS);

    	glColor3ub(50,50,50);
	glVertex3d((ARENA_SIZE + 2) / 2, -(ARENA_SIZE + 2) / 2, 0);
    	glVertex3d((ARENA_SIZE + 2) / 2, (ARENA_SIZE + 2) / 2, 0);
    	glVertex3d(-(ARENA_SIZE + 2) / 2, (ARENA_SIZE + 2) / 2, 0);
    	glVertex3d(-(ARENA_SIZE + 2) / 2, -(ARENA_SIZE + 2) / 2, 0);

    	glEnd();
}

void draw_cube(float x, float y, const float size, int r, int g, int v)
{
	x = ARENA_SIZE / 2 - x - size;
	y = ARENA_SIZE / 2 - y - size;

	glTranslatef(-x, -y, size + 0.1);
	glBegin(GL_QUADS);

	glColor3ub(r, g, v);

	// FRONT
	glVertex3f(-size,-size,size);
	glVertex3f(size,-size,size);
	glVertex3f(size,size,size);
	glVertex3f(-size,size,size);

	// BACK
	glVertex3f(-size,-size,-size);
	glVertex3f(-size,size,-size);
	glVertex3f(size,size,-size);
	glVertex3f(size,-size,-size);

	// LEFT
	glVertex3f(-size,-size,size);
	glVertex3f(-size,size,size);
	glVertex3f(-size,size,-size);
	glVertex3f(-size,-size,-size);

	// RIGHT
	glVertex3f(size,-size,-size);
	glVertex3f(size,size,-size);
	glVertex3f(size,size,size);
	glVertex3f(size,-size,size);

	// TOP
	glVertex3f(-size,size,size);
	glVertex3f(size,size,size);
	glVertex3f(size,size,-size);
	glVertex3f(-size,size,-size);

	// BOTTON
	glVertex3f(-size,-size,size);
	glVertex3f(-size,-size,-size);
	glVertex3f(size,-size,-size);
	glVertex3f(size,-size,size);

    	glEnd();
	glTranslatef(x, y, -(size + 0.1));
}


void draw_array(const char *array)
{
	for (int y = ARENA_SIZE - 1 ; y >= 0 ; y--) {
		for (int x = ARENA_SIZE ; x >= -1 ; x--) {
			switch(array[CHARPOS(x, y)]) {
				case '0':
					draw_cube(x, y, CUBE_SIZE, 230, 25, 75); break;
				case '1':
					draw_cube(x, y, CUBE_SIZE, 0, 255, 0); break;
				case '2':
					draw_cube(x, y, CUBE_SIZE, 255, 0, 255); break;
				case '3':
					draw_cube(x, y, CUBE_SIZE, 70, 240, 240); break;
				case '4':
					draw_cube(x, y, CUBE_SIZE, 0, 255, 255); break;
				case '5':
					draw_cube(x, y, CUBE_SIZE, 255, 225, 25); break;
				case '6':
					draw_cube(x, y, CUBE_SIZE, 145, 30, 180); break;
				case '7':
					draw_cube(x, y, CUBE_SIZE, 245, 130, 48); break;
				case '8':
					draw_cube(x, y, CUBE_SIZE, 0, 127, 127); break;
				case '9':
					draw_cube(x, y, CUBE_SIZE, 128, 0, 0); break;
			}
		}
	}
}

int key(const int key)
{
	if (key == SDLK_ESCAPE)
		return (TRUE);
	return (FALSE);
}

int manage_event()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_KEYUP:
			return (key(event.key.keysym.sym));
		case SDL_QUIT:
			return (TRUE);
		}
	}
	return (FALSE);
}

void clear_window()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    	glLoadIdentity();
}

void refresh_window()
{
	glFlush();
	SDL_GL_SwapBuffers();
}
