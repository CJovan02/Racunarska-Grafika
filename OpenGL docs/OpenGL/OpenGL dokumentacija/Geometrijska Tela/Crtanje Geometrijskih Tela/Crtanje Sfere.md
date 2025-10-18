
![[Sfera formula.png]]

Imamo dva nacina da crtamo sferu, mrzi me da objasnjvam vidi se slike o cemu se radi.

**Napomena: ovde se koristi GL_LINES kako bi se bolje videle linije, radi demonstracije.
Kada se iskoristi GL_TRIANGLE_STRIP povrsina sfere se popuni. Moze da se koristi GL_QUAD_STRIP ali je sa trouglom efikasnije.**

**Jos jedna napomena:**
Na jednom kolkovijumu (2024) se trazilo da se napise funkcija za kovertovanje polarnih u dekartove koordinate.
U sustini to je ova formula sa gornje slike (ako sam ja lepo razumeo).

```c++
void CGLRenderer::PolarToCartesian(double R, double phi, double theta, double& x, double& y, double& z)
{
	// phi je po onoj slici sa cs-a alpha ugao (-90 do 90)
	// theta je po slici sa cs-a beta ugao (0 do 360)

	x = R * cos(phi) * cos(theta);
	y = R * sin(phi);
	z = R * cos(phi) * sin(theta);
}
```

Kad se trazi ova funkcija, umesto da se unutar funkcije za crtanje sfere rucno pisu formule, samo se pozove ova funkcija:
```c++
PolarToCartesian(R, alpha, beta, x1, y1, z1);
PolarToCartesian(R, alpha, betaNext, x2, y2, z2);
```

## Crtanje sfere po horizintali (latitude)

```c++
void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	double alphaStep = PI / (double) nSegAlpha;
	double betaStep = 2 * PI / (double) nSegBeta;

	glBegin(GL_LINES);
	for (int i = 0; i < n; i++) {
		double beta = betaStep * i;

		for (int j = 0; j <= m; j++) {
			double alpha = (alphaStep * j) - PI / 2;

			double x1 = r * cos(alpha) * cos(beta);
			double y1 = r * sin(alpha);
			double z1 = r * cos(alpha) * sin(beta);

			double x2 = r * cos(alpha) * cos(beta + betaStep);
			double y2 = r * sin(alpha);
			double z2 = r * cos(alpha) * sin(beta + betaStep);

			glVertex3d(x1, y1, z1);
			glVertex3d(x2, y2, z2);
		}
	}
	glEnd();
}
```

Po horizontali se uzme sledeca tacka kada se poveca **beta ugao.**

![[Sfera horizontalno.png]]

## Crtanje sfere po vertikali (longitude)

```c++
void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	int alphaStep = 180 / nSegAlpha;
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

				double x2 = r * cos(TO_RAD(alpha) + TO_RAD(alphaStep)) * 
				   cos(TO_RAD(beta));
				double y2 = r * sin(TO_RAD(alpha) + TO_RAD(alphaStep));
				double z2 = r * cos(TO_RAD(alpha) + TO_RAD(alphaStep)) * 
				   sin(TO_RAD(beta));

				glVertex3d(x, y, z);
				glVertex3d(x2, y2, z2);
			}
		}
	}
	glEnd();
}
```

Po vertikali se uzme sledeca tacka kada se poveca **alpha ugao.**

![[Sfera vertikanlno.png]]

Jedina razlika je sto petlje zamenjuju **(jer kada se kasnije pozove sa GL_TRIAGLE_STRIP nece lepo da se popuni sfera)** i koja tacka se sledeca uzima:
- Po horizontali se uzme sledeca tacka kada se poveca **beta ugao,**
- Po vertikali se uzme sledeca tacka kada se poveca **alpha ugao.**