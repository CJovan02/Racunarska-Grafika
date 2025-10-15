**Mipmape** su *manje kopije teksture* koje se automatski prave u različitim nivoima rezolucije.

Naziv dolazi od latinskog *"multum in parvo"* → "mnogo u malom".

#### **Mipmape = više verzija iste teksture, svaka sledeća upola manja po širini i visini.**

| Nivo | Dimenzije | Smanjenje |
| :--- | :-------- | :-------- |
| 0    | 512×512   | original  |
| 1    | 256×256   | 1/2       |
| 2    | 128×128   | 1/4       |
| 3    | 64×64     | 1/8       |
| 4    | 32×32     | 1/16      |
| 5    | 16×16     | 1/32      |
| 6    | 8×8       | 1/64      |
| 7    | 4×4       | 1/128     |
| 8    | 2×2       | 1/256     |
| 9    | 1×1       | 1/512     |

## Ideja iza mipmape

Bez mipmapa, kad gledaš neki **udaljeni objekat**, na ekranu imaš:

- recimo 1 piksel zida,
- ali ta 1 tačka na zidu „poklapa“ 20×20 teksela iz originalne velike teksture.

GPU tada mora da „sabije“ mnogo detalja u jedan piksel, što dovodi do:

- **treperenja (aliasing)**,
- **moiré efekta** (čudni šabloni),
- **nestabilne slike pri kretanju kamere**.

**Rešenje:** koristi manju verziju teksture koja _odgovara veličini na ekranu_ → mipmap nivo.

Nama u zadacima su mipmape trebaju da bi mogli "bolje" filtere da koristimo za *minimizaciju* teksture. ([[Parametri tekstura]])

## Kako se prave mipmap-e u OpenGL-u

Imamo dva nacina:
- *rucno* -> koriscenjem [[glTexImage2D]].

```c++
glTexImage2D( GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data0 ); 

glTexImage2D( GL_TEXTURE_2D, 1, 3, TextureImage->sizeX/2, TextureImage->sizeY/2, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data1 ); 

glTexImage2D( GL_TEXTURE_2D, 2, 3, TextureImage->sizeX/4, TextureImage->sizeY/4, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data2 ); 

glTexImage2D( GL_TEXTURE_2D, 3, 3, TextureImage->sizeX/8, TextureImage->sizeY/8, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data3 );

...
```

- *automatski* -> koriscenjem [[gluBuild2DMipmaps]]
``` c++
gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

```