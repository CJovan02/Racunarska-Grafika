Sve ovo se vrsi koriscenjem funkcije `glLightModelfv`, gde se samo prosledjuju razliciti parametri.

Pod definisanjem modela osvetljenja podrazumeva se: 
### 1. Određivanje jačine globalnog ambijentalnog osvetljenja 
[[Globalno ambijentalno osvetljenje]]

### 2. Izbor lokalnog ili udaljenog posmatrača:

Izbor lokalnog ili udaljenog posmatrača utiče na način izračunavanja odsjaja objekta (spekularno osvetljnje) gde odsjaj bolje prati kameru.

Lokalni posmatrač zahteva mnogo složenije izračunavanje, jer je potrebno za svako teme svakog od objekata izračunati ugao pod kojim ga vidi posmatrač.

```c++
glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
```

### 3. Izbor jednostranog ili dvostranog osvetljenja

# MNOGO BITNO!!!!!!!
Ako se postavi dvostrano osvetljenje, postoji sanse da se **invertuju normale**
spoljasnje strane u zavisnosti od crtanja objekta, evo kako da se resi ovo [[Normale kod dvostranog osvetljnja]]


Izbor jednostranog ili dvostranog osvetljenja vrši se parametrom GL_LIGHT_MODEL_ TWO_SIDE. Postavljanjem ovog parametra na GL_FALSE, samo strana poligona okrenuta izvoru svetlosti biće osvetljena.
```c++
glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
```

