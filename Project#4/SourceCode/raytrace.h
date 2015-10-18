
/******************************************************************/
/*         Raytracer declarations								  */
/******************************************************************/

#ifndef RAYTRACE_H
#define RAYTRACE_H

// Colour when no objects hit
#define BACKGROUND_R 0.0f
#define BACKGROUND_G 0.0f
#define BACKGROUND_B 0.0f

// Minimum allowed time to hit another object so a ray from
// a point can't hit its starting point from rounding issues
#define INTERSECTION_TOLERANCE 0.001

// Constants to change screen size
#define WINDOW_W 640
#define WINDOW_H 480

// Uncomment this include to make it work on Windows
//#include <windows.h>

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

#include "structures.h"
#include "RTObject.h"
#include <vector>
#include <iostream>

// Global variables
extern point viewpoint;
extern GLfloat pnear;
extern GLfloat fovx;
extern int width;
extern int height;

// Objects and lights in the scene
extern std::vector<RTObject*> sceneObjects;

// Functions in tracer.cpp
void findPointOnRay(const ray &r, double t, point &p);
RTObject* trace(const ray &r, point &p, double bestT = 99999);

// Functions in shader.cpp
void shade(point &p, RTObject *rto, const ray &r, color* c, GLint nColorFilter = 0, int depth = 2); // depth two bounches
vector operator* (const point &p, float scalar);
vector operator* (float scalar, const point &p);
vector operator+ (const vector &v1, const vector &v2);
vector operator- (const vector &v1, const vector &v2);
std::ostream & operator<< (std::ostream &os, const vector &v);
std::ostream & operator<< (std::ostream &os, const color &c);
inline float length(const vector &v);
void normalize(vector *v);

// Functions in raytrace.cpp
void drawScene(GLint nColorFilter = 0);

inline float dotproduct(const vector &v1, const vector &v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline vector crossproduct(const vector &v1, const vector &v2)
{
	if ( v1.w != 0 || v2.w != 0 )
	{
		std::cout << v1 << " or " << v2 << "not a vector! Aborting and returning zero vector.\n";
		vector zero = {0.0f, 0.0f, 0.0f, 0.0f};
		return zero;
	}
	else
	{
		vector cross = { v1.y*v2.z - v1.z*v2.y,
						 v1.z*v2.x - v1.x*v2.z,
						 v1.x*v2.y - v1.y*v2.x,
						 0.0f };
		return cross;
	}
}

// set point/vector to new location
void set(point &p, float p0, float p1, float p2, float p3);

// set color to new color
void set(color &c, float r, float g, float b);



#endif








