Ovo je slika koja se sastoji od 3 teksture. Mi moramo da "odsecemo" jedan deo slike i da ga zalepimo na neko geometrijsko telo.

![[Pauk tekstura.png]]

### **tx = texR * x / r + texU**  
### **ty = texR * z / r + texV**  

## Objasnjenje formule -> [[Preslikavanje 3D tacke na 2D teksturu]]

![[Lepljenje teksture na sferu.png]]

[[Crtanje Kupe]]

```c++
void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR)
{
	int alphaStep = 360 / nSeg;

	glColor3d(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texSpider);

	// Omotac
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			double x = r * cos(TO_RAD(alpha));
			double z = r * sin(TO_RAD(alpha));


			double tx = x / r * texR + texU;
			double ty = z / r * texR + texV;

			glTexCoord2d(tx, ty);
			glVertex3d(x, 0, z);

			glTexCoord2d(texU, texV);
			glVertex3d(0, h, 0);
		}
	}
	glEnd();
	
	// Baza
	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2d(texU, texV);
		glVertex3d(0, 0, 0);
		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			double x = r * cos(TO_RAD(alpha));
			double z = r * sin(TO_RAD(alpha));

			double tx = x / r * texR + texU;
			double ty = z / r * texR + texV;

			glTexCoord2d(tx, ty);
			glVertex3d(x, 0, z);
		}
	}
	glEnd();
}
```