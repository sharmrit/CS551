

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "osuGraphics.h"
#include "matlib.h"
#include "lines.h"
#include <Windows.h>
#include <iostream>
using namespace std;
#include <vector>
#include "ObjLoader.h"

int xsize = 360;
int ysize = 360;


void simpleTest()
{
	osuEnable(OSU_DEPTH_TEST);
	osuPerspective(90.0, 1.0, -1000.);



	float from[3]={3.0,3.0,3.0};
	float at[3] = {0.0,0.0,-8.0};
	float up[3] = {0.0, 1.0, 0.0};

	osuLookat (from, at, up);

	osuClear(0.0,0.0,0.0);


	osuDiffuse(0.0, 0.0 , 1.0 );
	//osuSpecular(1.0, 1.0, 1.0, 1.0);

	/*float lpos[3]={0.0, 1.5, 5.0};

	osuPointLight(lpos,0.9);*/

	float dir[3] = {-2.0, -0.0, -3.0};


	osuDirectionalLight(dir,0.7);



	osuAmbientLight(0.4);
	osuNormal3f(0.0,0.0,1.0);
	//YOU MUST CONVERT THIS TO TWO TRIANGLES!!!
	osuBegin(OSU_POLYGON);
	osuVertex3f(-4.5, -1.75, -5.5);
	osuVertex3f(-4.5, 1.75, -5.5);
	osuVertex3f(4.5, 1.75, -5.5);
	osuVertex3f(4.5, -1.75, -5.5);
	osuEnd();

	osuNormal3f(1.0,0.0,0.0);
	osuDiffuse(1.0, 0.0 , 0.0 );
	//osuSpecular(1.0, 1.0, 1.0, 1.0);

	osuColor3f(0.0,0.0,1.0);
	osuBegin(OSU_POLYGON);
	osuVertex3f(0.0, -1.75, -2.5);
	osuVertex3f(0.0, 1.75, -2.5);
	osuVertex3f(0.0, 1.75, -7.5);
	osuVertex3f(0.0, -1.75, -7.5);
	osuEnd();
	//osuPerspective(90.0, 1.0, -1000.);

	//float from[3]={3.0,7.0,3.0};
	//float at[3] = {0.0,0.0,-8.0};
	//float up[3] = {0.0, 1.0, 0.0};

	////float from[3]={3.0,7.0,3.0};
	////float at[3] = {0.0,0.0,0.0};
	////float up[3] = {0.0, 1.0, 0.0};
	//osuLookat (from, at, up);

	//osuClear(0.0,0.0,0.0);

	//osuShadeModel(OSU_SMOOTH);
	////osuShadeModel(OSU_SMOOTH);
	//osuDiffuse(0,0,1.0);
	//
	//float lpos[3]={2.0, 2.5, 1.0};

	//osuPointLight(lpos,0.5);
	//
	//osuAmbientLight(0.5);

	//osuSpecular(1.0, 1.0, 1.0, 1.0);
	//osuNormal3f(0.0,1.0,0.0);
	//////YOU MUST CONVERT THIS TO TWO TRIANGLES!!!
	//osuBegin(OSU_POLYGON);
	//osuVertex3f(-4.5, -1.75, -5.5);
	//osuVertex3f(-4.5, 1.75, -5.5);
	//osuVertex3f(4.5, 1.75, -5.5);
	//osuVertex3f(4.5, -1.75, -5.5);
	//osuEnd();


	//osuDiffuse(1.0, 0.0 , 0.0 );

	//osuBegin(OSU_POLYGON);
	//osuVertex3f(0.0, -1.75, -2.5);
	//osuVertex3f(0.0, 1.75, -2.5);
	//osuVertex3f(0.0, 1.75, -7.5);
	//osuVertex3f(0.0, -1.75, -7.5);
	//osuEnd();

}

