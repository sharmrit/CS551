
/******************************************************************/
/*         Pixel operations										  */
/******************************************************************/

#include "lowlevel.h"

// Constants
#define RED 0      // Offset to red byte
#define GREEN 1    // Offset to green byte
#define BLUE 2     // Offset to blue byte

// Local data





// Pointer to image
GLubyte* canvas;

void initCanvas(int w, int h)
{
	// Get memory
	canvas = (GLubyte*)malloc(w * h * 3 * sizeof(GLubyte));
	
	// Clear it
	for ( int i = 0; i < w * h * 3; i++ )
	{
		canvas[i] = 0xFF;
	}
	width = w; 
	height = h;
}

void drawPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b)
{
	if ( (x < 0) || (x >= width) || (y < 0) || (y >= height) )
		return;
	
	canvas[3 * width * y + 3 * x + RED] = (GLubyte)(r * 255);
	canvas[3 * width * y + 3 * x + GREEN] = (GLubyte)(g * 255);
	canvas[3 * width * y + 3 * x + BLUE] = (GLubyte)(b * 255);
}

// Draw the canvas array on the screen
void flushCanvas()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glRasterPos3f(0.0, 0.0, 0.0);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, canvas);
}

