
Svaka tacka (vertex) na nekom objektu dobija **teksturne koordinate** — obično označene sa **(u, v)**, gde:

- *u* ide po širini teksture (x-osa u slici),
- *v* ide po visini teksture (y-osa u slici).

U većini slučajeva, ove vrednosti su u intervalu **[0, 1]**, gde:

- *u = 0* → leva ivica teksture,

- *u = 1* → desna ivica teksture,

- *v = 0* → donja ivica teksture,

- *v = 1* → gornja ivica teksture.


Ali, ako program ili shader koristi koordinate **van tog opsega** (npr. `u = 1.3`, `v = -0.2`), OpenGL mora da zna **šta da radi** sa tim. [[Parametri tekstura]]

Koordinate se zadaju pozivom [[glTexCoord2f]] funkcijom.


![[Teksturne koordinate temena.png]]