void Cube()
{
	osuEnable(OSU_DEPTH_TEST);
	osuPerspective(40, 7.5, 100);

	float from[3]={5.0,5.0,3.0};
	float at[3] = {0.0,0.0,0.0};
	float up[3] = {0.0, 1.0, 0.0};

	osuLookat (from, at, up);

	osuClear(0.0, 0.0, 0.0);
	osuClearZ();

	osuDiffuse(0.5, 0.5 , 0.0 );
	osuSpecular(1.0, 1.0, 1.0, 4.0);

	float lpos[3]={3.0, 3.5, 3.0};
	float dir[3] = {-3.0, -1.0, -2.0};

		float dir1[3] = {2.0, -0.0, -3.0};


	osuDirectionalLight(dir1,0.7);


	osuShadeModel(OSU_FLAT);
	osuPointLight(lpos,0.5);
	osuDirectionalLight(dir,0.3);
	osuAmbientLight(0.3);
	osuNormal3f(0.0,1.0,0.0);
	//YOU MUST CONVERT THESE TO USE TRIANGLES!!!
	//back
	osuNormal3f(1.0,1.0,0.0);
	osuBegin(OSU_POLYGON);
	osuVertex3f( -1, -1, -1);
	osuVertex3f(  1, -1, -1);
	osuVertex3f(  1,  1, -1);
	osuVertex3f( -1,  1, -1);
	osuEnd();

	osuDiffuse(0.5, 0.5 , 0.0 );
	osuNormal3f(1.0,0.0,0.0);
	//right
	osuBegin(OSU_POLYGON);
	osuVertex3f(  1, -1, -1);
	osuVertex3f(  1 ,-1,  1);
	osuVertex3f ( 1 , 1,  1);
	osuVertex3f ( 1 , 1, -1);
	osuEnd();

	osuDiffuse(0.5, 0.5 , 0.0 );
	osuNormal3f(0.0,0.0,1.0);
	//front
	osuBegin(OSU_POLYGON);
	osuVertex3f( -1, -1,  1);
	osuVertex3f( -1,  1,  1);
	osuVertex3f(  1,  1,  1);
	osuVertex3f(  1, -1,  1);
	osuEnd();

	osuDiffuse(0.5, 0.5 , 0.0 );
	osuNormal3f(0.0,1.0,0.0);
	//top
	osuBegin(OSU_POLYGON);
	osuVertex3f( -1,  1, -1);
	osuVertex3f(  1,  1, -1);
	osuVertex3f(  1,  1,  1);
	osuVertex3f( -1,  1,  1);
	osuEnd();

	osuDiffuse(0.5, 0.5 , 0.0 );
	osuNormal3f(0.0,1.0,0.0);
	//bottom
	osuBegin(OSU_POLYGON);
	osuVertex3f( -1, -1, -1);
	osuVertex3f( -1, -1,  1);
	osuVertex3f(  1, -1,  1);
	osuVertex3f(  1 ,-1, -1);
	osuEnd();

	osuDiffuse(0.5, 0.5 , 0.0 );
	osuNormal3f(1.0,0.0,0.0);
	//left
	osuBegin(OSU_POLYGON);
	osuVertex3f( -1, -1, -1);
	osuVertex3f( -1,  1 ,-1);
	osuVertex3f( -1,  1,  1);
	osuVertex3f( -1, -1,  1);
	osuEnd();
	//osuPerspective(40, 7.5, 100);

	//float from[3]={5.0,5.0,5.0};
	//float at[3] = {0.0,0.0,0.0};
	//float up[3] = {0.0, 1.0, 0.0};

	////float from[3]={7.0,5.0,5.0};
	////float at[3] = {0.0,0.0,0.0};
	////float up[3] = {0.0, 1.0, 0.0};

	//osuLookat (from, at, up);

	//osuClear(0.0, 0.0, 0.0);
	//osuClearZ();

	//osuDiffuse(0.0, 0.0 , 1.0 );
	//osuSpecular(1.0, 1.0, 1.0, 1.0);

	//float lpos[3]={5.0, 3.5, 4.0};
	//float dir[3] = {0.0, -2.0, -3.0};

	//osuPointLight(lpos,0.5);
	//osuDirectionalLight(dir,0.5);

	//osuAmbientLight(0.5);

	//osuShadeModel(OSU_SMOOTH);
	////YOU MUST CONVERT THESE TO USE TRIANGLES!!!
	////back
	//osuNormal3f(0.0,1.0,0.0);
	//osuBegin(OSU_POLYGON);

	//osuVertex3f( -1, -1, -1);
	//osuVertex3f(  1, -1, -1);
	//osuVertex3f(  1,  1, -1);
	//osuVertex3f( -1,  1, -1);
	//osuEnd();

	////right
	//osuBegin(OSU_POLYGON);
	//osuVertex3f(  1, -1, -1);
	//osuVertex3f(  1 ,-1,  1);
	//osuVertex3f ( 1 , 1,  1);
	//osuVertex3f ( 1 , 1, -1);
	//osuEnd();

	////front
	//osuBegin(OSU_POLYGON);
	//osuVertex3f( -1, -1,  1);
	//osuVertex3f( -1,  1,  1);
	//osuVertex3f(  1,  1,  1);
	//osuVertex3f(  1, -1,  1);
	//osuEnd();

	////top
	//osuBegin(OSU_POLYGON);
	//osuVertex3f( -1,  1, -1);
	//osuVertex3f(  1,  1, -1);
	//osuVertex3f(  1,  1,  1);
	//osuVertex3f( -1,  1,  1);
	//osuEnd();

	////bottom
	//osuBegin(OSU_POLYGON);
	//osuVertex3f( -1, -1, -1);
	//osuVertex3f( -1, -1,  1);
	//osuVertex3f(  1, -1,  1);
	//osuVertex3f(  1 ,-1, -1);
	//osuEnd();

	////left
	//osuBegin(OSU_POLYGON);
	//osuVertex3f( -1, -1, -1);
	//osuVertex3f( -1,  1 ,-1);
	//osuVertex3f( -1,  1,  1);
	//osuVertex3f( -1, -1,  1);
	//osuEnd();
}


