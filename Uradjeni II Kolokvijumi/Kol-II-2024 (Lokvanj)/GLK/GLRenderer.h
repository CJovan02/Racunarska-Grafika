#pragma once

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	UINT LoadTexture(char* fileName);

	void DrawSphFlower(float R, int n, int m, float factor, unsigned
		char R1, unsigned char G1, unsigned char B1, unsigned char R2, unsigned char G2,
		unsigned char B2);

	void DrawAxes();
	void PolarToCartesian(double R, double phi, double theta, double& x, double& y, double& z);
	void CalcRotAxis(double x1, double y1, double z1, double x2, double y2, double z2, double& x3, double& y3, double& z3);
	void DrawSphere(float R, int n, int m);

	void CalculateCameraPosition();
	void RotateCamera(double dXY, double dXZ);
	void ZoomCamera(bool zoomIn);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	double eyex = 10, eyey = 10, eyez = 10;
	double centerx = 0, centery = 0, centerz = 0;
	double viewAngleXY = 0, viewAngleXZ = 0, viewR = 20;

	UINT envTex;

public:
	double rotationFactor = 0;
};
