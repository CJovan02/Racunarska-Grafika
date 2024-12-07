Pre crtanja tacaka poziva se:
```c++
void glBegin(GLenummode);
```
A nakon crtanja poziva se:
```c++
void glEnd();
```

### GLenummode

Specificira mod za spajanje i popunjavanje tacaka.
Modovi su sledeci:

**GL_POINTS**
	Tretira svaki vertex kao tacku i crta ih tako.
**GL_LINES**
	Tretira svaki par od 2 vertexa kao liniju i crta ih tako.
**GL_LINE_STRIP**
**GL_LINE_LOOP**
**GL_TRIANGLES**
	Tretira grupu od 3 vertex-a kao nezavistan trougao.
**GL_TRIANGLE_STRIP**
**GL_TRIANGLE_FAN**
**GL_QUADS**
	Treteira grupu od 4 vertex-a kao nezavistan cetvorougao.
**GL_QUAD_STRIP**
**GL_POLYGON**
	Crta jedan poligon tako sto spoji sve vertex-e.

Mrzi me za sve da pisem sta su, pogledaj prezentacije ili *docs.gl* za celu dokumentaciju.