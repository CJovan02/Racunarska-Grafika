
Planarano mapiranje za neku krug ili sferu:
```c++
tx = texU + texR * x / r;
ty = texV + texR * z / r;
```

*tx i ty* - teksturne koordinate
*texU i texV* - U je x koordinata u teksturi, V je y. Odnosno za ovu formulu ovde dve koordinate predstavljaju centar kruga oko koga se uzima projekcia
*texR* - poluprecnik teksturnog kruga koji se uzima za lepljenje teksture
*x i z* - radi se o XZ ravni pa se ove koordinate uzimaju za projekciju, to su koordinate objekta cija se projekcija uzima
*r* - poluprecnik kruga objekta (koristi se za normalizaciju)

### Objasnjenje formule - [[Preslikavanje 3D tacke na 2D teksturu]]

Uzima se **projekcija tačke na ravan** (npr. XZ ili XY ravan) i koristi se **linearnu skala** za pretvaranje u teksturne koordinate.

To znači da tekstura funkcioniše kao **projekcija kroz prostor** – zamisliš da bacaš sliku kao projektor na površinu, i koristiš položaj tačke na ravni da odrediš gde pada piksel.

Ili moze da se zamisli kao pustis svetlo ispred nekog objekta, njegova senka na zid (odnosno na neku ravan) je zapravo njegova projekcija.
Ti uzmes njegovu projekciju i mapiras je u teksturne koordinate.

Za neki poligon je se rucno unose koordinate, za krug i sferu ima gornja formula.