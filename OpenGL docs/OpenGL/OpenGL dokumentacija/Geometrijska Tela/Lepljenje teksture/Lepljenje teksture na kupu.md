Ovo je slika koja se sastoji od 3 teksture. Mi moramo da "odsecemo" jedan deo slike i da ga zalepimo na neko geometrijsko telo.

![[Pauk tekstura.png]]

### **tx = texR * x / r + texU**  
### **ty = texR * z / r + texV**  

## Objasnjenje formule -> [[Preslikavanje 3D tacke na 2D teksturu]]

![[Lepljenje teksture na kupu.png]]

[[Crtanje Sfere]]

```c++
void CGLRenderer::DrawSphere(double r, int nSeg, double texU, double texV, double texR)
{
	// Longituda
	int alphaStep = 180 / (nSeg * 2);
	// Latituda
	int betaStep = 360 / nSeg;

	glColor3d(1, 1, 1);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, m_texSpider);
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int beta = 0; beta <= 360; beta += betaStep)
		{
			for (int alpha = -90; alpha <= 90; alpha += alphaStep)
			{
				double x1 = r * cos(TO_RAD(alpha)) * cos(TO_RAD(beta));
				double y1 = r * sin(TO_RAD(alpha));
				double z1 = r * cos(TO_RAD(alpha)) * sin(TO_RAD(beta));

				int beta2 = beta + betaStep;
				double x2 = r * cos(TO_RAD(alpha)) * cos(TO_RAD(beta2));
				double y2 = r * sin(TO_RAD(alpha));
				double z2 = r * cos(TO_RAD(alpha)) * sin(TO_RAD(beta2));

				// Lepljenje teksture preko sfere
				double tx1 = texR * x1 / r + texU;
				double ty1 = texR * z1 / r + texV;

				double tx2 = texR * x2 / r + texU;
				double ty2 = texR * z2 / r + texV;

				glTexCoord2f(tx1, ty1);
				glVertex3d(x1, y1, z1);

				glTexCoord2f(tx2, ty2);
				glVertex3d(x2, y2, z2);
			}
		}
	}
	glEnd();
}
```