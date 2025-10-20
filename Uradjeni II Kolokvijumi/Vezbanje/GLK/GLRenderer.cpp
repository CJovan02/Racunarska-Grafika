#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
#include <vector>
#include <tuple>
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

#pragma region Konfiguracija projekta i DrawScene

void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glClearColor(1, 1, 0, 1);
	glEnable(GL_DEPTH_TEST);

	spiderTex = LoadTexture("spider.png");
	glTex = LoadTexture("lamp.jpg");
	glEnable(GL_TEXTURE_2D);

	SetLightningParams();

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, 0, 1, 0);

	SetLightParams(GL_LIGHT0);

	DrawAxes();
	float diffuse[] = { 1, 1, 1, 1 };
	SetMaterial(diffuse);

	glPushMatrix();
	{
		glTranslated(0, 0, -15);
		DrawSphere(5, 60, 60);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(0, 0, 15);
		DrawCylinder(5, 5, 10, 60);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(15, 0, 0);
		DrawCone(5, 10, 60);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(-15, 0, 0);
		DrawPrism(10, 20, 15, 60);
		//DrawCubeGrid(10, 60);
	}
	glPopMatrix();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC* pDC, int w, int h)
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

void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 

	glDeleteTextures(1, &spiderTex);
	glDeleteTextures(1, &glTex);

	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
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
	GLfloat diffuse[] = { 1, 1, 1, 1 };
	GLfloat specular[] = { .8, .8, .8, 1 };
	GLfloat position[] = { 0, 0, 0, 1 };

	glLightfv(glLight, GL_AMBIENT, ambient);
	glLightfv(glLight, GL_DIFFUSE, diffuse);
	glLightfv(glLight, GL_SPECULAR, specular);
	glLightfv(glLight, GL_POSITION, position);

	// Atenuacija
	glLightf(glLight, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(glLight, GL_LINEAR_ATTENUATION, 0.03);
	glLightf(glLight, GL_QUADRATIC_ATTENUATION, 0.01);

	// Spotlight
	//GLfloat direction[] = { 0, 0, -1 };

	//glLightfv(glLight, GL_SPOT_DIRECTION, direction);
	//glLightf(glLight, GL_SPOT_CUTOFF, 45.0);
	//glLightf(glLight, GL_SPOT_EXPONENT, 1.0);

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

#pragma endregion

#pragma region Crtanje

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

#pragma endregion

#pragma region Geometrijska tela

void CGLRenderer::PolarToCartesian(double R, double phi, double theta, double& x, double& y, double& z)
{
	// phi je alpha ugao (-90 do 90)
	// theta je beta ugao (0 do 360)

	x = R * cos(phi) * cos(theta);
	y = R * sin(phi);
	z = R * cos(phi) * sin(theta);
}

void CGLRenderer::DrawSphere(double r, int n, int m)
{
	// Longituda
	double alphaStep = PI / m;
	// Latituda
	double betaStep = (2.0 * PI) / n;
	// Sferno mapiranje
	glBindTexture(GL_TEXTURE_2D, glTex);
	
	// Planarno (projekciono) mapiranje
	//glBindTexture(GL_TEXTURE_2D, spiderTex);
	//double texU = 0.25, texV = 0.25, texR = 0.24;

	// Kad je upaljen GL_LIGHT_MODEL_TWO_SIDE moramo vertex-e da crtamo smeru suprotim od kazaljke na satu
	// ili mozemo da promenimo mod, da front side objekta bude u smeru kazaljke na satu
	glFrontFace(GL_CW);

	glColor3f(1, 1, 1);
	for (int i = 0; i < n; i++) {
		double beta = betaStep * i;
		double betaNext = beta + betaStep;

		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= m; j++) {
			double alpha = (alphaStep * j) - PI / 2.0;

			double x1, x2, y1, y2, z1, z2;
			PolarToCartesian(r, alpha, beta, x1, y1, z1);
			PolarToCartesian(r, alpha, betaNext, x2, y2, z2);

			// Sferno mapiranje (tex se obmotava oko sfere)
			// ako neku koordinatu (tx ili ty) pomnozimo sa *0.5, onda se uzima prva polovina te strane teksture (mozemo da dodamo +0.5 NA KRAJU ako hocemo da uzmemo drugu polovinu teksture)
			// ako pomnozimo sa *2 onda se teksture zalepi samo na polovinu objekta, ako zelimo da se ponovi i na drugoj strani, promenimo GL_CLAMP na GL_REPEAT u LoadTexture

			// invertovanje koordinata:
			// 1 - tx -> onda se koord invertuje (ako tx ide od 0 do 1, odnosno ne mnozi se sa 0.5 ili 2)
			// ako tx ide od 0 do 0.5, onda ide 0.5 - tx, 
			// ako ide od 0 do 2 onda 2 - tx ..., 
			double tx1 =1 -(double)i / n;
			double ty1 =0.5 - (double) j / n * 0.5 + 0.5;

			double tx2 =1 -(double)(i + 1) / n;
			double ty2 =0.5 - (double) j / n * 0.5 + 0.5;




			// Planarno mapiranje ("isece" se krug iz teksture i zalepi se odozgo od lopte)
			// u sustini projekcija lopte na ravan
			// taj isecen krug je definisan sa, texU i texV -> centar tog kruga iz teksture
			// i texR -> poluprecnik tog kruga

			// Invertovanje koordinata:
			// samo se doda "-" ispred koordinate
			//double tx1 = texR *x1 / r + texU;
			//double ty1 = texR * z1 / r + texV;

			//double tx2 = texR *x2 / r + texU;
			//double ty2 = texR * z2 / r + texV;


			glNormal3d(x1 / r, y1 / r, z1 / r);
			glTexCoord2d(tx1, ty1); 
			glVertex3d(x1, y1, z1);
			glNormal3d(x2 / r, y2 / r, z2 / r);
			glTexCoord2d(tx2, ty2); 
			glVertex3d(x2, y2, z2);
		}
		glEnd();
	}

	// Vratimo nazad kako je bilo
	glFrontFace(GL_CCW);
}

