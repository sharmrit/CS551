/*

Dummy routines for matrix transformations.

These are for you to write!

*/


#include <stdio.h>
using namespace std;

#include <math.h>
#include <fstream>
#include <Windows.h>
#include <ostream>
#include <sstream>
#include <string>

#include "osuGraphics.h"
#include "lines.h"
#include <vector>
#include "icVector.h"

int WinHeight = 360;
int WinWidth  = 360;
double Near = 0;
double Far =  0;
// For lines
double xCoordinate1,xCoordinate2,yCoordinate1,yCoordinate2,zCoordinate1,zCoordinate2;

int VertexCount = 1;
int drawLine = 0;
int drawPolygon = 0;
#define M_PI 3.141915
#define  MAT_MAX  20   
typedef float Matrix[4][4];
typedef float Vector[4];
static float matrixStack [MAT_MAX] [4] [4];   
static int countranslationMatrix;
static Matrix transformationMatrix_current;            
static Matrix projectionMatrix_current;
int zBuffer = 0;
double **zBufferValues = NULL;
int OrthoPerspect = -1;
double xNormal = 0.0;
double yNormal = 0.0;
double zNormal = 0.0;
int ShadeModel = OSU_SMOOTH;
float pLight[3] = {0.0,0.0,0.0};
float pIntensity = 1.0;
float dLight[3] = {0.0,0.0,0.0};
float dIntensity = 1.0;
float Ka = 0.0;

float KdR = 0.0;
float KdG = 0.0;
float KdB = 0.0;

float KsR = 0.0;
float KsG = 0.0;
float KsB = 0.0;
float phongCoefficient =0.0;

struct VColor{
	float redColor;
	float greenColor;
	float blueColor;
};

struct osuVertex {
	float x,y,z;
	float r,g,b;
	double xNormal,yNormal,zNormal;
};

struct pLightSource{
	float pLight[3];
	float pIntensity;
};

struct dLightSource{
	float dLight[3];
	float dIntensity;
};

struct Vertex
{
	double XVal;
	double YVal;
};

VColor triColor;
VColor GetVertexColor(double x, double y, Vertex Vertex1, Vertex Vertex2, Vertex Vertex3, osuVertex v1,osuVertex v2,osuVertex v3);
double GetPixelZValue(double x, double y, Vertex Vertex1, Vertex Vertex2, Vertex Vertex3, double z1, double z2, double z3);
int CheckPointInsideTriangle(double x, double y, Vertex v1, Vertex v2, Vertex v3);

static vector<osuVertex> ListVertices;
static vector<pLightSource> pLightSourceList;
static vector<dLightSource> dLightSourceList;


int WindowWidth;
int WindowHeight;
//int IsATriangleCall = 0;


/******************************************************************************
Copy the contents of a source vertex to a destination vertex.
******************************************************************************/

void vertex_copy (osuVertex *dest, osuVertex *source)
{
	dest->x = source->x;
	dest->y = source->y;
	dest->z = source->z;

	dest->r = source->r;
	dest->g = source->g;
	dest->b = source->b;
}


/******************************************************************************
Create a new vertex that is the intersection between a plane and a line
segment between two given vertices.

Entry:
v0,v1 - two vertex endpoints of the line segment
a,b,c,d - coefficients for the plane ax + by + cz + d = 0

Exit:
vnew - the new vertex at the intersection between the line and plane
******************************************************************************/

void create_vertex (
	osuVertex *v0,
	osuVertex *v1,
	osuVertex *newv,
	float a,
	float b,
	float c,
	float d
	)
{
	float t;
	float x0,y0,z0;
	float x1,y1,z1;
	float r0,g0,b0;
	float r1,g1,diffX1;
	float dx,dy,dz;

	/* shorthands */

	x0 = v0->x;
	y0 = v0->y;
	z0 = v0->z;

	x1 = v1->x;
	y1 = v1->y;
	z1 = v1->z;

	dx = x1 - x0;
	dy = y1 - y0;
	dz = z1 - z0;

	/* find parameter t saying how far between v0 and v1 the intersection is */

	t = -1.0 * (a*x0 + b*y0 + c*z0 + d) / (a*dx + b*dy + c*dz);

	/* interpolate between values in v0 and v1 for location and color */

	newv->x = x0 + t * (x1 - x0);
	newv->y = y0 + t * (y1 - y0);
	newv->z = z0 + t * (z1 - z0);

	newv->r = v0->r + t * (v1->r - v0->r);
	newv->g = v0->g + t * (v1->g - v0->g);
	newv->b = v0->b + t * (v1->b - v0->b);
}


/******************************************************************************
Clip a polygon to a plane.

Entry:
verts   - vertices of polygon to clip
count   - number of vertices in polygon
a,b,c,d - coefficients of plane equation against which to clip:
positive side described by ax + by + cz + d > 0 are kept

Exit:
out_verts - vertices of clipped polygon
out_count - number of vertices in the clipped polygon, or 0 if the entire
polygon is on the wrong side of the clipping plane
******************************************************************************/

void poly_clip (
	osuVertex *verts,
	int count,
	osuVertex *out_verts,
	int *out_count,
	float a,
	float b,
	float c,
	float d
	)
{
	int i,ii;
	int new_count = 0;
	osuVertex *v0,*v1;
	int in0,in1;  /* are v0 or v1 in the proper half-space */

	v0 = &verts[0];
	in0 = (a * v0->x + b * v0->y + c * v0->z + d > 0);

	for (i = 0; i < count; i++) {

		v0 = &verts[i];
		v1 = &verts[(i+1) % count];
		in1 = (a * v1->x + b * v1->y + c * v1->z + d > 0);

		if (in0 && in1) {
			vertex_copy (&out_verts[new_count++], v1);
		}
		else if (!in0 && in1) {
			create_vertex (v0, v1, &out_verts[new_count++], a, b, c, d);
			vertex_copy (&out_verts[new_count++], v1);
		}
		else if (in0 && !in1) {
			create_vertex (v0, v1, &out_verts[new_count++], a, b, c, d);
		}
		else {
			/* both are not in, so we add no vertices to the clipped polygon */
		}

		in0 = in1;
	}

	*out_count = new_count;
}

