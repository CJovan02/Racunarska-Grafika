```c++
void CGLRenderer::DrawEnvCube(double a)
{
	double aHalf = a / 2;

	// Right
	glPushMatrix();
	{
		glTranslated(0, 0, aHalf);
		DrawCubeFace(a, texSide);
	}
	glPopMatrix();

	// Left
	glPushMatrix();
	{
		glTranslated(0, 0, -aHalf);
		glRotated(180, 0, 1, 0);
		DrawCubeFace(a, texSide);
	}
	glPopMatrix();

	// Front
	glPushMatrix();
	{
		glTranslated(-aHalf, 0, 0);
		glRotated(-90, 0, 1, 0);
		DrawCubeFace(a, texSide);
	}
	glPopMatrix();

	// Back
	glPushMatrix();
	{
		glTranslated(aHalf, 0, 0);
		glRotated(90, 0, 1, 0);
		DrawCubeFace(a, texSide);
	}
	glPopMatrix();

	// Top
	glPushMatrix();
	{
		glTranslated(0, aHalf, 0);
		glRotated(-90, 1, 0, 0);
		DrawCubeFace(a, texTop);
	}
	glPopMatrix();

	// Bot
	glPushMatrix();
	{
		glTranslated(0, -aHalf, 0);
		glRotated(90, 1, 0, 0);
		DrawCubeFace(a, texBot);
	}
	glPopMatrix();
}
```