void CGLRenderer::CalculateConeNormals(double h, double rBot, double rTop, double alphaRad, double& nx, double& ny, double& nz)
{
	double r = (rBot - rTop) / 2;
	double l = sqrt(pow(h, 2) + pow(r, 2));

	double nr = h / l;

	ny = r / l;
	nx = nr * cos(alphaRad);
	nz = nr * sin(alphaRad);
}

void CGLRenderer::DrawCylinder(double r1, double r2, double h, int nSeg)
{
	double alphaStep = (PI * 2.0) / nSeg;

	glBindTexture(GL_TEXTURE_2D, glTex);
	double texR = 0.25, texU = 0.5, texV = 0.25;

	glColor3f(1, 1, 1);

	// Donja osnova - koristi planarno (projekciono) mapiranje teksture
	glBegin(GL_TRIANGLE_FAN);
	{
		double y = - h/2.0;

		glNormal3f(0, -1, 0);
		glTexCoord2d(texU, texV);
		glVertex3f(0, y, 0);
		for (int i = 0; i <= nSeg; i++) {
			double alpha = alphaStep * i;

			double x = r1 * cos(alpha);
			double z = r1 * sin(alpha);

			double tx = - (texR * 2.0) * x / r1 + texU;
			double ty = texR * z / r1 + texV;

			glTexCoord2d(tx, ty);
			glVertex3f(x, y, z);
		}
	}
	glEnd();


	// Gornja osnova - koristi planarno (projekciono) mapiranje teksture
	glFrontFace(GL_CW);
	glBegin(GL_TRIANGLE_FAN);
	{
		double y = h/2.0;

		glNormal3f(0, 1, 0);
		glTexCoord2d(texU, texV);
		glVertex3f(0, y, 0);
		for (int i = 0; i <= nSeg; i++) {
			double alpha = alphaStep * i;

			double x = r2 * cos(alpha);
			double z = r2 * sin(alpha);

			double tx = (texR * 2.0) * x / r1 + texU;
			double ty = texR * z / r1 + texV;

			glTexCoord2d(tx, ty);
			glVertex3f(x, y, z);
		}
	}
	glEnd();
	glFrontFace(GL_CCW);

	// Omotac - koristi sferno (cilindricno) mapiranje teksture
	glFrontFace(GL_CW);
	glBegin(GL_TRIANGLE_STRIP);
	{
		double yTop = h / 2.0;
		double yBot = -h / 2.0;
		for (int i = 0; i <= nSeg; i++) {
			double alpha = alphaStep * i;

			double xTop = r2 * cos(alpha);
			double zTop = r2 * sin(alpha);

			double xBot = r1 * cos(alpha);
			double zBot = r1 * sin(alpha);

			double tx = 2 - (double) i / nSeg * 2;
			double tyTop = 0.5, tyBot = 1;

			double nx, ny, nz;

			// Normale samo kada je r1 = r2
			//nx = xTop / r2;
			//ny = 0;
			//nz = zTop / r2;

			// Normale kada je r1 != r2
			CalculateConeNormals(h, r1, r2, alpha, nx, ny, nz);
			
			
			glNormal3f(nx, ny, nz);
			glTexCoord2d(tx, tyTop); glVertex3d(xTop, yTop, zTop);

			glTexCoord2d(tx, tyBot); glVertex3d(xBot, yBot, zBot);
		}
	}
	glEnd();
	glFrontFace(GL_CCW);
}

