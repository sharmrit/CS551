/**
* OSU Graphics support functions
* Header File													
* 
* Author:	Turk, Metoyer
* Date:		1/9/07
*/

#ifndef OSUGFXSUPPORT
#define OSUGFXSUPPORT
#include "windows.h"
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library		
#include <glut.h>		//glut library
#include <stdio.h>

/**
*	This extern allows us to call C code from C++ code.  That means
*	students can write their renderersin an object oriented fashion 
*	if they would like while still using our support code.	
*/

#ifdef __cplusplus
extern "C" {
#endif
  

// osuImage Data Type Definition            

typedef struct {
   // an image structure holds the image size 
   // and a dynamic array of pixels 
   int w, h;
   int *values;
} osuImage;

void osuImageInit ( osuImage *I );
void osuImageDestroy ( osuImage *I );
void osuImageGetSize ( osuImage *I, int *w, int *h );
void osuImageSetSize ( osuImage *I, int w, int h );
void osuImageWritePixel ( osuImage *I, int w, int h, int r, int g, int b );
void osuImageReadPixel ( osuImage *I, int w, int h, int *r, int *g, int *b );

enum OSUWriteMode { OSU_OVERWRITE, OSU_REPLACE, OSU_XOR };
enum OSUColor { OSU_RED, OSU_GREEN, OSU_BLUE };
enum OSUEnable { OSU_DEPTH_TEST };
enum OSUDrawable { OSU_NONE, OSU_TRIANGLE, OSU_LINES, OSU_POLYGON };
enum OSUShadeModel { OSU_FLAT, OSU_SMOOTH };
enum OSUColorFilter {COLOR_FILTER_NONE,COLOR_FILTER_RED,COLOR_FILTER_GREEN,COLOR_FILTER_BLUE};

void osuBeginGraphics ( int w, int h );
void osuEndGraphics ();
void osuClear ( int r, int g, int b );
void osuFlush();
void osuWritePixel ( int x, int y, int r, int g, int b );
void osuSetWriteMode ( int /* OSUWriteMode */ mode );
void osuWaitOnEscape ();
void osuGetFramebufferSize ( int *w, int *h );

void osuColor3f ( float r, float g, float b );
void osuObjectMaterial(float ambientRef, float diffusePer, float specPer, float reflPer);
//void osuLookat(float from[3], float at[3], float up[3]);
void osuLookat();
void osuVertex3f(float, float, float);
void osuDrawSphere(float x, float y, float z, float R);
void osuVertex2f ( float x, float y );
void osuBegin ( int /* OSUDrawable */ );
void osuEnd();
void osuPointLight(float pos[3], float i);
void osuAmbientLight(float r, float g, float b);
void osuDiffuse(float r, float g, float b);
void osuSpecular(float r, float g, float b);

extern int width;
extern int height;

#ifdef __cplusplus
}
#endif  //end extern "C"


#endif  //end GFXSUPPORT

//static int width;
//static int height;