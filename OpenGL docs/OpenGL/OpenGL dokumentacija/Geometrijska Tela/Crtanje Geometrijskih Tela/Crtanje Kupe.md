![[Kupa Formula.png]]

```c++
void CGLRenderer::DrawCone(double h, double r, int nSeg)
{
	int alphaStep = 360 / nSeg;
	
	// Omotac kupe
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			double x = r * cos(TO_RAD(alpha));
			double z = r * sin(TO_RAD(alpha));

			glVertex3d(x, 0, z);
			glVertex3d(0, h, 0);
		}

		// Omotac se ne zatvori do kraja pa mozemo pocetku tacku da hardkodiramo
		glVertex3d(r, 0, 0);  // (x = r, z = 0 at alpha = 0)
		glVertex3d(0, h, 0); 
	}
	glEnd();

	// Baza kupe
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3d(0, 0, 0);

		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			double x = r * cos(TO_RAD(alpha));
			double z = r * sin(TO_RAD(alpha));
			glVertex3d(x, 0, z);
		}
	}
	glEnd();
}

```