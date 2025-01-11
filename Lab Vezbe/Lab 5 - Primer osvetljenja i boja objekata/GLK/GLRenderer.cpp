#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include <vector>
#include <tuple>
//#pragma comment(lib, "GL\\glut32.lib")

using namespace std;

#define RAD(a) 3.141592*(a)/180

#define TO_RAD(x) ( x * 3.14159 / 180 )

CGLRenderer::CGLRenderer(void)
{
	InitView();
	CalculatePosition();

	InitMaterials();
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	{
		glClearColor(POZADINA, 1.0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);

		// Globalno ambijentalno osvetljenje
		GLfloat global_ambient[] = { .2, .2, .2, 1.0 };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

		ConfigureLightSources();
	}
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		gluLookAt(eyex, eyey, eyez,
			centerx, centery, centerz,
			upx, upy, upz);

		DrawAxis(100);

		ConfigureLightPositions();
		
		redMat.SetFace(GL_FRONT);
		glPushMatrix();
		{
			glTranslated(0, 0, 20);
			DrawCylinder(5, 2, 6, 60, true);
		}
		glPopMatrix();

		greenMat.SetFace(GL_FRONT);
		glPushMatrix();
		{
			glTranslated(0, 5, 7);
			DrawSphere(5, 60, 60);
		}
		glPopMatrix();

		blueMat.SetFace(GL_FRONT);
		glPushMatrix();
		{
			glTranslated(0, 0, -7);
			DrawEnvCube(10);
		}
		glPopMatrix();

		whiteMat.SetFace(GL_FRONT);
		glPushMatrix();
		{
			glTranslated(0, 0, -20);
			DrawCylinder(5, 5, 5, 60, true);
		}
		glPopMatrix();
	}
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	{
		glViewport(0, 0, w, h);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, (double)w / h, 1.0, 100.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

#pragma region Konfiguracija

void CGLRenderer::InitMaterials()
{
	double specR = 0.4, specG = 0.4, specB = 0.4;
	double shininess = 32;

	// Env cube
	envCubeMat.SetAmbient(.4, .1, .1, 1);
	envCubeMat.SetDiffuse(.8, .2, .2, 1);
	envCubeMat.SetSpecular(specR, specG, specB, 1.0);
	envCubeMat.SetShininess(shininess);

	// Red mat
	redMat.SetAmbient(.4, .1, .1, 1);
	redMat.SetDiffuse(.8, .2, .2, 1);
	redMat.SetSpecular(specR, specG, specB, 1.0);
	redMat.SetShininess(shininess);

	// Green mat
	greenMat.SetAmbient(.1, .4, .1, 1);
	greenMat.SetDiffuse(.2, .8, .2, 1);
	greenMat.SetSpecular(specR, specG, specB, 1.0);
	greenMat.SetShininess(shininess);

	// Blue mat
	blueMat.SetAmbient(.1, .1, .4, 1);
	blueMat.SetDiffuse(.2, .2, .8, 1);
	blueMat.SetSpecular(specR, specG, specB, 1.0);
	blueMat.SetShininess(shininess);

	// White mat
	whiteMat.SetAmbient(.4, .4, .4, 1);
	whiteMat.SetDiffuse(.7, .7, .7, 1);
	whiteMat.SetSpecular(specR, specG, specB, 1.0);
	whiteMat.SetShininess(shininess);
}

void CGLRenderer::InitView()
{
	viewR = 10;

	viewAngleXY = 0;
	viewAngleXZ = 0;

	eyex = 0, eyey = 0, eyez = 0;
	centerx = 0, centery = 0, centerz = 0;
	upx = 0, upy = 1, upz = 0;
}

void CGLRenderer::ConfigureLightSources()
{
	// Light 0
	GLfloat ambient0[] = { .2, .2, .2, 1 };
	GLfloat diffuse0[] = { .9, .9, .9, 1 };
	GLfloat specular0[] = { .8, .8, .8, 1 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
	light0Col = { .9, .9, .9 };

	// Light 1 - Red
	GLfloat ambient1[] = { .4, 0, 0, 1 };
	GLfloat diffuse1[] = { .9, 0, 0, 1 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular0);
	light1Col = { .9, 0, 0 };

	// Light 2 - Green
	GLfloat ambient2[] = { 0, .4, 0, 1 };
	GLfloat diffuse2[] = { 0, .9, 0, 1 };

	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular0);
	light2Col = { 0, .9, 0 };

	// Light 3 - Blue
	GLfloat ambient3[] = { 0, 0, .4, 1 };
	GLfloat diffuse3[] = { 0, 0, .9, 1 };

	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient3);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse3);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specular0);
	light3Col = { 0, 0, .9 };
}

void CGLRenderer::ConfigureLightPositions()
{
	SetLightPositionAndDirection(GL_LIGHT1, { 15, 0, 3 }, { -1, 0, 0 }, light1On, light1Col);
	SetLightPositionAndDirection(GL_LIGHT2, { 15, 0, 1 }, { -1, 0, 0 }, light2On, light2Col);
	SetLightPositionAndDirection(GL_LIGHT3, { 15, 0, -1 }, { -1, 0, 0 }, light3On, light3Col);
	SetLightPositionAndDirection(GL_LIGHT0, { 15, 0, -3 }, { -1, 0, 0 }, light0On, light0Col);
}

