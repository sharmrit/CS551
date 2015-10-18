
//*****************************************************************/
/*         Main drawing file for raytracer						  */
//*****************************************************************/

#include "raytrace.h"
#include "lowlevel.h"
#include <iostream>

// Vector from point p to point q is returned in v
void calculateDirection(point* p, point* q, point* v)
{
	v->x = q->x - p->x;
	v->y = q->y - p->y;
	v->z = q->z - p->z;
	// A direction is a point at infinity (a vector!)
	v->w = 0.0;
}

// returns the color seen by ray r in parameter c
void rayColor(const ray &r, color* c, GLint nColorFilter)
{
	point p;  // First intersection point
	RTObject *rto; // Intersected object
	
	p.w = 0.0;  // Inialize to "no intersection"
	rto = trace(r, p);	// p.w = 1.0 if hit, p.w = 0.0 if no hit! rto = closest hit object! rto = null if no hit
	
	if ( p.w != 0.0 )
	{
		// Do the lighting calculations ONLY IF HIT SOMETHING!
		shade(p, rto, r, c, nColorFilter);	// p point where hit, rto hit object, r ray with startpoint worldPix and vector direction, color uninitialized
	}
	else
	{
		// Nothing was hit
		c->r = BACKGROUND_R;
		c->g = BACKGROUND_G;
		c->b = BACKGROUND_B;
	}
}


void drawScene (GLint nColorFilter)
{
	// Declare data structures on stack to avoid malloc & free
	point worldPix;  // Current pixel in world coordinates
	point direction; // direction.w will be set to 0 since it is a vector
	ray r;		// ray with start point worldPix and end vector direction
	color c;
	
	// z and w are constant for all pixels
	worldPix.w = 1.0;
	worldPix.z = pnear;	// viewing plane is (x, y, z=1.0)
	
	r.start = &worldPix;
	r.end = &direction;
	
	GLfloat imageWidth = 2 * pnear * tan(fovx / 2);
	
	// Trace a ray for every pixel
	for ( int i = 0; i < width; i++ )
	{
		for ( int j = 0; j < height; j++ )
		{
			// find position of pixel in world coordinates
			// y position = (pixel height/middle) scaled to world coords 
			worldPix.y = (j - (height / 2))* imageWidth / width;
			// x position = (pixel width/middle) scaled to world coords 
			worldPix.x = (i - (width / 2))* imageWidth / width;
			
			// find direction
			// note: direction vector is NOT NORMALIZED
			calculateDirection(&viewpoint, &worldPix, &direction);	// direction calculated here... direction.w <-- 0
			
			// Find color for pixel
			rayColor(r, &c, nColorFilter);
			
			// write the pixel!
			drawPixel(i, j, c.r, c.g, c.b);
		}
	}
}

void set(point &p, float p0, float p1, float p2, float p3)
{
	p.x = p0;
	p.y = p1;
	p.z = p2;
	p.w = p3;
}

void set(color &c, float r, float g, float b)
{
	c.r = r;
	c.g = g;
	c.b = b;
}