void poly_clip_negative (
	osuVertex *verts,
	int count,
	osuVertex *out_verts,
	int *out_count,
	float a,
	float b,
	float c,
	float d
	)
{
	int i,ii;
	int new_count = 0;
	osuVertex *v0,*v1;
	int in0,in1;  /* are v0 or v1 in the proper half-space */

	v0 = &verts[0];
	in0 = (a * v0->x + b * v0->y + c * v0->z + d < 0);

	for (i = 0; i < count; i++) {

		v0 = &verts[i];
		v1 = &verts[(i+1) % count];
		in1 = (a * v1->x + b * v1->y + c * v1->z + d < 0);

		if (in0 && in1) {
			vertex_copy (&out_verts[new_count++], v1);
		}
		else if (!in0 && in1) {
			create_vertex (v0, v1, &out_verts[new_count++], a, b, c, d);
			vertex_copy (&out_verts[new_count++], v1);
		}
		else if (in0 && !in1) {
			create_vertex (v0, v1, &out_verts[new_count++], a, b, c, d);
		}
		else {
			/* both are not in, so we add no vertices to the clipped polygon */
		}

		in0 = in1;
	}

	*out_count = new_count;
}
void Identity( Matrix m)
{
	int i;

	for (i = 0; i <= 3; i++) {
		m [i] [0] = 0.0;
		m [i] [1] = 0.0;
		m [i] [2] = 0.0;
		m [i] [3] = 0.0;
		m [i] [i] = 1.0;
	}
}

void createCopyofmatrix (Matrix dest, Matrix source)
{
	int i,j;

	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
			dest[i][j] = source[i][j];
}

void matrixmultiplication (Matrix prod, Matrix a, Matrix b)
{
	int i,j;
	Matrix m;

	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
			m [i] [j] = a [0] [j] * b [i] [0] + a [1] [j] * b [i] [1] +
			a [2] [j] * b [i] [2] + a [3] [j] * b [i] [3];

	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
			prod [i] [j] = m [i] [j];
}

void applymatrix (Matrix m, Vector v)
{
	int j;
	Vector t;

	for (j = 0; j <= 3; j++)
		t [j] = v[0] * m[0][j] + v[1] * m[1][j] + v[2] * m[2][j] + v[3]*m[3][j];

	v[0] = t[0];
	v[1] = t[1];
	v[2] = t[2];
	v[3] = t[3];
}

// End Matrix Operations

void osuOrtho(double left, double right, double bottom, double top, double nearp,double farp)
{ 
	OrthoPerspect = 1;
	Near = nearp;
	Far =  farp;

	double translateX,translateY,translateZ;
	Near = nearp;
	Far =  farp;

	translateX = (-1)*((right+left)/(right-left));
	translateY = (-1)*((top+bottom)/(top-bottom));
	translateZ = (-1)*((farp+nearp)/(farp-nearp));
	projectionMatrix_current[0][0] = 2/(right-left);
	projectionMatrix_current[1][1] = 2/(top-bottom);
	projectionMatrix_current[2][2] = (-1)*(2/(farp-nearp));
	projectionMatrix_current[3][0] = translateX;
	projectionMatrix_current[3][1] = translateY;
	projectionMatrix_current[3][2] = translateZ;
}

void osuPerspective(double fovy, double nearp, double farp) 
{  	
	OrthoPerspect = 2;
	Near = nearp;
	Far =  farp;
	double f = 1/(tan((fovy/2)*M_PI/180));
	projectionMatrix_current[0][0] = f;
	projectionMatrix_current[1][1] = f;
	projectionMatrix_current[2][2] = (nearp + farp)/(nearp - farp);
	projectionMatrix_current[2][3] = -1;
	projectionMatrix_current[3][2] = (2*nearp*farp)/(nearp - farp);
	projectionMatrix_current[3][3] = 0;

}

void osuBegin(OSUDrawable mode)
{
	if(mode == OSU_LINES)
	{
		drawLine = 1;
	}

	if(mode == OSU_POLYGON)
	{
		drawPolygon = 1;
	}
	osuGetFramebufferSize(&WindowWidth, &WindowHeight);
}