void CGLRenderer::SetLightPositionAndDirection(GLenum light, tuple<double, double, double> position, tuple<double, double, double> direction, bool lightOn, tuple<double, double, double> lightCol)
{
	if (lightOn) glEnable(light);
	else glDisable(light);

	double posX, posY, posZ,
		dirX, dirY, dirZ;
	tie(posX, posY, posZ) = position;
	tie(dirX, dirY, dirZ) = direction;

	GLfloat lightPos[] = { posX, posY, posZ, 1 };
	GLfloat lightDir[] = { dirX, dirY, dirZ };
	glLightfv(light, GL_POSITION, lightPos);
	//glLightfv(light, GL_SPOT_DIRECTION, lightDir);
	DrawLight(position, lightOn, lightCol);
}

#pragma endregion

#pragma region Crtanje

void CGLRenderer::DrawLight(tuple<double, double, double> position, bool lightOn, tuple<double, double, double> color, double r)
{
	if (!lightOn)
		return;

	double colR, colG, colB;
	tie(colR, colG, colB) = color;

	double posX, posY, posZ;
	tie(posX, posY, posZ) = position;

	glDisable(GL_LIGHTING);
	glColor3d(colR, colG, colB);
	glPushMatrix();
	{
		glTranslated(posX, posY, posZ);
		DrawSphere(r, 10, 10);
	}
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawNormals(vector<tuple<double, double, double, double, double, double>> normals, double scale)
{
	glDisable(GL_LIGHTING);
	if (drawNormals)
	{
		glColor3d(1, 1, 1);
		glBegin(GL_LINES);
		{
			for (const auto& normal : normals)
			{
				double vx, vy, vz, nx, ny, nz;
				tie(vx, vy, vz, nx, ny, nz) = normal;

				glVertex3d(vx, vy, vz);
				glVertex3d(vx + nx * scale, vy + ny * scale, vz + nz * scale);
			}
		}
		glEnd();
	}
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawAxis(double width)
{
	glDisable(GL_LIGHTING);
	glLineWidth(1);

	glBegin(GL_LINES);
	{
		glColor3d(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(width, 0, 0);

		glColor3d(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, width, 0);

		glColor3d(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, width);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta, bool onlyUpperHalf)
{
	int alphaStep = 180 / nSegAlpha;
	int betaStep = 360 / nSegBeta;

	// Ovaj niz cuva normale jer cemo kasnije da ih crtamo
	// Ovo radimo jer ne smemo da nestujemo glBegin unutar glBegin
	// DrawNormal se crta pomocu glBegin i to stvara problem
	vector<tuple<double, double, double, double, double, double>> normals;

	glBegin(GL_TRIANGLE_STRIP);
	for (int beta = 0; beta <= 360; beta += betaStep)
	{
		for (int alpha = onlyUpperHalf ? 0 : -90; alpha <= 90; alpha += alphaStep)
		{
			double alphaRad = TO_RAD(alpha);
			double betaRad = TO_RAD(beta);

			double nx = cos(alphaRad) * cos(betaRad);
			double ny = sin(alphaRad);
			double nz = cos(alphaRad) * sin(betaRad);
			double vx = r * nx, vy = r * ny, vz = r * nz;

			double betaNextRad = TO_RAD(beta) + TO_RAD(betaStep);
			
			double nx2 = cos(alphaRad) * cos(betaNextRad);
			double ny2 = sin(alphaRad);
			double nz2 = cos(alphaRad) * sin(betaNextRad);
			double vx2 = r * nx2, vy2 = r * ny2, vz2 = r * nz2;

			glNormal3d(nx, ny, nz);
			glVertex3d(vx, vy, vz);

			glNormal3d(nx2, ny2, nz2);
			glVertex3d(vx2, vy2, vz2);

			normals.emplace_back(vx, vy, vz, nx, ny, nz);
		}
	}
	glEnd();

	DrawNormals(normals);
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg, bool includeBase)
{
	int alphaStep = 360 / nSeg;
	vector<tuple<double, double, double, double, double, double>> normals;

	//envCubeMat.SetFace(GL_FRONT_AND_BACK);

	if (includeBase)
	{
		// Gornja Osnova
		glBegin(GL_TRIANGLE_FAN);
		{
			double vx, vy = h, vz;
			double nx = 0, ny = 1, nz = 0;
			
			glNormal3d(nx, ny, nz);

			glVertex3d(0, vy, 0);

			for (int alpha = 0; alpha <= 360; alpha += alphaStep)
			{
				double vx = r2 * cos(TO_RAD(alpha));
				double vz = r2 * sin(TO_RAD(alpha));
				glVertex3d(vx, vy, vz);

				normals.emplace_back(vx, vy, vz, nx, ny, nz);
			}
		}
		glEnd();

		// Donja Osnova
		glBegin(GL_TRIANGLE_FAN);
		{
			double vx, vy = 0, vz;
			double nx = 0, ny = -1, nz = 0;

			glNormal3d(nx, ny, nz);

			glVertex3d(0, 0, 0);

			for (int alpha = 0; alpha <= 360; alpha += alphaStep)
			{
				double vx = r1 * cos(TO_RAD(alpha));
				double vz = r1 * sin(TO_RAD(alpha));
				glVertex3d(vx, vy, vz);

				normals.emplace_back(vx, vy, vz, nx, ny, nz);
			}
		}
		glEnd();
	}

	double nr, ny;
	tie(nr, ny) = CalculateCylinderNormals(h, r1, r2);

	// Omotac
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			double alphaRad = TO_RAD(alpha);

			double nx = nr * cos(alphaRad);
			double nz = nr * sin(alphaRad);

			glNormal3d(nx, ny, nz);

			double vxDown = r1 * cos(alphaRad),
				   vyDown = 0,
				   vzDown = r1 * sin(alphaRad);

			glVertex3d(vxDown, vyDown, vzDown);
			normals.emplace_back(vxDown, vyDown, vzDown, nx, ny, nz);

			double vxUp = r2 * cos(alphaRad),
				   vyUp = h,
				   vzUp = r2 * sin(alphaRad);

			glVertex3d(vxUp, vyUp, vzUp);
			normals.emplace_back(vxUp, vyUp, vzUp, nx, ny, nz);

		}
	}
	glEnd();

	DrawNormals(normals);
}

void CGLRenderer::DrawEnvCube(double a)
{
	double aHalf = a / 2;

	// Bottom
	glPushMatrix();
	{
		glRotated(90, 1, 0, 0);
		DrawCubeFace(a);
	}
	glPopMatrix();

	// Top
	glPushMatrix();
	{
		glTranslated(0, a, 0);
		glRotated(-90, 1, 0, 0);
		DrawCubeFace(a);
	}
	glPopMatrix();

	// Front
	glPushMatrix();
	{
		glTranslated(aHalf, aHalf, 0);
		glRotated(90, 0, 1, 0);
		DrawCubeFace(a);
	}
	glPopMatrix();

	// Back
	glPushMatrix();
	{
		glTranslated(-aHalf, aHalf, 0);
		glRotated(-90, 0, 1, 0);
		DrawCubeFace(a);
	}
	glPopMatrix();

	// Left
	glPushMatrix();
	{
		glTranslated(0, aHalf, aHalf);
		DrawCubeFace(a);
	}
	glPopMatrix();

	// Right
	glPushMatrix();
	{
		glTranslated(0, aHalf, -aHalf);
		glRotated(180, 0, 1, 0);
		DrawCubeFace(a);
	}
	glPopMatrix();
}

void CGLRenderer::DrawCubeFace(double a)
{
	double aHalf = a / 2;

	vector<tuple<double, double, double, double, double, double>> normals;
	double nx = 0, ny = 0, nz = 1;

	//envCubeMat.SetFace(GL_FRONT);

	glNormal3d(nx, ny, nz);
	glBegin(GL_QUADS);
	{
		glVertex3d(aHalf, -aHalf, 0);
		normals.emplace_back(aHalf, -aHalf, 0, nx, ny, nz);

		glVertex3d(aHalf, aHalf, 0);
		normals.emplace_back(aHalf, aHalf, 0, nx, ny, nz);

		glVertex3d(-aHalf, aHalf, 0);
		normals.emplace_back(-aHalf, aHalf, 0, nx, ny, nz);

		glVertex3d(-aHalf, -aHalf, 0);
		normals.emplace_back(-aHalf, -aHalf, 0, nx, ny, nz);
	}
	glEnd();

	DrawNormals(normals);
}

tuple<double, double> CGLRenderer::CalculateCylinderNormals(double h, double r1, double r2)
{
	double r = (r1 - r2) / 2;
	double l = sqrt(pow(h, 2) + pow(r, 2));

	double nr = h / l;
	double ny = r / l;

	return { nr, ny };
}

#pragma endregion

#pragma region Rotiranje i Zumiranje Pogleda

void CGLRenderer::RotateView(double dXY, double dXZ)
{
	viewAngleXY += dXY;

	// Clamp
	viewAngleXY = min(90, viewAngleXY);
	viewAngleXY = max(-90, viewAngleXY);

	viewAngleXZ += dXZ;
	CalculatePosition();
}

void CGLRenderer::ZoomView(bool zoomIn)
{
	double zoomSensitivity = 1;

	viewR += zoomIn ? -zoomSensitivity : zoomSensitivity;

	// Clamp
	viewR = max(1, viewR);
	viewR = min(1000, viewR);
	
	CalculatePosition();
}

void CGLRenderer::CalculatePosition()
{
	double alpha = TO_RAD(viewAngleXY);
	double beta = TO_RAD(viewAngleXZ);

	eyex = viewR * cos(alpha) * cos(beta);
	eyey = viewR * sin(alpha);
	eyez = viewR * cos(alpha) * sin(beta);

	upy = signbit(cos(alpha)) ? -1 : 1;
}

#pragma endregion
