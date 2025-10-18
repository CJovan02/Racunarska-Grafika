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

	void DrawLamp();
	void DrawLampBase();
	void DrawLampArm();
	void DrawLampHead();

	void DrawAxes();
	void DrawEnvCube(double a);
	void DrawCubeFace(double a, UINT texID);
	void DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool isOpen);

	UINT LoadTexture(char* fileName);

	/// <summary>
	/// na osnovu viewAngle racuna eyex, eyey i eyez
	/// </summary>
	void CalculatePosition();

	/// <summary>
	/// Na osnovu parametara koji se salju iz GDI aplikacije, racuna viewAngle i ostalu business logiku
	/// </summary>
	void RotateView(double dXY, double dXZ);

	void ZoomView(bool zoomIn);

	void UpdateCamera();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

	double centerx = 0, centery = 0, centerz = 0;
	double upx = 0, upy = 1, upz = 0;
	double viewR = 40;
	double eyex = 0, eyey = 0, eyez = 0;

	UINT texLamp, texSide, texTop, texBot;


public:
	double viewAngleXZ = 0, viewAngleXY = 0;

	// uglovi rotacije lampe
	double arm1Angle = -40, arm2Angle = 80, headAngle = -70;

	double m_CamBeta = 0, m_CamAlpha = 0;
};
