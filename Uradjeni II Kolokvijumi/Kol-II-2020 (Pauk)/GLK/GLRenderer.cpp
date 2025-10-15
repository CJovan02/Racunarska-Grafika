#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h";
//#pragma comment(lib, "GL\\glut32.lib")

constexpr double PI = 3.141592;
#define TO_RAD(x) ( x * PI / 180 )

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
	
	glClearColor(0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	m_texSpider = LoadTexture("spider.png");

	m_texEnv[0] = LoadTexture("front.jpg");
	m_texEnv[1] = LoadTexture("back.jpg");

	m_texEnv[2] = LoadTexture("left.jpg");
	m_texEnv[3] = LoadTexture("right.jpg");

	m_texEnv[4] = LoadTexture("top.jpg");
	m_texEnv[5] = LoadTexture("bot.jpg");

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
	gluLookAt(eyex, eyey, eyez,
		centerx, centery, centerz,
		upx, upy, upz);
	DrawAxes();

	glTranslatef(0.0, -10, 0.0);

	glPushMatrix();
	{
		glTranslated(0, 50, 0);

		DrawEnvCube(100);
		glPushMatrix();
		{
			glTranslated(0, -45, 0);
			glRotated(-90, 0, 1, 0);
			DrawSpider();
		}
		glPopMatrix();
	}
	glPopMatrix();

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
	gluPerspective(50, (double)w/h, 0.1, 2000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 

	glDeleteTextures(1, &m_texSpider);
	glDeleteTextures(6, m_texEnv);

	// ...
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
	GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(),
	GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());
	return texID;
}

#pragma region Crtanje
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
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);

	// Front
	glPushMatrix();
	{
		glTranslated(0, 0, aHalf);
		DrawCubeFace(a, m_texEnv[0]);
	}
	glPopMatrix();

	// Back
	glPushMatrix();
	{
		glTranslated(0, 0, -aHalf);
		DrawCubeFace(a, m_texEnv[1]);
	}
	glPopMatrix();

	// Left
	glPushMatrix();
	{
		glTranslated(-aHalf, 0, 0);
		glRotated(90, 0, 1, 0);
		DrawCubeFace(a, m_texEnv[2]);
	}
	glPopMatrix();

	// Right
	glPushMatrix();
	{
		glTranslated(aHalf, 0, 0);
		glRotated(-90, 0, 1, 0);
		DrawCubeFace(a, m_texEnv[3]);
	}
	glPopMatrix();


	// Top
	glPushMatrix();
	{
		glTranslated(0, aHalf, 0);
		glRotated(-90, 1, 0, 0);
		DrawCubeFace(a, m_texEnv[4]);
	}
	glPopMatrix();

	// Bottom
	glPushMatrix();
	{
		glTranslated(0, -aHalf, 0);
		glRotated(90, 1, 0, 0);
		DrawCubeFace(a, m_texEnv[5]);
	}
	glPopMatrix();
}

void CGLRenderer::DrawCubeFace(double a, UINT texID)
{
	double aHalf = a / 2;

	glBindTexture(GL_TEXTURE_2D, texID);
	glBegin(GL_QUADS);
	{
		// Bot right
		glTexCoord2f(1, 1);
		glVertex3d(aHalf, -aHalf, 0);

		// Top right
		glTexCoord2f(1, 0);
		glVertex3d(aHalf, aHalf, 0);

		// Top left
		glTexCoord2f(0, 0);
		glVertex3d(-aHalf, aHalf, 0);

		// Bot Left
		glTexCoord2f(0, 1);
		glVertex3d(-aHalf, -aHalf, 0);
	}
	glEnd();
}

