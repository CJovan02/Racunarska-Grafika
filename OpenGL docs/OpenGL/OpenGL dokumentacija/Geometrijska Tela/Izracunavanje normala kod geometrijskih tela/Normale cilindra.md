![[Normale cilindra.png]]

Ova formula vazi **samo kada su poluprecnici gornje i donje osnove isti**.
Ukoliko nisu isti onda se normale omotaca menjaju, domenstrirano je na sledecim slikama:

- *Kada su osnove iste*, koriscenjem gornje formule vidimo da je ovo dobro, odnosno da je formiran ugao od 90 stepeni izmedju **povrsine omotaca** i **linije normale.**

![[Normale cilindra u openGL-u 1.png]]

- *Kada osnove nisu iste* vidimo da ovo zapravo i nije normala, ne formira se ugao od 90 stepeni.
 ![[Normale cilindra u OpenGL-u 2.png]]


Da bi ovo resili postoji jos jedna formula za racunanje normala omotaca. Ova formula se takodje koristi i za omotac kupe. [[Normale kupe]]
![[Normale omotaca.png]]

Ugao teta nam zavisi od *r* kod kupe, dok kod cilindra imamo *r1* i *r2* gde ugao teta zavisi od njih.

Trougao cilinda moze lako da se pretvori u trougao dat kao kod kupe:
![[Pretvaranje trougla cilinda u trougao kupe]]

Ovom formulom se dobija ovakva slika:
![[Normale cilindra u opengl-u 3.png]]

```c++
void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg, bool includeBase)
{
	int alphaStep = 360 / nSeg;
	vector<tuple<double, double, double, double, double, double>> normals;

	if (includeBase)
	{
		// Gornja Osnova
		glBegin(GL_TRIANGLE_FAN);
		{
			double vx, vy = h, vz;
			double nx = 0, ny = 1, nz = 0;
			
			glNormal3d(nx, ny, nz);

			glVertex3d(0, vy, 0);

			for (int alpha = 0; alpha <= 360; alpha += alphaStep)
			{
				double vx = r2 * cos(TO_RAD(alpha));
				double vz = r2 * sin(TO_RAD(alpha));
				glVertex3d(vx, vy, vz);

				normals.emplace_back(vx, vy, vz, nx, ny, nz);
			}
		}
		glEnd();

		// Donja Osnova
		glBegin(GL_TRIANGLE_FAN);
		{
			double vx, vy = 0, vz;
			double nx = 0, ny = -1, nz = 0;

			glNormal3d(nx, ny, nz);

			glVertex3d(0, 0, 0);

			for (int alpha = 0; alpha <= 360; alpha += alphaStep)
			{
				double vx = r1 * cos(TO_RAD(alpha));
				double vz = r1 * sin(TO_RAD(alpha));
				glVertex3d(vx, vy, vz);

				normals.emplace_back(vx, vy, vz, nx, ny, nz);
			}
		}
		glEnd();
	}

	double nr, ny;
	tie(nr, ny) = CalculateCylinderNormals(h, r1, r2);

	// Omotac
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int alpha = 0; alpha <= 360; alpha += alphaStep)
		{
			double alphaRad = TO_RAD(alpha);

			double nx = nr * cos(alphaRad);
			double nz = nr * sin(alphaRad);

			glNormal3d(nx, ny, nz);

			double vxDown = r1 * cos(alphaRad),
				   vyDown = 0,
				   vzDown = r1 * sin(alphaRad);

			glVertex3d(vxDown, vyDown, vzDown);
			normals.emplace_back(vxDown, vyDown, vzDown, nx, ny, nz);

			double vxUp = r2 * cos(alphaRad),
				   vyUp = h,
				   vzUp = r2 * sin(alphaRad);

			glVertex3d(vxUp, vyUp, vzUp);
			normals.emplace_back(vxUp, vyUp, vzUp, nx, ny, nz);

		}
	}
	glEnd();

	DrawNormals(normals);
}
```

```c++
tuple<double, double> CGLRenderer::CalculateCylinderNormals(double h, double r1, double r2)
{
	double r = (r1 - r2) / 2;
	double l = sqrt(pow(h, 2) + pow(r, 2));

	double nr = h / l;
	double ny = r / l;

	return { nr, ny };
}
```