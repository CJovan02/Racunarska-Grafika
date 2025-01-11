
Crtanje se radi tako sto se specificiraju koordinate tacaka i nacin na koji se one povezuju.

Primer crtanja linija oko koordinatnog pocetka
```c++
void CGLRenderer::DrawAxis(double width)
{
	glLineWidth(1);
	glPointSize(10);

	glBegin(GL_LINES);
	{
		glColor3d(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(width, 0, 0);

		glColor3d(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(0, width, 0);

		glColor3d(0, 0, 1);
		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, width);
	}
	glEnd();
}
```

[[Crtanje objekata u OpenGL-u]]

