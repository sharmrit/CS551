#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct point
{
	float x;
	float y;
	float z;
	float w;
	
} point;

// a vector is just a point
typedef point vector;

// a line segment
typedef struct segment
{
	point *start;
	point *end;
} segment;

// a ray is just a segment with an endpoint at infinity (.i.e, a vector)
typedef segment ray;

typedef struct color
{
	float r;
	float g;
	float b; 
	// these should be between 0 and 1
} color;

typedef struct light
{
	point p;	
	color ambient;	
	color difuse;	
	color specular;	// color of specular light
} light;

#endif
