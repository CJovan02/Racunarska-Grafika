
**Vektor normale** je vektor koji je normalan na povrsinu nekog 3D objekta u nekoj datoj tacki. 
On definise pod kojim uglom pada svetlost na objekat.

Koristi se za kalkulaciju osvetljenja, odnosno, da bi se podesilo kako svetlost utice na izgled nekog objekta (njegovo osvetljenje, senke i reflekcija).

Za ovo se koristi [[glNormal]] funkcija.
**GL_NORMALIZE** mora da bude upaljen ukoliko zelimo da normalizacija ovog vektora bude automatska, odnoso ako se koriste neke funkcija transformacije kao sto su scale...

To znaci da funkcije transformacije kao sto su na primer *glRotate* **menjaju vektor normale**. Ovo se koristi na primer kod odredjivanja normala kocke. [[Normale kocke]]

Za 3D objekte koje smo crtali ([[Crtanje geometrijskih tela]]) pre toga postoje vec unapred definisane formule za odredjivanje normale tih objekata.

[[Izracunavanje normala g. tela]]

[[Crtanje vektora normale]]