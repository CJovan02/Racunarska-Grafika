
Ovo je jako bitno da se podesi jer definise **kako 3D scene se projektuju u 2D ekran**. Bez njega, OpenGL nema pojma kako da transformise 3D koordinate objekata u view koju moze da se prikaze na ekranu.
Ovo je krucijalan deo rendering pipelina-a u OpenGL-u.

## Sta je matrica projekcije?

Definise pravila za transfomaciju 3D koordinata scene u 3D viewport prozora.
Postoje 2 glavne projekcije:
- **Perspective projection:** Imitira kako ljudsko oko vidi svet, objekti koji su dalji od naseg oka izgledaju manje.
- **Orthographic Projection:** Paralelna projekcija gde objekti zadrzavaju njihove prave dimenzije bez obzira koliko su udaljeni od kamere.

## Kako se podasava projekcija za izradu zadataka?

1. **Postavljamo Matrix Mode:**
```c++
glMatrixMode(GL_PROJECTION);
```
Ovde govorimo OpenGL-u da zelimo da modifikujemo matricu projekcije

2. **Ucitamo Identity Matricu:**
```c++
glLoadIdentity()
```
Identity matrica je default-na matrica, ovime omogucavamo da prethodne transformacije nemaju uticaj na trenutni setup koji zelimo da napravimo.

3. **Ucitamo projekciju perspektive:**
```c++
gluPerspective(45.0, (double)w / h, 1.0, 100.0);
```
Parametri:
- *FOV - Field of View*: ovde je stavljeno 45, ova brojka se daje u zadatku
- *Aspect Ratio:* width-to-height ratio viewport-a, znaci **w / h**
- *Near Clilpping Plane:* Minimalna distanca od kamere gde se objekti renderuju, mislim da se ovde uvek stavlja *1*
- *Far Clipping Plane:* Maksimalna distanca od kamere gde se objekti renderuju, mislim da se ovde uvak stavlja *100*

![[viewing-frustim.jpg]]
Na kraju se formira **viewing frustum**.
Na osnovu njega se objekti skaliraju u odnosu na njihovu udaljenost od kamere, kreirajuci osecaj dubine (perspektive).

4. **Vracamo se u prethodni mod**:
```c++
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
```
Nakon konfiguracije matrice projekcije moramo da se vradimo u default-nu **model-view matricu**.
Ova matrica konfigurise transformacije (translate, scale, rotate)



## Opciono: Podesavanje orthographic perspektive radi testiranja


**Umesto gluPerspective mozemo ovo da stavimo:**
```c++
if (w <= h)
    glOrtho(-1.0, 1.0, -1.0 * h / w, 1.0 * h / w, -1.0, 1.0);
else
    glOrtho(-1.0 * w / h, 1.0 * w / h, -1.0, 1.0, -1.0, 1.0);
```

NAPOMENA: ovo ne radi i mora kod za crtanje da se menja. Da bi otprilike videli kako Ortho radi mozemo fiksno da kazemo ovako:

```c++
glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
```