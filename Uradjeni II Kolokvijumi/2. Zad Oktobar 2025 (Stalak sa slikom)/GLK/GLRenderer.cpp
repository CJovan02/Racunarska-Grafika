#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
//#pragma comment(lib, "GL\\glut32.lib")

constexpr auto PI = 3.141592;
#define TO_RAD(x) ( x * PI / 180 )

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
	
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);

	envTex = LoadTexture("Env.jpg");
	SetLightningParams();
	/*glEnable(GL_TEXTURE_2D);*/

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	float stalakDiffuse[] = {0.9, 0.9, 0.9, 1};
	float pozadinaDiffuse[] = { 0.5, 1, 1 };

	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, 0, 1, 0);


	glTranslated(0, -1, 0);
	SetLightParams(GL_LIGHT0);

	DrawAxes();

	SetMaterial(pozadinaDiffuse);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	DrawPrism(10, 6, 10, 80, true);
	glDisable(GL_CULL_FACE);

	glPushMatrix();
	{
		SetMaterial(stalakDiffuse);
		glTranslated(-0.5, -0.5, -1.5);
		DrawPaintEastel(0.25, 3, 60, slide, alpha, beta, gama);
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
	gluPerspective(45, (double)w / h, 0.1, 2000);

	glMatrixMode(GL_MODELVIEW);

	//---------------------------------
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

UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texID;
	DImage img;
	
	img.Load(CString(fileName));
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

void CGLRenderer::SetLightningParams()
{
	GLfloat global_ambient[] = { .3, .3, .3, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glEnable(GL_LIGHTING);
}

void CGLRenderer::SetLightParams(int glLight)
{
	// Boja, intenzitet i pozicija svetla
	GLfloat ambient[] = { .2, .2, .2, 1 };
	GLfloat diffuse[] ={ .9, .9, .9, 1 };
	GLfloat specular[] = { .8, .8, .8, 1 };
	GLfloat position[] = { 0, 0, 0, 1 };

	glLightfv(glLight, GL_AMBIENT, ambient);
	glLightfv(glLight, GL_DIFFUSE, diffuse);
	glLightfv(glLight, GL_SPECULAR, specular);
	glLightfv(glLight, GL_POSITION, position);

	// Atenuacija
	glLightf(glLight, GL_CONSTANT_ATTENUATION, 1);
	glLightf(glLight, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(glLight, GL_QUADRATIC_ATTENUATION, 0.05);

	// Spotlight
	GLfloat direction[] = { 0, 0, -1 };

	glLightfv(glLight, GL_SPOT_DIRECTION, direction);
	glLightf(glLight, GL_SPOT_CUTOFF, 45.0);
	glLightf(glLight, GL_SPOT_EXPONENT, 1.0);

	glEnable(glLight);
}

void CGLRenderer::SetMaterial(float diffuse[])
{
	GLfloat diffuse2[] = { 1, 1, 1, 1 };
	GLfloat ambient[] = { diffuse[0] / 2, diffuse[1] / 2, diffuse[2] / 2, diffuse[3] / 2 };
	GLfloat specular[] = { 1, 1, 1, 1 };
	GLfloat emission[]{ 0, 0, 0, 1 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 64);

}

void CGLRenderer::DrawLampHead(float w, float h, float n)
{
	// Glava uopste ne izgleda kao sa slike od ispita sa ovim parametrima sto su dali
	// h mora da se smanji, mozda sam i ja lose razumeo sta se trazi
	h /= 4;
	DrawPrism(h, 0.5*w, 0.5*w, n, true);

	glPushMatrix();
	{
		glTranslated(h * 0.75, 0, 0);
		DrawPrism(h * 0.5, w, w, n, false);
	}
	glPopMatrix();
}

void CGLRenderer::DrawLampBody(float w, float h, int n, float alpha)
{
	// Ja ove vrednosti za translate na umem da objasnim :)

	glPushMatrix();
	{
		glTranslated(0, h / 2, 0);
		glRotated(-90, 1, 0, 0);
		DrawPrism(0.2 * w, h, 0.2 * w, n, true);
	}
	glPopMatrix();

	glPushMatrix();
	{
		// Ovo pomera sipku na gore, do bi se "nastavla" od prethodne sipke
		// i takodje rotira oko tacke sa kojom se spojila
		glTranslated(0, h, 0);
		glRotated(alpha, 0, 0, -1);

		// Ovo su transformacije koje rotiraju i pomeraju ovu sipku tako da
		// bude postavljena da krece od koordinatnog pocetka i ide pre +x osi
		glPushMatrix();
		{
			glRotated(90, 0, 1, 0);
			glTranslated(0, 0, 0.15 * h - 0.1 * w);
			DrawPrism(0.2 * w, 0.3 * h, 0.2 * w, n, true);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glRotated(90, 0, 0, -1);
			glTranslated(0, (h / 5) / 4 + h / 4, 0);
			DrawLampHead(w, h, n);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void CGLRenderer::DrawPaintEastel(float w, float h, int n, float slide, float alpha, float beta, float gama)
{
	// Postolje
	glPushMatrix();
	{
		DrawPrism(0.2 * w, 0.2 * h, 0.2 * w, n, true);

		glRotated(90, 0, 1, 0);
		DrawPrism(0.2 * w, 0.2 * h, 0.2 * w, n, true);
	}
	glPopMatrix();

	// Ostatak drzaca
	glPushMatrix();
	{
		glRotatef(gama, 0, 0, -1);

		// Visoka uspravna sipka
		glPushMatrix();
		{
			glRotated(90, 1, 0, 0);
			glTranslated(0, 0, -(h / 2));
			DrawPrism(0.2 * w, h, 0.2 * w, n, true);
		}
		glPopMatrix();

		// Sipke sa slikom
		glPushMatrix();
		{
			glTranslatef(0, slide * h, 0);

			// Sipke koje drze sliku
			glPushMatrix();
			{
				// Gornja
				glTranslated(0, (1 - 0.1) * h, 0);
				DrawPrism(0.2 * w, 0.4 * h, 0.2 * w, n, true);

				// Donja
				glTranslated(0, -0.3 * h, 0);
				DrawPrism(0.2 * w, 0.4 * h, 0.2 * w, n, true);
			}
			glPopMatrix();

			// Slika
			glPushMatrix();
			{
				glTranslated(0.01, (1 - 0.25) * h, 0);
				glRotated(90, 0, 1, 0);
				glRotated(90, 1, 0, 0);
				DrawPrism(0.6 * h, 0.3 * h, 0.2 * w, n, true, true);
			}
			glPopMatrix();
		}
		glPopMatrix();

		// Glava lampe
		glPushMatrix();
		{

			glTranslated(0, 0.9 * h, 0);
			glRotated(-90, 0, 0, 1);

			glRotatef(beta, 0, 0, -1);
			// Lamp body je prevelik za h, pa sam stavio na h / 2
			DrawLampBody(w, h / 2, n, alpha);
		}
		glPopMatrix();

	}
	glPopMatrix();
}

void CGLRenderer::DrawPrism(float a, float b, float c, int n, bool drawRight, bool topTexture)
{
	// Mozemo da tretiramo DrawRect kao provougaonik koje je na 'front' strani
	// On lazi na XY ravan i "gleda" prema +z osi

	// Ja sam strane kvadra rotirao zbog normale osvetljenja
	// Ako ih ne rotiramo, onda ce neke normale da gledaju ka unutrasnjosti kvadra umesto spolje
	// U ovom zadatku to verovatno nema potrebe jer se i unutrasnja i spoljasnja strana nekog poligona osvetljava

	float aHalf = a / 2;
	float bHalf = b / 2;
	float cHalf = c / 2;

	glDisable(GL_TEXTURE_2D);

	// Front
	glPushMatrix();
	{
		glTranslatef(0, 0, bHalf);
		DrawRect(a, c, n);
	}
	glPopMatrix();

	// Back
	glPushMatrix();
	{
		glTranslatef(0, 0, -bHalf);
		glRotatef(180, 0, 1, 0);
		DrawRect(a, c, n);
	}
	glPopMatrix();

	// Left
	glPushMatrix();
	{
		glTranslatef(-aHalf, 0, 0);
		glRotatef(-90, 0, 1, 0);
		DrawRect(b, c, n);
	}
	glPopMatrix();

	// Right
	if (drawRight) 
	{
		glPushMatrix();
		{
			glTranslatef(aHalf, 0, 0);
			glRotatef(90, 0, 1, 0);
			DrawRect(b, c, n);
		}
		glPopMatrix();
	}


	// Top
	if (topTexture) glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	{
		glTranslatef(0, cHalf, 0);
		glRotatef(-90, 1, 0, 0);
		DrawRect(a, b, n);
	}
	glPopMatrix();
	if (topTexture) glDisable(GL_TEXTURE_2D);

	// Bot
	glPushMatrix();
	{
		glTranslatef(0, -cHalf, 0);
		glRotatef(90, 1, 0, 0);
		DrawRect(a, b, n);
	}
	glPopMatrix();
}

void CGLRenderer::DrawRect(float a, float b, int n)
{
	double aHalf = a / 2.0;
	double aStep = a / (double)n;

	double bHalf = b / 2.0;
	double bStep = b / (double)n;


	glColor3i(1, 1, 1);
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
	for (int i = 0; i < n; i++) {
		float x0 = aStep * i - aHalf;
		float x1 = x0 + aStep;

		// teksturna x koordinata
		float tx0 = (float) i / n;
		float tx1 = (float)(i + 1) / n;

		for (int j = 0; j < n; j++) {
			float y0 = bStep * j - bHalf;
			float y1 = y0 + bStep;

			// teksturna y koordinata (invertovana je slika, pa samo invertujemo i y koordinatu)
			float ty0 = 1 - (float)j / n;
			float ty1 = 1 - (float)(j + 1) / n;

			glTexCoord2f(tx0, ty0); glVertex3f(x0, y0, 0);
			glTexCoord2f(tx1, ty0); glVertex3f(x1, y0, 0);
			glTexCoord2f(tx1, ty1); glVertex3f(x1, y1, 0);
			glTexCoord2f(tx0, ty1); glVertex3f(x0, y1, 0);
		}
	}
	glEnd();
}

void CGLRenderer::DrawAxes()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	{
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(50, 0, 0);

		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 50, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 50);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void CGLRenderer::RotateCamera(double dX, double dY)
{
	viewAngleXZ += dX;

	viewAngleXY += dY;
	viewAngleXY = min(90, viewAngleXY);
	viewAngleXY = max(-90, viewAngleXY);
	CalculateCameraPosition();
}

void CGLRenderer::ZoomView(bool zoomIn)
{
	double sensitivity = 2;

	viewR += zoomIn ? -sensitivity : sensitivity;

	viewR = max(0.1, viewR);

	CalculateCameraPosition();
}

void CGLRenderer::CalculateCameraPosition()
{
	double alpha = TO_RAD(viewAngleXY);
	double beta = TO_RAD(viewAngleXZ);

	eyex = viewR * cos(alpha) * cos(beta);
	eyey = viewR * sin(alpha);
	eyez = viewR * cos(alpha) * sin(beta);
}

void CGLRenderer::DrawCubeFace(double a)
{
	double aHalf = a / 2;

	double nx = 0, ny = 0, nz = 1;

	glNormal3d(nx, ny, nz);
	glColor3d(1, 0, 1);
	glBegin(GL_QUADS);
	{
		glVertex3d(aHalf, -aHalf, 0);

		glVertex3d(aHalf, aHalf, 0);

		glVertex3d(-aHalf, aHalf, 0);

		glVertex3d(-aHalf, -aHalf, 0);
	}
	glEnd();

}

void CGLRenderer::DrawCube(double a)
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
