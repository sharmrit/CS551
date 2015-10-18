#include "raytracing.h"
#include <iostream>

#define M_PI 3.1415926538f
int drawtype;
point p[3];
int count;
color tempColor;
point eye;
point ViewPort;
light light1;	
point ViewPoint;
float pnear;	
float fieldofViewfromX;	
int width;	
int height;	
ObjectMaterial *newObjectMaterial;
extern std::vector<osuObject*> ObjectList;

void calculateDirection(point* p, point* q, point* v)
{
	v->x = q->x - p->x;
	v->y = q->y - p->y;
	v->z = q->z - p->z;
	v->w = 0.0;
}

void rayColor(const ray &r, color* c, int nColorFilter)
{
	point p;  
	osuObject *rto;
	
	p.w = 0.0;  
	rto = track(r, p);	
	
	if ( p.w != 0.0 )
	{
		
		shade(p, rto, r, c, nColorFilter);	
	}
	else
	{
		
		c->r = BACKGROUND_R;
		c->g = BACKGROUND_G;
		c->b = BACKGROUND_B;
	}
}


void DisplayScene()
{
	point worldPix;  
	point direction; 
	ray r;		
	color c;
	
	worldPix.w = 1.0;
	worldPix.z = pnear;
	
	r.start = &worldPix;
	r.end = &direction;
	
    float imageWidth = 2 * pnear * tan(fieldofViewfromX / 2);
	
	for ( int i = 0; i < width; i++ )
	{
		for ( int j = 0; j < height; j++ )
		{
			worldPix.y = (j - (height / 2))* imageWidth / width;
			worldPix.x = (i - (width / 2))* imageWidth / width;
			calculateDirection(&ViewPoint, &worldPix, &direction);
			rayColor(r, &c, COLOR_FILTER_NONE);
			osuWritePixel(i, j, c.r*255, c.g*255, c.b*255);
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




void osuLookat()
{
    ViewPoint.x = 0.0;
    ViewPoint.y = 0.0;
    ViewPoint.z = 2.0;
    ViewPoint.w = 1.0;
    pnear = 1.0;
    fieldofViewfromX = M_PI / 6;
    eye = ViewPoint;
    ViewPort = eye;
    ViewPort.z = pnear;    
}

void osuBegin(int OSUDrawable)
{
    if (OSUDrawable == OSU_TRIANGLE)
	{
        drawtype = OSU_TRIANGLE;
        count = 0;
    }
}

void osuVertex3f(float x, float y, float z)
{
	p[count].x= x;
	p[count].y= y;
	p[count].z= z;
    count++;
}

void osuColor3f(float r, float g, float b)
{
    tempColor.r = r;
    tempColor.g = g;
    tempColor.b = b;
}
void osuObjectMaterial(float ambientRef, float diffusePer, float specPer, float reflPer)
{
    newObjectMaterial = new ObjectMaterial();
    newObjectMaterial->set(tempColor.r, tempColor.g, tempColor.b, ambientRef, diffusePer, specPer, reflPer);
}

void osuPointLight(float pos[3], float i)
{
    set(light1.p, pos[0], pos[1], pos[2], 1.0f);
}
void osuAmbientLight(float r,float g,float b)
{
    set(light1.ambient, r, g, b);
}

void osuDiffuse(float r, float g, float b)
{
    set(light1.difuse, r, g, b);
}

void osuSpecular(float r, float g, float b)
{
    set(light1.specular, r, g, b);
}

void osuEnd()
{
    if (drawtype == OSU_TRIANGLE)
	{
        osuObject *newTriangle = new osuTriangle(p[0], p[1], p[2]);
        newTriangle->setObjectMaterial(*newObjectMaterial);
        ObjectList.push_back(newTriangle);
    }
}

void osuDrawSphere(float x, float y, float z, float R)
{
    osuObject *newSphere = new osuSphere(x, y, z, R);
    newSphere->setObjectMaterial(*newObjectMaterial);
    ObjectList.push_back(newSphere);
}








