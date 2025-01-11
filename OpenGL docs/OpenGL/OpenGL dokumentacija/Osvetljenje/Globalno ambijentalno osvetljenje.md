Bez ikakvog osvetljenja svi objekti su potpuno crni.
Gledamo da izbegnemo potpunu tamninu objekata kada u njih nije upereno svetlo.

Da bi smo ovo resili koristimo **globalno ambijetalno osvetljenje**.

Globalno ambijetalno osvetljenje dolazi **iz svih pravaca** i svaki deo svakog objekta osvetljava.
To omogucava da svi objekti reflektuju **barem malo svetlosti** kako bi se izbegla potupuna tama.

Zbog toga sto ovo svetlo dolazi iz svih pravaca ono **ne kreira** senke i osecaj dubine (ne kreira shadows, hightlights i sense of depth).

#### **Po default-u je globalno ambijentalno osvetljenje postavljeno na {0.2, 0.2, 0.2, 1.0}**

## Koja je korist globalnog ambijentalnog osvetljenja?

Na primer imamo potpuno mracnu prostoriju i lampu u njoj.
Lampa osvetljava jedan deo zida dok su ostali u potunom mraku.

Ako zelimo da osvetlimo celu prostoriju, umesto da dodamo jos lampa mozemo da dodamo ambijentalno osvetljenje kako soba ne bi bilo potpuno mracna.

**Globalno ambijentalno osvetljenje se koristi da se izbegne potpuni mrak u sceni.**

## Kako ovo svetlo utice na materijale

Materijal ima ambijentalo svojstvo ([[Svojstva Materijala]]). Ono odredjuje kolko taj materijal reflektuje ambijentalo osvetljenje.

Ako imam ambijentalo osvetljenje {1, 1, 1, 1} i ambijentalno svojstvo materijala {0.2, 0.2, 0.2, 1} to znaci da ce materijal reflektovati **20% od svakog spektra**.

Bez globalnog ambijentalnog (*namerno je postavljeno na 0, po default-u je {0.2, 0.2, 0.2, 1.0}*):
![[Bez globalnog ambijentalnog.png]]

Sa globalnom ambijentalnim.
**Konfiguracija koriscena:**
*Globalno ambijentalno* - [.6, .6, .6, 1.0]
*Materijal (ambijentalno svojstvo)* - [.4, .1, .1, 1]
![[Sa globalnim ambijentalnim.png]]
Na slici se vidi da se senke i osecaj dubine ne formira.

Odavde vidimo da boja objekta zavisi **od izvora svetlosti kao i on materijala**.
Ako zelimo da neki objekat bude crven, najlogicnije bi bilo da materijal tog objekta **reflektuje najvise crveni spektar**.

To naravno vazi ako uperimo u taj objekat svetlost koja emituje crveni spektar.
Ukoliko uperimo potpuno plavo svetlo u taj objekat on nece biti crvene boje.
## Podesavanja globalnog ambijentalnog osvetljenja

Koriste se 2 funkcije za podesavanje:
#### 1.
```c++
void glLightModelfv(GLenum pname, const GLfloat *params);
```
*fv* - **floating array**, koristi se za podesavanje **globalnog ambijentalog osvetljanja**, parametar joj je floating array
*Primer:*
```c++
GLfloat global_ambient[] = { .2, .2, .2, 1.0 };
glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
```

#### 2.
```c++
void glLightModeli(GLenum pname, GLint param);
```
*i* - parametar je **integer**, koristi se za konfiguraciju nekih drugih stvari u vezi modela osvetljenja. Na primer da OpenGL racuna osvetljenje za spoljasnju i unutrasnju stranu pologina.
```c++
glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
```

## Kako da scena izgleda realisticno?
[[Kreiranje realisticnih materijala i izvora svetlosti]]