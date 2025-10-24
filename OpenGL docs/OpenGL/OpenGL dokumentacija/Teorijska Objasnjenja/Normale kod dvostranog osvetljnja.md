Ovo je mnogo bitno da se razume, jer ako se ne podesi kako treba, vrlo verovatno ce neki objekti da imaju invertovane spoljesnje normale, odnosno material tog objekta uopste nece da radi kako je ocekivano.

**Na ispitu se ponekat trazi da se upali ovo dvostrano osvetljnje!!!**

## Ponasanje kad je dvostrano osvetljenje iskljuceno

Znamo da moramo da podesimo normale objekta, da normala poligona gleda ka "solja" da bi se ta strane objekta osvetlila kada uperimo svetlost u nju.
[[Podesavanje vektora normale]]

Kad je ovo podesavanje iskljuceno, i kada postavimo *FRONT* stranu materijala, onda ce se "osvetliti" ona strana materijala onako kako je je podesen vektor normale.
[[Konfiguracija materijala]]

Drugim recima, ako normala gleda ka spolja, ta strana objekta se racuna da je **FRONT**.

## Ponasanje kad je dvostrano osvetljnje UKLJUCENO

Ja sam uz pomoc GPT-a dosao do ovoga, nisam video u prezentacijama da se spominje ovo.

E sada ono bitno, 
# OpenGL razlikuje "front" i "back" strane po redosledu temena

Svaki objekat ima **redosled temena** kojima se crta:

- ako su u **counter-clockwise (CCW)** redosledu, OpenGL ih smatra **front-facing** (to je default),
- ako su u **clockwise (CW)** redosledu, smatra ih **back-facing**.

To određuje _koja je spoljašnja, a koja unutrašnja_ strana sfere.

`glFrontFace(GL_CCW); // default`

Znači:  
Ako tvoja sfera crta svoje vertexe u CW redosledu (iz perspektive kamere), OpenGL će misliti da gledaš **unutrašnju** stranu,  
Ako ih crta u CCW, misliće da gledaš **spoljašnju**.

Znam da je mnogo logicnije da se se temena crtaju u smeru kazaljke na satu, ali ako se trazi na ispitu da se koristi dvostrano osvetljnje, moramo u suprotnom smeru da ih crtamo.

## Kako da resimo ovaj problem?

Mozemo da privremeno promenimo mod u `glFrontFace(GL_CW);` dok crtamo, i kasnije ga vratimo na *GL_CWW*.

Mozemo da invertujemo normale, svakoj komponenti normale da dodamo '-' ispred.

Mozemo da crtamo objekat tako sto njegova temena navodimo u smeru suprotniom od kazaljke na satu.
