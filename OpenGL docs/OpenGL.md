
OpenGL je jedna biblioteka koja se na ovom predmetu koristi u skladu sa MFC aplikacijom. Ona omogucava novu vrstu renderovanja, odnosno 3D renderovanja.

Kod openGL projekta osim defaultnih klasa koje MFC aplikacija generise postoje i **GL folder** gde se nalaze openGL funkcije koje se koristie i takodje posotji **GLRenderer fajl** koji sluzi kao bridge izmedju openGL-a i MFC.

U tom fajlu se nalaze funkcije za crtanje u OpenGL-u. Taj fajl izgleda dosta slicno kao View.cpp fajl kod MFC aplikacije, zadrzi funkcije za inicijalizaciju scene, crtanje scene, brisanje scene itd...
**Sve glavne funkcije iz GLRenderer.cpp fajla se pozivaju u View.cpp fajlu.**

## GLRenderer glavne funkcije

Ovo su funkcije koje se nalaze u ovom fajlu i koje se pozivaju u view.cpp funkcijama od strane MFC aplikacije

- #### [[PrepareScene function]]

- ### [[Reshape]]

- ### [[PrepareScene function]]

- [[DrawScene]]

## Kamera i pogled u OpenGL-u

Pre crtanja necega mora da se podesi pogled, u smislu, od koje tacke *'gledamo'* nesto, gde se to *'nesto'* nalazi i koji pravac je *'gore'*.

To se radi pomocu [[gluLookAt]] funkcije.

### [[Rotacija kamere oko centra sa povlacenjem misa]]

## Crtanje u OpenGL-u

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
## [[Boilerplate kod za izradu zadataka]]

## Funkcije za crtanje geometrijskih objekata

Geometrijski objekti kao sto su sfera, kupa, cilindar se crtaju po nekoj odredjenoj matematickoj formuli.

