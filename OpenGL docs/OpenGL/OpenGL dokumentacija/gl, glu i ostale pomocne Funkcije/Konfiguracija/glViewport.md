
U OpenGL-u viewport je pravougaoni deo prozora gde se renderovana slika smesta.

```c++
glViewport(x, y, width, height);
```

*x, y*: Donji levi cosak provaougaonika
*width, height*: dimenzija viewport-a

Najcesce koristimo ovako:

```c++
glViewport(0, 0, w, h);
```
gde nam je w i h dato u Reshape funkciji.