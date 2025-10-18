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

	void DrawAxes();
	void RotateCamera(double dX, double dY);
	void ZoomView(bool zoomIn);
	void CalculateCameraPosition();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	double viewR = 10, eyex = 10, eyey = 10, eyez = 10;
	double centerx = 0, centery = 0, centerz = 0;

	double viewAngleXZ = 0, viewAngleXY = 0;
};
