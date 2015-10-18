#include "raytracing.h"
#include <iostream>
#include <string>
#include "osuGraphics.h"

#ifndef M_PI
#define M_PI 3.1415926538f
#define winormalwidth 600
#define winormalheight 600
#endif

using namespace std;

void initializeScene();

std::vector<osuObject*> ObjectList;



int main(int argc, char** argv)
{
	  int num;

	cout<<"Test Cases:"<<endl;
	cout<<"1: Scene 1"<<endl;
	cout<<"2: Scene 2"<<endl;
	cout<< "Enter the choice bewteen 1 to 2:"<<endl;
	cin>>num;
	cout<< "Selected choice:"<<num<<endl;

	if((num <0 ) || (num > 2))
	{
		fprintf(stderr, "Please call this program with a number from 1 to 2 \n");
		exit(-1);
	}


    osuBeginGraphics(winormalwidth, winormalheight);
	osuLookat();
    float pos[3] = { 0.0f, 5.0f, 4.0f};
    osuPointLight(pos, 1.0);
    osuAmbientLight(1.0f, 1.0f, 1.0f);
    osuDiffuse(1.0f, 1.0f, 1.0f);
    osuSpecular(0.5f, 1.0f, 1.0f);

	switch (num) {
	case 1:
    osuColor3f(1.0, 1.0, 1.0);
    osuObjectMaterial(0.4f, 0.7f, 1.0f, 0.4f);
    osuDrawSphere(-0.1f, 0.2f, -1.0f, 0.25f);

	osuColor3f(0.0, 1.0, 0.0);
    osuObjectMaterial(0.4f, 0.7f, 1.0f, 0.4f);
    osuDrawSphere(0.5f, -0.3f, -2.0f, 0.1f);

	osuBegin(OSU_TRIANGLE);
    osuColor3f(1.0, 0.1, 0.0);
    osuObjectMaterial(0.9f, 1.0f, 0.5f, 0.8f);
    osuVertex3f(-0.70f, 0.00f, -1.0f);
    osuVertex3f(0.70f, 0.00f, -1.0f);
    osuVertex3f(0.00f, 0.70f, -1.0f);
    osuEnd();

	osuBegin(OSU_TRIANGLE);
    osuColor3f(1.0, 0.1, 0.0);
    osuObjectMaterial(0.2f, 0.7f, 2.5f, 0.8f);
    osuVertex3f(-0.10f, -0.40f, 0.10f);
    osuVertex3f(0.40f, -0.30f, 0.10f);
    osuVertex3f(0.70f, -0.40f, -2.0f);
    osuEnd();

	osuBegin(OSU_TRIANGLE);
    osuColor3f(0.5, 2.0, 5.0);
    osuObjectMaterial(0.2f, 0.7f, 2.5f, 0.8f);
    osuVertex3f(0.30f, 0.30f, 0.50f);
    osuVertex3f(0.20f, 0.30f, 0.75f);
    osuVertex3f(0.10f, 0.30f, 0.0f);
    osuEnd();


	osuBegin(OSU_TRIANGLE);
    osuColor3f(0.4, 0.2, 3.0);
    osuObjectMaterial(1.0f, 1.0f, 0.0f, 0.0f);
    osuVertex3f(0.0f, -0.20f, -0.25f);
    osuVertex3f(-0.40f, -0.20f, -0.25f);
    osuVertex3f(-0.20f, -0.40f, -0.28f);
    osuEnd();

	break;

    case 2:
    osuColor3f(1.0, 1.0, 1.0);
    osuObjectMaterial(0.4f, 0.7f, 1.0f, 0.4f);
    osuDrawSphere(-0.1f, 0.2f, -1.0f, 0.25f);

	osuColor3f(0.0, 1.0, 1.0);
    osuObjectMaterial(0.4f, 0.7f, 1.0f, 0.4f);
    osuDrawSphere(0.5f, -0.3f, -2.0f, 0.1f);

	osuBegin(OSU_TRIANGLE);
    osuColor3f(1.0, 0.1, 0.0);
    osuObjectMaterial(0.9f, 1.0f, 0.5f, 0.8f);
    osuVertex3f(-0.70f, 0.10f, -1.0f);
    osuVertex3f(0.70f, 0.10f, -1.0f);
    osuVertex3f(0.00f, 0.80f, -1.0f);
    osuEnd();

	osuBegin(OSU_TRIANGLE);
    osuColor3f(1.0, 0.1, 0.0);
    osuObjectMaterial(0.2f, 0.7f, 2.5f, 0.8f);
    osuVertex3f(0.30f, -0.40f, 0.10f);
    osuVertex3f(0.10f, -0.30f, 0.10f);
    osuVertex3f(0.40f, -0.40f, -2.0f);
    osuEnd();

	osuBegin(OSU_TRIANGLE);
    osuColor3f(0.5, 2.0, 5.0);
    osuObjectMaterial(0.2f, 0.7f, 2.5f, 0.8f);
    osuVertex3f(-0.30f, 0.30f, 0.50f);
    osuVertex3f(-0.20f, 0.30f, 0.75f);
    osuVertex3f(-0.10f, 0.30f, 0.0f);
    osuEnd();


	osuBegin(OSU_TRIANGLE);
    osuColor3f(0.4, 0.2, 3.0);
    osuObjectMaterial(1.0f, 1.0f, 0.0f, 0.0f);
    osuVertex3f(0.0f, -0.30f, -0.25f);
    osuVertex3f(-0.40f, -0.30f, -0.25f);
    osuVertex3f(-0.20f, -0.50f, -0.28f);
    osuEnd();
	break;

	 default:
		 fprintf (stderr, "Please use a number from 1 to 2.\n");
         exit (-1);
  } 

    DisplayScene();
    osuFlush();
    osuWaitOnEscape();
    osuEndGraphics();

	return 0;
}