void osuEnd()
{

	if(1 == drawPolygon)
	{
		osuVertex *vertsInitial = new osuVertex[ListVertices.size()];
		osuVertex *verts = new osuVertex[ListVertices.size()];
		double *zValues = new double[ListVertices.size()];
		for(int i = 0; i<ListVertices.size(); i++)
		{
			osuVertex currentVert = ListVertices[i];
			vertsInitial[i] =  currentVert;
			Vector currentCoordinate1 = {currentVert.x,currentVert.y,currentVert.z,1.0}; 

			applymatrix (transformationMatrix_current, currentCoordinate1);

			if( OrthoPerspect==2)
			{
				zValues[i] = 1/(currentCoordinate1[2]/currentCoordinate1[3]);
			}

			if(OrthoPerspect==1)
			{
				zValues[i] = currentCoordinate1[2]/currentCoordinate1[3];
			}
			applymatrix (projectionMatrix_current, currentCoordinate1);

			double x1,y1,z1;
			x1= currentCoordinate1[0]/currentCoordinate1[3];
			y1 = currentCoordinate1[1]/currentCoordinate1[3];
			z1 = currentCoordinate1[2]/currentCoordinate1[3];

			osuVertex addCurrentVertex;
			addCurrentVertex.x = x1;
			addCurrentVertex.y = y1;
			addCurrentVertex.z = z1;

			verts[i] = addCurrentVertex;			
		}


		// Plane x=-1
		osuVertex *outputVertexNegativeX = new osuVertex[ListVertices.size()*2];
		int* outputVertexCountNegativeX = new int;
		poly_clip (verts, ListVertices.size(),outputVertexNegativeX, outputVertexCountNegativeX,  0,  0,  1, 1 ); 

		osuVertex *outputVertexPositiveX = new osuVertex[*outputVertexCountNegativeX*2];
		int* outputVertexCountPositiveX = new int;
		poly_clip_negative (outputVertexNegativeX, *outputVertexCountNegativeX, outputVertexPositiveX, outputVertexCountPositiveX,  1,  0,  0, -1 ); 

		osuVertex *outputVertexNegativeY = new osuVertex[*outputVertexCountPositiveX*2];
		int* outputVertexCountNegativeY = new int;
		poly_clip (outputVertexPositiveX, *outputVertexCountPositiveX, outputVertexNegativeY, outputVertexCountNegativeY,  0,  1,  0, 1 ); 

		osuVertex *outputVertexPositiveY = new osuVertex[*outputVertexCountNegativeY*2];
		int* outputVertexCountPositiveY = new int;
		poly_clip_negative (outputVertexNegativeY, *outputVertexCountNegativeY, outputVertexPositiveY, outputVertexCountPositiveY,  0,  1,  0, -1 ); 

		osuVertex *outputVertexNegativeZ = new osuVertex[*outputVertexCountPositiveY*2];
		int* outputVertexCountNegativeZ = new int;
		poly_clip (outputVertexPositiveY, *outputVertexCountPositiveY, outputVertexNegativeZ, outputVertexCountNegativeZ,  0,  0,  1, 1 ); 

		osuVertex *outputVertexPositiveZ = new osuVertex[*outputVertexCountNegativeZ*2];
		int* outputVertexCountPositiveZ = new int;
		poly_clip (outputVertexNegativeZ, *outputVertexCountNegativeZ, outputVertexPositiveZ, outputVertexCountPositiveZ,  0,  0,  1, -1 ); 

		int vertexCount = ListVertices.size();
		for(int j = 0; j<vertexCount-2; j++)
		{

			osuVertex vertex1 = verts[0];
			osuVertex vertex2 = verts[j+1];
			osuVertex vertex3 = verts[j+2];

			osuVertex vertex1ForColor = ListVertices[0];
			osuVertex vertex2ForColor = ListVertices[j+1];
			osuVertex vertex3ForColor = ListVertices[j+2];

			double z1 = zValues[0];
			double z2 = zValues[j+1];
			double z3 = zValues[j+2];

			vertex1.x = vertex1.x * WindowWidth/2 + WindowWidth/2;
			vertex1.y = vertex1.y *WindowHeight/2 + WindowHeight/2;

			vertex2.x = vertex2.x *WindowWidth/2 + WindowWidth/2;
			vertex2.y = vertex2.y *WindowHeight/2 + WindowHeight/2;

			vertex3.x = vertex3.x *WindowWidth/2 + WindowWidth/2;
			vertex3.y = vertex3.y *WindowHeight/2 + WindowHeight/2;

			double xValues[3]={vertex1.x,vertex2.x,vertex3.x};
			double yValues[3]={vertex1.y,vertex2.y,vertex3.y};

			double tempx,tempy;
			for (int mm = 0 ; mm < 2; mm++)
			{
				for (int nn = 0 ; nn < 2 - mm ; nn++)
				{
					if (xValues[nn] > xValues[nn+1])
					{
						tempx = xValues[nn];
						xValues[nn] = xValues[nn+1];
						xValues[nn+1] =tempx;
					}
					if (yValues[nn] > yValues[nn+1])
					{
						tempy= yValues[nn];
						yValues[nn]= yValues[nn+1];
						yValues[nn+1] =tempy;
					}

				}
			}
			double xMin,xMax,yMin,yMax;
			xMin=xValues[0];
			xMax=xValues[2];
			yMin=yValues[0];
			yMax=yValues[2];

			Vertex vert1,vert2,vert3;

			vert1.XVal = vertex1.x;
			vert2.XVal = vertex2.x;
			vert3.XVal = vertex3.x;
			vert1.YVal = vertex1.y;
			vert2.YVal = vertex2.y;
			vert3.YVal = vertex3.y;


			for (int pp = xMin; pp < xMax; pp++)
			{
				for (int qq = yMin; qq < yMax;qq++)
				{
					int CheckCurrentPointInsideTriangle = CheckPointInsideTriangle(pp, qq, vert1, vert2, vert3);
					if( CheckCurrentPointInsideTriangle ==1) 
					{
						int DisplayPixelWithRespectToZBuffer =1;
						double zValCurrentPixel = 0;
						if(zBuffer==1)
						{
							zValCurrentPixel = GetPixelZValue(pp, qq, vert1, vert2, vert3,z1,z2,z3);
							double 	zPrev = zBufferValues[pp][qq];
							if(zValCurrentPixel < zPrev)
							{
								DisplayPixelWithRespectToZBuffer =0;
							}
						}
						if(DisplayPixelWithRespectToZBuffer ==1)
						{
							VColor colorCurrPoint = GetVertexColor(pp, qq,vert1, vert2, vert3,vertex1ForColor,vertex2ForColor, vertex3ForColor);
							int rColor = 255;
							int gColor = 255;
							int bColor = 255;

							if(ShadeModel==OSU_SMOOTH)
							{
								rColor = colorCurrPoint.redColor*255;
								gColor = colorCurrPoint.greenColor*255;
								bColor = colorCurrPoint.blueColor*255;
							}
							else if(ShadeModel==OSU_FLAT)
							{
								rColor = ListVertices[0].r*255;
								gColor = ListVertices[0].g*255;
								bColor = ListVertices[0].b*255;
							}

							if(pp<WindowWidth && qq<WindowHeight)
								osuWritePixel(pp, qq, rColor,gColor,bColor);							
							if(1 == zBuffer)
							{
								zBufferValues[pp][qq] = zValCurrentPixel;
							}
						}						
					}
				}
			}

		}
		ListVertices.clear();
		drawPolygon = 0;
	}
	drawLine = 0;
}

