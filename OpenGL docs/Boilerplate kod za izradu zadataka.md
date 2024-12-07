```c++
void CGLRenderer::DrawAxis(float width)
{
	glLineWidth(1);
	glPointSize(10);

	glBegin(GL_LINES);
	{
		// za x osu
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(width, 0, 0);

		// za y osu
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, width, 0);

		// za z osu
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, width);
	}
	glEnd();
}
```