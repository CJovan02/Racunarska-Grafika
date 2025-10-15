Za crtanje se primarno koriste dve funkcije:

- [[glBegin i glEnd]]
- [[glVertex]]

Moze da se specificira i boja pri crtanju pozivom [[glColor]] pre crtanja figure.

Takodje gasimo teksture pre crtanja linija, [[glDisable]]
Primer crtanja linija oko koordinatnog pocetka:
```c++
void CGLRenderer::DrawAxes() 
{ 
	glLineWidth(2.0); 
	glDisable(GL_TEXTURE_2D); 

	glBegin(GL_LINES);
	{
		glColor3f(0, 0, 1); 
		glVertex3f(0, 0, 0); 
		glVertex3f(50, 0, 0); 
		
		glColor3f(1, 0, 0); 
		glVertex3f(0, 0, 0); 
		glVertex3f(0, 50, 0); 
		
		glColor3f(0, 1, 0); 
		glVertex3f(0, 0, 0); 
		glVertex3f(0, 0, 50); 
	}
	glEnd(); 
}
```

GL_LINES pravi grupu od 2 vertex-e i vuce liniju izmedju njih. 