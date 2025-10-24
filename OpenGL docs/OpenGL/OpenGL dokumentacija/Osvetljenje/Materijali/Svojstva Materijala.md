## ***Source: GPT :))***

**Svojstva materijala** definisu to kako objekat prima svetlost, odnosno kako taj objekat izgleda pod razlicitim osvetljenjem.
## 1. Ambient (Ambijentalno)

**Ambijentalno** svojstvo određuje boju objekta kada ga osvetljava ambijentalno svetlo (ukljucuje i globalno i lokalno). 

[[Globalno ambijentalno osvetljenje]] predstavlja opšte, pozadinsko svetlo koje je prisutno u sceni i dolazi iz svih pravaca.

- **Komponente**: `GL_AMBIENT` prihvata niz od 4 vrednosti: `[red, green, blue, alpha]`.
- **Tipične vrednosti**: Vrednosti se kreću od `0.0` (bez refleksije) do `1.0` (maksimalna refleksija).

**Primer**: 
Imamo ambijentalno osvetljnje [0.3, 0.3, 0.3, 1] sto je slabo belo osvetljenje. 
Zatim imamo materijal sa ambijentalnim svojstvom [0.1, 0.1, 0.1, 1]. 
Ovo znaci da ce materijal reflektovati **10% crvenog, zelenog i plavog** spektra.

Finalna boja materijala pod ambijentalnim osvetljenjem bi bila **0.3 * 0.1 = 0.03** za red, green i blue spektar.
## 2. Diffuse (Difuzno)

**Difuzno** svojstvo određuje boju objekta pod difuznim svetlom. Difuzno svetlo dolazi iz određenog pravca i stvara senke na osnovu oblika i orijentacije objekta.

- **Komponente**: `GL_DIFFUSE` prihvata niz od 4 vrednosti: `[red, green, blue, alpha]`.
- **Tipične vrednosti**:
    - `[0.8, 0.0, 0.0, 1.0]` čini da objekat izgleda svetlo crven pod difuznim osvetljenjem.
    - `[0.5, 0.5, 0.5, 1.0]` daje neutralno sivu boju.
- **Saveti**: Postavite vrednosti koje odgovaraju boji objekta na jakom svetlu.

## 3. Shininess (Sjaj)

**Sjaj** određuje veličinu i intenzitet svetlećih tačaka na objektu.

- **Komponenta**: `GL_SHININESS` prihvata jednu vrednost između `0.0` i `128.0`.
- **Tipične vrednosti**:
    - `0.0`: Bez sjaja (mat površina).
    - `128.0`: Male, intenzivne svetleće tačke (glatke, sjajne površine).
- **Saveti**:
    - Visoke vrednosti koristite za staklo ili polirane metale.
    - Niske vrednosti koristite za grube materijale poput kamena ili drveta.

## 4. Emission (Emisija)

**Emisija** definiše boju koju objekat "emituje", kao da je izvor svetlosti.

- **Komponente**: `GL_EMISSION` prihvata niz od 4 vrednosti: `[red, green, blue, alpha]`.
- **Tipične vrednosti**:
    - `[0.0, 0.0, 0.0, 1.0]` za objekte koji ne emituju svetlost.
    - `[0.3, 0.3, 0.3, 1.0]` čini da objekat deluje blago svetleće.
- **Saveti**: Koristite ovo svojstvo za objekte poput lampi ili vatre.

## 5. Specular (Spekularno)

**Spekularno** svojstvo određuje boju i intenzitet svetlećih tačaka na objektu, koje nastaju kada se svetlost reflektuje direktno ka posmatraču.

- **Komponente**: `GL_SPECULAR` prihvata niz od 4 vrednosti: `[red, green, blue, alpha]`.
- **Tipične vrednosti**:
    - `[1.0, 1.0, 1.0, 1.0]` za bele sjajne tačke.
    - `[0.0, 0.0, 0.0, 1.0]` za mat površine bez sjaja.
- **Saveti**: Visoke vrednosti koristite za sjajne površine poput metala, a niske za mat površine.

## Kako napraviti realisticne materijale?
[[Kreiranje realisticnih materijala i izvora svetlosti]]