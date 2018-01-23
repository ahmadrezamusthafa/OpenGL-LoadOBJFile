
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <windows.h>
#include <math.h>
#include <stdarg.h>
#include <mmsystem.h>
#include <iostream>
#include <iomanip>
#include <fstream>
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;

#include "glm.c"
GLuint	texture[6];		
using namespace std;

#define PI 3.1416
static float tick=0;
float x=0, y=0, z=0, graus=0,graus2=0, kan=1;
int fps=0, displayList=0;

GLfloat lightPos[] = { 3.2,-0.1 ,0.1, 0.0};
GLfloat lightAmb[3] = {0.0, 0.0, 0.1};
GLfloat lightDiff[3] = {1.0, 1.0, 1.0};
GLfloat lightSpecular[3] = {0.0, 0.0, 0.0};

GLMmodel* f16;
GLuint MyDisplayList;

int tipe_obj=0;
bool enable_animasi_ganti=false;
bool enable_animasi_hujan=false;
float tmp_inc_animasi=0;

/*---------------------------------------------------------------------*/
AUX_RGBImageRec *LoadBMP(char *Filename)				
{
	FILE *File=NULL;								

	if (!Filename)									
	{
		return NULL;								
	}

	File=fopen(Filename,"r");						

	if (File)										
	{
		fclose(File);								
		return auxDIBImageLoad(Filename);				
	}

	return NULL;									
}
/*---------------------------------------------------------------------*/

void LoadGLTextures()                                    
{
	char input[200];
	char buffer[200];

	int num=0;

	std::ifstream fin("Images/textures.txt");
	fin>>num;
	for (int i=0;i<num;i++)
	{
		fin>>input;
		wsprintf(buffer,"Images/%s",input);

		printf("\nBaca tekstur: /%s",input);

		AUX_RGBImageRec *TextureImage[1];               

		memset(TextureImage,0,sizeof(void *)*1);        


		if (TextureImage[0]=auxDIBImageLoad(buffer))
		{
			glGenTextures(1, &texture[i]);         

			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//GL_LINEAR_MIPMAP_NEAREST
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
			printf(" <ok> \n");
		}
		if (TextureImage[0])                            
		{
			if (TextureImage[0]->data)              
			{
				free(TextureImage[0]->data);    
			}

			free(TextureImage[0]);                  
		}
	}
}


typedef struct
{
	float x;
	float y;
}point2D_t;

point2D_t p[360];
double rad,r;

typedef struct
{
	float r;
	float g;
	float b;
} color_t;

typedef struct
{	
	float v[3];
}vector2D_t;

typedef struct 
{
	float m[3][3];
} matrix2D_t;

typedef struct{
	point2D_t p,p0;
	float d,s;
} obyek_t;


typedef struct {
	float m[4][4];
} matrix3D_t;

typedef struct {
	float v[4];
} vector3D_t;

typedef struct {
	float x;
	float y;
	float z;
} point3D_t;

typedef struct {
	int NumberofVertices; 
	short int pnt[50];
	color_t col;
	float z;
} face_t;
typedef struct {
	int NumberofVertices; 
	point3D_t pnt[1600];
	vector3D_t NormalVector[8192];
	color_t col;
	int NumberofFaces; 
	face_t fc[1000];
} object3D_t;



int xmouse=0,ymouse=0;

//daftar warna---------------------------------------
color_t	pink={0.7,0.4,0.5},
	merah={1,0,0},
	biru={0,0,1},
	biru2={0,0.5,0.75},
	kuning={1,1,0}, 
	coklat={0.5,0,0},
	coklat2={0.7,0.2,0},
	coklat3={0.5,0.2,0},
	putih={1,1,1},
	violet={0.8,0.5,0.9},
	birutua={0.0,0.1,0.2},
	hijau={0,1,0},
	cyan={0,1,1},
	magenta={1,0,1},
	ungumuda={0.5,0.5,1},
	orange={1,0.5,0},
	orange2={1,0.5,0.5},
	hijau2={0,0.75,0},
	birumuda={0.2,0.8,0.8},
	abu={0.5,0.5,0.5},
	abu3={0.2,0.2,0.2},
	abu2={0.75,0.75,0.75},
	pink1={1,0,0.5},
	pink2={1,0,1},
	pink3={1,0.5,1},
	hijau3={0,0.5,0},
	mbulak={0.1,0.2,0.5},
	kuning2={0.8,0.5,0.1},
	hitam={0,0,0},
	ungu={0.3, 0.2, 0.6},
	kuningkulit={1,1,0.8},
	rambutkuning={1,0.8,0},
	rambutcoklat={0.8,0.2,0},
	pinkbibir={1,0.6,0.8},
	birubaju1={0.4,0.6,1},
	birubaju2={0,0.2,0.6},
	birubajuas={0.8,0.8,1};

matrix2D_t createIdentity2d(void) {
	matrix2D_t u;
	int i,j;
	for (i=0;i<3;i++) {
		for(j=0;j<3;j++) u.m[i][j]=0.;
		u.m[i][i]=1.;
	}
	return u;
}

matrix2D_t translationMTX(float dx,float dy)
{
	matrix2D_t trans=createIdentity2d();
	trans.m[0][2]=dx;
	trans.m[1][2]=dy;
	return trans;
}

matrix2D_t scalingMTX(float mx,float my)
{
	matrix2D_t scale=createIdentity2d();
	scale.m[0][0]=mx;
	scale.m[1][1]=my;
	return scale;
}

matrix2D_t rotationMTX(float theta)
{
	matrix2D_t rotate=createIdentity2d();
	float cs=cos(theta);
	float sn=sin(theta);
	rotate.m[0][0]=cs; rotate.m[0][1]=-sn;
	rotate.m[1][0]=sn; rotate.m[1][1]=cs;
	return rotate;
}
matrix2D_t operator * (matrix2D_t a, matrix2D_t b)
{
	matrix2D_t c;//c=a*b
	int i,j,k;
	for (i=0;i<3;i++) for (j=0;j<3;j++) {
		c.m[i][j]=0;
		for (k=0;k<3;k++) 
			c.m[i][j]+=a.m[i][k]*b.m[k][j];
	}
	return c;
}
vector2D_t operator * (matrix2D_t a, vector2D_t b)
{
	vector2D_t c;//c=a*b
	int i,j;
	for (i=0;i<3;i++) {
		c.v[i]=0;
		for (j=0;j<3;j++) 
			c.v[i]+=a.m[i][j]*b.v[j];
	}
	return c;
}

vector2D_t point2vector(point2D_t pnt,point2D_t transPoint)
{
	vector2D_t vec;
	vec.v[0]=pnt.x - transPoint.x;
	vec.v[1]=pnt.y - transPoint.y;
	vec.v[2]=1.;
	return vec;
}


point2D_t vector2point(vector2D_t vec,point2D_t transPoint)
{
	point2D_t pnt;
	pnt.x=vec.v[0] + transPoint.x;
	pnt.y=vec.v[1] + transPoint.y;
	return pnt;
}

void transform2D(point2D_t p[], matrix2D_t mat, int n,float xCentre, float yCentre){
	vector2D_t vec;
	point2D_t transPoint;
	transPoint.x = xCentre;
	transPoint.y = yCentre;
	color_t white={255,255,255};
	//drawDot(xCentre,yCentre,12,white);	//untuk menandai titik pusat transformasi
	for(int i=0;i<n;i++){
		vec=point2vector(p[i],transPoint);
		vec=mat*vec;
		p[i]=vector2point(vec,transPoint);
	}
}

void setColor(float red,float green,float blue)
{
	glColor3f(red, green, blue);	
}

void setColor(color_t col)
{
	glColor3f(col.r, col.g, col.b);	
}


