
#ifndef LOWLEVEL_H
#define LOWLEVEL_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <OpenGL/OpenGL.h>
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
//#include <GLUT/GLUT.h>
#include "windows.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glut.h>

/* lowlevel drawing funtions */
void initCanvas(int,int);
void drawPixel(int,int, GLfloat,GLfloat,GLfloat);
void flushCanvas(void);

/* global variables accessed */
extern int width;
extern int height;

#endif









