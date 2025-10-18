Postoje nekoliko vrsta teskture koje mogu da se koriste, na ovom predmetu se koriste samo **2D teskture**.

Prvi korak u koriscenju teskture je da se tesktura ucita i da se pripremi za upotrebu.
Za ovo se uglavnom pise jedna funkcija **UINT LoadTexture(chat* fileName)** koja ucitava teskturu iz ime fajla.

# Load Texture

``` c++
UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texID;
	DImage img;
	
	// Ucitamo sliku
	img.Load(CString(fileName));

	// Podesimo kako ce tekstura da se cuva u memoriji
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	
	// Generisemo ID tekstrure i rezervisemo mesto u memoriji
    glGenTextures(1, &texID);
    
    // Kreiramo i aktiviramo objekat teksture
    glBindTexture(GL_TEXTURE_2D, texID);

	// Podesimo parametre "selektove" teksture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    GL_LINEAR_MIPMAP_LINEAR);
    
    // Podesavamo kako ce boja teksture da se mesa sa materijalom
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    // Generesimo mipmape, da bi smo mogli da koristimo 
    // "GL_LINEAR_MIPMAP_LINEAR"
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), 
	    GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBITS());
	    
	/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.Width(), img.Height(), 0,
	 GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());*/
	    
	return texID;
}
```

Funkcija treba da ucita teksturu i vradi njen ID.

Vidimo da se koristi biblioteka *DImage* za ucitavanje slike

Nekon toga vidimo da se koriste nekoliko funkcija za pripremu teksture.

**[[glPixelStorei]]** -> definise nacin na koji je bitmapa (slika) smestena u memoriji. Za ovaj predmet mi uvek koristimo iste parametre *GL_UNPACK_ALIGNMENT* i *4*

[[glGenTextures]] -> pribavlja jedinstveni identifikator (ili vise njih) koje smesta u 2. parametar funkcije.
Znaci ako pozovemo *glGenTextures(1, &texID)* znaci da ce generisati *1* ID i smestiti ga u *texID*

[[glBindTexture]] -> kreira i aktivira objekat teskture, prvi argument je tip teksture (nama treba *GL_TEXTURE_2D*) a drugi argument je ID teskture, koji smo dobili prethodnom funkcijom.

*glTexPrameteri* -> podesava parametre za selektovanu teksturu. Postoje mnogo parametara koji mogu da se podese i koriste, za vise info [[Parametri tekstura]].

[[glTexEnv]] -> odredjuje kako se boja teksture kombinuje sa bojom materijala (ili osvetljenjem). *GL_MODULATE* znaci da, ako koristimo osvetljenje ili boju materijala (`mislim da se boja materijala ne racuna ako obojimo linije kojom smo iscrtali figuru`), tekstura ce biti pod uticajem svetla osvetljenja, odnosno deluje prirodnije.

[[gluBuild2DMipmaps]] -> da bi smo mogli da koristimo *GL_LINEAR_MIPMAP_LINEAR* moramo da generisemo *mipmape* koristeci ovu funkciju.

[[glTexImage2D]] -> ako ne zelimo mipmape da koristimo, mozemo podatke u teksturu ubaciti koriscenjem ove funkcije.

## Koriscenje tekstura

[[Teksturne koordinate]]

Pre koriscenja tekstura moramo da, za svaku teskturu, pozovemo *LoadTexture* i da zapamtimo njen ID. To pisemo u prepare scene.

```c++
void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	m_texSpider = LoadTexture("spider.png");

	m_texEnv[0] = LoadTexture("front.jpg");
	m_texEnv[1] = LoadTexture("back.jpg");

	m_texEnv[2] = LoadTexture("left.jpg");
	m_texEnv[3] = LoadTexture("right.jpg");

	m_texEnv[4] = LoadTexture("top.jpg");
	m_texEnv[5] = LoadTexture("bot.jpg");

	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}
```
Takodje vidimo da moramo da upalimo 2D texture.

```c++
void CGLRenderer::DrawCubeFace(double a, UINT texID)
{
	double aHalf = a / 2;

	glBindTexture(GL_TEXTURE_2D, texID);
	glBegin(GL_QUADS);
	{
		// Bot right
		glTexCoord2f(1, 1);
		glVertex3d(aHalf, -aHalf, 0);

		// Top right
		glTexCoord2f(1, 0);
		glVertex3d(aHalf, aHalf, 0);

		// Top left
		glTexCoord2f(0, 0);
		glVertex3d(-aHalf, aHalf, 0);

		// Bot Left
		glTexCoord2f(0, 1);
		glVertex3d(-aHalf, -aHalf, 0);
	}
	glEnd();
}
```

Ovde se iscrtava jedna strana kocke na koju se "lepi" tekstura. Vidimo da se koristi [[glTexCoord2f]] da bi se mapirali temena teksture se temenima strane kocke.

Ovde sam zamenio *T* vrednost `(S, T)` jer iz nekog razloga kad se ucita slike iz DImage ona bude **zaokrenuta** (Gpt kaze jer DImage koristi drugi koordinatni sistem, pa je zbog toga).

Nakon toga obavezno moramo da oslobodimo zauzetu memoriju tako sto obrisemo teksture.

```c++
void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 

	glDeleteTextures(1, &m_texSpider);
	glDeleteTextures(6, m_texEnv);

	// ...
	wglMakeCurrent(NULL,NULL); 

	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}
```


Ovo je jako prost primer za lepnjenje teksture na kocki, malo je komplikovanije za druga geometrijska tela.

Takodje dosta zavisi od zadatka kako se trazi da se tekstura zalepi, sa ova tri primera ispod bi trebalo vecina slucajeva da se pokrije.

### [[Lepljenje teksture na env cube]]
### [[Lepljenje teksture na sferu]]
### [[Lepljenje teksture na kupu]]
### [[Lepljenje teksture na cilindar]]

Na jednom kolokvijumu se trazilo da se izracuna jedinicni vektor izmedju dve tacke: [[CalcRotAxis]]
