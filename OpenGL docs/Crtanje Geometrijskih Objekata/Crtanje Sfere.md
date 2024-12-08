
![[Sfera formula.png]]

Imamo dva nacina da crtamo sferu, mrzi me da objasnjvam vidi se slike o cemu se radi.

**Napomena: ovde se koristi GL_LINES kako bi se bolje videlo o cemu se radi. Kada se iskoristi GL_TRIANGLE_STRIP povrsina sfere se popuni. Moze da se koristi GL_QUAD_STRIP ali je sa trouglom efikasnije.**

## Crtanje sfere po horizintali
```c++
void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	int alphaStep = 360 / nSegAlpha;
	int betaStep = 360 / nSegBeta;

	glColor3d(1, 1, 1);
	glBegin(GL_LINES);
	{

		for (int beta = 0; beta <= 360; beta += betaStep)
		{
			for (int alpha = -90; alpha <= 90; alpha += alphaStep)
			{
				double x = r * cos(TO_RAD(alpha)) * cos(TO_RAD(beta));
				double y = r * sin(TO_RAD(alpha));
				double z = r * cos(TO_RAD(alpha)) * sin(TO_RAD(beta));

				double x2 = r * cos(TO_RAD(alpha)) * cos(TO_RAD(beta) + TO_RAD(betaStep));
				double y2 = r * sin(TO_RAD(alpha));
				double z2 = r * cos(TO_RAD(alpha)) * sin(TO_RAD(beta) + TO_RAD(betaStep));

				glVertex3d(x, y, z);
				glVertex3d(x2, y2, z2);
			}
		}
	}
	glEnd();
}
```

![[Sfera horizontalno.png]]

## Crtanje sfere po vertikali

```c++
void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	int alphaStep = 360 / nSegAlpha;
	int betaStep = 360 / nSegBeta;

	glColor3d(1, 1, 1);
	glBegin(GL_LINES);
	{

		for (int alpha = -90; alpha <= 90; alpha += alphaStep)
		{
			for (int beta = 0; beta <= 360; beta += betaStep)
			{

				double x = r * cos(TO_RAD(alpha)) * cos(TO_RAD(beta));
				double y = r * sin(TO_RAD(alpha));
				double z = r * cos(TO_RAD(alpha)) * sin(TO_RAD(beta));

				double x2 = r * cos(TO_RAD(alpha) + TO_RAD(alphaStep)) * cos(TO_RAD(beta));
				double y2 = r * sin(TO_RAD(alpha) + TO_RAD(alphaStep));
				double z2 = r * cos(TO_RAD(alpha) + TO_RAD(alphaStep)) * sin(TO_RAD(beta));

				glVertex3d(x, y, z);
				glVertex3d(x2, y2, z2);
			}
		}
	}
	glEnd();
}
```

![[Pasted image 20241208141825.png]]

Jedina razlika je sto petlje zamenjuju **(jer kada se kasnije pozove sa GL_TRIAGLE_STRIP nece lepo da se popuni sfera)** i koja tacka se sledeca uzima:
- Po horizontali se uzme sledeca tacka kada se poveca **beta ugao,**
- Po vertikali se uzme sledeca tacka kada se poveca **alpha ugao.**