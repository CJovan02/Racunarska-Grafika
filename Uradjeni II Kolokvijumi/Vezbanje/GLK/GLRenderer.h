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

	void SetLightningParams();

	void SetLightParams(int glLight);

	void SetMaterial(float diffuse[]);

	void DrawAxes();

	void PolarToCartesian(double R, double phi, double theta, double& x, double& y, double& z);

	void DrawSphere(double r, int n, int m);

	void DrawCylinder(double r1, double r2, double h, int nSeg);

	void DrawCone(double r, double h, int nSeg);

	void DrawCubeFace(double a);

	void DrawCubeFaceGrid(double a, int n);

	void DrawCube(double a);

	void DrawCubeGrid(double a, int n);

	void DrawRect(float a, float b, int n);

	void DrawPrism(float a, float b, float c, int n);

	void CalculateConeNormals(double h, double rBot, double rTop, double alphaRad, double& nx, double& ny, double& nz);

	void RotateCamera(double dX, double dY);
	void ZoomView(bool zoomIn);
	void CalculateCameraPosition();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	double viewAngleXY = 0, viewAngleXZ = 0, viewR = 10;
	double eyex = 10, eyey = 10, eyez = 10;
	double centerx = 0, centery = 0, centerz = 0;

	UINT spiderTex, glTex;

};
