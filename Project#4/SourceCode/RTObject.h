
#ifndef RTOBJECT_H
#define RTOBJECT_H

#include "structures.h"
#include "Material.h"
#include <string>



/****************************
 * Abstract base class for objects in scene
 ****************************/
class RTObject
{
public:
	virtual ~RTObject() {}
	
	virtual double intersection(const ray &r) = 0;
	virtual vector normalAt(const point &p, const point &from) = 0;
	
	void setMaterial(const Material &mat) { m = mat; }
	Material* getMaterialPointer() { return &m; };
	std::string & getName(void);
	
protected:
	Material m;
	std::string name_;		// for debugging refraction
};

/****************************
 * Sphere objects in the scene
 ****************************/
class RTSphere: public RTObject
{
public:
	RTSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r, std::string name);
	
	double intersection(const ray &r);
	vector normalAt(const point &p, const point &from);
	
protected:
	point center;
	float radius;
};

/****************************
 * Planes in the scene
 ****************************/
class RTPlane: public RTObject
{
public:
	RTPlane(GLfloat qX, GLfloat qY, GLfloat qZ, GLfloat nX, GLfloat nY, GLfloat nZ, std::string name);
	
	double intersection(const ray &r);
	vector normalAt(const point &p, const point &from);
	
protected:
	point q;
	vector n;
};

/****************************
 * Triangles in the scene
 ****************************/
class RTTriangle: public RTObject
{
public:
	RTTriangle(point &p0, point &p1, point &p2, std::string name);	
	double intersection(const ray &r);
	vector normalAt(const point &p, const point &from);
	
protected:
	point p0_;
	point p1_;
	point p2_;
	vector n_;
};



#endif