void drawDot(float x,float y)
{
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

void drawDot(point2D_t p)
{
	glBegin(GL_POINTS);
	glVertex2f(p.x, p.y);
	glEnd();
}

void drawLine(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}
void drawLine(point2D_t p1,point2D_t p2)
{
	drawLine(p1.x,p1.y,p2.x,p2.y);
}

//n: number of points
void drawPolyline(point2D_t pnt[],int n)
{
	int i;
	glBegin(GL_LINE_STRIP);
	for (i=0;i<n;i++) {
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}


void fillPolygon(point2D_t pnt[],int n,color_t color)
{
	int i;
	setColor(color);
	glBegin(GL_POLYGON);
	for (i=0;i<n;i++) {
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void gradatePolygon(point2D_t pnt[],color_t col[],int num)
{
	int i;
	glBegin(GL_POLYGON);
	for (i=0;i<num;i++) {
		setColor(col[i]);
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}


//---------------menggambar poligon----------------------
void fillPolygon(point2D_t pnt[],int n)
{
	int i;
	setColor(putih);
	glBegin(GL_POLYGON);
	for (i=0;i<n;i++) {
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}
void drawPolygon(point2D_t nama[],int n)
{
	int i;
	glBegin(GL_LINE_LOOP);
	for (i=0;i<n;i++) 
	{
		glVertex2f(nama[i].x, nama[i].y);
	}
	glEnd();
}

void drawCircle0(float r,int n){
	point2D_t p[360];
	float a=6.28/n;
	for(int i=0;i<n;i++){
		p[i].x=r*(float)cos((float)i*a);
		p[i].y=r*(float)sin((float)i*a);
	}
	drawPolygon(p,n);
}

void drawCircle(point2D_t p[],point2D_t p0,float r,int n){
	float a=6.28/n;
	for(int i=0;i<n;i++){
		p[i].y=p0.x+r*cos(i*a);
		p[i].y=p0.y+r*sin(i*a);
	}
	fillPolygon(p,n);
}

matrix3D_t createIdentity(void)
{
	matrix3D_t u;
	int i,j;
	for (i=0;i<4;i++) {
		for(j=0;j<4;j++) u.m[i][j]=0.;
		u.m[i][i]=1.;
	}
	return u;
}

matrix3D_t operator * (matrix3D_t a,matrix3D_t b)
{
	matrix3D_t c;//c=a*b
	int i,j,k;
	for (i=0;i<4;i++) for (j=0;j<4;j++) {
		c.m[i][j]=0;
		for (k=0;k<4;k++) c.m[i][j]+=a.m[i][k]*b.m[k][j];
	}
	return c;
}

vector3D_t operator * (matrix3D_t a, vector3D_t b)
{
	vector3D_t c;//c=a*b
	int i,j;
	for (i=0;i<4;i++) {
		c.v[i]=0;
		for (j=0;j<4;j++) c.v[i]+=a.m[i][j]*b.v[j];
	}
	return c;
}

matrix3D_t translationMTX(float dx,float dy,float dz)
{
	matrix3D_t trans=createIdentity();
	trans.m[0][3]=dx;
	trans.m[1][3]=dy;
	trans.m[2][3]=dz;
	return trans;
}

matrix3D_t rotationXMTX(float theta)
{
	matrix3D_t rotate=createIdentity();
	float cs=cos(theta);
	float sn=sin(theta);
	rotate.m[1][1]=cs; rotate.m[1][2]=-sn;
	rotate.m[2][1]=sn; rotate.m[2][2]=cs;
	return rotate;
}

matrix3D_t rotationYMTX(float theta)
{
	matrix3D_t rotate=createIdentity();
	float cs=cos(theta);
	float sn=sin(theta);
	rotate.m[0][0]=cs; rotate.m[0][2]=sn;
	rotate.m[2][0]=-sn; rotate.m[2][2]=cs;
	return rotate;
}

matrix3D_t rotationZMTX(float theta)
{
	matrix3D_t rotate=createIdentity();
	float cs=cos(theta);
	float sn=sin(theta);
	rotate.m[0][0]=cs; rotate.m[0][1]=-sn;
	rotate.m[1][0]=sn; rotate.m[1][1]=cs;
	return rotate;
}

matrix3D_t scalingMTX(float factorx,float factory,float factorz)
{
	matrix3D_t scale=createIdentity();
	scale.m[0][0]=factorx;
	scale.m[1][1]=factory;
	scale.m[2][2]=factorz;
	return scale;
}

matrix3D_t perspectiveMTX(float eyelength)
{
	matrix3D_t perspective=createIdentity();
	perspective.m[3][2]=-1./eyelength;
	return perspective;
}

point2D_t Vector2Point2D(vector3D_t vec)
{
	point2D_t pnt;
	pnt.x=vec.v[0];
	pnt.y=vec.v[1];
	return pnt;
}

point3D_t Vector2Point3D(vector3D_t vec)
{
	point3D_t pnt;
	pnt.x=vec.v[0];
	pnt.y=vec.v[1];
	pnt.z=vec.v[2];
	return pnt;
}

vector3D_t Point2Vector(point3D_t pnt)
{
	vector3D_t vec;
	vec.v[0]=pnt.x;
	vec.v[1]=pnt.y;
	vec.v[2]=pnt.z;
	vec.v[3]=1.;
	return vec;
}

vector3D_t homogenizeVector(vector3D_t vec)
{
	int i;
	for (i=0;i<3;i++) {
		vec.v[i]/=vec.v[3];
	}
	vec.v[3]=1.;
	return vec;
}

vector3D_t unitVector(vector3D_t vec)
{
	int i;
	float vec2=0.;
	float vec1,invvec1;
	for (i=0;i<3;i++) {
		vec2+=vec.v[i]*vec.v[i];
	}
	vec1=sqrt(vec2);
	if (vec1!=0.) {
		invvec1=1./vec1;
		for (i=0;i<3;i++) {
			vec.v[i]*=invvec1;
		}
	}
	vec.v[3]=1.;
	return vec;
}


float operator * (vector3D_t a, vector3D_t b)
{
	float c;//c=a*b
	int i;
	c=0;
	for (i=0;i<3;i++) {
		c+=a.v[i]*b.v[i];
	}
	return c;
}
vector3D_t operator ^ (vector3D_t a, vector3D_t b)
{
	vector3D_t c;//c=a*b
	c.v[0]=a.v[1]*b.v[2]-a.v[2]*b.v[1];
	c.v[1]=a.v[2]*b.v[0]-a.v[0]*b.v[2];
	c.v[2]=a.v[0]*b.v[1]-a.v[1]*b.v[0];
	c.v[3]=1.;
	return c;
}

vector3D_t operator - (vector3D_t v1,vector3D_t v0)
{
	vector3D_t c;//c=v1-v0
	c.v[0]=v1.v[0]-v0.v[0];
	c.v[1]=v1.v[1]-v0.v[1];
	c.v[2]=v1.v[2]-v0.v[2];
	c.v[3]=1.;
	return c;
}

vector3D_t operator - (vector3D_t v)
{
	vector3D_t c;//c=-v
	c.v[0]=-v.v[0];
	c.v[1]=-v.v[1];
	c.v[2]=-v.v[2];
	c.v[3]=1.;
	return c;
}

vector3D_t operator * (float r, vector3D_t b)
{
	vector3D_t c;//c=r*b
	int i;
	for (i=0;i<3;i++) {
		c.v[i]=r*b.v[i];
	}
	c.v[3]=1.;
	return c;
}

vector3D_t operator * (vector3D_t b, float r)
{
	vector3D_t c;//c=r*b
	int i;
	for (i=0;i<3;i++) {
		c.v[i]=r*b.v[i];
	}
	c.v[3]=1.;
	return c;
}


float funcPositive(float x)
{
	if (0.<x) return x;
	else return 0.;
}

// x to yth power
float power(float x,float y)
{
	//ln z = y ln x        z = exp (y ln x)
	if (x==0.) return 0;
	return exp(y*log(x));
}


color_t operator + (color_t c1, color_t c2)
{
	color_t col;
	col.r=c1.r+c2.r;
	col.g=c1.g+c2.g;
	col.b=c1.b+c2.b;
	return col;
}

color_t operator * (float r, color_t c)
{
	color_t col;
	col.r=r*c.r;
	col.g=r*c.g;
	col.b=r*c.b;
	return col;
}

color_t operator * (color_t c, float r)
{
	color_t col;
	col.r=r*c.r;
	col.g=r*c.g;
	col.b=r*c.b;
	return col;
}

color_t PhongModel(vector3D_t Light,vector3D_t Normal,vector3D_t View,color_t col)
{
	float kspe=0.7; 
	float kdif=0.6; 
	float kamb=0.4; 
	float tmp,NL,RV;
	color_t ColWhite={1,1,1};
	vector3D_t ReflectionVector=(2.*(Light*Normal)*Normal)-Light;
	tmp=Normal*Light;
	NL=funcPositive(tmp);
	tmp=ReflectionVector*View;
	RV=funcPositive(tmp);
	return kdif*NL*col+kspe*power(RV,4)*ColWhite+kamb*col;
}

void draw3Dc(object3D_t obyek,matrix3D_t mat, color_t col){
	vector3D_t vec[1600], vecbuff[50];
	vector3D_t vecNormal;
	vector3D_t lightVector={0,0,1,1};
	vector3D_t viewVector={0,0,1,1};
	color_t colbuff;
	point2D_t p[500];
	int i,j;
	for(i=0;i<obyek.NumberofVertices;i++){
		vec[i]=Point2Vector(obyek.pnt[i]);
		vec[i]=mat*vec[i];
	}
	for(i=0;i<obyek.NumberofFaces;i++){
		for(j=0;j<obyek.fc[i].NumberofVertices;j++)
			vecbuff[j]=vec[obyek.fc[i].pnt[j]];
		vecNormal=(vecbuff[1]-vecbuff[0])^(vecbuff[2]-vecbuff[0]);
		if(vecNormal.v[2]<0){
			for(j=0;j<obyek.fc[i].NumberofVertices;j++){
				p[j]=Vector2Point2D(vecbuff[j]);
			}
			vecNormal=unitVector(vecNormal);
			colbuff=PhongModel(lightVector,vecNormal,viewVector,col);
			fillPolygon(p,obyek.fc[i].NumberofVertices,colbuff);
		}
		for(i=0;i<obyek.NumberofFaces;i++){
			for(j=0;j<obyek.fc[i].NumberofVertices;j++)
				vecbuff[j]=vec[obyek.fc[i].pnt[j]];
			vecNormal=(vecbuff[1]-vecbuff[0])^(vecbuff[2]-vecbuff[0]);
			if(vecNormal.v[2]>=0){
				for(j=0;j<obyek.fc[i].NumberofVertices;j++){
					p[j]=Vector2Point2D(vecbuff[j]);
				}
				vecNormal=unitVector(vecNormal);
				colbuff=PhongModel(lightVector,vecNormal,viewVector,col);
				fillPolygon(p,obyek.fc[i].NumberofVertices,colbuff);
			}
		}
	}					
}

void draw3D(object3D_t obyek,matrix3D_t mat){
	vector3D_t vec[1600], vecbuff[50];
	vector3D_t vecNormal;
	point2D_t p[50];
	int i,j;
	for(i=0;i<obyek.NumberofVertices;i++){
		vec[i]=Point2Vector(obyek.pnt[i]);
		vec[i]=mat*vec[i];
	}
	/*	setColor(1,0,0);
	for(i=0;i<obyek.NumberofFaces;i++){
	for(j=0;j<obyek.fc[i].NumberofVertices;j++)
	vecbuff[j]=vec[obyek.fc[i].pnt[j]];
	vecNormal=(vecbuff[1]-vecbuff[0])^(vecbuff[2]-vecbuff[0]);
	if(vecNormal.v[2]<0){
	for(j=0;j<obyek.fc[i].NumberofVertices;j++){
	p[j]=Vector2Point2D(vecbuff[j]);
	}
	drawPolygon(p,obyek.fc[i].NumberofVertices);
	}
	}
	*/
	setColor(1,1,1);
	for(i=0;i<obyek.NumberofFaces;i++){
		for(j=0;j<obyek.fc[i].NumberofVertices;j++)
			vecbuff[j]=vec[obyek.fc[i].pnt[j]];
		vecNormal=(vecbuff[1]-vecbuff[0])^(vecbuff[2]-vecbuff[0]);
		if(vecNormal.v[2]>=0){
			for(j=0;j<obyek.fc[i].NumberofVertices;j++){
				p[j]=Vector2Point2D(vecbuff[j]);
			}
			drawPolygon(p,obyek.fc[i].NumberofVertices);
		}
	}
}

//------------------output bitmap------------------------
void bitmap_output(int x,int y,char*string,void*font)
{
	int len,i;
	glColor3f(1,1,1);
	glRasterPos2f(x,y);
	len=(int)strlen(string);
	for(i=0;i<len;i++)
	{
		glutBitmapCharacter(font,string[i]);
	}
}

void keyboard(unsigned char tecla, int xx, int yy) {
	glutPostRedisplay();
	switch(tecla) {
	case 'a' : x-=0.01; break; 
	case 'd' : x+=0.01; break; 
	case 's' : z+=0.01; break; 
	case 'w' : z-=0.01; break; 
		//case '.' : y-=0.01; break; 
		//case ',' : y+=0.01; break; 
		//case 'k' : graus-=0.5; kan=0; break; 
		//case 'i' : graus+=0.5; kan=0; break; 
		//case 'j' : graus2-=0.5; kan=1; break; 
		//case 'l' : graus2+=0.5; kan=1; break; 
		//case 'h': tick+=0.01; break;
		//case 'k': tick-=0.01; break;
	case 27  : exit(0);
	}
}

void changeSize(int w, int h) {
	if(h == 0) h = 1;
	float ratio = 1.0* w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(25, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
}


void createCylinder(object3D_t &silinder, int n, float r, float h){ 
	float a=6.28/n; 
	int i; 
	for(i=0;i<n;i++){ 
		silinder.pnt[i].x=r*cos(i*a); 
		silinder.pnt[i].y=-200; 
		silinder.pnt[i].z=r*sin(i*a); 
		silinder.pnt[n+i].x=r*cos(i*a); 
		silinder.pnt[n+i].y=0; 
		silinder.pnt[n+i].z=r*sin(i*a); 
	} 
	silinder.NumberofVertices=2*n; 
	for(i=0;i<n;i++){ 
		silinder.fc[i].NumberofVertices=4; 
		silinder.fc[i].pnt[0]=i; 
		silinder.fc[i].pnt[1]=n+i; 
		silinder.fc[i].pnt[2]=n+i+1; 
		silinder.fc[i].pnt[3]=i+1; 
		if(i==(n-1)){ 
			silinder.fc[i].pnt[2]=n; 
			silinder.fc[i].pnt[3]=0; 
		} 
	} 
	silinder.fc[n].NumberofVertices=n; 
	for(i=0;i<n;i++) silinder.fc[n].pnt[i]=i; 
	silinder.fc[n+1].NumberofVertices=n; 
	for(i=0;i<n;i++) silinder.fc[n+1].pnt[i]=2*n-1-i; 
	silinder.NumberofFaces=n+2; 
}

void createCone(object3D_t &kerucut, int n, float r, float h){
	float a=6.28/n;
	int i;
	kerucut.pnt[0].x=0;
	kerucut.pnt[0].y=h;
	kerucut.pnt[0].z=0;
	for(i=1;i<=n;i++){
		kerucut.pnt[i].x=r*cos(i*a);
		kerucut.pnt[i].y=0;
		kerucut.pnt[i].z=r*sin(i*a);
	}
	for(i=0;i<n;i++){
		kerucut.fc[i].NumberofVertices=3;
		kerucut.fc[i].pnt[0]=0;
		kerucut.fc[i].pnt[1]=i+2;
		kerucut.fc[i].pnt[2]=i+1;
		if(i==(n-1)) kerucut.fc[i].pnt[1]=1;
	}
	kerucut.fc[n].NumberofVertices=n;
	for(i=0;i<n;i++) kerucut.fc[n].pnt[i]=i+1;
	kerucut.NumberofVertices=n+1;
	kerucut.NumberofFaces=n+1;
}

void GradatePolygon (point2D_t pnt[],int n, color_t color [])
{ 
	int i;
	glBegin(GL_POLYGON); 
	for (i=0;i<n;i++) 
	{ 
		setColor (color[i]);
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

int interpolateInteger(int p1,int p2,float s1,float s2)
{
	if (s1<0.) s1=0.;
	if (s2<0.) s2=0.;
	if (s1+s2<1.) return p1;
	return (int)((p1*s2+p2*s1)/(s1+s2)+.5);
}

float interpolateFloat(float p1,float p2,float s1,float s2)
{
	if (s1<0.) s1=0.;
	if (s2<0.) s2=0.;
	if (s1+s2<1.) return p1;
	return (p1*s2+p2*s1)/(s1+s2);
}

color_t interpolateColor(color_t c1,color_t c2,float s1,float s2)
{
	color_t ret;
	if (s1<0.) s1=0.;
	if (s2<0.) s2=0.;
	if (s1+s2<1.) {
		ret=c1;
	} else {
		ret.r=(1./(s1+s2))*(c1.r*s2+c2.r*s1);
		ret.g=(1./(s1+s2))*(c1.g*s2+c2.g*s1);
		ret.b=(1./(s1+s2))*(c1.b*s2+c2.b*s1);
	}
	return ret;
}

void fillPolygonZBuff(point3D_t pnt[],int num,color_t color,float zbuffer[640][480])
{
	int top,bottom;
	int topleft,topright,bottomleft,bottomright;
	int ymax,ymin,xleft,xright;
	float zleft,zright,z;
	int i,y,x;
	top=0,bottom=0;
	for(i=0;i<num;i++) {
		if (pnt[top].y<pnt[i].y) top=i;
		if (pnt[i].y<pnt[bottom].y) bottom=i;
	}
	ymax=(int)(pnt[top].y+0.5);
	ymin=(int)(pnt[bottom].y+0.5);
	bottomleft=bottomright=bottom;
	topleft=(bottomleft-1+num)%num;
	topright=(bottomright+1)%num;
	setColor(color);
	for (y=ymin;y<=ymax;y++) {
		if ((int)(pnt[topleft].y+0.5)<y) {
			bottomleft=topleft;
			topleft=(bottomleft-1+num)%num;
		}
		if ((int)(pnt[topright].y+0.5)<y) {
			bottomright=topright;
			topright=(bottomright+1)%num;
		}
		xleft=interpolateInteger(pnt[topleft].x,pnt[bottomleft].x,pnt[topleft].y-y,y-pnt[bottomleft].y);
		xright=interpolateInteger(pnt[topright].x,pnt[bottomright].x,pnt[topright].y-y,y-pnt[bottomright].y);
		zleft=interpolateFloat(pnt[topleft].z,pnt[bottomleft].z,pnt[topleft].y-y,y-pnt[bottomleft].y);
		zright=interpolateFloat(pnt[topright].z,pnt[bottomright].z,pnt[topright].y-y,y-pnt[bottomright].y);
		for (x=xleft;x<=xright;x++) {
			z=interpolateFloat(zright,zleft,xright-x,x-xleft);
			if ((z>zbuffer[x+320][y+240])&&(-320<=x)&&(x<320)&&(-240<=y)&&(y<240)) {
				drawDot(x,y);
				zbuffer[x+320][y+240]=z;
			}
		}
	}
}

void gradatePolygonZBuff(point3D_t pnt[],color_t col[],int num,float zbuffer[640][480])
{
	int top,bottom;
	int topleft,topright,bottomleft,bottomright;
	int ymax,ymin,xleft,xright;
	float zleft,zright,z;
	color_t colorleft,colorright,color;
	int i,y,x;
	top=0,bottom=0;
	for(i=0;i<num;i++) {
		if (pnt[top].y<pnt[i].y) top=i;
		if (pnt[i].y<pnt[bottom].y) bottom=i;
	}
	ymax=(int)(pnt[top].y+0.5);
	ymin=(int)(pnt[bottom].y+0.5);
	bottomleft=bottomright=bottom;
	topleft=(bottomleft-1+num)%num;
	topright=(bottomright+1)%num;
	for (y=ymin;y<=ymax;y++) {
		if ((int)(pnt[topleft].y+0.5)<y) {
			bottomleft=topleft;
			topleft=(bottomleft-1+num)%num;
		}
		if ((int)(pnt[topright].y+0.5)<y) {
			bottomright=topright;
			topright=(bottomright+1)%num;
		}
		xleft=interpolateInteger(pnt[topleft].x,pnt[bottomleft].x,pnt[topleft].y-y,y-pnt[bottomleft].y);
		xright=interpolateInteger(pnt[topright].x,pnt[bottomright].x,pnt[topright].y-y,y-pnt[bottomright].y);
		zleft=interpolateFloat(pnt[topleft].z,pnt[bottomleft].z,pnt[topleft].y-y,y-pnt[bottomleft].y);
		zright=interpolateFloat(pnt[topright].z,pnt[bottomright].z,pnt[topright].y-y,y-pnt[bottomright].y);
		colorleft=interpolateColor(col[topleft],col[bottomleft],pnt[topleft].y-y,y-pnt[bottomleft].y);
		colorright=interpolateColor(col[topright],col[bottomright],pnt[topright].y-y,y-pnt[bottomright].y);
		for (x=xleft;x<=xright;x++) {
			z=interpolateFloat(zright,zleft,xright-x,x-xleft);
			if ((z>zbuffer[x+320][y+240])&&(-320<=x)&&(x<320)&&(-240<=y)&&(y<240)) {
				color=interpolateColor(colorleft,colorright,x-xleft,xright-x);
				setColor(color);
				drawDot(x,y);
				zbuffer[x+320][y+240]=z;
			}
		}
	}
}

static void makeSpherePointData(float r,object3D_t &sphere)
{
	// r : radius of the sphere
	float latitude;
	float longitude;
	int NumLatEdge=18;
	int NumLngEdge=32;
	int i,j,k;
	vector3D_t unitz={0,0,1,1};
	vector3D_t unityn={0,1,0,1};
	vector3D_t unitys={0,-1,0,1};
	matrix3D_t mat;

	sphere.NumberofVertices=1+(NumLatEdge-1)*NumLngEdge+1;

	sphere.NormalVector[0]=unityn;
	sphere.pnt[0]=Vector2Point3D(r*sphere.NormalVector[0]);
	for(i=1;i<NumLatEdge;i++) {
		latitude=(NumLatEdge/2.-i)/(float)NumLatEdge*M_PI;
		mat=rotationXMTX(-latitude);
		for(j=0;j<NumLngEdge;j++) {
			k=NumLngEdge*(i-1)+j+1;
			longitude=(float)j/NumLngEdge*2.*M_PI;
			sphere.NormalVector[k]=rotationYMTX(longitude)*mat*unitz;
			sphere.pnt[k]=Vector2Point3D(r*sphere.NormalVector[k]);
		}
	}
	k=(NumLatEdge-1)*NumLngEdge+1;
	sphere.NormalVector[k]=unitys;
	sphere.pnt[k]=Vector2Point3D(r*sphere.NormalVector[k]);

	sphere.NumberofFaces=NumLatEdge*NumLngEdge;

	for(i=0;i<NumLatEdge;i++) {
		if (i==0) {
			for(j=0;j<NumLngEdge;j++) {
				k=j;
				sphere.fc[k].NumberofVertices=3;
				sphere.fc[k].pnt[0]=0;
				sphere.fc[k].pnt[1]=k+1;
				if (j!=NumLngEdge-1) {
					sphere.fc[k].pnt[2]=k+2;
				} else {
					sphere.fc[k].pnt[2]=1;
				}
			}
		} else if (i<NumLatEdge-1) {
			for(j=0;j<NumLngEdge;j++) {
				k=NumLngEdge*i+j;
				sphere.fc[k].NumberofVertices=4;
				sphere.fc[k].pnt[0]=k+1-NumLngEdge;
				sphere.fc[k].pnt[1]=k+1;
				if (j!=NumLngEdge-1) {
					sphere.fc[k].pnt[2]=k+2;
					sphere.fc[k].pnt[3]=k+2-NumLngEdge;
				} else {
					sphere.fc[k].pnt[2]=NumLngEdge*i+1;
					sphere.fc[k].pnt[3]=NumLngEdge*i+1-NumLngEdge;
				}
			}
		} else {
			for(j=0;j<NumLngEdge;j++) {
				k=NumLngEdge*i+j;
				sphere.fc[k].NumberofVertices=3;
				sphere.fc[k].pnt[0]=k+1-NumLngEdge;
				sphere.fc[k].pnt[1]=(NumLatEdge-1)*NumLngEdge+1;
				if (j!=NumLngEdge-1) {
					sphere.fc[k].pnt[2]=k+2-NumLngEdge;
				} else {
					sphere.fc[k].pnt[2]=NumLngEdge*i+1-NumLngEdge;
				}
			}
		}
	}
}

static void makeTorusPointData(float r0,float r1,object3D_t &torus)
{
	// r1 : radius of the ring
	// r0 : radius of the circle
	float circleangle;
	float ringangle;
	int NumCclEdge=20;
	int NumRngEdge=40;
	int i,j,k;
	vector3D_t unitx={1,0,0,1};
	vector3D_t vec;
	matrix3D_t mat;

	torus.NumberofVertices=NumCclEdge*NumRngEdge;

	mat=translationMTX(r1,0,0)*scalingMTX(r0,r0,r0);
	for(j=0;j<NumCclEdge;j++) {
		circleangle=j/(float)NumCclEdge*2.*M_PI;
		torus.NormalVector[j]=rotationZMTX(circleangle)*unitx;
		torus.pnt[j]=Vector2Point3D(mat*torus.NormalVector[j]);
	}
	for(i=1;i<NumRngEdge;i++) {
		ringangle=i/(float)NumRngEdge*2.*M_PI;
		mat=rotationYMTX(ringangle);
		for(j=0;j<NumCclEdge;j++) {
			k=NumCclEdge*i+j;
			torus.NormalVector[k]=mat*torus.NormalVector[j];
			vec=Point2Vector(torus.pnt[j]);
			torus.pnt[k]=Vector2Point3D(mat*vec);
		}
	}

	torus.NumberofFaces=NumCclEdge*NumRngEdge;

	for(i=0;i<NumRngEdge;i++) {
		if (i<NumRngEdge-1) {
			for(j=0;j<NumCclEdge;j++) {
				k=NumCclEdge*i+j;
				torus.fc[k].NumberofVertices=4;
				torus.fc[k].pnt[0]=k;
				torus.fc[k].pnt[1]=k+NumCclEdge;
				if (j!=NumCclEdge-1) {
					torus.fc[k].pnt[2]=k+NumCclEdge+1;
					torus.fc[k].pnt[3]=k+1;
				} else {
					torus.fc[k].pnt[2]=NumCclEdge*i+NumCclEdge;
					torus.fc[k].pnt[3]=NumCclEdge*i;
				}
			}
		} else {
			for(j=0;j<NumCclEdge;j++) {
				k=NumCclEdge*i+j;
				torus.fc[k].NumberofVertices=4;
				torus.fc[k].pnt[0]=k;
				torus.fc[k].pnt[1]=j;
				if (j!=NumCclEdge-1) {
					torus.fc[k].pnt[2]=j+1;
					torus.fc[k].pnt[3]=k+1;
				} else {
					torus.fc[k].pnt[2]=0;
					torus.fc[k].pnt[3]=NumCclEdge*i;
				}
			}
		}
	}
}


void draw3Dz(object3D_t obyek,matrix3D_t mat,color_t col){
	vector3D_t vec[2096],vb[180];
	point2D_t p[180];
	vector3D_t NormalVector;
	vector3D_t light={0,0,1,1};
	vector3D_t view={0,0,1,1};
	color_t cyan={0,1,1};
	color_t biru={0,0,1};
	color_t fillcol;
	int i,j;
	for(i=0;i<obyek.NumberofVertices;i++)
	{
		vec[i]=Point2Vector(obyek.pnt[i]);
		vec[i]=mat*vec[i];
	}

	// hitung rata-rata z per-face
	float zr;
	for(i=0;i<obyek.NumberofFaces;i++)
	{
		zr=0;
		for(j=0;j<obyek.fc[i].NumberofVertices;j++){
			vb[j]=vec[obyek.fc[i].pnt[j]];
			zr=zr+vb[j].v[2];
		}
		zr=zr/obyek.fc[i].NumberofVertices;
		obyek.fc[i].z=zr;
	}

	// mengurutkan nomor obyek
	int n=obyek.NumberofFaces;
	face_t temp;
	for(i=0;i<n-1;i++)
		for(j=0;j<n-1;j++){
			if(obyek.fc[j].z>obyek.fc[j+1].z){
				temp=obyek.fc[j];
				obyek.fc[j]=obyek.fc[j+1];
				obyek.fc[j+1]=temp;
			}
		}

		for(i=0;i<obyek.NumberofFaces;i++)
		{
			for(j=0;j<obyek.fc[i].NumberofVertices;j++)
			{
				vb[j]=vec[obyek.fc[i].pnt[j]];
				p[j]=Vector2Point2D(vb[j]);
			}
			NormalVector=(vb[1]-vb[0])^(vb[2]-vb[0]);
			if(NormalVector.v[2]>0.)
			{
				NormalVector=unitVector(NormalVector);
				fillcol=PhongModel(light,NormalVector,view,col);
				fillPolygon(p,obyek.fc[i].NumberofVertices,fillcol);
			}
			else{
				NormalVector=unitVector(NormalVector);
				fillcol=PhongModel(light,NormalVector,view,col);
				fillPolygon(p,obyek.fc[i].NumberofVertices,fillcol);
			}
		}
}
static void buatKerucut(object3D_t &kerucut,int n, float r, float h){
	float a=6.28/n;
	kerucut.NumberofVertices=n+1;
	int i;
	kerucut.pnt[0].x=0;
	kerucut.pnt[0].z=0;
	kerucut.pnt[0].y=h;
	for(i=0;i<n;i++){
		kerucut.pnt[i+1].x=r*(float)cos(i*a);
		kerucut.pnt[i+1].z=r*(float)sin(i*a);
		kerucut.pnt[i+1].y=0;
	}
	kerucut.NumberofFaces=n;
	for(i=0;i<n-1;i++){
		kerucut.fc[i].NumberofVertices=3;
		kerucut.fc[i].pnt[0]=0;
		kerucut.fc[i].pnt[1]=i+2;
		kerucut.fc[i].pnt[2]=i+1;
	}
	kerucut.fc[n-1].NumberofVertices=3;
	kerucut.fc[n-1].pnt[0]=0;
	kerucut.fc[n-1].pnt[1]=1;
	kerucut.fc[n-1].pnt[2]=n;
}

static void buattrapesium(object3D_t &kerucut,int n, float r, float h){
	float a=6.28/n;
	kerucut.NumberofVertices=n+1;
	int i;
	kerucut.pnt[0].x=0;
	kerucut.pnt[0].z=0;
	kerucut.pnt[0].y=h;
	for(i=0;i<n;i++){
		kerucut.pnt[i+1].x=r*(float)cos(i*a);
		kerucut.pnt[i+1].z=r*(float)sin(i*a);
		kerucut.pnt[i+1].y=0;
	}
	kerucut.NumberofFaces=n;
	for(i=0;i<n-1;i++){
		kerucut.fc[i].NumberofVertices=3;
		kerucut.fc[i].pnt[0]=0;
		kerucut.fc[i].pnt[1]=i+2;
		kerucut.fc[i].pnt[2]=i+1;
	}
	kerucut.fc[n-1].NumberofVertices=3;
	kerucut.fc[n-1].pnt[0]=0;
	kerucut.fc[n-1].pnt[1]=1;
	kerucut.fc[n-1].pnt[2]=n;
}



static void buatSilinderN(object3D_t &silinder,int m,int n,float r[],float h[],color_t col){
	float a=6.28/n;
	int i,j;
	silinder.col=col;
	silinder.NumberofVertices=m*n;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			silinder.pnt[i*n+j].x=r[i]*(float)cos(j*a);
			silinder.pnt[i*n+j].z=r[i]*(float)sin(j*a);
			silinder.pnt[i*n+j].y=h[i];
		}
		//cout << r[i] << endl;
	}
	silinder.NumberofFaces=(m-1)*n;
	for(i=0;i<m-1;i++){
		for(j=0;j<n-1;j++){
			silinder.fc[i*n+j].NumberofVertices=4;
			silinder.fc[i*n+j].pnt[0]=i*n+j;
			silinder.fc[i*n+j].pnt[1]=(i+1)*n+j;
			silinder.fc[i*n+j].pnt[2]=(i+1)*n+j+1;
			silinder.fc[i*n+j].pnt[3]=i*n+j+1;
		}
		silinder.fc[(i+1)*n-1].NumberofVertices=4;
		silinder.fc[(i+1)*n-1].pnt[0]=(i+1)*n-1;
		silinder.fc[(i+1)*n-1].pnt[1]=(i+2)*n-1;
		silinder.fc[(i+1)*n-1].pnt[2]=(i+1)*n;
		silinder.fc[(i+1)*n-1].pnt[3]=i*n;
	}
}

static void buatSilinderLN(object3D_t &silinder,int m,int n,float r[],float h[]){
	float a=6.28/n;
	int i,j,ni =n/2;
	silinder.NumberofVertices=m*n;
	for(i=0;i<m;i++){
		for(j=0;j<ni;j++){
			silinder.pnt[i*ni+j].x=r[i]*(float)cos(j*a);
			silinder.pnt[i*ni+j].z=r[i]*(float)sin(j*a);
			silinder.pnt[i*ni+j].y=h[i];
		}
		//cout << r[i] << endl;
	}
	silinder.NumberofFaces=(m-1)*n;
	for(i=0;i<m-1;i++){
		for(j=0;j<ni-1;j++){
			silinder.fc[i*ni+j].NumberofVertices=4;
			silinder.fc[i*ni+j].pnt[0]=i*ni+j;
			silinder.fc[i*ni+j].pnt[1]=(i+1)*ni+j;
			silinder.fc[i*ni+j].pnt[2]=(i+1)*ni+j+1;
			silinder.fc[i*ni+j].pnt[3]=i*ni+j+1;
		}
		silinder.fc[(i+1)*ni-1].NumberofVertices=4;
		silinder.fc[(i+1)*ni-1].pnt[0]=(i+1)*ni-1;
		silinder.fc[(i+1)*ni-1].pnt[1]=(i+2)*ni-1;
		silinder.fc[(i+1)*ni-1].pnt[2]=(i+1)*ni;
		silinder.fc[(i+1)*ni-1].pnt[3]=i*ni;
	}
}

static void buatSilinder(object3D_t &silinder,int n, float r, float h){
	float a=6.28/n;
	silinder.NumberofVertices=2*n;
	int i;
	for(i=0;i<n;i++){
		// bawah
		silinder.pnt[i].x=r*(float)cos(i*a);
		silinder.pnt[i].z=r*(float)sin(i*a);
		silinder.pnt[i].y=-200;
		// atas
		silinder.pnt[n+i].x=r*(float)cos(i*a);
		silinder.pnt[n+i].z=r*(float)sin(i*a);
		silinder.pnt[n+i].y=h;
	}
	silinder.NumberofFaces=n;
	for(i=0;i<n-1;i++){
		silinder.fc[i].NumberofVertices=4;
		silinder.fc[i].pnt[0]=i;
		silinder.fc[i].pnt[1]=n+i;
		silinder.fc[i].pnt[2]=n+i+1;
		silinder.fc[i].pnt[3]=i+1;
	}
	silinder.fc[n-1].NumberofVertices=4;
	silinder.fc[n-1].pnt[0]=n-1;
	silinder.fc[n-1].pnt[1]=2*n-1;
	silinder.fc[n-1].pnt[2]=n;
	silinder.fc[n-1].pnt[3]=0;
}

static void buatSilinderL(object3D_t &silinder,int n, float r, float h){
	float a=6.28/n;

	int i, ni = n/2;

	silinder.NumberofVertices=n;
	for(i=0;i<ni;i++){
		// bawah
		silinder.pnt[i].x=r*(float)cos(i*a);
		silinder.pnt[i].z=r*(float)sin(i*a);
		silinder.pnt[i].y=0;
		// atas
		silinder.pnt[ni+i].x=r*(float)cos(i*a);
		silinder.pnt[ni+i].z=r*(float)sin(i*a);
		silinder.pnt[ni+i].y=h;
	}
	silinder.NumberofFaces=ni;
	for(i=0;i<ni-1;i++){
		silinder.fc[i].NumberofVertices=4;
		silinder.fc[i].pnt[0]=i;
		silinder.fc[i].pnt[1]=ni+i;
		silinder.fc[i].pnt[2]=ni+i+1;
		silinder.fc[i].pnt[3]=i+1;
	}

	silinder.fc[ni-1].NumberofVertices=4;
	silinder.fc[ni-1].pnt[0]=ni-1;
	silinder.fc[ni-1].pnt[1]=n-1;
	silinder.fc[ni-1].pnt[2]=ni;
	silinder.fc[ni-1].pnt[3]=0;

}


float zrata2(object3D_t obyek,matrix3D_t mat){
	float z=0;
	vector3D_t vec;
	for(int i=0;i<obyek.NumberofVertices;i++){
		vec=Point2Vector(obyek.pnt[i]);
		vec=mat*vec;
		z=z+vec.v[2];
	}
	z=z/obyek.NumberofVertices;
	return z;
}

static void buatKubah(object3D_t &bola,float r, int n, int m,int m1,int m2)
{
	float d=6.28/n,rl,h;
	int i,j;

	for(j=0;j<m1;j++)
	{
		rl=r*sin(j*3.14/(m-1));
		h=r*cos(j*3.14/(m-1))*m2;

		for(i=0;i<n;i++) 
		{
			bola.pnt[i+j*n].x=rl*cos(i*d);
			bola.pnt[i+j*n].z=rl*sin(i*d);
			bola.pnt[i+j*n].y=h;
		}
	}

	bola.NumberofVertices=m*n;
	for(j=0;j<m-1;j++)
	{
		for(i=0;i<n-1;i++) 
		{
			bola.fc[i+j*n].NumberofVertices=4;
			bola.fc[i+j*n].pnt[0]=j*n+i;
			bola.fc[i+j*n].pnt[1]=(j+1)*n+i;
			bola.fc[i+j*n].pnt[2]=(j+1)*n+i+1;
			bola.fc[i+j*n].pnt[3]=j*n+i+1;
		}
		bola.fc[(j+1)*n-1].NumberofVertices=4;
		bola.fc[(j+1)*n-1].pnt[0]=(j+1)*n-1;
		bola.fc[(j+1)*n-1].pnt[1]=(j+2)*n-1;
		bola.fc[(j+1)*n-1].pnt[2]=(j+1)*n;
		bola.fc[(j+1)*n-1].pnt[3]=j*n;
	}
	bola.NumberofFaces=(m-1)*n;
}
static void buatLKubah(object3D_t &bola,float r, int n, int m,int m1,int m2)
{
	float d=3.14/n,rl,h;
	int i,j;

	for(j=0;j<m1;j++)
	{
		rl=r*sin(j*3.14/(m-1));
		h=r*cos(j*3.14/(m-1))*m2;

		for(i=0;i<n;i++) 
		{
			bola.pnt[i+j*n].x=rl*cos(i*d);
			bola.pnt[i+j*n].z=rl*sin(i*d);
			bola.pnt[i+j*n].y=h;
		}
	}

	bola.NumberofVertices=m*n;
	for(j=0;j<m-1;j++)
	{
		for(i=0;i<n-1;i++) 
		{
			bola.fc[i+j*n].NumberofVertices=4;
			bola.fc[i+j*n].pnt[0]=j*n+i;
			bola.fc[i+j*n].pnt[1]=(j+1)*n+i;
			bola.fc[i+j*n].pnt[2]=(j+1)*n+i+1;
			bola.fc[i+j*n].pnt[3]=j*n+i+1;
		}
		bola.fc[(j+1)*n-1].NumberofVertices=4;
		bola.fc[(j+1)*n-1].pnt[0]=(j+1)*n-1;
		bola.fc[(j+1)*n-1].pnt[1]=(j+2)*n-1;
		bola.fc[(j+1)*n-1].pnt[2]=(j+1)*n;
		bola.fc[(j+1)*n-1].pnt[3]=j*n;
	}
	bola.NumberofFaces=(m-1)*n;
}

float findZ(object3D_t obyek,matrix3D_t mat)
{
	float z=0;
	int i;
	vector3D_t vec;

	for(i=0;i<obyek.NumberofVertices;i++)
	{
		vec=Point2Vector(obyek.pnt[i]);
		vec=mat*vec;
		z+=vec.v[2];
	}
	return z/obyek.NumberofVertices;
}

static void buatSilinderN2(object3D_t &silinder,int m,int n,float r[],float h[],float sbx[], float sbz[],color_t col){
	float a=6.28/n;
	int i,j;
	silinder.col=col;
	silinder.NumberofVertices=m*n;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			silinder.pnt[i*n+j].x=r[i]*(float)cos(j*a)+sbx[i];
			silinder.pnt[i*n+j].z=r[i]*(float)sin(j*a)+sbz[i];
			silinder.pnt[i*n+j].y=h[i];
		}
		//cout << r[i] << endl;
	}
	silinder.NumberofFaces=(m-1)*n;
	for(i=0;i<m-1;i++){
		for(j=0;j<n-1;j++){
			silinder.fc[i*n+j].NumberofVertices=4;
			silinder.fc[i*n+j].pnt[0]=i*n+j;
			silinder.fc[i*n+j].pnt[1]=(i+1)*n+j;
			silinder.fc[i*n+j].pnt[2]=(i+1)*n+j+1;
			silinder.fc[i*n+j].pnt[3]=i*n+j+1;
		}
		silinder.fc[(i+1)*n-1].NumberofVertices=4;
		silinder.fc[(i+1)*n-1].pnt[0]=(i+1)*n-1;
		silinder.fc[(i+1)*n-1].pnt[1]=(i+2)*n-1;
		silinder.fc[(i+1)*n-1].pnt[2]=(i+1)*n;
		silinder.fc[(i+1)*n-1].pnt[3]=i*n;
	}
}
static void buatBalokN(object3D_t &balok,point2D_t r[][4],float h[], int m,int geserZ,color_t col){

	int i,j, n = 4;
	balok.col=col;
	balok.NumberofVertices=m*n;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			balok.pnt[i*n+j].x=r[i][j].x;
			balok.pnt[i*n+j].z=r[i][j].y;
			if (j>1){
				if(i==0)
					balok.pnt[i*n+j].y=h[i]+geserZ;
				else
					balok.pnt[i*n+j].y=h[i]-geserZ;
			}
			else
				balok.pnt[i*n+j].y=h[i];
		}
		//cout << r[i] << endl;
	}
	balok.NumberofFaces=6;
	for(i=0;i<m-1;i++){
		for(j=0;j<n-1;j++){
			balok.fc[i*n+j].NumberofVertices=4;
			balok.fc[i*n+j].pnt[0]=i*n+j;
			balok.fc[i*n+j].pnt[1]=(i+1)*n+j;
			balok.fc[i*n+j].pnt[2]=(i+1)*n+j+1;
			balok.fc[i*n+j].pnt[3]=i*n+j+1;
		}
		balok.fc[(i+1)*n-1].NumberofVertices=4;
		balok.fc[(i+1)*n-1].pnt[0]=(i+1)*n-1;
		balok.fc[(i+1)*n-1].pnt[1]=(i+2)*n-1;
		balok.fc[(i+1)*n-1].pnt[2]=(i+1)*n;
		balok.fc[(i+1)*n-1].pnt[3]=i*n;
	}

	balok.fc[4].NumberofVertices=4;
	balok.fc[4].pnt[0]=5;
	balok.fc[4].pnt[1]=4;
	balok.fc[4].pnt[2]=7;
	balok.fc[4].pnt[3]=6;

	balok.fc[5].NumberofVertices=4;
	balok.fc[5].pnt[0]=0;
	balok.fc[5].pnt[1]=1;
	balok.fc[5].pnt[2]=2;
	balok.fc[5].pnt[3]=3;
}
void langit(){

	static float tick=0,tick2=0.0005;

	point2D_t langit[]={{0,0},{0,600},{1000,600},{1000,0}};
	//color_t warnalgt[]={{1,1,1},{0.18-tick,0.608-tick,0.961-tick},{0.18-tick,0.608-tick,0.961-tick},{1,1,1}};
	color_t warnalgt[]={{0.18,0.608,0.91},{0.3,0.3,0.3},{1,1,1},{1,1,1}};
	//color_t warnalgt[]={{0.2,0.2,0.2},{0,0,0},{0.18,0.608,0.91},{0.18,0.608,0.91}};
	GradatePolygon(langit,4,warnalgt);	

	tick+=tick2;
	if(tick>1||tick<=0)
		tick2=-tick2; 

}

void kotak(){

	matrix3D_t tilting=translationMTX(0.7,0.65,0.5)*rotationXMTX(0.1)*rotationYMTX(tick);
	matrix3D_t tilting2=translationMTX(0.7,-0.35,0.5)*rotationXMTX(0.1);
	color_t abu2={0.2,0.5,0.75};
	color_t abu={1,1,1};
	object3D_t kucurut,silin; 
	buatKerucut(kucurut,50,0.3, -0.3);
	//	draw3Dc(kucurut,tilting,abu);
	//	draw3D(kucurut,tilting);
	buatSilinderL(silin,90, 0.005, 0.9);
	draw3Dc(silin,tilting2,abu);


}

void kott(){
	matrix3D_t tilting2=translationMTX(0.7,-0.35,-0.1)*rotationXMTX(0.1);
	color_t abu={1,1,1};
	object3D_t silin2;
	buatSilinderL(silin2,90, 0.01, 0.5);
	draw3Dc(silin2,tilting2,abu);

}

void kot(){
	object3D_t kucurut2; 
	matrix3D_t tilting0=translationMTX(0.7,0.45,0.5)*rotationXMTX(0.1)*rotationYMTX(tick)*translationMTX(0,0,0);
	buatSilinderL(kucurut2,50,0.07, 0.04);	
	color_t abu={1,1,1};
	draw3D(kucurut2,tilting0);

	object3D_t kucurut3; 
	matrix3D_t tilting1=translationMTX(0.7,0.45,0.5)*rotationXMTX(0.1)*rotationYMTX(-tick)*translationMTX(0,0.05,0);
	buatSilinderL(kucurut3,50,0.07, 0.04);	
	draw3D(kucurut3,tilting1);
}

int temp_ring=0;
void selektor_ring(){
	glPushMatrix();

	point2D_t polygon5[36] = {{765.166,403.199},{765.62,403.19202},{766.059,403.107},{766.424,402.992},{766.901,402.711},{767.238,402.403},{767.371,402.22998},{767.528,401.976},{767.617,401.77698},{767.738,401.318},{767.773,400.983},{767.762,400.548},{767.668,400.10498},{767.505,399.68402},{767.254,399.26898},{767.013,399.026},{766.622,398.759},{766.247,398.57098},{765.885,398.462},{765.475,398.401},{765.081,398.419},{764.59,398.53302},{764.216,398.669},{763.81,398.93298},{763.473,399.267},{763.199,399.633},{763.044,400.021},{762.943,400.40198},{762.925,400.923},{762.993,401.288},{763.129,401.714},{763.313,402.09302},{763.528,402.38098},{763.927,402.759},{764.332,402.998},{764.756,403.137}};
	point2D_t polygon6[36] = {{764.984,382.976},{765.439,382.969},{765.877,382.884},{766.242,382.76898},{766.719,382.488},{767.056,382.18},{767.189,382.00702},{767.346,381.755},{767.435,381.55402},{767.557,381.095},{767.591,380.761},{767.58,380.325},{767.486,379.883},{767.323,379.461},{767.072,379.047},{766.831,378.80298},{766.44,378.536},{766.065,378.34802},{765.703,378.24},{765.294,378.17798},{764.899,378.19702},{764.408,378.31},{764.034,378.44702},{763.628,378.711},{763.292,379.04498},{763.018,379.41},{762.862,379.799},{762.761,380.17902},{762.744,380.7},{762.811,381.065},{762.947,381.492},{763.131,381.87},{763.346,382.15802},{763.745,382.536},{764.15,382.775},{764.575,382.914}};
	point2D_t polygon7[36] = {{775.26,393.101},{775.714,393.096},{776.154,393.009},{776.519,392.896},{776.995,392.615},{777.333,392.307},{777.465,392.133},{777.622,391.88},{777.71,391.681},{777.832,391.221},{777.866,390.887},{777.856,390.451},{777.761,390.009},{777.598,389.586},{777.348,389.173},{777.107,388.92902},{776.716,388.662},{776.34,388.474},{775.979,388.365},{775.569,388.305},{775.175,388.323},{774.683,388.436},{774.31,388.573},{773.904,388.836},{773.567,389.17102},{773.293,389.536},{773.138,389.924},{773.038,390.306},{773.019,390.825},{773.088,391.19202},{773.222,391.61798},{773.407,391.996},{773.621,392.284},{774.021,392.662},{774.427,392.901},{774.851,393.04102}};
	point2D_t polygon8[36] = {{754.439,393.469},{754.894,393.462},{755.333,393.377},{755.698,393.263},{756.174,392.98102},{756.512,392.674},{756.644,392.5},{756.801,392.247},{756.891,392.047},{757.012,391.588},{757.046,391.254},{757.035,390.818},{756.941,390.377},{756.778,389.95398},{756.527,389.53998},{756.286,389.29602},{755.895,389.029},{755.52,388.841},{755.158,388.733},{754.749,388.672},{754.354,388.69},{753.863,388.80298},{753.491,388.94},{753.083,389.20398},{752.747,389.538},{752.474,389.903},{752.318,390.292},{752.218,390.673},{752.2,391.193},{752.267,391.55902},{752.402,391.985},{752.586,392.363},{752.801,392.651},{753.201,393.029},{753.606,393.268},{754.031,393.40698}};

	matrix2D_t mat1;

	static float tick=0;

	if(temp_ring==0){
		mat1=rotationMTX(tick*3.14/180);
	}
	else if(temp_ring==1){
		mat1=translationMTX(0,-72)*rotationMTX(tick*3.14/180);
	}
	else if(temp_ring==2){
		mat1=translationMTX(0,-147)*rotationMTX(tick*3.14/180);
	}

	transform2D(polygon5,mat1,36,765,391);
	fillPolygon(polygon5,36,putih);
	transform2D(polygon6,mat1,36,765,391);
	fillPolygon(polygon6,36,putih);
	transform2D(polygon7,mat1,36,765,391);
	fillPolygon(polygon7,36,putih);
	transform2D(polygon8,mat1,36,765,391);
	fillPolygon(polygon8,36,putih);

	tick+=20;
	glPopMatrix();
}

void loading_harap(){
	//glPushMatrix();

	point2D_t polygon0[4] = {{477.0,315.0},{469.0,315.0},{469.0,324.0},{477.0,324.0}};
	point2D_t polygon1[4] = {{491.0,328.0},{483.0,328.0},{483.0,337.0},{491.0,337.0}};
	point2D_t polygon2[4] = {{491.0,315.0},{483.0,315.0},{483.0,324.0},{491.0,324.0}};
	point2D_t polygon3[4] = {{504.0,315.0},{496.0,315.0},{496.0,324.0},{504.0,324.0}};
	point2D_t polygon4[4] = {{491.0,303.0},{482.0,303.0},{482.0,312.0},{491.0,312.0}};

	matrix2D_t mat1;

	static float tick=0;

	if(temp_ring==0){
		mat1=rotationMTX(tick*3.14/180);
	}
	else if(temp_ring==1){
		mat1=translationMTX(0,-72)*rotationMTX(tick*3.14/180);
	}
	else if(temp_ring==2){
		mat1=translationMTX(0,-147)*rotationMTX(tick*3.14/180);
	}

	transform2D(polygon0,mat1,4,487,321);
	fillPolygon(polygon0,4,abu);
	transform2D(polygon1,mat1,4,487,321);
	fillPolygon(polygon1,4,abu);
	transform2D(polygon2,mat1,4,487,321);
	fillPolygon(polygon2,4,abu);
	transform2D(polygon3,mat1,4,487,321);
	fillPolygon(polygon3,4,abu);
	transform2D(polygon4,mat1,4,487,321);
	fillPolygon(polygon4,4,abu);

	tick+=2;
	//glPopMatrix();

	glColor3f(1,1,1);
}

void samping(){	
	glPushMatrix();
	gluLookAt(0.0,1.0,	5.0,	
		0.0,	1.0,	0.0,	
		0.0f,	1.0f, 0.0f); // kamar
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]); 

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0); glVertex2f(0.1f, 0.1f);
	glTexCoord2f(1.0, 0.0); glVertex2f(0.2f, 0.1f);
	glTexCoord2f(1.0, 1.0); glVertex2f(0.2f, 0.2f);
	glTexCoord2f(0.0, 1.0); glVertex2f(0.1f, 0.2f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void background(){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(40,(GLfloat)1000/(GLfloat)600,0.5f,20.0f);
	//gluPerspective(0,100,10,500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	gluLookAt(0.0, 0.4,10.2,	
		0.0,	0.0,	0.0,	
		0.0f,	1.0f, 0.0f); // kamar

	glTranslatef(0.3,0,-1);
	glEnable(GL_TEXTURE_2D);
	if(enable_animasi_hujan==true){
		glBindTexture(GL_TEXTURE_2D, texture[9]);//0 
	}
	else{
		glBindTexture(GL_TEXTURE_2D, texture[0]);
	}
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); 
	glTexCoord2f(0.0, 0.0); glVertex2f(-8.0f, -8.0f);
	glTexCoord2f(1.0, 0.0); glVertex2f(8.0f, -8.0f);
	glTexCoord2f(1.0, 1.0); glVertex2f(8.0f, 8.0f);
	glTexCoord2f(0.0, 1.0); glVertex2f(-8.0f, 8.0f);


	glEnd();
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}


