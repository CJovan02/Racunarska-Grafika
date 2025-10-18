#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
//#pragma comment(lib, "GL\\glut32.lib")

#define PI 3.141592
#define TO_RAD(x) ( x * 3.141592 / 180 )

CGLRenderer::CGLRenderer(void)
{
	CalculatePosition();
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

	texLamp = LoadTexture("lamp.jpg");
	texBot = LoadTexture("bot.jpg");
	texTop = LoadTexture("top.jpg");
	texSide = LoadTexture("side.jpg");

	glEnable(GL_TEXTURE_2D);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);

	glTranslated(0, -15, 0);

	DrawAxes();
	DrawEnvCube(100);
	DrawLamp();



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
	gluPerspective(50, (double)w / h, 0.1, 2000);

	glMatrixMode(GL_MODELVIEW);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 

	glDeleteTextures(1, &texLamp);
	glDeleteTextures(1, &texBot);
	glDeleteTextures(1, &texTop);
	glDeleteTextures(1, &texSide);


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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}

#pragma region Lampa

void CGLRenderer::DrawLamp()
{
	glPushMatrix();
	{
		glTranslated(0, 1, 0);

		DrawLampBase();

		// Druga ruka
		glPushMatrix();
		{
			// Rotacija oko prvog tockica
			glRotated(arm1Angle, 0, 0, -1);

			// Prva ruka
			DrawLampArm();

			// Druga Ruka
			glTranslated(0, 20, 0);
			glRotated(arm2Angle, 0, 0, -1);
			DrawLampArm();

			// Glava
			glTranslated(0, 19, 0);
			glRotated(headAngle, 0, 0, -1);
			DrawLampHead();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void CGLRenderer::DrawLampBase()
{
	glPushMatrix();
	{
		glTranslated(0, -1, 0);
		DrawCylinder(7, 8, 2, 30, 1, false);
	}
	glPopMatrix();
}

void CGLRenderer::DrawLampArm()
{
	glPushMatrix();
	{
		glRotated(90, 1, 0, 0);
		glTranslated(0, -1, 0);
		DrawCylinder(3, 3, 2, 30, 1, false);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(0, 2.5, 0);
		DrawCylinder(1, 1, 15, 30, 1, false);
	}
	glPopMatrix();
}

void CGLRenderer::DrawLampHead()
{
	// Tockic oko koga se rotira
	glPushMatrix();
	{
		glRotated(90, 1, 0, 0);
		glTranslated(0, -1, 0);
		DrawCylinder(2, 2, 2, 30, 1, false);
	}
	glPopMatrix();


	// "vrat" lampe
	glPushMatrix();
	{
		glTranslated(3, -2.5, 0);
		DrawCylinder(3, 3, 5, 30, 1, false);
	}
	glPopMatrix();

	// "kapica" lampe
	glPushMatrix();
	{
		glTranslated(3, 2.5, 0);
		DrawCylinder(2, 3, 1, 30, 1, false);
	}
	glPopMatrix();

	// ono gde se stavlja sijalica :)
	glPushMatrix();
	{
		glTranslated(3, -7.5, 0);
		DrawCylinder(3, 5, 5, 30, 0, true);
	}
	glPopMatrix();
}

#pragma endregion

#pragma region Tela

void CGLRenderer::DrawAxes()
{
	glLineWidth(2.0);

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	{
		// x - plava
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(50, 0, 0);

		// y - crvena
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 50, 0);

		// z - zelena
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 50);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawEnvCube(double a)
{
	double aHalf = a / 2;

	glPushMatrix();
	{
		glTranslated(0, aHalf, 0);

		// Right
		glPushMatrix();
		{
			glTranslated(0, 0, aHalf);
			DrawCubeFace(a, texSide);
		}
		glPopMatrix();

		// Left
		glPushMatrix();
		{
			glTranslated(0, 0, -aHalf);
			glRotated(180, 0, 1, 0);
			DrawCubeFace(a, texSide);
		}
		glPopMatrix();

		// Front
		glPushMatrix();
		{
			glTranslated(-aHalf, 0, 0);
			glRotated(-90, 0, 1, 0);
			DrawCubeFace(a, texSide);
		}
		glPopMatrix();

		// Back
		glPushMatrix();
		{
			glTranslated(aHalf, 0, 0);
			glRotated(90, 0, 1, 0);
			DrawCubeFace(a, texSide);
		}
		glPopMatrix();

		// Top
		glPushMatrix();
		{
			glTranslated(0, aHalf, 0);
			glRotated(-90, 1, 0, 0);
			DrawCubeFace(a, texTop);
		}
		glPopMatrix();

		// Bot
		glPushMatrix();
		{
			glTranslated(0, -aHalf, 0);
			glRotated(90, 1, 0, 0);
			DrawCubeFace(a, texBot);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void CGLRenderer::DrawCubeFace(double a, UINT texID)
{
	// Bez ikakfih translacija, ovo je desna strana kocke (tako se poklapa tekstura)
	double aHalf = a / 2;

	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, texID);
	glBegin(GL_QUADS);
	{
		// Top right
		glTexCoord2f(1, 0);
		glVertex3f(aHalf, aHalf, 0);

		// Top left
		glTexCoord2f(0, 0);
		glVertex3f(-aHalf, aHalf, 0);

		// Bot left
		glTexCoord2f(0, 1);
		glVertex3f(-aHalf, -aHalf, 0);

		// Bot right
		glTexCoord2f(1, 1);
		glVertex3f(aHalf, -aHalf, 0);
	}
	glEnd();
}

void CGLRenderer::DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool isOpen)
{
	glColor3d(1, 1, 1);

	glBindTexture(GL_TEXTURE_2D, texLamp);
	double texU = 0.5;
	// samo se Y koordinata razlikuje na gornjoj/donjoj polovini teksture
	double texVTop = 0.25;
	double texVBot = 0.75;
	double texR_X = 0.5;   // horizontalni poluprečnik
	double texR_Y = 0.25;

	// Omotac
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int i = 0; i <= nSeg; i++)
		{
			double alpha = (double) 360 / nSeg * i;

			double xTop = r1 * cos(TO_RAD(alpha));
			double zTop = r1 * sin(TO_RAD(alpha));

			double xBot = r2 * cos(TO_RAD(alpha));
			double zBot = r2 * sin(TO_RAD(alpha));

			double tx = - (double) alpha / 360 * 2;
			double tyTop = texMode == 0 ? 0 : 0.5;
			double tyBot = texMode == 0 ? 0.5 : 1;

			glTexCoord2d(tx, tyBot);
			glVertex3d(xBot, 0, zBot);

			glTexCoord2d(tx, tyTop);
			glVertex3d(xTop, h, zTop);
		}
	}
	glEnd();


	if (isOpen) return;

	// Gornja osnova
	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2f(texU, texVTop);
		glVertex3f(0, h, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			double alpha = (double) 360 / nSeg * i;

			double x = r1 * cos(TO_RAD(alpha));
			double z = r1 * sin(TO_RAD(alpha));

			double tx = texU + texR_X * x / r1;
			double ty = texVTop + texR_Y * z / r1;

			glTexCoord2f(tx, ty);
			glVertex3f(x, h, z);
		}
	}
	glEnd();

	// Donja osnova
	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2f(texU, texVTop);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= nSeg; i++)
		{
			double alpha = (double) 360 / nSeg * i;

			double x = r2 * cos(TO_RAD(alpha));
			double z = r2 * sin(TO_RAD(alpha));

			double tx = texU - texR_X * x / r2;
			double ty = texVTop + texR_Y * z / r2;

			glTexCoord2f(tx, ty);
			glVertex3f(x, 0, z);
		}
	}
	glEnd();
}

#pragma endregion

#pragma region Pomeranje kamere

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
	double sensitivity = 3;

	viewR += zoomIn ? -sensitivity : sensitivity;

	// Clamp
	viewR = max(1, viewR);
	viewR = min(50, viewR);

	CalculatePosition();
}

void CGLRenderer::UpdateCamera()
{
	double toDeg = 57.29577951308; // konverzija iz radijana u stepene

	glTranslatef(0, 0, -viewR);
	glRotatef(m_CamBeta * toDeg, 1, 0, 0);
	glRotatef(-m_CamAlpha * toDeg, 0, 1, 0);
}

void CGLRenderer::CalculatePosition()
{
	double alfa = TO_RAD(viewAngleXY);
	double beta = TO_RAD(viewAngleXZ);

	eyex = viewR * cos(alfa) * cos(beta);
	eyey = viewR * sin(alfa);
	eyez = viewR * cos(alfa) * sin(beta);

	upy = signbit(cos(alfa)) ? -1 : 1;
}
#pragma endregion