VColor GetVertexColor(double x, double y, Vertex Vertex1, Vertex Vertex2, Vertex Vertex3, osuVertex v1,osuVertex v2,osuVertex v3)
{

	double diffY1=0, diffX1=0, diffXY1=0, diffY2=0, diffX2=0, diffXY2=0, diffY3=0, diffX3=0, diffXY3=0;
	double alpha=0, beta=0, gamma=0;

	double arediffY123, s123;

	double area012, s012;
	double area023, s023;
	double area031, s031;

	//double diffY1, diffX1, diffXY1, diffY2, diffX2, diffXY2, diffY3, diffX3, diffXY3;
	//double alpha, beta, gamma;
	double Ledge1, Ledge2, Ledge3, Ledge01, Ledge02, Ledge03;

	Ledge1 = sqrt((Vertex3.XVal - Vertex2.XVal)*(Vertex3.XVal - Vertex2.XVal) + (Vertex3.YVal - Vertex2.YVal)*(Vertex3.YVal - Vertex2.YVal));
	Ledge2 = sqrt((Vertex1.XVal - Vertex3.XVal)*(Vertex1.XVal - Vertex3.XVal) + (Vertex1.YVal - Vertex3.YVal)*(Vertex1.YVal - Vertex3.YVal));
	Ledge3 = sqrt((Vertex2.XVal - Vertex1.XVal)*(Vertex2.XVal - Vertex1.XVal) + (Vertex2.YVal - Vertex1.YVal)*(Vertex2.YVal - Vertex1.YVal));

	Ledge01 = sqrt((x - Vertex1.XVal)*(x - Vertex1.XVal) + (y - Vertex1.YVal)*(y - Vertex1.YVal));
	Ledge02 = sqrt((x - Vertex2.XVal)*(x - Vertex2.XVal) + (y - Vertex2.YVal)*(y - Vertex2.YVal));
	Ledge03 = sqrt((x - Vertex3.XVal)*(x - Vertex3.XVal) + (y - Vertex3.YVal)*(y - Vertex3.YVal));

	s123 = (Ledge1 + Ledge2 + Ledge3)*0.5;
	arediffY123 = sqrt(s123*(s123 - Ledge1)*(s123 - Ledge2)*(s123 - Ledge3));

	s012 = (Ledge01 + Ledge02 + Ledge3)*0.5;
	area012 = sqrt(s012*(s012 - Ledge01)*(s012 - Ledge02)*(s012 - Ledge3));

	s023 = (Ledge1 + Ledge02 + Ledge03)*0.5;
	area023 = sqrt(s023*(s023 - Ledge1)*(s023 - Ledge02)*(s023 - Ledge03));

	s031 = (Ledge01 + Ledge2 + Ledge03)*0.5;
	area031 = sqrt(s031*(s031 - Ledge01)*(s031 - Ledge2)*(s031 - Ledge03));

	//alpha = area023/arediffY123;
	//beta =  area031/arediffY123;
	//gamma = area012/arediffY123;
	//s = (Ledge1 + Ledge2 + Ledge3)*0.5;
	//area = sqrt(s*(s - Ledge1)*(s - Ledge2)*(s - Ledge3));

	diffY1 = Vertex3.YVal - Vertex2.YVal;
	diffX1 = Vertex2.XVal - Vertex3.XVal;
	diffXY1 = Vertex3.XVal*Vertex2.YVal - Vertex2.XVal*Vertex3.YVal;

	diffY2 = Vertex1.YVal - Vertex3.YVal;
	diffX2 = Vertex3.XVal - Vertex1.XVal;
	diffXY2 = Vertex1.XVal*Vertex3.YVal - Vertex3.XVal*Vertex1.YVal;

	diffY3 = Vertex2.YVal - Vertex1.YVal;
	diffX3 = Vertex1.XVal - Vertex2.XVal;
	diffXY3 = Vertex2.XVal*Vertex1.YVal - Vertex1.XVal*Vertex2.YVal;

	alpha = (fabs(x * diffY1 + y * diffX1 + diffXY1)*0.5*Ledge1) / (sqrt(diffY1*diffY1 + diffX1*diffX1)*arediffY123);
	beta = (fabs(x * diffY2 + y * diffX2 + diffXY2)*0.5*Ledge2) / (sqrt(diffY2*diffY2 + diffX2*diffX2)*arediffY123);
	gamma = (fabs(x * diffY3 + y * diffX3 + diffXY3)*0.5*Ledge3) / (sqrt(diffY3*diffY3 + diffX3*diffX3)*arediffY123);

	VColor currentVertexColor;
	currentVertexColor.redColor = alpha*v1.r + beta*v2.r + gamma*v3.r;
	currentVertexColor.greenColor = alpha*v1.g + beta*v2.g + gamma*v3.g;
	currentVertexColor.blueColor = alpha*v1.b + beta*v2.b + gamma*v3.b;
	return currentVertexColor;
}