void lengan_robot(){

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(40,(GLfloat)1000/(GLfloat)600,0.5f,5.0f);
	//gluPerspective(0,100,10,500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	gluLookAt(0.0, 0.4,10.2,	
		0.0,	0.0,	0.0,	
		0.0f,	1.0f, 0.0f); // kamar



	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}

void hujan(){
	// Mengacak posisi awal garis-garis hujan
	static int tick=0;
	static point2D_t hujan[200];
	int i;
	if(tick==0){
		for(i=0;i<100;i++){
			hujan[i].x=rand()%1000;
			hujan[i].y=rand()%600;
		}
	}

	// Menggerakkan posisi hujan
	for(i=0;i<100;i++){
		hujan[i].x++;
		hujan[i].y--;
		// Jika garis hujan di luar window
		// diganti dengan yang baru
		if((hujan[i].x>400) || (hujan[i].y<0)){
			hujan[i].x=rand()%1000;
			hujan[i].y=rand()%600;
		}
	}

	// Menggambar hujan
	setColor(1,1,1);
	for(i=0;i<100;i++)
		drawLine(hujan[i].x,hujan[i].y,hujan[i].x+5,hujan[i].y-5);
}

void gelembung(){
	static point2D_t gel[3];
	static int tick=0;
	static float d[3];
	int i;
	if(tick==0){
		for(i=0;i<3;i++){
			gel[i].x=rand()%1000;
			gel[i].y=10;
			d[i]=(float)(rand()%100)/100+0.5;
		}
	}

	//menjalankan gelembuung
	for(i=0;i<3;i++){
		gel[i].y+=d[i];
		if(gel[i].y>300){
			gel[i].x=rand()%1000;
			gel[i].y=10;
			d[i]=(float)(rand()%100)/100+0.5;

		}
	}

	//draw gelembung
	point2D_t p[90],p0;
	setColor(putih);
	for(i=0;i<3;i++){
		drawCircle(p,gel[i],40,90);
		drawPolygon(p,90);
		p0.x=gel[i].x+7;
		p0.y=gel[i].y+6;

		drawCircle(p,p0,50,90);

		drawPolygon(p,90);
		p0.x=gel[i].x-2;
		p0.y=gel[i].y+12;

		drawCircle(p,p0,30,90);

		drawPolygon(p,90);
	}
	tick++;
}

