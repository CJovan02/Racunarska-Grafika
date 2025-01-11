
Da bi omogucili da OpenGL izracunava osetljenje moramo ga prvo upaliti.
U [[PrepareScene function]]:
```c++
glEnable(GL_LIGHTING);
```

Kada je osvetljenje ukljuceno objekti **nemaju boju vec reflektuju svetlost.**
Ista stvar vazi u stvarnom zivotu, boju objekta koju vidimo je zapravo **boja koju taj objekat reflektuje.**

Postoje nekoliko izvora svetlosti:

- [[Globalno ambijentalno osvetljenje]]
- [[Lokalano (poziciono) osvetljenje]]

Svetlost moze imati uticaj na spoljasnju stranu objekta i na unutrasju. Po default-u OpenGL racuna osvetljnje samo za spoljasnji deo poligona.

Moze se podesiti da se svetlost kalulise i za unutrasni deo objekta. Ovo je korisno kada zelimo staklo da prikazemo ili nesto slicno, medjutim ovo ima **los uticaj na performanse**.
```c++
glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE || GL_TRUE);
```

Zatim, da bi scena izgledala realnisticno, moramo korektno podesiti svojstva materijalima i izvorima svetlosti:
[[Kreiranje realisticnih materijala i izvora svetlosti]]