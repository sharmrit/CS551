
#ifndef ObjectMaterial_H
#define ObjectMaterial_H

#include "windows.h"
#include <string>

class ObjectMaterial
{
public:
	color c;
	float ambient;  
    float difuse;   
    float specular;	 
    float refl;	
    
	ObjectMaterial()
	{
		c.r = 1.0f;
		c.g = 1.0f;
		c.b = 1.0f;
		ambient = 1.0f;
		difuse = 1.0f;
		specular = 1.0f;
		refl = 1.0f;		
	}

    void set(float r, float g, float b, float ambientRef, float diffusePer, float specPer, float reflPer)
	{
		c.r = r;
		c.g = g;
		c.b = b;
		ambient = ambientRef;
		difuse = diffusePer;
		specular = specPer;
		refl = reflPer;		
	}
};

#endif











