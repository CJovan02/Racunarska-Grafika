
Ova funkcija inicijalizuje pocetni state OpenGL aplikacije, **ovde se ne crta nista nego se pise konfiguracija.**

Poziva se u view.cpp fajlu u funkciji **OnInitialUpdate**. Ova funkcija se automatski poziva kada se view.cpp klasa prvi put inicijalizuje (slicno kao konstruktor samo za MFC lifeclycle).
Ovde se uglavnom kreira Device Context i inicijalizuje OpenGL evrionment.

**Ne moraju sve ove funkcije da se koriste ovo je samo primer nekih**
```c++
void CGLRenderer::PrepareScene(CDC* pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);
	{
	    // Set background clear color
	    glClearColor(POZADINA, 1.0); 
	
	    // Enable depth testing
	    glEnable(GL_DEPTH_TEST);
	
	    // Enable smooth shading
	    glShadeModel(GL_SMOOTH);
	
	    // Optionally: Set up lighting
	    glEnable(GL_LIGHTING);
	    glEnable(GL_LIGHT0);
	
	    // Optionally: Set up blending or transparency
	    glEnable(GL_BLEND);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
    wglMakeCurrent(NULL, NULL);
}

```

view.cpp:
```c++
void CGLKView::OnInitialUpdate()  
{  
    CView::OnInitialUpdate();  
  
    CDC* pDC = GetDC();  
    m_glRenderer.PrepareScene(pDC);  
    ReleaseDC(pDC);  
}
```

- [[wglMakeCurrent]]
- [[glClearColor i glClear]]