void rumput(){
	//mengacak posisi awal rumput
	static obyek_t rumput[500];
	static int tick=0;
	int i;
	if(tick==0){
		for(i=0;i<300;i++){
			rumput[i].p.x=rand()%1000;
			rumput[i].p.y=50+rand()%10;
			rumput[i].d=rand()%8+2;
			rumput[i].s=float(rand()%100+1)/1000;
			rumput[i].p0=rumput[i].p;
		}
	}


	//menggoyang rumput
	float xa,xb;
	for(i=0;i<300;i++){
		rumput[i].p.x+=rumput[i].s;
		xa=rumput[i].p0.x-0.7*rumput[i].d;
		xb=rumput[i].p0.x+0.5*rumput[i].d;
		if((rumput[i].p.x<xa)||(rumput[i].p.x>xb))
			rumput[i].s=-rumput[i].s;
	}

	//menggambar
	point2D_t p[3];
	color_t cp[3]={{0.1,1,0.3},{0,0.2,0},{0,0.2,0}};
	for(i=0;i<300;i++){
		p[0].x=rumput[i].p.x;
		p[0].y=rumput[i].p.y;
		p[1].x=rumput[i].p0.x-rumput[i].d;
		p[1].y=0;
		p[2].x=rumput[i].p0.x+rumput[i].d;
		p[2].y=0;
		GradatePolygon(p,3,cp);
	}
	tick++;


}