void loadAndDrawObj(char *fname)
{
	ObjModel data;
	ObjLoader LoaderClass;

	LoaderClass.LoadObj(fname);
	data = LoaderClass.ReturnObj();


	//For flat shading, only make one call to osuNormal in the beginning, and use the following
	// to access the faceNormal
	//
	//
	// osuNormal((data.TriangleArray[i].faceNormal[0], 
	//		 (data.TriangleArray[i].faceNormal[1], 
	//		  data.TriangleArray[i].faceNormal[2]); 
		
	for(int i = 0; i < data.NumTriangle; i++)  {						

		osuBegin(OSU_POLYGON);


		osuNormal3f(data.NormalArray[data.TriangleArray[i].Vertex[0]].X, 
				data.NormalArray[data.TriangleArray[i].Vertex[0]].Y, 
				data.NormalArray[data.TriangleArray[i].Vertex[0]].Z); 

		osuVertex3f(data.VertexArray[data.TriangleArray[i].Vertex[0]].X, 
				data.VertexArray[data.TriangleArray[i].Vertex[0]].Y, 
				data.VertexArray[data.TriangleArray[i].Vertex[0]].Z);

	
		osuNormal3f(data.NormalArray[data.TriangleArray[i].Vertex[1]].X, 
				data.NormalArray[data.TriangleArray[i].Vertex[1]].Y, 
				data.NormalArray[data.TriangleArray[i].Vertex[1]].Z); 
		osuVertex3f(data.VertexArray[data.TriangleArray[i].Vertex[1]].X, 
				data.VertexArray[data.TriangleArray[i].Vertex[1]].Y, 
				data.VertexArray[data.TriangleArray[i].Vertex[1]].Z);

		osuNormal3f(data.NormalArray[data.TriangleArray[i].Vertex[2]].X, 
				data.NormalArray[data.TriangleArray[i].Vertex[2]].Y, 
				data.NormalArray[data.TriangleArray[i].Vertex[2]].Z); 
		osuVertex3f(data.VertexArray[data.TriangleArray[i].Vertex[2]].X, 
				data.VertexArray[data.TriangleArray[i].Vertex[2]].Y, 
				data.VertexArray[data.TriangleArray[i].Vertex[2]].Z);
		osuEnd();

	}
}


void ModelTest() 
{ 
	osuEnable(OSU_DEPTH_TEST); 
	osuPerspective(90.0, 1.0, 1000);
	osuClear(0.0,0.0,0.0);	
	osuShadeModel(OSU_FLAT);
   /* float from[3]={0.0,2,-3.0};
	float at[3] = {0.0,0.0,10.0};
	float up[3] = {0.0, 1.0, 0.0};*/
	float from[3]={0.0,0,2};
	float at[3] = {0.0,0.0,0.0};
	float up[3] = {0.0, 1.0, 0.0};

	osuLookat (from, at, up);
	//Diffuse blue color
	osuDiffuse(1.0, 0.5, 0.0 );

	//Specular white color
	osuSpecular(1.0, 1.0, 1.0, 5.0);

	//float lpos[3]={4.0, 3.0, -2.0};
	float lpos[3]={204.0, 204.0, 204.0};

	osuPointLight(lpos,0.7);

	//float lpos1[3]={-1.0, 3.0, -1.0};

	//osuPointLight(lpos1,0.7);

	osuAmbientLight(0.2);

	// To test flath shading, use test.obj 
	// Look at it from behind... ie osuLookAt(0,1,-3, 0,0,10,0,1,0) 
	//loadAndDrawObj("test.obj"); 

	// To test smooth shading, use face.ws.obj 
	// Look at it from in front osuLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0) 
  
	//loadAndDrawObj("sphere.obj"); 
	loadAndDrawObj("face.ws.obj"); 
	//loadAndDrawObj("test.obj");
}


/******************************************************************************
Test out drawing routines.
******************************************************************************/
void main(int argc , char **argv)
{
  int num;

	cout<<"Test Cases:"<<endl;
	cout<<"1: Simple Test"<<endl;
    cout<<"2: Cube"<<endl;
	cout<<"3: Model"<<endl;
	cout<< "Enter the choice bewteen 1 to 3:"<<endl;
	cin>>num;
	cout<< "Selected choice:"<<num<<endl;

	if((num <0 ) || (num > 3))
	{
		fprintf(stderr, "Please call this program with a number from 1 to 3 \n");
		exit(-1);
	}


  osuBeginGraphics (xsize, ysize);

  /* inialize the matrix stack*/
  osuInitialize();
  /* select routine to execute */
  switch (num) {
    case 1:
	 simpleTest();
      break;
    case 2:
	 Cube();
      break;
    case 3:
		ModelTest();
		break;

    default:
		 fprintf (stderr, "Please use a number from 1 to 3.\n");
         exit (-1);
  } 

  osuFlush();

  printf ("Press 'escape' to exit.\n");
  osuWaitOnEscape();
  osuEndGraphics();
}