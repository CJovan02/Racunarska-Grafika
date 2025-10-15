Imamo 2 glavna nacina za crtanje kocke:

- **Navodjenjem svake tacke posebno**
```c++
void CGLRenderer::DrawCube(double a)
{
	double aHalf = a / 2;

	glBegin(GL_QUADS);
	{
		// Bottom
		glVertex3d(aHalf, 0, -aHalf);
		glVertex3d(-aHalf, 0, -aHalf);
		glVertex3d(-aHalf, 0, aHalf);
		glVertex3d(aHalf, 0, aHalf);

		// Top
		glVertex3d(aHalf, a, -aHalf);
		glVertex3d(-aHalf, a, -aHalf);
		glVertex3d(-aHalf, a, aHalf);
		glVertex3d(aHalf, a, aHalf);

		// Front
		glVertex3d(aHalf, 0, -aHalf);
		glVertex3d(aHalf, a, -aHalf);
		glVertex3d(aHalf, a, aHalf);
		glVertex3d(aHalf, 0, aHalf);

		// Back
		glVertex3d(-aHalf, 0, -aHalf);
		glVertex3d(-aHalf, a, -aHalf);
		glVertex3d(-aHalf, a, aHalf);
		glVertex3d(-aHalf, 0, aHalf);

		// Left
		glVertex3d(aHalf, 0, aHalf);
		glVertex3d(aHalf, a, aHalf);
		glVertex3d(-aHalf, a, aHalf);
		glVertex3d(-aHalf, 0, aHalf);

		// Right
		glVertex3d(aHalf, 0, -aHalf);
		glVertex3d(aHalf, a, -aHalf);
		glVertex3d(-aHalf, a, -aHalf);
		glVertex3d(-aHalf, 0, -aHalf);
	}
	glEnd();
}
```

- Koriscenjem funkcija transformacije: napravimo funkciju koja crta jednu stranu kocke. Zatim u glavnoj funkciji crtamo svaku stranu i koriscenjem transformacija ([[Transformacije u OpenGL-u]]) pomeramo i rotiramo stranu.
```c++
void CGLRenderer::DrawCube(double a)
{
	double aHalf = a / 2;

	// Bottom
	glPushMatrix();
	{
		glRotated(90, 1, 0, 0);
		DrawCubeFace(a);
	}
	glPopMatrix();

	// Top
	glPushMatrix();
	{
		glTranslated(0, a, 0);
		glRotated(-90, 1, 0, 0);
		DrawCubeFace(a);
	}
	glPopMatrix();

	// Front
	glPushMatrix();
	{
		glTranslated(aHalf, aHalf, 0);
		glRotated(90, 0, 1, 0);
		DrawCubeFace(a);
	}
	glPopMatrix();

	// Back
	glPushMatrix();
	{
		glTranslated(-aHalf, aHalf, 0);
		glRotated(-90, 0, 1, 0);
		DrawCubeFace(a);
	}
	glPopMatrix();

	// Left
	glPushMatrix();
	{
		glTranslated(0, aHalf, aHalf);
		DrawCubeFace(a);
	}
	glPopMatrix();

	// Right
	glPushMatrix();
	{
		glTranslated(0, aHalf, -aHalf);
		glRotated(180, 0, 1, 0);
		DrawCubeFace(a);
	}
	glPopMatrix();
}

void CGLRenderer::DrawCubeFace(double a)
{
	double aHalf = a / 2;

	glBegin(GL_QUADS);
	{
		glVertex3d(aHalf, -aHalf, 0);
		glVertex3d(aHalf, aHalf, 0);
		glVertex3d(-aHalf, aHalf, 0);
		glVertex3d(-aHalf, -aHalf, 0);
	}
	glEnd();

}
```
### NAPOMENA
Uglovi rotacije u gornjem primeru su takvi zbog normala kocke. 
Za vise informacija: [[Normale kocke]]