void harap_tunggu(){



	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[10]); 

	glBegin(GL_QUADS);

	glTexCoord2i(0, 0); 
	glVertex2i(453,287);
	glTexCoord2i(1, 0); 
	glVertex2i(695,287);
	glTexCoord2i(1, 1); 
	glVertex2i(695,352);
	glTexCoord2i(0, 1); 
	glVertex2i(453,352);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	loading_harap();
}

float tick_awan1=0;
void awan1(int y){
	glPushMatrix();
	glEnable(GL_BLEND);									
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture[12]); 

	int xx=1000;
	glBegin(GL_QUADS);

	glTexCoord2i(0, 0); 
	glVertex2i(306+xx+tick_awan1,y);
	glTexCoord2i(1, 0); 
	glVertex2i(600+xx+tick_awan1,y);
	glTexCoord2i(1, 1); 
	glVertex2i(600+xx+tick_awan1,y+74);
	glTexCoord2i(0, 1); 
	glVertex2i(306+xx+tick_awan1,y+74);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);	
	glPopMatrix();

	if(tick_awan1<-2000){
		tick_awan1=0;
	}
	else{
		tick_awan1-=0.2;
	}
}


float tick_awan2=0;
void awan2(int y){
	glPushMatrix();
	glEnable(GL_BLEND);									
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture[12]); 

	int xx=800;
	glBegin(GL_QUADS);

	glTexCoord2i(0, 0); 
	glVertex2i(306+xx+tick_awan2,y);
	glTexCoord2i(1, 0); 
	glVertex2i(580+xx+tick_awan2,y);
	glTexCoord2i(1, 1); 
	glVertex2i(580+xx+tick_awan2,y+74);
	glTexCoord2i(0, 1); 
	glVertex2i(306+xx+tick_awan2,y+74);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);	
	glPopMatrix();

	if(tick_awan2<-1700){
		tick_awan2=0;
	}
	else{
		tick_awan2-=0.4;
	}
}

