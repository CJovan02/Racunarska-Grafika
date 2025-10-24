
Cilindar je malo komplikovaniji zbog omotaca, odnosno sve zavisi od toga kako se u zadatku trazi da se tekstura "lepi" na telo.

Za ovaj primer je korisceno **planarno mapiranje** za baze, dok je **sferno mapiranje** korisceno za omotac.

#### [[Planarno (projekciono) mapiranje]]
**Planarno** je kad se uzme projekcija baze na neku ravan, XZ na primer, i na osnovu nje se izracunaju teksturne koordinate, kao da se maltene isece krug ili elipsa iz teksture i zalepi preko neke povrsine. 

#### [[Sferno (polarno) mapiranje]]
**Polarno** je kad se uzme tekstura i obmota se oko nekog objekta

![[lamp.jpg]]

```c++
void CGLRenderer::DrawCylinder(double r1, double r2, double h, int nSeg, int texMode, bool isOpen)
{
	glColor3d(1, 1, 1);

	glBindTexture(GL_TEXTURE_2D, texLamp);
	double texU = 0.5;
	// samo se Y koordinata razlikuje na gornjoj/donjoj polovini teksture
	double texVTop = 0.25;
	double texVBot = 0.75;
	double texR_X = 0.5;   // horizontalni poluprečnik
	double texR_Y = 0.25;

	// Gornja osnova
	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2f(texU, texVTop);
		glVertex3f(0, h, 0);
		for (int i = 0; i <= nSeg; i ++)
		{
			double alpha = (double) 360 / nSeg * i;
			
			double x = r1 * cos(TO_RAD(alpha));
			double z = r1 * sin(TO_RAD(alpha));

			double tx = texU + texR_X * x / r1;
			double ty = texVTop + texR_Y * z / r1;

			glTexCoord2f(tx, ty);
			glVertex3f(x, h, z);
		}
	}
	glEnd();

	// Donja osnova
	glBegin(GL_TRIANGLE_FAN);
	{
		glTexCoord2f(texU, texVTop);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= nSeg; i ++)
		{
			double alpha = (double) 360 / nSeg * i;
		
			double x = r2 * cos(TO_RAD(alpha));
			double z = r2 * sin(TO_RAD(alpha));

			double tx = texU + texR_X * x / r2;
			double ty = texVTop - texR_Y * z / r2;

			glTexCoord2f(tx, ty);
			glVertex3f(x, 0, z);
		}
	}
	glEnd();

	// Omotac
	glBegin(GL_TRIANGLE_STRIP);
	{
		for (int i = 0; i <= nSeg; i ++)
		{
			double alpha = (double) 360 / nSeg * i;
			
			double xTop = r1 * cos(TO_RAD(alpha));
			double zTop = r1 * sin(TO_RAD(alpha));

			double xBot = r2 * cos(TO_RAD(alpha));
			double zBot = r2 * sin(TO_RAD(alpha));

			double tx = - (double) alpha / 360;
			double tyTop = 0.5;
			double tyBot = 1;

			glTexCoord2d(tx, tyBot);
			glVertex3d(xBot, 0, zBot);

			glTexCoord2d(tx, tyTop);
			glVertex3d(xTop, h, zTop);
		}
	}
	glEnd();
}

```

Za ovaj zadatak se trazilo da se ovaj gornji deo teksture (tu gde pise OpenGl) zalepi **odozgo** na gornji deo osnove kupe, znaci skroz isto kao primer sa loptom i kupom.
Posto lepimo teksturu koja odgovara ravni **XZ**, imamo formule:

Takodje se iz ovog primera vidi da je gornja tekstura razvucena, mi mozemo da je skupimo tako sto uzmemo veci poluprecnik za X osu teksture, **texR_X = 0.5**, dok je za Y **texR_Y = 0.25**

### double tx = texU + texR_X * x / r1;
### duble ty = texVTop + texR_Y * z / r1;

---


Donja osnova, ako se zalepi istim formulama, bice invertovana.
To je zato sto lepimo teksturu odozgo a donja osnova je vidljiva sa donje strane.

![[Kupa tekstura inverted.png]]

Da bi smo je invertovali, mozemo ili preko translacije da rotiramo bazu za 180 stepeni, ili samo da invertujemo jednu koordinatu iz teksture
### double ty = texVTop **-** texR_Y * z / r2;

---

E sad trebamo omotac da zelipimo, ovde se vidi da se omotac ne lepi odozgo nego se **obmotava** oko omotaca.
Takodje je i formula za mapiranje koordinata drugacija.

### double tx = - (double) alpha / 360;
### double tyTop = 0.5;
### double tyBot = 1;

top i bot su malo invertovani, jer opet kazem, ili DImage naopako ucita sliku, ili su sa cs-a malo pomesali UV koordinate tekstura.

**NAPOMENA**
Ako zelimo da se ponavlja ova tekstura sa omotaca, na primer dva puda da se zalepi, jednom sa prednje i zadnje strane, onda mozemo da promenimo ovaj prametar u *LoadTexture* -> 	
`glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);`, 
znaci dodajmo repeat umesto GL_CLAMP.

Takodje promenimo **tx = - (double) alpha / 360 * n * gde je n broj puta koliko zelimo da ponovimo teksturu.


Ja sam ovde dodao - na tx jer (iz nekog razloga) jer invertovana tekstura kad se ucita.
![[Lepljnje teksture na omotac kupe]]


