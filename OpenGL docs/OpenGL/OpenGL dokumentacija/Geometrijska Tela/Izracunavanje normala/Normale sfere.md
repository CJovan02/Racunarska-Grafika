![[Normale sfere.png]]

```c++
void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta, bool onlyUpperHalf)
{
	int alphaStep = 180 / nSegAlpha;
	int betaStep = 360 / nSegBeta;

	glBegin(GL_TRIANGLE_STRIP);
	for (int beta = 0; beta <= 360; beta += betaStep)
	{
		for (int alpha = onlyUpperHalf ? 0 : -90; alpha <= 90; alpha += alphaStep)
		{
			double alphaRad = TO_RAD(alpha);
			double betaRad = TO_RAD(beta);

			double x = cos(alphaRad) * cos(betaRad);
			double y = sin(alphaRad);
			double z = cos(alphaRad) * sin(betaRad);

			double betaNextRad = TO_RAD(beta) + TO_RAD(betaStep);
			
			double x2 = cos(alphaRad) * cos(betaNextRad);
			double y2 = sin(alphaRad);
			double z2 = cos(alphaRad) * sin(betaNextRad);

			glNormal3d(x, y, z);
			glVertex3d(r * x, r * y, r * z);

			glNormal3d(x2, y2, z2);
			glVertex3d(r * x2, r * y2, r * z2);
		}
	}
	glEnd();
}
```

Kada se iscrtaju normale to izgleda ovako:
![[Normale Sfere u openGL-u.png]]