double GetPixelZValue(double x, double y, Vertex Vertex1, Vertex Vertex2, Vertex Vertex3, double z1, double z2, double z3)
{

	double diffY1=0, diffX1=0, diffXY1=0, diffY2=0, diffX2=0, diffXY2=0, diffY3=0, diffX3=0, diffXY3=0;
	double alpha=0, beta=0, gamma=0;

	double arediffY123, s123;

	double area012, s012;
	double area023, s023;
	double area031, s031;

	//double diffY1, diffX1, diffXY1, diffY2, diffX2, diffXY2, diffY3, diffX3, diffXY3;
	//double alpha, beta, gamma;
	double Ledge1, Ledge2, Ledge3, Ledge01, Ledge02, Ledge03;

	Ledge1 = sqrt((Vertex3.XVal - Vertex2.XVal)*(Vertex3.XVal - Vertex2.XVal) + (Vertex3.YVal - Vertex2.YVal)*(Vertex3.YVal - Vertex2.YVal));
	Ledge2 = sqrt((Vertex1.XVal - Vertex3.XVal)*(Vertex1.XVal - Vertex3.XVal) + (Vertex1.YVal - Vertex3.YVal)*(Vertex1.YVal - Vertex3.YVal));
	Ledge3 = sqrt((Vertex2.XVal - Vertex1.XVal)*(Vertex2.XVal - Vertex1.XVal) + (Vertex2.YVal - Vertex1.YVal)*(Vertex2.YVal - Vertex1.YVal));

	Ledge01 = sqrt((x - Vertex1.XVal)*(x - Vertex1.XVal) + (y - Vertex1.YVal)*(y - Vertex1.YVal));
	Ledge02 = sqrt((x - Vertex2.XVal)*(x - Vertex2.XVal) + (y - Vertex2.YVal)*(y - Vertex2.YVal));
	Ledge03 = sqrt((x - Vertex3.XVal)*(x - Vertex3.XVal) + (y - Vertex3.YVal)*(y - Vertex3.YVal));

	s123 = (Ledge1 + Ledge2 + Ledge3)*0.5;
	arediffY123 = sqrt(s123*(s123 - Ledge1)*(s123 - Ledge2)*(s123 - Ledge3));

	s012 = (Ledge01 + Ledge02 + Ledge3)*0.5;
	area012 = sqrt(s012*(s012 - Ledge01)*(s012 - Ledge02)*(s012 - Ledge3));

	s023 = (Ledge1 + Ledge02 + Ledge03)*0.5;
	area023 = sqrt(s023*(s023 - Ledge1)*(s023 - Ledge02)*(s023 - Ledge03));

	s031 = (Ledge01 + Ledge2 + Ledge03)*0.5;
	area031 = sqrt(s031*(s031 - Ledge01)*(s031 - Ledge2)*(s031 - Ledge03));

	//alpha = area023/arediffY123;
	//beta =  area031/arediffY123;
	//gamma = area012/arediffY123;
	//s = (Ledge1 + Ledge2 + Ledge3)*0.5;
	//area = sqrt(s*(s - Ledge1)*(s - Ledge2)*(s - Ledge3));

	diffY1 = Vertex3.YVal - Vertex2.YVal;
	diffX1 = Vertex2.XVal - Vertex3.XVal;
	diffXY1 = Vertex3.XVal*Vertex2.YVal - Vertex2.XVal*Vertex3.YVal;

	diffY2 = Vertex1.YVal - Vertex3.YVal;
	diffX2 = Vertex3.XVal - Vertex1.XVal;
	diffXY2 = Vertex1.XVal*Vertex3.YVal - Vertex3.XVal*Vertex1.YVal;

	diffY3 = Vertex2.YVal - Vertex1.YVal;
	diffX3 = Vertex1.XVal - Vertex2.XVal;
	diffXY3 = Vertex2.XVal*Vertex1.YVal - Vertex1.XVal*Vertex2.YVal;

	alpha = (fabs(x * diffY1 + y * diffX1 + diffXY1)*0.5*Ledge1) / (sqrt(diffY1*diffY1 + diffX1*diffX1)*arediffY123);
	beta = (fabs(x * diffY2 + y * diffX2 + diffXY2)*0.5*Ledge2) / (sqrt(diffY2*diffY2 + diffX2*diffX2)*arediffY123);
	gamma = (fabs(x * diffY3 + y * diffX3 + diffXY3)*0.5*Ledge3) / (sqrt(diffY3*diffY3 + diffX3*diffX3)*arediffY123);

	//VColor currentVertexColor;
	//currentVertexColor.RColor = alpha*Vertex1.Color.RColor + beta*Vertex2.Color.RColor + gamma*Vertex3.Color.RColor;
	//currentVertexColor.GColor = alpha*Vertex1.Color.GColor + beta*Vertex2.Color.GColor + gamma*Vertex3.Color.GColor;
	//currentVertexColor.BColor = alpha*Vertex1.Color.BColor + beta*Vertex2.Color.BColor + gamma*Vertex3.Color.BColor;

	double zValue = alpha*z1 + beta*z2 + gamma*z3;
	return zValue;
}

void osuColor3f(double red, double green, double blue)
{
	triColor.redColor = red;
	triColor.greenColor = green;
	triColor.blueColor = blue;
}


