## Planarno mapiranje

#### Teorijsko objasnjenje -> [[Planarno (projekciono) mapiranje]]

Ovo je slika koja se sastoji od 3 teksture. Mi moramo da "odsecemo" jedan deo slike i da ga zalepimo na neko geometrijsko telo.

![[Pauk tekstura.png]]

### **tx = texR * x / r + texU**  
### **ty = texR * z / r + texV**

## Objasnjenje formule -> [[Preslikavanje 3D tacke na 2D teksturu]]

Mislim da se ovo u matematici zove **planarno (projekciono) mapiranje**.
Zapravo se uzima projekcija tacke na neku od ravan (XZ ili XY) i koristi se linearna skala da se projekcija pretvori teksturnu koordinatu.

Ovakvo mapiranje se koristi kod za baze kod kupe i cilindra

![[Lepljenje teksture na sferu.png]]

[[Crtanje Sfere]]

```c++
void CGLRenderer::DrawSphere(double r, int nSeg, double texU, double texV, double texR)
{
	// Longituda
	double alphaStep = PI / (double) (nSeg * 2);
	// Latituda
	double betaStep = 2 * PI / (double) nSeg;

	glColor3d(1, 1, 1);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, m_texSpider);
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int i = 0; i < nSeg; i++)
		{
			double beta = betaStep * i;

			for (int j = 0; j <= nSeg * 2; j++)
			{
				double alpha = (alphaStep * j) - PI / 2;

				double x1 = r * cos(alpha) * cos(beta);
				double y1 = r * sin(alpha);
				double z1 = r * cos(alpha) * sin(beta);

				double beta2 = beta + betaStep;
				double x2 = r * cos(alpha) * cos(beta2);
				double y2 = r * sin(alpha);
				double z2 = r * cos(alpha) * sin(beta2);

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

## Sferno mapiranje
#### Teorijsko objasnjenje -> [[Sferno (polarno) mapiranje]]

Ovo uglavnom koristimo kada je lopda zapravo env, odnosno predstavlja "pozadinu" scene.
Uglavnom, tekstura se ucitava kao da se "obmotava" oko sfere, umesto da se samo zalepi odozgo projekcijom.

```c++
void CGLRenderer::DrawSphere(float R, int n, int m)
{
	double alphaStep = PI / (double)m;
	double betaStep = 2 * PI / (double)n;

	glColor3f(1, 1, 1);

	for (int i = 0; i < n; i++) {
		double beta = betaStep * i;
		double betaNext = beta + betaStep;

		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= m; j++) {
			double alpha = (alphaStep * j) - PI / 2;
			double alphaNext = alpha + alphaStep;

			double x1 = R * cos(alpha) * cos(beta);
			double y1 = R * sin(alpha);
			double z1 = R * cos(alpha) * sin(beta);

			double x2 = R * cos(alpha) * cos(betaNext);
			double y2 = R * sin(alpha);
			double z2 = R * cos(alpha) * sin(betaNext);

			double u1 = beta / (2 * PI);
			double v1 = (-alpha + PI / 2) / PI;

			double u2 = betaNext / (2 * PI);
			double v2 = v1;

			glTexCoord2d(u1, v1);
			glVertex3d(x1, y1, z1);

			glTexCoord2d(u2, v2);
			glVertex3d(x2, y2, z2);
		}
		glEnd();
	}
}
```

**(-alpha)** se koristi, jer naravno, slika je ucitana naglavacke.