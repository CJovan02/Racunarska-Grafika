#include "pch.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

const double PI = 3.14159;
#define TO_RAD(a) (PI * a / 180 )

CGLRenderer::CGLRenderer(void)
{
	m_hrc = NULL;
	this->viewR = 10;

	this->viewAngleXY = 0;
	this->viewAngleXZ = 0;

	eyex = 0, eyey = 0, eyez = 0;
	centerx = 0, centery = 0, centerz = 0;
	upx = 0, upy = 1, upz = 0;

	cactusPartAngle = 0;

	this->CalculatePosition();
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc) return false;

	return true;
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	{
		glClearColor(POZADINA, 1.0);
		glEnable(GL_DEPTH_TEST);
	}
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		gluLookAt(
			eyex, eyey, eyez,
			centerx, centery, centerz,
			upx, upy, upz
		);

		glTranslated(0, -5, 0);
		DrawAxis(100);
		DrawGrid(10, 10, 10, 10);
		DrawFigure(cactusPartAngle);
	}
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	if (h == 0) h = 1;

	wglMakeCurrent(pDC->m_hDC, m_hrc);
	{
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, (double)w / h, 1.0, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

#pragma region Crtanje

void CGLRenderer::DrawAxis(float width)
{
	glLineWidth(1);
	glPointSize(10);

	glBegin(GL_LINES);
	{
		// za x osu
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(width, 0, 0);

		// za y osu
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, width, 0);

		// za z osu
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, width);
	}
	glEnd();
}

void CGLRenderer::DrawGrid(double width, double height, int nSegW, int nSegH)
{
	double wStep = width / nSegW;
	double hStep = height / nSegH;
	glLineWidth(3);
	glPushMatrix();
	{
		glTranslated(-width / 2, 0, -height / 2);
		glBegin(GL_LINES);
		{
			glColor3f(GRID_COLOR);
			for (float offset = 0; offset <= width; offset += wStep)
			{
				glVertex3d(0, 0, offset);
				glVertex3d(width, 0, offset);
			}

			for (float offset = 0; offset <= height; offset += hStep)
			{
				glVertex3d(offset, 0, 0);
				glVertex3d(offset, 0, height);
			}

		}
		glEnd();
	}
	glPopMatrix();

}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	int alphaStep = 360 / nSegAlpha;
	int betaStep = 360 / nSegBeta;

	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int beta = 0; beta <= 360; beta += betaStep)
		{
			for (int alpha = -90; alpha <= 90; alpha += alphaStep)
			{
				double x = r * cos(TO_RAD(alpha)) * cos(TO_RAD(beta));
				double y = r * sin(TO_RAD(alpha));
				double z = r * cos(TO_RAD(alpha)) * sin(TO_RAD(beta));

				double x2 = r * cos(TO_RAD(alpha)) * cos(TO_RAD(beta) + TO_RAD(betaStep));
				double y2 = r * sin(TO_RAD(alpha));
				double z2 = r * cos(TO_RAD(alpha)) * sin(TO_RAD(beta) + TO_RAD(betaStep));

				glVertex3d(x, y, z);
				glVertex3d(x2, y2, z2);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg)
{
	int alphaStep = 360 / nSeg;

	// Gornja osnova
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3d(0, h, 0);

		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			double x = r2 * cos(TO_RAD(alpha));
			double z = r2 * sin(TO_RAD(alpha));
			glVertex3d(x, h, z);
		}
	}
	glEnd();

	// Donja osnova
	glBegin(GL_LINES);
	{
		glVertex3d(0, 0, 0);

		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			double x = r1 * cos(TO_RAD(alpha));
			double z = r1 * sin(TO_RAD(alpha));
			glVertex3d(x, 0, z);
		}
	}
	glEnd();

	// Omotac
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			// Tacka gornje osnove
			double xGornje = r2 * cos(TO_RAD(alpha));
			double zGornje = r2 * sin(TO_RAD(alpha));

			double xDonje = r1 * cos(TO_RAD(alpha));
			double zDonje = r1 * sin(TO_RAD(alpha));

			glVertex3d(xGornje, h, zGornje);
			glVertex3d(xDonje, 0, zDonje);
		}
	}
	glEnd();
}

void CGLRenderer::DrawCone(double h, double r, int nSeg)
{
	int alphaStep = 360 / nSeg;

	// Omotac kupe
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			double x = r * cos(TO_RAD(alpha));
			double z = r * sin(TO_RAD(alpha));

			glVertex3d(x, 0, z);
			glVertex3d(0, h, 0);
		}

		// Omotac se ne zatvori do kraja pa mozemo pocetku tacku da hardkodiramo
		glVertex3d(r, 0, 0);  // (x = r, z = 0 at alpha = 0)
		glVertex3d(0, h, 0);
	}
	glEnd();

	// Baza kupe
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3d(0, 0, 0);

		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			double x = r * cos(TO_RAD(alpha));
			double z = r * sin(TO_RAD(alpha));
			glVertex3d(x, 0, z);
		}
	}
	glEnd();
}

