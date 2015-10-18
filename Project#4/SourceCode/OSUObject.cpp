#include "osuObject.h"
#include "raytracing.h"
#include <iostream>

extern point eye;

osuSphere::osuSphere(float x, float y, float z, float r)
{
	center.x = x;
	center.y = y;
	center.z = z;
	center.w = 1.0;
	radius = r;   
}

// r: ray with start point worldPix and end vector direction
double osuSphere::intersection(const ray &r)
{
	point p;         // start of transformed ray
	double a, b, c;  // coefficients of quadratic equation
	double D;        // discriminant
	point* v;
	double t;		// if there's a hit, t will return positive (> 0.001)
	
	// Transform ray so that sphere center is at origin
	// don't use matrix, just translate!
	p.x = r.start->x - center.x;
	p.y = r.start->y - center.y;
	p.z = r.start->z - center.z;
	v = r.end; // Point to direction vector
	
	
	a = v->x * v->x  +  v->y * v->y  +  v->z * v->z;
	b = 2 * (v->x * p.x  +  v->y * p.y  +  v->z * p.z);
	c = p.x * p.x + p.y * p.y + p.z * p.z - radius * radius;
	
	D = b * b - 4 * a * c;
	
	if ( D < 0 )	// no intersection
	{
		return 0;
	}
	else
	{
		D = sqrt(D);
		
		// First check the root with the lower value of t:
		// this one, since D is positive
		t = (-b - D) / (2 * a);
		
		// Ignore roots which are less than zero (behind ViewPoint)
		if ( t < INTERSECTIOnormalTOLERANCE )
		{
			t = (-b + D) / (2 * a);
		}
		if ( t < INTERSECTIOnormalTOLERANCE )
			return 0.0;
		else
			return t;	// t <-- positive value if something hits, 0 value otherwise
						// if t hits, then track (calling function) will return rto that was hit, else track returns null
	}
}

// Note division by radius to normalize, returns an orthonormal normal to point p where ray hit
vector osuSphere::normalAt(const point &p, const point &from)	// p: where ray hit, from: ray start point, the worldPix you are going to color
{
	// normally, normal vector tangent to sphere's surface is just a unit vector from center to point on surface!
	vector n;
	n.x = (p.x - center.x) / radius;	// normalizes the whole vector
	n.y = (p.y - center.y) / radius;	// normalizes the whole vector
	n.z = (p.z - center.z) / radius;	// normalizes the whole vector
	n.w = 0.0;	// 0.0 since a vector
	
	// If incoming ray has some portion in same direction as normal
	// then it must have been coming from inside the sphere, flip
	// direction of normal ... (to show refraction?)
	vector r;	// vector
	r.x = p.x - from.x;
	r.y = p.y - from.y;
	r.z = p.z - from.z;
	if ( dotproduct(n, r) > 0 )	// n DOT r = cos(theta) = positive (0,1] value if in the same direction
	{
		n.x *= -1;	// ... (to show refraction and reflection?)
		n.y *= -1;	// ... (to show refraction and reflection?)
		n.z *= -1;	// ... (to show refraction and reflection?)
	}
	
	return n;	// gimme the normal vector from the point this object was hit yo!
}


osuTriangle::osuTriangle(point &p0, point &p1, point &p2)
{
	// set p0, p1, p2 points
	point_0 = p0;
	point_1 = p1;
	point_2 = p2;
	
	// calculate normal n that points out the front of triangle
	normal = crossproduct( (point_1 - point_0), (point_2 - point_0) );
	normalize(&normal);
}


// r.start = &worldPix point
// r.end = &direction vector
double osuTriangle::intersection(const ray &r)
{
	// t is the distance with an intersected plane!
	float t = (-1)*( dotproduct((*r.start - point_0), normal) / dotproduct(*r.end, normal));
	
	// x is the point on the plane where hit
	point x = *r.start + t*(*r.end);
	
//	// cull back facing triangles
//	if ( dotproduct(*r.start, normal) <= INTERSECTIOnormalTOLERANCE )
//	{
//		return 0.0f;
//	}
	
	// first test if triangle is behind camera or if normal and direction vector orthogonal (t = 0)
	if ( t < INTERSECTIOnormalTOLERANCE )
	{
		return 0.0f;
	}
	
	// testing if x is on the "left" of the point and vectors that make up the 3 triangle lines (ccw)
	if ( dotproduct( crossproduct(point_1-point_0, x-point_0), normal ) >= 0.0f &&
  		 dotproduct( crossproduct(point_2-point_1, x-point_1), normal ) >= 0.0f &&
		 dotproduct( crossproduct(point_0-point_2, x-point_2), normal ) >= 0.0f )
	{
		return t;
	}
	// otherwise the point x is not inside the triangle
	else
	{
		return 0.0f;
	}
}

vector osuTriangle::normalAt(const point &p, const point &from)
{
	return normal;
}