void osuVertex3f(double x, double y, double z)
{
	if(1 == drawLine)
	{
		int draw = 0;
		if(1 == VertexCount%2)  
		{
			xCoordinate1 = x;
			yCoordinate1 = y;
			zCoordinate1 = z;
			VertexCount++;
		}
		else if(0 == VertexCount%2)  
		{
			draw =1;
			xCoordinate2 = x;
			yCoordinate2 = y;
			zCoordinate2 = z;
			VertexCount++;
		}

		if(1 == draw) // start drawing pixel
		{

			// Apply transformation 
			Vector currentCoordinate1 = {xCoordinate1,yCoordinate1,zCoordinate1,1.0}; 
			Vector currentCoordinate2 = {xCoordinate2,yCoordinate2,zCoordinate2,1.0}; 

			applymatrix (transformationMatrix_current, currentCoordinate1);
			applymatrix (projectionMatrix_current, currentCoordinate1);

			applymatrix (transformationMatrix_current, currentCoordinate2);
			applymatrix (projectionMatrix_current, currentCoordinate2);

			double x1,x2,y1,y2,z1,z2;
		    x1 = currentCoordinate1[0]/currentCoordinate1[3];
			y1 = currentCoordinate1[1]/currentCoordinate1[3];
			z1 = currentCoordinate1[2]/currentCoordinate1[3];
			x2 = currentCoordinate2[0]/currentCoordinate2[3];
			y2 = currentCoordinate2[1]/currentCoordinate2[3];
			z2 = currentCoordinate2[2]/currentCoordinate2[3];

			int retrieveValue = near_far_clip(-1, 1, &x1, &y1, &z1, &x2, &y2, &z2);

			if(retrieveValue)
			{
				x1 = WinWidth/2 + x1* (WinWidth/2);
				x2 = WinWidth/2 + x2* (WinWidth/2);
				y1 = WinHeight/2 + y1* (WinHeight/2);
				y2 = WinHeight/2 + y2* (WinHeight/2);
				draw_line(x1,y1,x2,y2);
			}
			VertexCount = 1;
		}	
	}
	else if(1 == drawPolygon)
	{

		osuVertex currentVertex;	
		currentVertex.x = x;
		currentVertex.y = y;
		currentVertex.z = z;	

		currentVertex.xNormal = xNormal;
		currentVertex.yNormal = yNormal;
		currentVertex.zNormal = zNormal;

		currentVertex.r = triColor.redColor;
		currentVertex.g = triColor.greenColor;
		currentVertex.b = triColor.blueColor;

		float Red,Green,Blue;
		Red =   Ka*KdR;
		Green = Ka*KdG;
		Blue =  Ka*KdB;

		for(int zz = 0; zz<pLightSourceList.size(); zz++)
		{

			pLightSource currentSource = pLightSourceList[zz];
			icVector3 normal(xNormal,yNormal,zNormal);
			icVector3 position(x,y,z);
			icVector3 lightvector(currentSource.pLight[0],currentSource.pLight[1],currentSource.pLight[2]);
			icVector3 pLightvector = lightvector-position;
			normalize(normal);
			normalize(pLightvector);
			
			Red +=   currentSource.pIntensity*KdR*dot(pLightvector,normal);
			Green += currentSource.pIntensity*KdG*dot(pLightvector,normal);
			Blue +=  currentSource.pIntensity*KdB*dot(pLightvector,normal);

			
			float Ks = pow(max(dot(pLightvector,normal), 0.0), phongCoefficient);
			Red += currentSource.pIntensity*KsR*Ks;
			Green += currentSource.pIntensity*KsG*Ks;
			Blue += currentSource.pIntensity*KsB*Ks;

		}

	
		for(int zz = 0; zz<dLightSourceList.size(); zz++)
		{

			dLightSource currentSource = dLightSourceList[zz];
			icVector3 normal(xNormal,yNormal,zNormal);
			icVector3 currentSourceVector(currentSource.dLight[0],currentSource.dLight[1],currentSource.dLight[2]);
			normalize(currentSourceVector);
			normalize(normal);
			Red +=currentSource.dIntensity*KdR*dot(currentSourceVector,normal);
			Green +=currentSource.dIntensity*KdG*dot(currentSourceVector,normal);
			Blue +=currentSource.dIntensity*KdB*dot(currentSourceVector,normal);
			float Ks = pow(max(dot(currentSourceVector,normal), 0.0), phongCoefficient);
			Red += currentSource.dIntensity*KsR*Ks;
			Green += currentSource.dIntensity*KsG*Ks;
			Blue += currentSource.dIntensity*KsB*Ks;
		}


		if(Red<=0)
		{
			Red = 0.0;
		}
		if(Green<=0)
		{
			Green = 0.0;
		}
		if(Blue<=0)
		{
			Blue = 0.0;
		}
		currentVertex.r = Red;
		currentVertex.g = Green;
		currentVertex.b = Blue;


		ListVertices.push_back(currentVertex);		
	}
}

void osuInitialize() 
{ 
	countranslationMatrix = 0;
	Identity (transformationMatrix_current);
	Identity (projectionMatrix_current);
}

void osuPushMatrix() 
{ 

	int i,j;

	if (countranslationMatrix > MAT_MAX)
		printf ("Error: Stack Overflow\n");
	else {
		for (i = 0; i <= 3; i++)
			for (j = 0; j <= 3; j++)
				matrixStack [countranslationMatrix] [i] [j] = transformationMatrix_current [i] [j];
		countranslationMatrix++;
	}
}

void osuPopMatrix() 
{
	int i,j;

	if (countranslationMatrix == 0)
		printf ("Error: Stack Underflow\n");
	else {
		countranslationMatrix--;
		for (i = 0; i <= 3; i++)
			for (j = 0; j <= 3; j++)
				transformationMatrix_current [i] [j] = matrixStack [countranslationMatrix] [i] [j];
	}
}

