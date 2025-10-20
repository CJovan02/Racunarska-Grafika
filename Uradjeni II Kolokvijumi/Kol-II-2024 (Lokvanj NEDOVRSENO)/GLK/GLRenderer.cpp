#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
//#pragma comment(lib, "GL\\glut32.lib")

constexpr auto PI = 3.141592;
#define TO_RAD(a) ( a * 3.141592 / 180 )

CGLRenderer::CGLRenderer(void)
{
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
	//---------------------------------

	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	envTex = LoadTexture("env.jpg");

	
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, 0, 1, 0);

	DrawAxes();
	//DrawSphere(5, 30, 30);
	DrawSphFlower(10, 30, 30, rotationFactor, 'r', 'g', 'b', 'b', 'g', 'r');

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (double) w / h, 0.1, 2000);

	glMatrixMode(GL_MODELVIEW);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 

	glDeleteTextures(1, &envTex);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texID;
	DImage img;
	img.Load(CString(fileName));

	if (img.Width() == 0 || img.Height() == 0)
		AfxMessageBox(CString("Greska pri ucitavanju: ") + fileName);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

void CGLRenderer::DrawSphFlower(float R, int n, int m, float factor, unsigned char R1, unsigned char G1, unsigned char B1, unsigned char R2, unsigned char G2, unsigned char B2)
{
	double alphaStep = (double) PI / m;
	double betaStep = (double) 2.0 * PI / n;

	glColor3f(1, 0, 1);
	glDisable(GL_TEXTURE_2D);
	for (int i = 0; i < n; i++) {
		double beta = betaStep * i;
		double betaNext = beta + betaStep;

		// Računanje ose rotacije između dve linije (meridijana)
		double x1, y1, z1, x2, y2, z2;
		PolarToCartesian(R, 0, betaNext, x1, y1, z1);
		PolarToCartesian(R, 0, betaNext + betaStep, x2, y2, z2);

		double vx, vy, vz;
		CalcRotAxis(x1, y1, z1, x2, y2, z2, vx, vy, vz);

		glPushMatrix();
		glRotated(factor * 10.0, vx + alphaStep, vy + alphaStep * 2, vz + alphaStep);


		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= m; j++) {
			double alpha = (alphaStep * j) - PI / 2.0;

			double x1, x2, y1, y2, z1, z2;
			PolarToCartesian(R, alpha, beta, x1, y1, z1);
			PolarToCartesian(R, alpha, betaNext, x2, y2, z2);

			glVertex3d(x1, y1 + R, z1);
			glVertex3d(x2, y2 + R, z2);
		}
		glEnd();
		glPopMatrix();
	}
	glEnable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawAxes()
{
	glLineWidth(2);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	{
		// x - plavo
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(50, 0, 0);

		// y - crveno
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 50, 0);

		// z - zeleno
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 50);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void CGLRenderer::PolarToCartesian(double R, double phi, double theta, double& x, double& y, double& z)
{
	// phi je po onoj slici sa cs-a alpha ugao (-90 do 90)
	// theta je po slici sa cs-a beta ugao (0 do 360)

	x = R * cos(phi) * cos(theta);
	y = R * sin(phi);
	z = R * cos(phi) * sin(theta);
}

void CGLRenderer::CalcRotAxis(double x1, double y1, double z1, double x2, double y2, double z2, double& x3, double& y3, double& z3)
{
	// Analiticka geometrija :D
	// imamo vektor izmedju 1 i 2 (v12)
	// treba da nadjemo njegovu duzinu
	// i da ga normalizujemo (da ima duzinu 1)

	// v12 = (x2 - x1, y2 - y1, z2 - z1);
	// |v12| = sqrt(dx ^ 2 + dy ^ 2 + dz ^ 2)
	// normalizacija -> v12/|v12|

	// ovo su koordinate vektora v12
	double dx = x2 - x1;
	double dy = y2 - y1;
	double dz = z2 - z1;

	// ovo je njegov intenzitet/duzina
	double len = sqrt(dx * dx + dy * dy + dz * dz);

	// ovo je pretvaranje tog vektora u jedinicni vektor (normalizacija)
	x3 = dx / len;
	y3 = dy / len;
	z3 = dz / len;
}

void CGLRenderer::DrawSphere(float R, int n, int m)
{
	double alphaStep = PI / (double)m;
	double betaStep = 2 * PI / (double)n;

	glColor3f(1, 1, 1);

	for (int i = 0; i < n; i++) {
		double beta = betaStep * i;
		double betaNext = beta + betaStep;

		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= m; j++) {
			double alpha = (alphaStep * j) - PI / 2;
			double alphaNext = alpha + alphaStep;
			
			double x1, x2, y1, y2, z1, z2;

			PolarToCartesian(R, alpha, beta, x1, y1, z1);
			PolarToCartesian(R, alpha, betaNext, x2, y2, z2);

			/*double x1 = R * cos(alpha) * cos(beta);
			double y1 = R * sin(alpha);
			double z1 = R * cos(alpha) * sin(beta);*/

			//double x2 = R * cos(alpha) * cos(betaNext);
			//double y2 = R * sin(alpha);
			//double z2 = R * cos(alpha) * sin(betaNext);

			double u1 = beta / (2 * PI);
			double v1 = (-alpha + PI / 2) / PI;

			double u2 = betaNext / (2 * PI);
			double v2 = v1;

			glTexCoord2d(u1, v1);
			glVertex3d(x1, y1, z1);

			glTexCoord2d(u2, v2);
			glVertex3d(x2, y2, z2);
		}
		glEnd();
	}
}


void CGLRenderer::CalculateCameraPosition()
{
	double alpha = TO_RAD(viewAngleXY);
	double beta = TO_RAD(viewAngleXZ);

	eyex = viewR * cos(alpha) * cos(beta);
	eyey = viewR * sin(alpha);
	eyez = viewR * cos(alpha) * sin(beta);
}

void CGLRenderer::RotateCamera(double dXY, double dXZ)
{
	viewAngleXY += dXY;

	// Clamp
	viewAngleXY = min(90, viewAngleXY);
	viewAngleXY = max(-90, viewAngleXY);

	viewAngleXZ += dXZ;

	CalculateCameraPosition();
}

void CGLRenderer::ZoomCamera(bool zoomIn)
{
	double sensitivity = 3;

	viewR += zoomIn ? -sensitivity : sensitivity;

	viewR = max(1, viewR);
	viewR = min(50, viewR);

	CalculateCameraPosition();
}
