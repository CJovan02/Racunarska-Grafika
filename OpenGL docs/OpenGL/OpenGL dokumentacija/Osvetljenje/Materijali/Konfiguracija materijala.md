
**Svojstva materijala** definisu to kako objekat prima svetlost, odnosno kako taj objekat izgleda pod razlicitim osvetljenjem.
## [[Svojstva Materijala]]

## Kako primeniti materijal na telo?

Kada definisemo svojstva materijala mozemo ih primeniti na neki objekat rucno ili koriscenjem *CGLMaterial*.

- Rucno. Nakon ovog isecka koda kada crtamo neko telo ono ce primiti ovaj materijal.
```c++
glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
glMaterialfv(GL_FRONT, GL_EMISSION, emission);
glMaterialf(GL_FRONT, GL_SHININESS, shininess);

```

- Koriscenjem *CGLMaterial*:

U konstruktoru kreiramo materijal:
```c++
CGLMaterijal wallMat;
wallMat.SetAmbient(.2, .2, .2, 1);
wallMat.SetDiffuse(.6, .6, .6, 1);
wallMat.SetEmission(0, 0, 0, 1);
...
```

Zatim kada zelimo da ga primenimo na neki objekat:
```c++
wallMat.SetFace(GL_FRONT);
```

- *GL_FRONT* materijal se primenjuje na spoljasnji deo poligona.
- *GL_BACK* materijal se primenjuje na unutrasni deo objekta.
- *GL_FRONT_AND_BACK* logicno je sta radi.