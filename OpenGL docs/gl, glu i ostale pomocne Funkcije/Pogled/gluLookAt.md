
Sluzi za podesavanja kamere odnosno transformacije pogleda.
Definisi *'view matrix'* koja pokazuje kako se scena vidi od strane kamere.

**Ovo je obavezno podesiti** jer objekti koji se crtaju nece moci da se vide.

Bez ovoga kamera je podesena da se nalazi u tacki (0, 0, 0) i da gleda u tacku (0, 0, 0) sto uglavnom oznacava **"NULL" pogled** odnosno nista se ne prikazuje.

```c++
gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez,
		  GLdouble centerx, GLdouble centery, GLdouble centerz,
		  GLdouble upx, GLdouble upy, GLdouble upz);
```

Ovo se uglavnoom ubacuje u DrawScene funkciji nakon ucitavanja identity matrice.

### Eye position (pozicija kamere)

*(eyex, eyey, eyez)*

Podesava gde se kamera nalazi, odnosno odakle se scena *"gleda".*

### Look-at Point (Target)

*(centerx, centery, centerz)*

Definise tacku gde kamera gleda.
(0, 0, 0) znaci da kamera gleda ka koordinatnom pocetku, sto je i najcesci slucaj.

### Up Direction

*(upx, upy, upz)*

Definise "up" vektor za kameru, odnosno definise sta predstavlja "gore" za kameru.
Tipicno se stavlja (0, 1, 0) gde pozitivna vrednost y-se predstavlja "gore".

## [[Rotacija kamere oko centra sa povlacenjem misa]]