void CGLRenderer::DrawCone(double r, double h, int nSeg)
{
	double alphaStep = (PI * 2.0) / nSeg;

	double texR = 0.25, texU = 0.5, texV = 0.25;
	
	glBindTexture(GL_TEXTURE_2D, glTex);
	glColor3f(1, 1, 1);
	// Osnova - moze planarno (projekciono) mapiranje teksture
	glBegin(GL_TRIANGLE_FAN);
	{
		double y = -h / 2.0;

		glNormal3f(0, -1, 0);
		glTexCoord2d(texU, texV);
		glVertex3f(0, y, 0);
		for (int i = 0; i <= nSeg; i++) {
			double alpha = alphaStep * i;

			double x = r * cos(alpha);
			double z = r * sin(alpha);

			// Sferno mapiranje
			double tx = -(texR * 2.0) * x / r + texU;
			double ty = texR * z / r + texV;

			glTexCoord2d(tx, ty);
			glVertex3f(x, y, z);
		}
	}
	glEnd();

	double texU2 = 0.75, texV2 = 0.75, texR2 = 0.24;

	glBindTexture(GL_TEXTURE_2D, spiderTex);
	// Omotac - moze sferno a moze i planarno
	glBegin(GL_TRIANGLE_STRIP);
	{
		double yTop = h / 2.0;
		double yBot = -h / 2.0;
		for (int i = 0; i <= nSeg; i++) {
			double alpha = alphaStep * i;

			double x = r * cos(alpha);
			double z = r * sin(alpha);

			// Sferno mapiranje
			//double tx = 0.5 - (double)i / nSeg * 0.5;
			//double tyTop = 0, tyBot = 0.5;

			// Planarno mapiranje
			double tx = x / r * texR2 + texU2;
			double ty = z / r * texR2 + texV2;

			double nx, ny, nz;

			// Normale kada je r1 != r2
			CalculateConeNormals(h, r, 0, alpha, nx, ny, nz);


			glNormal3f(nx, ny, nz);

			// planarno:
			glTexCoord2d(tx, ty);
			// sferno: glTexCoord2d(tx, tyTop); 
			glVertex3d(x, yBot, z);

			// planarno:
			glTexCoord2d(texU2, texV2);
			//sferno: glTexCoord2d(tx, tyBot); 
			glVertex3d(0, yTop, 0);
		}
	}
	glEnd();
}

void CGLRenderer::DrawCubeFace(double a)
{
	double aHalf = a / 2.0;

	glBindTexture(GL_TEXTURE_2D, glTex);
	glColor3i(1, 1, 1);

	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
	{
		// Top right
		glTexCoord2f(1, 0);
		glVertex3d(aHalf, aHalf, 0);

		// Top left
		glTexCoord2f(0, 0);
		glVertex3d(-aHalf, aHalf, 0);

		// Bot left
		glTexCoord2f(0, 1);
		glVertex3d(-aHalf, -aHalf, 0);

		// Bot right
		glTexCoord2f(1, 1);
		glVertex3d(aHalf, -aHalf, 0);
	}
	glEnd();
}