float tick_awan3=0;
void awan3(int y){
	glPushMatrix();
	glEnable(GL_BLEND);									
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture[12]); 

	int xx=1100;
	glBegin(GL_QUADS);

	glTexCoord2i(0, 0); 
	glVertex2i(306+xx+tick_awan3,y);
	glTexCoord2i(1, 0); 
	glVertex2i(560+xx+tick_awan3,y);
	glTexCoord2i(1, 1); 
	glVertex2i(560+xx+tick_awan3,y+74);
	glTexCoord2i(0, 1); 
	glVertex2i(306+xx+tick_awan3,y+74);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);	
	glPopMatrix();

	if(tick_awan3<-1900){
		tick_awan3=0;
	}
	else{
		tick_awan3-=0.3;
	}
}


float tick_awan4=0;
void awan4(int y){
	glPushMatrix();
	glEnable(GL_BLEND);									
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture[12]); 

	int xx=1300;
	glBegin(GL_QUADS);

	glTexCoord2i(0, 0); 
	glVertex2i(306+xx+tick_awan4,y);
	glTexCoord2i(1, 0); 
	glVertex2i(900+xx+tick_awan4,y);
	glTexCoord2i(1, 1); 
	glVertex2i(900+xx+tick_awan4,y+74);
	glTexCoord2i(0, 1); 
	glVertex2i(306+xx+tick_awan4,y+74);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);	
	glPopMatrix();

	if(tick_awan4<-2100){
		tick_awan4=0;
	}
	else{
		tick_awan4-=0.35;
	}
}


