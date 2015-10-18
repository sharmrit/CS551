#include "raytracing.h"
#include "ObjectMaterial.h"
#include <iostream>
#include <cmath>
#include <string>

const int maxDist = 999999;

extern point eye;
extern light light1; 

vector operator* (const point &p, float scalar)
{
	vector v;
	v.x = p.x * scalar;
	v.y = p.y * scalar;
	v.z = p.z * scalar;
	v.w = 1.0;
	return v;
}

vector operator* (float scalar, const point &p)
{
	return p*scalar;
}

vector operator+ (const vector &v1, const vector &v2)
{
	vector v;
	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;
	v.w = 1.0;
	return v;
}

vector operator- (const vector &v1, const vector &v2)
{
	vector v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;
	v.w = 0.0f;
	return v;
}

bool operator== (const vector &v1, const vector &v2)
{
	return (v1.x == v2.x &&
			v1.y == v2.y &&
			v1.z == v2.z &&
			v1.w == v2.w);
}

bool operator!= (const vector &v1, const vector &v2)
{
	return !(v1 == v2);
}

std::ostream & operator<< (std::ostream &os, const vector &v)
{
	os << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
	
	return os;
}

std::ostream & operator<< (std::ostream &os, const color &c)
{
	os << "(" << c.r << "," << c.g << "," << c.b << ")";
	
	return os;
}


inline float length(const vector &v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}


void normalize(vector *v)
{
	float lngth = length(*v);
	v->x /= lngth;
	v->y /= lngth;
	v->z /= lngth;
}


void shade(point &p, osuObject *rto, const ray &r, color* c, int nColorFilter, int depth )	
{

	vector n = rto->normalAt(p, *(r.start));
	ObjectMaterial *m = rto->getObjectMaterialPointer();
	
	
	c->r = 0.0f;
	c->g = 0.0f;
	c->b = 0.0f;
		
	color cAmb;
	cAmb.r = m->ambient * m->c.r;
	cAmb.g = m->ambient * m->c.g;
	cAmb.b = m->ambient * m->c.b;
		
	vector vPToEye = eye - p;
	vector vPToLight = light1.p - p;
		
	normalize(&vPToEye);
	normalize(&vPToLight);
		
    float cosTheta = dotproduct(n, vPToLight);
		
	color cDif = {0.0f, 0.0f, 0.0f};
	
	if ( cosTheta > 0.0f )
	{
		cDif.r = m->difuse * m->c.r * cosTheta;
		cDif.g = m->difuse * m->c.g * cosTheta;
		cDif.b = m->difuse * m->c.b * cosTheta;
	}
	vector v = *(r.end);
	
	vector l = p - light1.p;
		
	vector ref = l - 2.0f*dotproduct(l, n)*n;
	normalize(&ref);
	
	float dot = dotproduct(v, ref);
	color cSpec = {0.0f, 0.0f, 0.0f};
	
	if ( dot < 0.0f )	
	{
		float spec = powf(dot, 20.0f);
		
		cSpec.r += spec * m->specular;
		cSpec.g += spec * m->specular;
		cSpec.b += spec * m->specular;
	}
	
	ray reflected;
	
	point refStart = p;
	refStart = p + (-INTERSECTIOnormalTOLERANCE * (*r.end));
	reflected.start = &refStart;
	
	vector refl = *r.end - 2.0f * ( dotproduct(*r.end, n) ) * n;
	reflected.end = &refl;
	
	point reflectedHitPoint;
	
	osuObject *rtoReflected = track(reflected, reflectedHitPoint);
	
	color cRef = {0.0f, 0.0f, 0.0f};
	
	if ( reflectedHitPoint.w != 0.0 && depth-- > 0 && m->refl )
	{
		shade(reflectedHitPoint, rtoReflected, reflected, &cRef, nColorFilter, depth);
	}
	
	ray shadowRay;
	shadowRay.start = &p;
	vector shadowDir = light1.p - p;
	shadowRay.end = &shadowDir;
	
	point shadowHitPoint;
		
	osuObject *shadower = track(shadowRay, shadowHitPoint);
	
	bool pixShadowed = false;
		
	if ( shadower )
	{
		pixShadowed = true;
	}
	
	float da = (::maxDist - length(light1.p - p)) / ::maxDist;
	
	
	c->r += light1.ambient.r*cAmb.r + da*(light1.difuse.r*cDif.r + light1.specular.r*cSpec.r) + cRef.r*m->refl;
	c->g += light1.ambient.g*cAmb.g + da*(light1.difuse.g*cDif.g + light1.specular.g*cSpec.g) + cRef.g*m->refl;
	c->b += light1.ambient.b*cAmb.b + da*(light1.difuse.b*cDif.b + light1.specular.b*cSpec.b) + cRef.b*m->refl;
	
	switch ( nColorFilter )
	{
		case COLOR_FILTER_RED:
			c->g = 0.0f;
			c->b = 0.0f;
			break;
		case COLOR_FILTER_GREEN:
			c->r = 0.0f;
			c->b = 0.0f;
			break;
		case COLOR_FILTER_BLUE:
			c->r = 0.0f;
			c->g = 0.0f;
			break;
			
		case COLOR_FILTER_NONE:
		default:
			break;
	}
	
	if ( pixShadowed )
	{
		c->r *= 0.8f;	
		c->g *= 0.8f;
		c->b *= 0.8f;
	}
	
	if (c->r > 1.0) c->r = 1.0;
	if (c->g > 1.0) c->g = 1.0;
	if (c->b > 1.0) c->b = 1.0;
	
}
