
#ifndef MATERIAL_H
#define MATERIAL_H

//#include <OpenGL/gl.h>
#include "windows.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glut.h>
#include <string>

class Material
{
public:
	color c;
	GLfloat amb;     // ambient reflectivity
	GLfloat dif;     // diffuse percent
	GLfloat spec;	 // specular percent
	GLfloat refl;	// reflective percent
	GLfloat refr;	// refractive index of material

	Material()
	{
		c.r = 1.0f;
		c.g = 1.0f;
		c.b = 1.0f;
		amb = 1.0f;
		dif = 1.0f;
		spec = 1.0f;
		refl = 1.0f;
		refr = 1.0f;
	}

	void set(GLfloat r, GLfloat g, GLfloat b, GLfloat ambientRef, GLfloat diffusePer, GLfloat specPer, GLfloat reflPer, GLfloat refrInd)
	{
		c.r = r;
		c.g = g;
		c.b = b;
		amb = ambientRef;
		dif = diffusePer;
		spec = specPer;
		refl = reflPer;
		refr = refrInd;
	}
};

#endif











