
![[Cilindar Formula.png]]

```c++
void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg)
{
	int alphaStep = 360 / nSeg;

	// Gornja osnova
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3d(0, h, 0);

		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			double x = r2 * cos(TO_RAD(alpha));
			double z = r2 * sin(TO_RAD(alpha));
			glVertex3d(x, h, z);
		}
	}
	glEnd();

	// Donja osnova
	glBegin(GL_LINES);
	{
		glVertex3d(0, 0, 0);

		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			double x = r1 * cos(TO_RAD(alpha));
			double z = r1 * sin(TO_RAD(alpha));
			glVertex3d(x, 0, z);
		}
	}
	glEnd();

	// Omotac
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			// Tacka gornje osnove
			double xGornje = r2 * cos(TO_RAD(alpha));
			double zGornje = r2 * sin(TO_RAD(alpha));

			double xDonje = r1 * cos(TO_RAD(alpha));
			double zDonje = r1 * sin(TO_RAD(alpha));

			glVertex3d(xGornje, h, zGornje);
			glVertex3d(xDonje, 0, zDonje);
		}
	}
	glEnd();
}

```