float tick_awan5=0;
void awan5(int y){
	glPushMatrix();
	glEnable(GL_BLEND);									
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture[12]); 

	int xx=1700;
	glBegin(GL_QUADS);

	glTexCoord2i(0, 0); 
	glVertex2i(306+xx+tick_awan5,y);
	glTexCoord2i(1, 0); 
	glVertex2i(900+xx+tick_awan5,y);
	glTexCoord2i(1, 1); 
	glVertex2i(900+xx+tick_awan5,y+74);
	glTexCoord2i(0, 1); 
	glVertex2i(306+xx+tick_awan5,y+74);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);	
	glPopMatrix();

	if(tick_awan5<-2400){
		tick_awan5=0;
	}
	else{
		tick_awan5-=0.25;
	}
}

void tombol1(){
	bool hover=false;
	if(xmouse>10 && xmouse<190 && ymouse>360 && ymouse<410){
		//dalam
		hover=true;
		//printf("%d - %d\n",xmouse,ymouse);
	}else{
		//luar
		hover=false;
	}


	glEnable(GL_TEXTURE_2D);
	if(hover==true){
		glBindTexture(GL_TEXTURE_2D, texture[3]); 
	}else{
		glBindTexture(GL_TEXTURE_2D, texture[2]); 
	}
	glBegin(GL_QUADS);

	glTexCoord2i(0, 0); 
	glVertex2i(10,360);
	glTexCoord2i(1, 0); 
	glVertex2i(190,360);
	glTexCoord2i(1, 1); 
	glVertex2i(190,410);
	glTexCoord2i(0, 1); 
	glVertex2i(10,410);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	
}