void CGLRenderer::DrawSphere(double r, int nSeg, double texU, double texV, double texR)
{
	// Longituda
	int alphaStep = 180 / (nSeg * 2);
	// Latituda
	int betaStep = 360 / nSeg;

	glColor3d(1, 1, 1);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, m_texSpider);
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int beta = 0; beta <= 360; beta += betaStep)
		{
			for (int alpha = -90; alpha <= 90; alpha += alphaStep)
			{
				double x1 = r * cos(TO_RAD(alpha)) * cos(TO_RAD(beta));
				double y1 = r * sin(TO_RAD(alpha));
				double z1 = r * cos(TO_RAD(alpha)) * sin(TO_RAD(beta));

				int beta2 = beta + betaStep;
				double x2 = r * cos(TO_RAD(alpha)) * cos(TO_RAD(beta2));
				double y2 = r * sin(TO_RAD(alpha));
				double z2 = r * cos(TO_RAD(alpha)) * sin(TO_RAD(beta2));

				// Lepljenje teksture preko sfere
				double tx1 = texR * x1 / r + texU;
				double ty1 = texR * z1 / r + texV;

				double tx2 = texR * x2 / r + texU;
				double ty2 = texR * z2 / r + texV;

				glTexCoord2f(tx1, ty1);
				glVertex3d(x1, y1, z1);

				glTexCoord2f(tx2, ty2);
				glVertex3d(x2, y2, z2);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR)
{
	int alphaStep = 360 / nSeg;

	glColor3d(1, 1, 1);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, m_texSpider);

	// Omotac
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			double x = r * cos(TO_RAD(alpha));
			double z = r * sin(TO_RAD(alpha));


			double tx = x / r * texR + texU;
			double ty = z / r * texR + texV;

			glTexCoord2d(tx, ty);
			glVertex3d(x, 0, z);

			glTexCoord2d(texU, texV);
			glVertex3d(0, h, 0);
		}
	}
	glEnd();
	
	// Baza
	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2d(texU, texV);
		glVertex3d(0, 0, 0);
		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			double x = r * cos(TO_RAD(alpha));
			double z = r * sin(TO_RAD(alpha));

			double tx = x / r * texR + texU;
			double ty = z / r * texR + texV;

			glTexCoord2d(tx, ty);
			glVertex3d(x, 0, z);
		}
	}
	glEnd();
}

void CGLRenderer::DrawLegSegment(double r, double h, int nSeg)
{
	glPushMatrix();
	{
		//glTranslatef(0, r, 0);
		DrawSphere(r, 2 * nSeg, 0.25, 0.25, 0.24);
		DrawCone(r, h, nSeg, 0.75, 0.75, 0.24);
	}
	glPopMatrix();
}

void CGLRenderer::DrawLeg()
{
	glPushMatrix();
	{
		DrawLegSegment(1, 10, 5);
		
		glTranslated(0, 11, 0);
		glRotated(85, 1, 0, 0);
		DrawLegSegment(1, 15, 5);
	}
	glPopMatrix();
}

void CGLRenderer::DrawSpiderBody()
{
	// Grudni deo
	glPushMatrix();
	{
		glScaled(1, 0.5, 1);
		DrawSphere(3, 10, 0.25, 0.25, 0.24);
	}
	glPopMatrix();

	// Glava
	glPushMatrix();
	{
		glTranslated(-4, 0, 0);
		glScaled(1, 0.5, 1);
		DrawSphere(2, 10, 0.75, 0.25, 0.24);
	}
	glPopMatrix();

	// Guzica
	glPushMatrix();
	{
		glTranslated(6, 0, 0);
		glScaled(1, 0.8, 1);
		DrawSphere(5, 10, 0.25, 0.25, 0.24);
	}
	glPopMatrix();
}

void CGLRenderer::DrawSpider()
{
	glBindTexture(GL_TEXTURE_2D, m_texSpider);
	glPushMatrix();
	{
		DrawSpiderBody();
		for (int i = 0; i < 4; i++)
		{
			glPushMatrix();
			{
				glRotatef(i * 30 - 45, 0.0, 1.0, 0.0);
				glRotatef(45, 1.0, 0.0, 0.0);
				DrawLeg();
			}
			glPopMatrix();
		}

		for (int i = 0; i < 4; i++)
		{
			glPushMatrix();
			{
				glRotatef(i * 30 - 45 + 180, 0.0, 1.0, 0.0);
				glRotatef(45, 1.0, 0.0, 0.0);
				DrawLeg();
			}
			glPopMatrix();
		}
	}
	glPopMatrix();
}
#pragma endregion

#pragma region Rotiranje i zumiranje pogleda
void CGLRenderer::RotateView(double dXY, double dXZ)
{
	viewAngleXY += dXY;

	// Clamp
	viewAngleXY = min(90, viewAngleXY);
	viewAngleXY = max(-90, viewAngleXY);

	viewAngleXZ += dXZ;
	CalculatePosition();
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

void CGLRenderer::ZoomView(bool zoomIn)
{
	double zoomSensitivity = 3;

	viewR += zoomIn ? -zoomSensitivity : zoomSensitivity;

	// Clamp
	viewR = max(1, viewR);
	viewR = min(1000, viewR);

	CalculatePosition();
}
#pragma endregion