void CGLRenderer::DrawCubeFaceGrid(double a, int n) 
{
	double aHalf = a / 2.0;
	double step = a / (double)n;

	glBindTexture(GL_TEXTURE_2D, glTex);
	glColor3i(1, 1, 1);
	
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
	{
		for (int i = 0; i < n; i++) {
			double x0 = step * i - aHalf;
			double x1 = x0 + step;

			double tx0 = (double)i / n;
			double tx1 = (double)(i + 1) / n;

			for (int j = 0; j < n; j++) {
				double y0 = step * j - aHalf;
				double y1 = y0 + step;


				double ty0 = 1 - (double)j / n;
				double ty1 = 1 - (double)(j + 1) / n;

				glTexCoord2d(tx0, ty0);
				glVertex3d(x0, y0, 0);

				glTexCoord2d(tx1, ty0);
				glVertex3d(x1, y0, 0);

				glTexCoord2d(tx1, ty1);
				glVertex3d(x1, y1, 0);

				glTexCoord2d(tx0, ty1);
				glVertex3d(x0, y1, 0);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawCube(double a)
{
	double aHalf = a / 2.0;
	// DrawCubeFace je po default-u "front" strana

	// Front
	glPushMatrix();
	{
		glTranslated(0, 0, aHalf);
		DrawCubeFace(a);
	}
	glPopMatrix();

	// Back
	glPushMatrix();
	{
		glTranslated(0, 0,- aHalf);
		glRotated(180, 0, 1, 0);
		DrawCubeFace(a);
	}
	glPopMatrix();

	// Right
	glPushMatrix();
	{
		glTranslated(aHalf, 0, 0);
		glRotated(90, 0, 1, 0);
		DrawCubeFace(a);
	}
	glPopMatrix();

	// Left
	glPushMatrix();
	{
		glTranslated(-aHalf, 0, 0);
		glRotated(-90, 0, 1, 0);
		DrawCubeFace(a);
	}
	glPopMatrix();

	// Top
	glPushMatrix();
	{
		glTranslated(0, aHalf, 0);
		glRotated(-90, 1, 0, 0);
		DrawCubeFace(a);
	}
	glPopMatrix();

	// Bot
	glPushMatrix();
	{
		glTranslated(0, -aHalf, 0);
		glRotated(90, 1, 0, 0);
		DrawCubeFace(a);
	}
	glPopMatrix();
}

void CGLRenderer::DrawCubeGrid(double a, int n)
{
	double aHalf = a / 2.0;
	// DrawCubeFace je po default-u "front" strana

	// Front
	glPushMatrix();
	{
		glTranslated(0, 0, aHalf);
		DrawCubeFaceGrid(a, n);
	}
	glPopMatrix();

	// Back
	glPushMatrix();
	{
		glTranslated(0, 0, -aHalf);
		glRotated(180, 0, 1, 0);
		DrawCubeFaceGrid(a, n);
	}
	glPopMatrix();

	// Right
	glPushMatrix();
	{
		glTranslated(aHalf, 0, 0);
		glRotated(90, 0, 1, 0);
		DrawCubeFaceGrid(a, n);
	}
	glPopMatrix();

	// Left
	glPushMatrix();
	{
		glTranslated(-aHalf, 0, 0);
		glRotated(-90, 0, 1, 0);
		DrawCubeFaceGrid(a, n);
	}
	glPopMatrix();

	// Top
	glPushMatrix();
	{
		glTranslated(0, aHalf, 0);
		glRotated(-90, 1, 0, 0);
		DrawCubeFaceGrid(a, n);
	}
	glPopMatrix();

	// Bot
	glPushMatrix();
	{
		glTranslated(0, -aHalf, 0);
		glRotated(90, 1, 0, 0);
		DrawCubeFaceGrid(a, n);
	}
	glPopMatrix();
}

void CGLRenderer::DrawRect(float a, float b, int n)
{
	double aHalf = a / 2.0;
	double aStep = a / (double)n;

	double bHalf = b / 2.0;
	double bStep = b / (double)n;

	glBindTexture(GL_TEXTURE_2D, glTex);

	glColor3i(1, 1, 1);
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
	{
		for (int i = 0; i < n; i++) {
			float x0 = aStep * i - aHalf;
			float x1 = x0 + aStep;

			// teksturna x koordinata
			float tx0 = (float)i / n;
			float tx1 = (float)(i + 1) / n;

			for (int j = 0; j < n; j++) {
				double y0 = bStep * j - bHalf;
				double y1 = y0 + bStep;


				double ty0 = 1 - (double)j / n;
				double ty1 = 1 - (double)(j + 1) / n;

				glTexCoord2d(tx0, ty0);
				glVertex3d(x0, y0, 0);

				glTexCoord2d(tx1, ty0);
				glVertex3d(x1, y0, 0);

				glTexCoord2d(tx1, ty1);
				glVertex3d(x1, y1, 0);

				glTexCoord2d(tx0, ty1);
				glVertex3d(x0, y1, 0);
			}
		}
	}
	glEnd();
}

void CGLRenderer::DrawPrism(float a, float b, float c, int n)
{
	// Mozemo da tretiramo DrawRect kao provougaonik koje je na 'front' strani
	// On lazi na XY ravan i "gleda" prema +z osi

	// Ja sam strane kvadra rotirao zbog normale osvetljenja
	// Ako ih ne rotiramo, onda ce neke normale da gledaju ka unutrasnjosti kvadra umesto spolje
	// U ovom zadatku to verovatno nema potrebe jer se i unutrasnja i spoljasnja strana nekog poligona osvetljava

	float aHalf = a / 2;
	float bHalf = b / 2;
	float cHalf = c / 2;

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
	glPushMatrix();
	{
		glTranslatef(aHalf, 0, 0);
		glRotatef(90, 0, 1, 0);
		DrawRect(b, c, n);
	}
	glPopMatrix();


	// Top
	glPushMatrix();
	{
		glTranslatef(0, cHalf, 0);
		glRotatef(-90, 1, 0, 0);
		DrawRect(a, b, n);
	}
	glPopMatrix();

	// Bot
	glPushMatrix();
	{
		glTranslatef(0, -cHalf, 0);
		glRotatef(90, 1, 0, 0);
		DrawRect(a, b, n);
	}
	glPopMatrix();
}

#pragma endregion

#pragma region Rotacija pogleda

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

#pragma endregion