void tombol2(){
	bool hover=false;
	if(xmouse>10 && xmouse<190 && ymouse>305 && ymouse<355){
		//dalam
		hover=true;
		//printf("%d - %d\n",xmouse,ymouse);
	}else{
		//luar
		hover=false;
	}

	glEnable(GL_TEXTURE_2D);
	if(hover==true){
		glBindTexture(GL_TEXTURE_2D, texture[5]); 
	}else{
		glBindTexture(GL_TEXTURE_2D, texture[4]);
	}
	glBegin(GL_QUADS);

	glTexCoord2i(0, 0); 
	glVertex2i(10,305);
	glTexCoord2i(1, 0); 
	glVertex2i(190,305);
	glTexCoord2i(1, 1); 
	glVertex2i(190,355);
	glTexCoord2i(0, 1); 
	glVertex2i(10,355);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}


void tombol3(){
	bool hover=false;
	if(xmouse>10 && xmouse<190 && ymouse>250 && ymouse<300){
		//dalam
		hover=true;
		//printf("%d - %d\n",xmouse,ymouse);
	}else{
		//luar
		hover=false;
	}

	glEnable(GL_TEXTURE_2D);
	if(hover==true){
		glBindTexture(GL_TEXTURE_2D, texture[7]); 
	}else{
		glBindTexture(GL_TEXTURE_2D, texture[8]);
	}
	glBegin(GL_QUADS);

	glTexCoord2i(0, 0); 
	glVertex2i(10,250);
	glTexCoord2i(1, 0); 
	glVertex2i(190,250);
	glTexCoord2i(1, 1); 
	glVertex2i(190,300);
	glTexCoord2i(0, 1); 
	glVertex2i(10,300);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void refresh_objek(){
	f16 = (GLMmodel*)malloc(sizeof(GLMmodel));
	if(tipe_obj==0){
		f16 = glmReadOBJ("1.obj");
	}
	else if(tipe_obj==1){
		f16 = glmReadOBJ("2.obj");
	}else{
		f16 = glmReadOBJ("3.obj");
	}

	displayList=glGenLists(1);
	glNewList(displayList,GL_COMPILE);
	//glmList(f16, GLM_SMOOTH);
	glmUnitize(f16);
	glmFacetNormals(f16);
	glmVertexNormals(f16, 900.0);
	glmDraw(f16, GLM_MATERIAL|GLM_SMOOTH);//GLM_WIREFRAME
	glEndList();
}

GLdouble ox=0.0,oy=0.0,oz=0.0;
void Mouse(int button,int state,int x,int y) {

	GLint viewport[4];
	GLdouble modelview[16],projection[16];
	GLfloat wx=x,wy,wz;

	if(state!=GLUT_DOWN)
		return;
	//if(button==GLUT_RIGHT_BUTTON)

	glGetIntegerv(GL_VIEWPORT,viewport);
	y=viewport[3]-y;
	wy=y;
	glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
	glGetDoublev(GL_PROJECTION_MATRIX,projection);
	glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
	gluUnProject(wx,wy,wz,modelview,projection,viewport,&ox,&oy,&oz);
	glutPostRedisplay();

	printf("CLICK %d - %d - %d\n",x,y,oz);

	if(xmouse>10 && xmouse<190 && ymouse>250 && ymouse<300){
		//3
		tipe_obj=0;
		//refresh_objek();
		enable_animasi_ganti=true;
	}else if(xmouse>10 && xmouse<190 && ymouse>305 && ymouse<355){
		//2
		tipe_obj=1;
		//refresh_objek();
		enable_animasi_ganti=true;
	}else if(xmouse>10 && xmouse<190 && ymouse>360 && ymouse<410){
		//1
		tipe_obj=3;
		//refresh_objek();
		enable_animasi_ganti=true;
	}

}

void MouseKoor(int x,int y) {	
	GLint viewport[4];
	GLdouble modelview[16],projection[16];
	GLfloat wx=x,wy,wz;


	glGetIntegerv(GL_VIEWPORT,viewport);
	y=viewport[3]-y;
	//printf("%d - %d - %d\n",x,y,oz);
	xmouse=x;
	ymouse=y;
}

void menu_awal(){
	/*
	gluLookAt(0, 0.0,	5,	
	0,	0.0,	0.0,	
	0.0f,	1.0f, 0.0f); 
	*/
	//glTranslatef(-0,-1,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]); 
	//gluOrtho2D(0.,1000.,0.,600.);
	glBegin(GL_QUADS);

	glTexCoord2i(0, 0); 
	glVertex2i(0,0);
	glTexCoord2i(1, 0); 
	glVertex2i(200,0);
	glTexCoord2i(1, 1); 
	glVertex2i(200,600);
	glTexCoord2i(0, 1); 
	glVertex2i(0,600);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	//selektor_ring();
	rumput();
	//gelembung();
	glColor3f(1,1,1);
	tombol1();
	tombol2();
	tombol3();
	//hujan();
}


int tmp_ganti_animasi=0;
void renderScene(void) {
	graus2-=0.05;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(40,(GLfloat)1000/(GLfloat)600,0.5f,20.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_LIGHT0);	
	glEnable(GL_LIGHTING);	
	glEnable(GL_COLOR_MATERIAL);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular); 

	glEnable(GL_DEPTH_TEST); 

	gluLookAt(0.0, 0.4,2.2,	
		0.0,	0.0,	0.0,	
		0.0f,	1.0f, 0.0f); 
	
	if(enable_animasi_ganti==true){
		if(tmp_ganti_animasi==0){
			glTranslatef(0,0-tmp_inc_animasi,0);
			tmp_inc_animasi+=0.03;
			enable_animasi_hujan=true;
			if(tmp_inc_animasi>6){
				//glTranslatef(0,0+tmp_inc_animasi,0);
				tmp_ganti_animasi=1;
				//tmp_inc_animasi=0;
				refresh_objek();				
			}
		}else{
			glTranslatef(0,0-tmp_inc_animasi,0);
			tmp_inc_animasi-=0.03;
			if(tmp_inc_animasi<0){
				//glTranslatef(0,0+tmp_inc_animasi,0);
				tmp_ganti_animasi=0;
				tmp_inc_animasi=0;
				enable_animasi_ganti=false;
				enable_animasi_hujan=false;
			}
		}
	}


	glTranslatef(0.3,0,-1);
	glTranslatef(x,y,z);

	if(kan==1){
		glRotatef((GLfloat)graus2, (GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0);

	}	

	//kotak();
	//kot();



	tick+=0.01;

	glCallList(displayList);


	////penyangga
	GLfloat mat_ambient[] = {1.0, 1.0, 0.0, 0.0};
	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0};
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0};
	GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0};
	GLfloat light_position[]  = {1.0, 1.7, 1.6, 0.0};
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_shininess[] = {5.0};
	GLfloat spot_direction[]= {2.0, 2.0, 2.0};	
	GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_specular);
	glEnable(GL_LIGHTING);	
	glEnable(GL_COLOR_MATERIAL);
	glTranslatef(0,-0.4,0);
	auxSolidBox(2, 0.05, 2.4); 
	glTranslatef(0,0.4,0);

	glTranslatef(0,-2,0);
	auxSolidBox(0.5, 3, 0.5); 
	glTranslatef(0,2,0);

	glTranslatef(0,-0.9,0);
	glColor3f(0.3, 0.8, 0.3);
	auxSolidBox(0.6, 0.1, 0.6); 
	glTranslatef(0,0.9,0);

	glColor3f(1,1, 1);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);	
	glDisable(GL_COLOR_MATERIAL);	


	background();
	lengan_robot();

	//2d

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 1000, 0, 600, 0, 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	awan1(500);
	awan2(440);
	awan3(400);
	awan4(560);
	awan5(530);
	//gluOrtho2D(0.,1000.,0.,600.);
	if(enable_animasi_hujan==true){
		hujan();
		harap_tunggu();
	}
	//langit();

	menu_awal();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glutSwapBuffers();

}

void FreeTexture( GLuint texture )
{
	glDeleteTextures( 1, &texture ); 
}

int temp_tex=0;
int InitGL(GLvoid)									
{
	glShadeModel(GL_SMOOTH); //GL_SMOOTH
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	
	glClearDepth(1.0f);								
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);	

	if(temp_tex==0){
		LoadGLTextures();
		temp_tex=1;
	}
	glEnable(GL_TEXTURE_2D);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	return TRUE;	

}

int main(int argc, char **argv) {
	printf("\nBismillahirrohmanirrohim..");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1000,600);

	gluOrtho2D(0.,1000.,0.,600.);
	glutCreateWindow("OBJ Loader OpenGL | 5114201019");

	InitGL();

	glutIdleFunc(renderScene);
	glutDisplayFunc(renderScene);
	glutMouseFunc(Mouse);
	glutPassiveMotionFunc(MouseKoor);
	glutKeyboardFunc(keyboard);

	f16 = (GLMmodel*)malloc(sizeof(GLMmodel));
	if(tipe_obj==0){
		f16 = glmReadOBJ("1.obj");
	}
	else if(tipe_obj==1){
		f16 = glmReadOBJ("2.obj");
	}else{
		f16 = glmReadOBJ("3.obj");
	}

	displayList=glGenLists(1);
	glNewList(displayList,GL_COMPILE);
	glmUnitize(f16);
	glmFacetNormals(f16);
	glmVertexNormals(f16, 900.0);
	glmDraw(f16, GLM_MATERIAL|GLM_SMOOTH);
	glEndList();


	glutMainLoop();

	FreeTexture(texture[0]);
	FreeTexture(texture[1]);
	FreeTexture(texture[2]);
	FreeTexture(texture[3]);
	FreeTexture(texture[4]);

	return 0;
}