void osuLoadIdentityMatrix()
{
	Identity (transformationMatrix_current);
}

void osuTranslate(double tx, double ty, double tz) 
{
	Matrix tMatrix;

	Identity (tMatrix);
	tMatrix[3][0] = tx;
	tMatrix[3][1] = ty;
	tMatrix[3][2] = tz;

	Matrix CurrentMat;
	createCopyofmatrix (CurrentMat,transformationMatrix_current);

	matrixmultiplication (transformationMatrix_current, CurrentMat, tMatrix );

}

void osuScale(double sx, double sy, double sz) 
{ 
	Matrix sMatrix;
	Identity (sMatrix);
	sMatrix[0][0] = sx;
	sMatrix[1][1] = sy;
	sMatrix[2][2] = sz;

	Matrix CurrentMat;
	createCopyofmatrix (CurrentMat,transformationMatrix_current);

	matrixmultiplication (transformationMatrix_current,CurrentMat, sMatrix);
}



void osuRotate(double angle, double ax, double ay, double az) 
{ 
	double ang = angle*M_PI/180;

	double s = sin( ang );
	double c = cos( ang );
	double t = 1 - c;

	icVector3 vectorN(ax,ay,az);
	normalize(vectorN);

	ax = vectorN.x;
	ay = vectorN.y;
	az = vectorN.z;

	Matrix rMatrix = {
		ax * ax * t + c     ,  ay * ax * t + az * s,  az * ax * t - ay * s,  0,
		ax * ay * t - az * s,  ay * ay * t + c     ,  az * ay * t + ax * s,  0,
		ax * az * t + ay * s,  ay * az * t - ax * s,  az * az * t + c     ,  0,
		0                   ,  0                    , 0                   ,  1
	};

	Matrix CurrentMat;
	createCopyofmatrix (CurrentMat,transformationMatrix_current);

	matrixmultiplication (transformationMatrix_current,  CurrentMat,rMatrix);

}

void osuLookat(float from[3], float at[3], float up[3])
{
	icVector3 At(at[0],at[1],at[2]);
	icVector3 Eye(from[0],from[1],from[2]);
	icVector3 Up(up[0],up[1],up[2]);

	icVector3 zaxis = At - Eye;
	normalize(zaxis);

	icVector3 xaxis = cross(Up, zaxis);
	normalize(xaxis);

	icVector3 yaxis = -cross(zaxis, xaxis);
	normalize(yaxis);

	//Matrix lMatrix = {
	//	xaxis.x ,  xaxis.y ,  xaxis.z ,  -dot(xaxis, Eye),
	//	yaxis.x ,  yaxis.y ,  yaxis.z ,  -dot(yaxis, Eye),
	//	zaxis.x ,  zaxis.y ,  zaxis.z ,  -dot(zaxis, Eye),
	//	0       ,  0       ,  0       ,   1
	//};

	Matrix lMatrix = {
		xaxis.x ,  yaxis.x ,  zaxis.x ,  0,
		xaxis.y ,  yaxis.y ,  zaxis.y ,  0,
		xaxis.z  , yaxis.z ,  zaxis.z ,  0,
		-dot(xaxis, Eye)      ,  -dot(yaxis, Eye)       ,  -dot(zaxis, Eye)       ,   1
	};

	Matrix CurrentMat;
	createCopyofmatrix (CurrentMat,transformationMatrix_current);

	matrixmultiplication (transformationMatrix_current,  CurrentMat,lMatrix);

}

void osuNormal3f(double x, double y, double z)
{
	xNormal = x;
	yNormal = y;
	zNormal = z;
}

void osuEnable(int depthTestBit)
{
	if(depthTestBit == OSU_DEPTH_TEST)
	{
		zBuffer = 1;
		zBufferValues = (double**)malloc(sizeof(double*)*WinHeight);

		for(int zz = 0; zz<WinWidth; zz++)
		{
			zBufferValues[zz] = (double*)malloc(sizeof(double)*WinWidth);
		}
		for(int i = 0; i<WinHeight; i++)
		{
			for(int j = 0; j<WinWidth; j++)
			{
				zBufferValues[i][j] = -100000;
			}
		}
	}
}

void osuClearZ()
{
	if(zBuffer==1)
	{
		for(int i = 0; i<WinHeight; i++)
		{
			for(int j = 0; j<WinWidth; j++)
			{
				zBufferValues[i][j] = -100000;
			}
		}
	}
}

void osuShadeModel(int model)
{
	if(model == OSU_FLAT)
	{
		ShadeModel = OSU_FLAT;
	}
	if(model == OSU_SMOOTH)
	{
		ShadeModel = OSU_SMOOTH;
	}
}

void osuPointLight(float pos[3],float i)
{
	pLightSource currpLight;
	currpLight.pIntensity = i;
	currpLight.pLight[0] = pos[0];
	currpLight.pLight[1] = pos[1];
	currpLight.pLight[2] = pos[2];
	pLightSourceList.push_back(currpLight);
}

void osuDirectionalLight(float dir[3], float i)
{
	dLightSource currDLight;
	currDLight.dIntensity = i;
	currDLight.dLight[0] = -dir[0];
	currDLight.dLight[1] = -dir[1];
	currDLight.dLight[2] = -dir[2];
	dLightSourceList.push_back(currDLight);
}

void osuAmbientLight(float i)
{
	Ka = i;
}

void osuDiffuse(float r, float g, float b)
{
	KdR = r;
	KdG = g;
	KdB = b;
}

