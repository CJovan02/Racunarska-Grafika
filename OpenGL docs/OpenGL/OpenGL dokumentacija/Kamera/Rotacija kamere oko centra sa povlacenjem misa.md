U view klasi moramo da da pokupimo informacije o povlacenju misa kada se klikne i drzi taster, takodje mozemo i da pokupimo informacije o skrolovanju da bi priblizavali i udaljavali kameru od objekta.

```c++
void CGLKView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);

	m_lastPos = point;
	m_dragging = true;
}
```

```c++
void CGLKView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);

	m_dragging = false;
}
```

```c++
void CGLKView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);

	if (m_dragging)
	{
		int deltaX = point.x - m_lastPos.x;
		int deltaY = point.y - m_lastPos.y;

		double sensitivity = 1 / 5.0;

		m_glRenderer.RotateView(deltaY * sensitivity, deltaX * sensitivity);
		Invalidate();
	}
	m_lastPos = point;
}
```

Ukoliko zelimo da zumiramo scenu:
```c++
BOOL CGLKView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_glRenderer.ZoomView(zDelta > 0);
	Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
```

```c++
void CGLRenderer::ZoomView(bool zoomIn)
{
	double zoomSensitivity = 0.5;

	viewR += zoomIn ? -zoomSensitivity : zoomSensitivity;

	// Clamp
	viewR = max(1, viewR);
	viewR = min(1000, viewR);
	
	CalculatePosition();
}
```

U GLRenderer moramo na osnovu deltaX i deltaY (razlika izmedju pozicije kada se otpusti i kada se prvi put zadrzi levi klik misa) da izracunamo rotaciju kamere.

Rekli samo da prva 3 parametra [[gluLookAt]] funkcije su koordinate odakle krece pogled (ili kamera) i to su koordinate koje moramo da sracunamo.

*ViewR* je radius sfere oko koje rotira kamera, odnosno udaljenost kamere od objekta.
```c++
CGLRenderer::CGLRenderer()
{
	m_hrc = NULL;
	this->viewR = 10;

	this->viewAngleXY = 0;
	this->viewAngleXZ = 0;

	eyex = 0, eyey = 0, eyez = 0;
	centerx = 0, centery = 0, centerz = 0;
	upx = 0, upy = 1, upz = 0;

	this->CalculatePosition();
}
```

Imamo XY i XZ ugao (XY je vertikalna rotacija a XZ horizontalna), s tim sto kod XY, odnosno vertikalne rotacije imamo sledeci problem:
Kada ugao predje 90 ili -90 kamera se okrene naopacke ali 'UP' pravac kamere ostaje isti (ovo se podesava u [[gluLookAt]] funkciji) i ako ne zamenimo 'UP' pravac cela scena ce se invertovati.

Prvo resenje je da 'clampujemo' ugao da ne prelazi 90 i -90 stepeni.
Drugo resenje je da, kada ugao predje 90 ili -90, 'UP' pravac kamere se menja.
```c++
void CGLRenderer::RotateView(double dXY, double dXZ)
{
	viewAngleXY += dXY;

	// Clamp
	viewAngleXY = min(90, viewAngleXY);
	viewAngleXY = max(-90, viewAngleXY);

	viewAngleXZ += dXZ;
	CalculatePosition();
}
```

Ovo je formula za crtanje sfere, formula je data na CS-u. Putanja po kojoj se kamera rotira oko neke tacke je zapravo sfera. ([[Crtanje Sfere]])
```c++
void CGLRenderer::CalculatePosition()
{
	double dWXY = this->viewAngleXY * 3.14 / 180,
		   dWXZ = this->viewAngleXZ * 3.14 / 180;

	this->eyex = this->viewR * cos(dWXY) * cos(dWXZ);
	this->eyey = this->viewR * sin(dWXY);
	this->eyez = this->viewR * cos(dWXY) * sin(dWXZ);

	this->upy = signbit(cos(dWXY)) ? -1 : 1;
}
```

U [[DrawScene]] ubacimo ovo:
```c++
gluLookAt(eyex, eyey, eyez,
		  centerx, centery, centerz,
		  upx, upy, upz);
```