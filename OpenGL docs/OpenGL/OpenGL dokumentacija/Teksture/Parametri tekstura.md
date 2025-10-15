
Parametri govore kako tekstura treba da se ponasa kada se prikaze na objektu.

Imamo *4* najvaznija parametra teksture:
- filter koji se koristi pri uvecanju,
- filter koji se koristi pri umanjenju,
- nacin ponavljanja po S pravcu (x osa),
- nacin ponavljanja po T pravcu (y osa).

Sve ove parametre podesavamo jednom funkcijom, [[glTexPrameteri]].

## Filtriranje

Ako **jedan [[teksel]] utice na formiranje vise piksela**, to se naziva **uvecanje** teksture.

Ovo se desava kada se previse priblizimo objektu koji ima teksturu, pa je onda objekat veci i tekstura se "razvlaci". ([[]])

Kako se tekstura ponasa u ovakvoj situaciji definisemo pozivom funkcije:
``` c++
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
```
- *MAG* -> magnification
- *param* -> moze imati dve vrednosti:
	- *GL_NEAREST* -> uzima samo **najbliži piksel** (pikselizovano), ili
	- *GL_LINEAR* -> računa **prosek okolnih piksela** (glađe ivice)

![[Magnifikacija teskture.png]]

Mi uglavnom koristimo *GL_LINEAR* jer daje lepsi izgled.


Kada je objekat koji posmatramo daleko, **vise teksela stapaju se u samo jedan piksel**. 
Tada kazemo da nastaje umanjenje teksture. 

Kako ce se tekstura ponasati pri umanjenju definisemo pozivom funkcije:

```c++
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
```

- *param* -> moze imate vise vrednosti:
	- *GL_NEAREST* -> Najbrži, ali „blokast“ prikaz, 
	- *GL_LINEAR* -> Glađe, ali skuplje, 
	- *GL_NEAREST_MIPMAP_NEAREST* -> Mipmap + najbliži nivo + najbliži piksel, 
	- *GL_NEAREST_MIPMAP_LINEAR* -> Linearno između nivoa + najbliži piksel, 
	- *GL_LINEAR_MIPMAP_NEAREST* -> Mipmap + najbliži nivo + linearno između piksela, ili 
	- *GL_LINEAR_MIPMAP_LINEAR* -> Najlepši kvalitet (trilinear filtering).

CS prezentacija za vise info.

![[Minifikacija teksture.png]]

Mi cemo koristiti *GL_LINEAR_MIPMAP_LINEAR*.

### **JAKO BITNO!**
Ako zelimo da koristimo neke vrednosti koje ukljucuju mipmape, (vrednosti funkcije koje u imenu imaju MIPMAP), mipmapa mora biti generisana, inace ce openGL automatski iskljuciti teksturisanje.

Viso info na [[Mipmap]]

## Ponavljanje

```c++
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
```

De bi smo ovo bolje razumeli potrebno je da znamo kako izgledaju [[Teksturne koordinate]].

Uglavnom, ovo nam podesava kako se tekstura ponasa kada koordinate izlaze iz [0, 1] opsega.

![[Ponavljanje teksture.png]]

#### Vizuelna ilustraciju koju je GPT napisao :)

Zamisli da imaš teksturu „🟨🟩🟦🟥“ i koristiš je kao šablon:

| Wrap Mode              | Šta se vidi kada se izađe iz [0,1] opsega         |
| ---------------------- | ------------------------------------------------- |
| **GL_REPEAT**          | 🟨🟩🟦🟥🟨🟩🟦🟥 (ponavlja se beskonačno)         |
| **GL_MIRRORED_REPEAT** | 🟨🟩🟦🟥🟥🟦🟩🟨 (svaki drugi put „zrcali“)       |
| **GL_CLAMP_TO_EDGE**   | 🟨🟩🟦🟥🟥🟥🟥🟥 (ivica se rasteže)               |
| **GL_CLAMP_TO_BORDER** | 🟨🟩🟦🟥⬛⬛⬛⬛ (popunjava border bojom, npr. crnom) |
### Kada koristiš koji? (GPT preporucuje)

- **GL_REPEAT** → idealno za pločice, podove, zidove, šare koje treba da se beskonačno ponavljaju.

- **GL_MIRRORED_REPEAT** → koristi se kada želiš da izbegneš „tvrde ivice“ između ponavljanja (glatko spajanje).

- ***GL_CLAMP_TO_EDGE*** → najčešći izbor za cube-mape, nebo (skybox), sfere, ili bilo šta gde ne želiš spojeve. (Mislim da u starijoj verziji GL-a, koju mi koristimo, ne postoji ovo, nego samo *GL_CLAMP*)

- **GL_CLAMP_TO_BORDER** → koristi se ređe, kada hoćeš eksplicitno da definišeš boju „van teksture“.