#include <vector>
#include <tuple>
#include "CGLMaterial.h"

using namespace std;

#pragma once

class CGLRenderer
{

#define POZADINA (128 / 255.0), (192 / 255.0), (255 / 255.0)

public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void InitMaterials();

	void InitView();

	void ConfigureLightSources();

	void ConfigureLightPositions();

	void SetLightPositionAndDirection(GLenum light, tuple<double, double, double> position, tuple<double, double, double> direction, bool lightOn, tuple<double, double, double> lightCol);

	void DrawLight(tuple<double, double, double> position, bool lightOn, tuple<double, double, double> color, double r = 1);

	void DrawNormals(vector<tuple<double, double, double, double, double, double>> normals, double scale = 0.5);

	void DrawAxis(double width);

	void DrawSphere(double r, int nSegAlpha, int nSegBeta, bool onlyUpperHalf = false);

	void DrawCylinder(double h, double r1, double r2, int nSeg, bool includeBase = true);

	void DrawEnvCube(double a);

	void DrawCubeFace(double a);

	tuple<double, double> CalculateCylinderNormals(double h, double r1, double r2);

	void RotateView(double dXY, double dXZ);

	void ZoomView(bool zoomIn);

	void CalculatePosition();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

public:
	double viewAngleXY, viewAngleXZ, viewR;

	double eyex, eyey, eyez;
	double centerx, centery, centerz;
	double upx, upy, upz;

	bool drawNormals = false;

	CGLMaterial envCubeMat, redMat, greenMat, blueMat, whiteMat;

	tuple<double, double, double> light0Col, light1Col, light2Col, light3Col;

	bool light0On = true, light1On = true, light2On = true, light3On = true;
};
