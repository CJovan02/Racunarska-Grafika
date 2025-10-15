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

	void DrawAxes();

	void DrawEnvCube(double a);
	void DrawCubeFace(double a, UINT texID);

	void DrawSphere(double r, int nSeg, double texU, double texV, double texR);
	void DrawCone(double r, double h, int nSeg, double texU, double texV, double texR);

	void DrawLegSegment(double r, double h, int nSeg);
	void DrawLeg();
	void DrawSpiderBody();
	void DrawSpider();

	void CalculatePosition();
	void ZoomView(bool zoomIn);
	void RotateView(double dXY, double dXZ);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	double eyex = 0, eyey = 0, eyez = 0;
	double centerx = 0, centery = 0, centerz = 0;
	double upx = 0, upy = 1, upz = 0;

	double viewAngleXY = 0, viewAngleXZ = 0;

	double viewR = 30;

	// teksture
	UINT m_texSpider;
	UINT m_texEnv[6];

};
