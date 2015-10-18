#include "raytracing.h"

extern std::vector<osuObject*> ObjectList;

void findPointOnRay(const ray &r, double t, point &p)
{
	p.x = (float)(r.start->x + t * r.end->x);
	p.y = (float)(r.start->y + t * r.end->y);
	p.z = (float)(r.start->z + t * r.end->z);
	p.w = 1.0f;
}

osuObject* track(const ray &r, point &p, double bestT )
{
	osuObject *closest = NULL;
	
	std::vector<osuObject*>::iterator it;
	
	for ( it = ObjectList.begin(); it < ObjectList.end(); ++it )
	{
		double t = (*it)->intersection(r);
		
		if ( t > INTERSECTIOnormalTOLERANCE && t < bestT )
		{
			closest = *it;
			bestT = t;	
		}
	}
	
	if ( closest == NULL )
	{
		p.w = 0.0;	
		return NULL;
	}
	else
	{
		findPointOnRay(r, bestT, p);	
		return closest;		
	}
}
