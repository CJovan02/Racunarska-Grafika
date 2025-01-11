#pragma once

class CGLRenderer
{

#define POZADINA 0.50, 0.75, 1.00
#define GRID_COLOR 0.00, 0.80, 0.00
#define CACTUS_SPHERE_COLOR 0.47, 0.00, 0.00
#define CACTUS_PART_COLOR 1.00, 0.60, 0.00
#define CACTUS_ROTATE_PART_COLOR 1, 1, 0
#define VASE_COLOR 1, 1, 1

public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);

	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawAxis(float width);

	void RotateView(double dXY, double dXZ);

	void CalculatePosition();

	void DrawGrid(double width, double height, int nSegW, int nSegH);

	void DrawSphere(double r, int nSegAlpha, int nSegBeta);

	void DrawCylinder(double h, double r1, double r2, int nSeg);

	void DrawCone(double h, double r, int nSeg);

	void DrawFigure(double angle);


protected:
	HGLRC m_hrc; //OpenGL Rendering Context 
public:
	double viewAngleXY;
	double viewAngleXZ;
	double viewR;
	float cactusPartAngle;

	double eyex, eyey, eyez;
	double centerx, centery, centerz;
	double upx, upy, upz;


};