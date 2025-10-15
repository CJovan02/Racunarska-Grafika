
```c++
void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
```

Podesava boju koju OpenGL koristi da bi resetovao **color bufffer.**

Kad god se pozove funkcija **glClear(GL_COLOR_BUFFER_BIT)** openGL resetuje color buffer onako kako je specificirano u **glClearColor**

U DrawScene se ovo poziva:
```c++
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

*GL_COLOR_BUFFER_BIT* sluzi da resetuje color buffer.
*GL_DEPTH_BUFFER_BIT* sluzi da depth testing kalkulacije budu tacne za trenutni frame i ne sadrze podatke od prethodnog frame-a.