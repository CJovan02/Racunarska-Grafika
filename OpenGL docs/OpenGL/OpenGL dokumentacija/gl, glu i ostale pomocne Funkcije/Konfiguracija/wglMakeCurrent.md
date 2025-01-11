
Windows-specific OpenGL funkcija koja <mark style="background: #FFB86CA6;">binduje (ili unbinduje) rendering context sa device contextom (DC).</mark>
Na srpskom, kaze openGl aplikaciji gde da crta scenu.

```c++
BOOL wglMakeCurrent(HDC hdc, HGLRC hglrc);
```

Ovo koristimo zato sto OpenGL komande nisu linkovane sa nekim specificnim Windows prozorom osim ako mi to eksplicitno ne navedemo.

## Sta koristimo u kodu

Ovo pozivamo na pocetku neke glavne (vec definisane) funkcije u GLRenderer.cpp fajlu:
```c++
wglMakeCurrent(pDC->m_hDC, m_hrc);
```

A ovo na kraju te funkcije:
```c++
wglMakeCurrent(NULL, NULL);
```

**m_hrc je OpenGl Rendering Context**
**this->m_hDC je Device Context**

Radi preglednosti izmedju ovih funkcija mogu da se koriste {}.