void CGLRenderer::DrawFigure(double angle)
{
	double vaseHeight = 1.3;
	double vaseTopHeight = 0.6;
	double cactusCylinderR = 0.45;
	double sphereR = 0.3;
	double nSeg = 8;
	double nSegSphere = 50;

	// Vaza
	glColor3f(VASE_COLOR);
	DrawCylinder(vaseHeight, 1, 1.25, nSeg);
	glTranslated(0, vaseHeight, 0);
	DrawCylinder(vaseTopHeight, 1.55, 1.55, nSeg);

	glTranslated(0, vaseTopHeight, 0);
	glColor3f(CACTUS_PART_COLOR);
	DrawCylinder(vaseHeight, cactusCylinderR, cactusCylinderR, nSeg);

	glTranslated(0, vaseHeight + sphereR, 0);
	glColor3f(CACTUS_SPHERE_COLOR);
	DrawSphere(sphereR, nSegSphere, nSegSphere);

	// Donja leva "ruka"
	glPushMatrix();
	{
		glRotated(50, 1, 0, 0);
		glTranslated(0, sphereR, 0);
		glColor3f(CACTUS_PART_COLOR);
		DrawCone(vaseHeight, cactusCylinderR, nSeg);

		glTranslated(0, vaseHeight + sphereR, 0);
		glColor3f(CACTUS_SPHERE_COLOR);
		DrawSphere(sphereR, nSegSphere, nSegSphere);

		glPushMatrix();
		{
			glRotated(-50, 1, 0, 0);
			glTranslated(0, sphereR, 0);
			glColor3f(CACTUS_PART_COLOR);
			DrawCone(vaseHeight, cactusCylinderR, nSeg);

			glTranslated(0, vaseHeight + sphereR, 0);
			glColor3f(CACTUS_SPHERE_COLOR);
			DrawSphere(sphereR, nSegSphere, nSegSphere);
		}
		glPopMatrix();

		glRotated(40, 1, 0, 0);
		glTranslated(0, sphereR, 0);
		glColor3f(CACTUS_PART_COLOR);
		DrawCylinder(vaseHeight, cactusCylinderR, cactusCylinderR, nSeg);

		glTranslated(0, vaseHeight + sphereR, 0);
		glColor3f(CACTUS_SPHERE_COLOR);
		DrawSphere(sphereR, nSegSphere, nSegSphere);

		glPushMatrix();
		{
			glRotated(50, 1, 0, 0);
			glTranslated(0, sphereR, 0);
			glColor3f(CACTUS_PART_COLOR);
			DrawCylinder(vaseHeight, cactusCylinderR, cactusCylinderR, nSeg);
		
			glTranslated(0, vaseHeight + sphereR, 0);
			glColor3f(CACTUS_SPHERE_COLOR);
			DrawSphere(sphereR, nSegSphere, nSegSphere);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glRotated(-50, 1, 0, 0);
			glTranslated(0, sphereR, 0);
			glColor3f(CACTUS_PART_COLOR);
			DrawCylinder(vaseHeight, cactusCylinderR, cactusCylinderR, nSeg);

			glTranslated(0, vaseHeight + sphereR, 0);
			glColor3f(CACTUS_SPHERE_COLOR);
			DrawSphere(sphereR, nSegSphere, nSegSphere);
		}
		glPopMatrix();
	}
	glPopMatrix();

	// Donja desna "ruka"
	glPushMatrix();
	{
		glRotated(-50, 1, 0, 0);
		glTranslated(0, sphereR, 0);
		glColor3f(CACTUS_PART_COLOR);
		DrawCylinder(vaseHeight, cactusCylinderR, cactusCylinderR, nSeg);

		glTranslated(0, vaseHeight + sphereR, 0);
		glColor3f(CACTUS_SPHERE_COLOR);
		DrawSphere(sphereR, nSegSphere, nSegSphere);

		glTranslated(0, sphereR, 0);
		glColor3f(CACTUS_PART_COLOR);
		DrawCylinder(vaseHeight, cactusCylinderR, cactusCylinderR, nSeg);

		glTranslated(0, vaseHeight + sphereR, 0);
		glColor3f(CACTUS_SPHERE_COLOR);
		DrawSphere(sphereR, nSegSphere, nSegSphere);
	}
	glPopMatrix();

	glTranslated(0, sphereR, 0);
	glColor3f(CACTUS_PART_COLOR);
	DrawCylinder(vaseHeight, cactusCylinderR, cactusCylinderR, nSeg);

	glTranslated(0, vaseHeight + sphereR, 0);
	glColor3f(CACTUS_SPHERE_COLOR);
	DrawSphere(sphereR, nSegSphere, nSegSphere);

	// Zadnji deo oko koga se rotira
	glRotated(angle, 1, 0, 0);
	glTranslated(0, sphereR, 0);
	glColor3f(CACTUS_ROTATE_PART_COLOR);
	DrawCone(vaseHeight, cactusCylinderR, nSeg);

	glTranslated(0, vaseHeight + sphereR, 0);
	glColor3f(CACTUS_SPHERE_COLOR);
	DrawSphere(sphereR, nSegSphere, nSegSphere);

	glTranslated(0, sphereR, 0);
	glColor3f(CACTUS_PART_COLOR);
	DrawCone(vaseHeight, cactusCylinderR, nSeg);

	glTranslated(0, vaseHeight + sphereR, 0);
	glColor3f(CACTUS_SPHERE_COLOR);
	DrawSphere(sphereR, nSegSphere, nSegSphere);
}

#pragma endregion

#pragma region Rotiranje Pogleda

void CGLRenderer::RotateView(double dXY, double dXZ)
{
	this->viewAngleXY += dXY;

	// Clamp
	viewAngleXY = min(90, viewAngleXY);
	viewAngleXY = max(-90, viewAngleXY);

	this->viewAngleXZ += dXZ;
	this->CalculatePosition();
}

void CGLRenderer::CalculatePosition()
{
	double dWXY = this->viewAngleXY * 3.14 / 180,
		dWXZ = this->viewAngleXZ * 3.14 / 180;

	this->eyex = this->viewR * cos(dWXY) * cos(dWXZ);
	this->eyey = this->viewR * sin(dWXY);
	this->eyez = this->viewR * cos(dWXY) * sin(dWXZ);

	this->upy = signbit(cos(dWXY)) ? -1 : 1;
}

#pragma endregion
