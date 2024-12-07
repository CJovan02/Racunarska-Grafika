
Zaduzena je za promenu renderovanja kada se menja velicina prozora. Uglavnom sluzi da se podesi viewport i matrica projekcije omogucavajuci da se scena renderuje kako treba bez obrizra na velicinu prozora.

```c++
void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
    // Prevent division by zero
    if (h == 0)
        h = 1;

    wglMakeCurrent(pDC->m_hDC, m_hrc);
	{
	    // Set the viewport to cover the entire window
	    glViewport(0, 0, w, h);

	    // Adjust the projection matrix
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluPerspective(45.0, (double)w / h, 1.0, 100.0);

	    // Reset to the ModelView matrix
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	}
    wglMakeCurrent(NULL, NULL);
}

```

view.cpp:
```c++
void CGLKView::OnSize(UINT nType, int cx, int cy)  
{  
    CView::OnSize(nType, cx, cy);  
  
    CDC* pDC = GetDC();  
    m_glRenderer.Reshape(pDC, cx, cy);  
    ReleaseDC(pDC);  
}
```
## [[Podesavanje matrice projekcija]]

