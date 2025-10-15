
U ovoj funkciji pisemo kod za crtanje scene u OpenGL aplikaciji.

Ova funkcija se poziva u **OnDraw** funkciji u view.cpp fajlu.

Boilerplate kod koji uvek mora da stoji u ovoj funkciji:

```c++
void CGLRenderer::DrawScene(CDC *pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez,
				  GLdouble centerx, GLdouble centery, GLdouble centerz,
				  GLdouble upx, GLdouble upy, GLdouble upz);

		
	glFlush();
    SwapBuffers(pDC->m_hDC);
    }
    wglMakeCurrent(NULL, NULL);
}

```

view.cpp:
```c++
void CGLKView::OnDraw(CDC* pDC)  
{  
    CGLKDoc* pDoc = GetDocument();  
    ASSERT_VALID(pDoc);  
    if (!pDoc)  
       return;  
  
    m_glRenderer.DrawScene(pDC);  
}
```

- [[wglMakeCurrent]]
- [[glClearColor i glClear]]
- [[glLoadIdentity]]
- [[glFlush]]
- [[SwapBuffers]]

Za podesavanje pogleda [[gluLookAt]].