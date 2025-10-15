```c++

void glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *texels);

```

- *target* -> tip teksture (GL_TEXTURE_2D), 
- *level* -> nivo teksture koji se popunjava, 
- *internalFormat* -> broj komponenata jednog teksela (1, 2, 3, 4 ili jednu od 55 drugih predefinisanih vrednosti), najčešće koristimo GL_RGB (3) ili GL_RGBA (4) 
- *width* -> širina u [[teksel]]ima (mora biti stepen broja 2), 
- *height* -> visina u tekselima (mora biti stepen broja 2), 
- *border* -> širina okvira (čitava slika je dimenzija (2m+border) * (2n+border) ), 
- *format* -> format teksela (GL_RGB, GL_RGBA, ...), 
- *type* -> tip podataka koji formiraju teksel (GL_UNSIGNED_BYTE, ...), 
- *texels* -> pokazivač na bafer sa podacima, koje treba preneti u teksturu.