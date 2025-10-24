
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

Postoje jos neke konfiguracije oko definisanja modela osvetljenja: [[Definisanje modela osvetljenja]]

Svetlost moze imati uticaj na spoljasnju stranu objekta i na unutrasju. Po default-u OpenGL racuna osvetljnje samo za spoljasnji deo poligona.

Moze se podesiti da se svetlost kalkulise i za unutrasni deo objekta. Ovo je korisno kada zelimo staklo da prikazemo ili nesto slicno, medjutim ovo ima **los uticaj na performanse**.
```c++
glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE || GL_TRUE);
```

Zatim, da bi scena izgledala realnisticno, moramo korektno podesiti svojstva materijalima i izvorima svetlosti:
[[Kreiranje realisticnih materijala i izvora svetlosti]]

## Primer koda sa jednog ispitnog zadatka

```c++
void CGLRenderer::SetLightningParams()
{
	GLfloat global_ambient[] = { .3, .3, .3, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	
	glEnable(GL_LIGHTING);
}
```