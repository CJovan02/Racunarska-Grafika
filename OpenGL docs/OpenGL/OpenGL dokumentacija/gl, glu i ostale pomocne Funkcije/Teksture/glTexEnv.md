```c++
void glTexEnv{if}( GLenum target, GLenum pname, GLint param );
```

Funkcija kojom se definiše način primene teksture, tj. kako se vrednost pročitana iz teksture meša sa bojom fragmenta (materijala).

- *target* mora biti postavljen na **GL_TEXTURE_ENV**, 
- *pname* na **GL_TEXTURE_ENV_MODE**, 
- *param* može imati jednu od sledećih vrednosti:
	- *GL_REPLACE* -> potpuno menja boju fragmenta podacima iz teksture, 
	- *GL_DECAL* -> mešanje boje fragmenta i teksture definisano je providnošću teksture, 
	- *GL_MODULATE* -> boja fragmenata je „modulisana“ teksturom i
	- *GL_BLEND* -> boja se dobija mešanjem boje fragmenta, teksture i posebno zadate boje.