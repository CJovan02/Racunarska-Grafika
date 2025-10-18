## Planarno mapiranje

#### Objasnjenje planarnog mapiranja - [[Planarno (projekciono) mapiranje]]

#### Objasnjenje polarnog mapiranja - [[Sferno (polarno) mapiranje]]



Koristi se **planarno mapiranje** (maltene projekcija na neku ravan da bi se dobile teksturne koordinate) da se mapira baza kupe.

Za omotac se isto koristi planarano, ali moze ja mislim da za omotac moze i polarno mapiranje da se koristi ([[Lepljenje teksture na sferu]] - ima primer polarnog mapiranja)

Ovo je slika koja se sastoji od 3 teksture. Mi moramo da "odsecemo" jedan deo slike i da ga zalepimo na neko geometrijsko telo.

![[Pauk tekstura.png]]

### **tx = texR * x / r + texU**  
### **ty = texR * z / r + texV**  

## Objasnjenje formule -> [[Preslikavanje 3D tacke na 2D teksturu]]

![[Lepljenje teksture na kupu.png]]

[[Crtanje Kupe]]
```c++
void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR)
{
	double alphaStep = 2 * PI / nSeg;

	glColor3d(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texSpider);

	// Omotac
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int i = 0; i <= nSeg; i++)
		{
			double alpha = alphaStep * i;
		
			double x = r * cos(alpha);
			double z = r * sin(alpha);


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
		for (int i = 0; i <= nSeg; i++)
		{
			double alpha = alphaStep * i;
			double x = r * cos(alpha);
			double z = r * sin(alpha);

			double tx = x / r * texR + texU;
			double ty = z / r * texR + texV;

			glTexCoord2d(tx, ty);
			glVertex3d(x, 0, z);
		}
	}
	glEnd();
}
```
