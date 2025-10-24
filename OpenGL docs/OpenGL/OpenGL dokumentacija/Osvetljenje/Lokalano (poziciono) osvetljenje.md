
Lokalno osvetljenje je izvor svetlosti koji se nalazi u nekoj tacki, gleda u nekom smeru i osvetljava objekte u istom smeru (kao na primer lampa).

OpenGL dozvoljava da imamo **do 8 izvora** svetlosti.
Da bi koristili neki od njih moramo da ih ukljucimo pozivom:
```c++
glEnable(GL_LIGHT0); // bira broj od 0 do 7
```

Nakon toga moramo da podesimo **svojstva osvetljenja** koje smo ukljucili.
Funkcija za podesavanje svojstva: ***glLightfv*** i ***glLightf***
### 1. Ambient Light

Simulira pozadinsko osvetljenje iz svih pravaca, uglavnom se stavlja neko priguseno svetlo.

Ima isti efekat kao i globalno ambijentalno ali drugaciju svrhu:
Ako u mracnoj sobi upalimo lampu prema jednom zidu, taj zid ce reflekovati svetlosne zrake.
Ti zraci dolaze do nasih ociju i omogucavaju nam da vidimo taj zid.
Takodje ti zraci dopiru do ostalih zidova i osvetljavaju ih barem malo.

Ambijentalo svetlo koje polazi od lokalnog sveta simulira efekat iz prethodnoh primera.
```c++
GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
```
Materijali imaju *ambient* svojstvo koje utice na ko koliko reflektuju ambijentalno svetlo.

### 2. Diffuse Light

Simulira **direktnu svetlost** od izvora.
Ono zavisi od ugla izmedju povorsine objekta i od smera osvetljenja.
Ugao izmedju povrsine smo podesavali normalama ([[Podesavanje vektora normale]]).
```c++
GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
```
Materijali imaju *diffuse* svojstvo koje utice na to koliko reflektuju difuziono svetlo.

### 3. Specular Light

Simulira **sjajne odsjaje** na reflektujucim povrsinama.
```c++
GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
```
Materijali imaju *specular* svojstvo koje utice na specular light.

### 4. Pozicija svetla

Prva tri parametra su koordinate.

```c++
GLfloat position[] = {0.0f, 10.0f, 0.0f, 1.0f}; // Positional light
glLightfv(GL_LIGHT0, GL_POSITION, position);
```
4. parametar oznacava vrstu izvora:
- *Direkcioni izvor {0}* - Svi zraci koji dopiru do objekata smatraju se paralelnim (**sunce je takav izvor**)
- *Pozicioni izvor {1}* - Ugao zraka zavisi od polozaja svetla u sceni (npr. lampa). Zahteva mnogo vise izracunavanja, pa ukoliko nije potrebno treba koristiti direkciono svetlo.

#### **NAPOMENA**
Pozicija svetla mora da se navede nakon [[gluLookAt]] funkcije. U suprotom ce izvor svetlosti pratiti kameru.

Ovako scena izgleda bez lokalnog osvetljenja.
Vidimo da objekti nisu potpuno crni vec tamno crveni. To je zato sto imamo [[Globalno ambijentalno osvetljenje]] podeseno na {.3, .3, .3, 1}.

### 5. Slabljenje i atenuacija

Kod realnih izvora, intenzitet svetla opada sa rastojanjem. Ovo ima smisla samo kod pozicionih svetala.

Imamo tri parametara atenuacije:
- *GL_CONSTANT_ATTENUATION*
- *GL_LINEAR_ATTENUATION*
- *GL_QUADRATIC_ATTENUATION*

```c++
glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5); 
glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5); 
glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
```

### 6. Spotlight

Spotlight nije obavezan da se podesi. Bez ove konfiguracije izvor svetlosti nije usmeren, svetli kao sijalica.
Kada se podesi spotlight izvor ce biti usmeren, kao kada sijalicu zasrafimo na lampu.

![[Pasted image 20250111191344.png]]

Sa slike se vidi da svetlo nije usmereno, odnosno svetli u svim pravcima.
To ukljucuje i ambijentalo i difuziono svojstvo.

Izvor svetlosti usmeren ka negativnom smeru X ose:
![[Pasted image 20250111191717.png]]

Sada vidimo da svako svojstvo izvora usmereno (difuziono, ambijentalno i spekularno).

![[Pasted image 20250111192103.png]]

- *GL_SPOT_DIRECTION* - definise smer gde je izvor uperen.
- *GL_SPOT_CUTOFF* - ugao spotlight kupe. Objekti koji se nalaze u kupi su osvetljeni (po default-u je ovo 180 pa je ova kupa zapravo sfera. Zato izvor svetli na sve strane).
- *GL_SPOT_EXPONENT* - distribucija intenziteta svetlosti u okviru kupe. Na srpskom ovo znaci sledece: veci broj fokusira svetlo ka centru, odnosno, svetlo je jaece u centru kupe i slabi kako se udavljavamo od centra.

## Primer koda sa ispitnog zadatka

```c++
void CGLRenderer::SetLightParams(int glLight)
{
	// Boja, intenzitet i pozicija svetla
	GLfloat ambient[] = { .2, .2, .2, 1 };
	GLfloat diffuse[] ={ 1, 1, 1, 1 };
	GLfloat specular[] = { .8, .8, .8, 1 };
	GLfloat position[] = { 0, 0, 0, 1 };

	glLightfv(glLight, GL_AMBIENT, ambient);
	glLightfv(glLight, GL_DIFFUSE, diffuse);
	glLightfv(glLight, GL_SPECULAR, specular);
	glLightfv(glLight, GL_POSITION, position);

	// Atenuacija
	glLightf(glLight, GL_CONSTANT_ATTENUATION, 1);
	glLightf(glLight, GL_LINEAR_ATTENUATION, 0.2);
	glLightf(glLight, GL_QUADRATIC_ATTENUATION, 0.1);

	// Spotlight
	GLfloat direction[] = { 0, 0, -1 };

	glLightfv(glLight, GL_SPOT_DIRECTION, direction);
	glLightf(glLight, GL_SPOT_CUTOFF, 45.0);
	glLightf(glLight, GL_SPOT_EXPONENT, 2.0);


	glEnable(glLight);
}
```

## Kako da scena izgleda realisticno?

[[Kreiranje realisticnih materijala i izvora svetlosti]]

