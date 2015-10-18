#ifndef RAYtrack_H
#define RAYtrack_H

// Colour when no objects hit
#define BACKGROUND_R 0.0f
#define BACKGROUND_G 0.0f
#define BACKGROUND_B 0.0f

#define INTERSECTIOnormalTOLERANCE 0.001

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "osuGraphics.h"


#include "structures.h"
#include "osuObject.h"
#include <vector>
#include <iostream>
void findingthePointontheRay(const ray &r, double t, point &p);
osuObject* track(const ray &r, point &p, double bestT = 99999);

void shade(point &p, osuObject *rto, const ray &r, color* c, int nColorFilter = 0, int depth = 2); 
vector operator* (const point &p, float scalar);
vector operator* (float scalar, const point &p);
vector operator+ (const vector &v1, const vector &v2);
vector operator- (const vector &v1, const vector &v2);
std::ostream & operator<< (std::ostream &os, const vector &v);
std::ostream & operator<< (std::ostream &os, const color &c);
inline float length(const vector &v);
void normalize(vector *v);

void DisplayScene();

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

void set(point &p, float p0, float p1, float p2, float p3);

void set(color &c, float r, float g, float b);

#endif