void osuSpecular(float r, float g, float b, float s)
{
	KsR = r;
	KsG = g;
	KsB = b;
	phongCoefficient = s;
}
double findarea(Vertex pnt0, Vertex pnt1, Vertex tempoint)
{
	double a=sqrt((pnt0.XVal-pnt1.XVal)*(pnt0.XVal-pnt1.XVal)+(pnt0.YVal -pnt1.YVal )*(pnt0.YVal -pnt1.YVal ));
	double b=sqrt((pnt0.XVal-tempoint.XVal)*(pnt0.XVal-tempoint.XVal)+(pnt0.YVal -tempoint.YVal )*(pnt0.YVal -tempoint.YVal ));
	double c=sqrt((tempoint.XVal-pnt1.XVal)*(tempoint.XVal-pnt1.XVal)+(tempoint.YVal -pnt1.YVal )*(tempoint.YVal -pnt1.YVal ));
	double s= (a+b+c)/2;
	double area =sqrt(s*(s-a)*(s-b)*(s-c));
	return area;

}
int CheckPointInsideTriangle(double x, double y, Vertex Vertex1, Vertex Vertex2, Vertex Vertex3)
{

	
	 //   icVector3 V0(Vertex1.XVal,Vertex1.YVal,0);
		//icVector3 V1(Vertex2.XVal,Vertex2.YVal,0);
		//icVector3 V2(Vertex3.XVal,Vertex3.YVal,0);

		//icVector3 vect1= V0-V1;
		//icVector3 vect2= V2-V0;
	
		//icVector3 crossVect=cross(vect1,vect2);
		//double area=0.5*(length(crossVect));
		//area=findarea(Vertex1,Vertex2,Vertex3);


		//		
		//		icVector3 tempV(x,y,0);

		//		

		//		// for alpha
		//		icVector3 tempvect1 = V2-V1;
		//		icVector3 tempvect2 = tempV-V2;
		//		crossVect=cross(tempvect1,tempvect2);
		//		double A=0.5*(length(crossVect))/area;
		//

		//		//// for beta
		//		tempvect1 = V0-V2;
		//		tempvect2 = tempV-V0;
		//		crossVect=cross(tempvect1,tempvect2);
		//		double B=0.5*(length(crossVect))/area;


		//		// for gamma
		//		tempvect1 = V1-V0;
		//		tempvect2 = tempV-V1;
		//		crossVect=cross(tempvect1,tempvect2);
		//		double C=0.5*(length(crossVect))/area;
		//		//double C=1-A-B;
		//		int returnval = 1;
		//		if(A+B+C<0.00000000000000001)
		//		{
		//			returnval=0;
		//		}
		//		return returnval;


	double diffY1=0, diffX1=0, diffXY1=0, diffY2=0, diffX2=0, diffXY2=0, diffY3=0, diffX3=0, diffXY3=0;
	double alpha=0, beta=0, gamma=0;
	double alpha01=0, beta01=0, gamma01=0;
	double alpha10=0, beta10=0, gamma10=0;
	double slope1=0,slope2=0,slope3=0;


	diffY1 = Vertex3.YVal - Vertex2.YVal;
	diffX1 = Vertex2.XVal - Vertex3.XVal;
	diffXY1 = Vertex3.XVal*Vertex2.YVal - Vertex2.XVal*Vertex3.YVal;
	slope1 = diffY1/diffX1;

	diffY2 = Vertex1.YVal - Vertex3.YVal;
	diffX2 = Vertex3.XVal - Vertex1.XVal;
	diffXY2 = Vertex1.XVal*Vertex3.YVal - Vertex3.XVal*Vertex1.YVal;
	slope2 = diffY2/diffX2;

	diffY3 = Vertex2.YVal - Vertex1.YVal;
	diffX3 = Vertex1.XVal - Vertex2.XVal;
	diffXY3 = Vertex2.XVal*Vertex1.YVal - Vertex1.XVal*Vertex2.YVal;
	slope3 = diffY3/diffX3;

	alpha   = x * diffY1 + y * diffX1 + diffXY1;
	beta    = x * diffY2 + y * diffX2 + diffXY2;
	gamma   = x * diffY3 + y * diffX3 + diffXY3;

	alpha01 = x * diffY1 + (y+1) * diffX1 + diffXY1;
	beta01  = x * diffY2 + (y+1) * diffX2 + diffXY2;
	gamma01 = x * diffY3 + (y+1) * diffX3 + diffXY3;

	alpha10 = (x+1) * diffY1 + y * diffX1 + diffXY1;
	beta10  = (x+1) * diffY2 + y * diffX2 + diffXY2;
	gamma10 = (x+1) * diffY3 + y * diffX3 + diffXY3;

	int returnValue = 1;

	if (alpha <= 0 && beta <= 0 && gamma <= 0)
	{
		if(alpha==0 && slope1>=0)
		{
			if(alpha01>=0)
			{
				returnValue = 0;
			}
		}

		if(beta==0 && slope2>=0)
		{
			if(beta01>=0)
			{
				returnValue = 0;
			}
		}

		if(gamma==0 && slope3>=0)
		{
			if(gamma01>=0)
			{
				returnValue = 0;
			}
		}
		return returnValue;
	}
	else if (alpha >= 0 && beta >= 0 && gamma >= 0)
	{
		if(alpha==0 && slope1<=0)
		{
			if(alpha10<=0)
			{
				returnValue = 0;
			}
		}

		if(beta==0 && slope2<=0)
		{
			if(beta10<=0)
			{
				returnValue = 0;
			}
		}

		if(gamma==0 && slope3<=0)
		{
			if(gamma10<=0)
			{
				returnValue = 0;
			}
		}
		return returnValue;
	}
	else
	{
		return 0;
	}
}
