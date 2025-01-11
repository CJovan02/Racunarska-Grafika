
|                  |        |        |
| ---------------- | ------ | ------ |
| void glNormal3b( | GLbyte | nx,    |
|                  | GLbyte | ny,    |
|                  | GLbyte | nz`)`; |

|                  |          |        |
| ---------------- | -------- | ------ |
| void glNormal3d( | GLdouble | nx,    |
|                  | GLdouble | ny,    |
|                  | GLdouble | nz`)`; |

|                  |         |        |
| ---------------- | ------- | ------ |
| void glNormal3f( | GLfloat | nx,    |
|                  | GLfloat | ny,    |
|                  | GLfloat | nz`)`; |

|                  |       |        |
| ---------------- | ----- | ------ |
| void glNormal3i( | GLint | nx,    |
|                  | GLint | ny,    |
|                  | GLint | nz`)`; |

|                  |         |        |
| ---------------- | ------- | ------ |
| void glNormal3s( | GLshort | nx,    |
|                  | GLshort | ny,    |
|                  | GLshort | nz`)`; |

## Parametri

*nx, ny, nz*
Specificiraju **pravac i smer** vektora a ne poziciju u 3D prostoru.

Na primer (1, 0, 0) ima pravac direktno po X osi, a smer jer u prema pozitivnom delu X ose.
(1, 1, 0) - ako uzmemo ugao od 45 stepeni izmedju X i Y ose, to bi bio pravac. Smer je u pozitivnom kvadrantu XY ravni.

Ova funkcija se zove pre nego da se pozove [[glVertex]]. 
Funkcionise tako sto postavalja novi vektor normale, sto znaci da svaki sledeci vertex imati isto taj vektor.