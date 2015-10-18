#ifndef OSUOBJECT_H
#define OSUOBJECT_H

#include "structures.h"
#include "ObjectMaterial.h"
#include <string>

class osuObject
{
public:
	virtual ~osuObject() {}
	
	virtual double intersection(const ray &r) = 0;
	virtual vector normalAt(const point &p, const point &from) = 0;
	
	void setObjectMaterial(const ObjectMaterial &mat) { m = mat; }
	ObjectMaterial* getObjectMaterialPointer() { return &m; };
	
protected:
	ObjectMaterial m;
};


class osuSphere: public osuObject
{
public:
    osuSphere(float x, float y, float z, float r);
	
	double intersection(const ray &r);
	vector normalAt(const point &p, const point &from);
	
protected:
	point center;
	float radius;
};

class osuTriangle: public osuObject
{
public:
	osuTriangle(point &p0, point &p1, point &p2);	
	double intersection(const ray &r);
	vector normalAt(const point &p, const point &from);
	
protected:
	point point_0;
	point point_1;
	point point_2;
	vector normal;
};



#endif
