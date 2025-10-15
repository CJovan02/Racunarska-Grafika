
Imamo teksturu (sliku) koja sadrži više delova, a želimo da **izrežemo** jedan kružni deo i **zalepimo ga** na vrh sfere (ili nekog drugog tela).

Formule koje koristimo su:
## tx = texR * x / r + texU  
## ty = texR * z / r + texV


---

## 1. Odabir ravni projekcije

Prvo određujemo **u kojoj ravni projektujemo tačke** sa sfere.  
Pošto teksturu lepimo **na vrh sfere**, koristi se **XZ ravan** desnog koordinatnog sistema.

Dakle, od 3D tačke **(x, y, z)** uzimamo samo **x** i **z** komponente jer u ovoj ravni njih projektujemo na 2D teksturu.

Ova ravan zapravo **definiše projekciju tačke** sa površine sfere na 2D površinu teksture.

---

## 2. Normalizacija (dobijanje jediničnog vektora)

Sferu posmatramo kao skup tačaka **(x, y, z)** udaljenih od centra za poluprečnik **r**.  

Ako svaku tačku podelimo sa **r**:
**(x, y, z) / r = (x/r, y/r, z/r)**
dobijamo **jedinični vektor** u pravcu te tačke (tj. vektor dužine 1).

Pošto radimo u **XZ** ravni, posmatramo samo komponente:
- **x/r**
- **z/r**

Ove vrednosti predstavljaju **normalizovane komponente vektora** i služe da bi koordinate bile nezavisne od stvarne veličine sfere (tj. od *r*).

---

## 3. Skaliranje vektora na opseg teksture

Sada normalizovani vektor (dužine 1) treba da preslikamo na **opseg teksture**.  
To radimo tako što ga pomnožimo sa **texR**, što predstavlja poluprečnik kruga u teksturi (u teksturnim koordinatama).

**(texR * x/r, texR * z/r)**

Ovim zapravo „uvećavamo“ jedinični vektor tako da zauzima isti radijus kao i krug teksture.  
Drugim rečima, sada koordinate odgovaraju stvarnim udaljenostima unutar teksture.

---

## 4. Dodavanje ofseta (texU, texV)

Koordinate **(texU, texV)** predstavljaju **centar kruga teksture** — tj. tačku koja se lepi na „vrh“ sfere.

Da bismo dobili prave teksturne koordinate, naš rezultat pomeramo za taj centar:
### (tx, ty) = (texR * x / r + texU, texR * z / r + texV)

Ovo nam daje **konačne koordinate na teksturi** koje odgovaraju tački **(x, y, z)** na sferi.

---

## Rezime

- Normalizacijom **(x, y, z)/r** dobijamo pravac tačke (dužine 1).  
- Skaliranjem sa **texR** prilagođavamo veličinu teksture.  
- Dodavanjem **(texU, texV)** pomeramo sve u centar kruga teksture.
