Kocku crtamo tako sto napravimo funkciju koja crta jednu strane kocke zajedno sa **normalama** te strane i zatim u glavnoj funkciji rotairamo i transliramo svaku stranu posebno. [[Crtanje Kocke]]

Ova funkcija crta jednu stranu kocke, podesava normale te strane i crta te iste normale.
Za ovaj primer sam stavio da su normale podesene tako da su u smeru pozitivnog dela Z ose.
```c++
void CGLRenderer::DrawCubeFace(double a)
{
	double aHalf = a / 2;

	vector<tuple<double, double, double, double, double, double>> normals;
	double nx = 0, ny = 0, nz = 1;

	glNormal3d(nx, ny, nz);
	glColor3d(1, 0, 1);
	glBegin(GL_QUADS);
	{
		glVertex3d(aHalf, -aHalf, 0);
		normals.emplace_back(aHalf, -aHalf, 0, nx, ny, nz);

		glVertex3d(aHalf, aHalf, 0);
		normals.emplace_back(aHalf, aHalf, 0, nx, ny, nz);

		glVertex3d(-aHalf, aHalf, 0);
		normals.emplace_back(-aHalf, aHalf, 0, nx, ny, nz);

		glVertex3d(-aHalf, -aHalf, 0);
		normals.emplace_back(-aHalf, -aHalf, 0, nx, ny, nz);
	}
	glEnd();

	DrawNormals(normals);
}
```
![[Strana kocke u opengl-u.png]]

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
```

![[Kocka u opengl-u.png]]

Ovo je primer kada crtamo kocku kao geometrijsko telo.

Ako zelimo da napravimo takoznvani **"Environment Cube"**, odnosno da scenu stavimo u kocki, onda bi bilo logicno da se svetlo odbija **unutar kocke**, odnosno normale moraju da budu ka unutrasnosti kocke.

To se u nasem primeru postize tako sto stavimo da vektor normale u funkciji za crtanje strane bude **(0, 0, -1) umesto (0, 0, 1).**