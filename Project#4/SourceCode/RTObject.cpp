
#include "RTObject.h"
#include "raytrace.h"
#include <iostream>

extern point eye;

std::string & RTObject::getName(void)
{
	return name_;
}

/******************************************************************************/
/*************                     RTSphere                       *************/
/******************************************************************************/

RTSphere::RTSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r, std::string name)
{
	center.x = x;
	center.y = y;
	center.z = z;
	center.w = 1.0;
	radius = r;
	name_ = name;
}

// r: ray with start point worldPix and end vector direction
double RTSphere::intersection(const ray &r)
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
		
		// Ignore roots which are less than zero (behind viewpoint)
		if ( t < INTERSECTION_TOLERANCE )
		{
			t = (-b + D) / (2 * a);
		}
		if ( t < INTERSECTION_TOLERANCE )
			return 0.0;
		else
			return t;	// t <-- positive value if something hits, 0 value otherwise
						// if t hits, then trace (calling function) will return rto that was hit, else trace returns null
	}
}

// Note division by radius to normalize, returns an orthonormal normal to point p where ray hit
vector RTSphere::normalAt(const point &p, const point &from)	// p: where ray hit, from: ray start point, the worldPix you are going to color
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


/******************************************************************************/
/*************                     RTPlane                        *************/
/******************************************************************************/

RTPlane::RTPlane(GLfloat qX, GLfloat qY, GLfloat qZ, GLfloat nX, GLfloat nY, GLfloat nZ, std::string name)
{
	q.x = qX;
	q.y = qY;
	q.z = qZ;
	q.w = 1.0f;		// q is a point on the plane
	
	n.x = nX;
	n.y = nY;
	n.z = nZ;
	n.w = 0.0f;		// n is an orthonormal vector to the plane
	
	// ensure n is a unit vector
	normalize(&n);
	
	name_ = name;
}


// r.start = &worldPix point
// r.end = &direction vector
double RTPlane::intersection(const ray &r)
{
	// t is the distance with an intersected plane!
	float t = (-1)*( dotproduct((*r.start - q), n) / dotproduct(*r.end, n));


	
	if ( dotproduct(*r.end, n) == 0 )	// if normal and direction vector are orthogonal, ray cannot hit the plane! (parallel)
	{
		return 0.0;
	}
	
	if ( t < INTERSECTION_TOLERANCE )	// plane is behind eye point so no intersection
	{
		return 0.0;
	}
	else	// otherwise, we must have a hit!
	{
		return t;
	}
}

vector RTPlane::normalAt(const point &p, const point &from)
{
	return n;
}

/******************************************************************************/
/*************                     RTTriangle                     *************/
/******************************************************************************/

RTTriangle::RTTriangle(point &p0, point &p1, point &p2, std::string name)
{
	// set p0, p1, p2 points
	p0_ = p0;
	p1_ = p1;
	p2_ = p2;
	
	// calculate normal n that points out the front of triangle
	n_ = crossproduct( (p1_ - p0_), (p2_ - p0_) );
	normalize(&n_);
	
	name_ = name;
}


// r.start = &worldPix point
// r.end = &direction vector
double RTTriangle::intersection(const ray &r)
{
	// t is the distance with an intersected plane!
	float t = (-1)*( dotproduct((*r.start - p0_), n_) / dotproduct(*r.end, n_));
	
	// x is the point on the plane where hit
	point x = *r.start + t*(*r.end);
	
//	// cull back facing triangles
//	if ( dotproduct(*r.start, n_) <= INTERSECTION_TOLERANCE )
//	{
//		return 0.0f;
//	}
	
	// first test if triangle is behind camera or if normal and direction vector orthogonal (t = 0)
	if ( t < INTERSECTION_TOLERANCE )
	{
		return 0.0f;
	}
	
	// testing if x is on the "left" of the point and vectors that make up the 3 triangle lines (ccw)
	if ( dotproduct( crossproduct(p1_-p0_, x-p0_), n_ ) >= 0.0f &&
  		 dotproduct( crossproduct(p2_-p1_, x-p1_), n_ ) >= 0.0f &&
		 dotproduct( crossproduct(p0_-p2_, x-p2_), n_ ) >= 0.0f )
	{
		return t;
	}
	// otherwise the point x is not inside the triangle
	else
	{
		return 0.0f;
	}
}

vector RTTriangle::normalAt(const point &p, const point &from)
{
	return n_;
}













