
// GLKView.cpp : implementation of the CGLKView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GLK.h"
#endif

#include "GLKDoc.h"
#include "GLKView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TO_DEG(a) (a * 180 / 3.141592)


// CGLKView

IMPLEMENT_DYNCREATE(CGLKView, CView)

BEGIN_MESSAGE_MAP(CGLKView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGLKView construction/destruction

CGLKView::CGLKView()
{
	// TODO: add construction code here

}

CGLKView::~CGLKView()
{
}

BOOL CGLKView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGLKView drawing

void CGLKView::OnDraw(CDC* pDC)
{
	CGLKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}


// CGLKView printing

BOOL CGLKView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGLKView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGLKView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGLKView diagnostics

#ifdef _DEBUG
void CGLKView::AssertValid() const
{
	CView::AssertValid();
}

void CGLKView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGLKDoc* CGLKView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGLKDoc)));
	return (CGLKDoc*)m_pDocument;
}
#endif //_DEBUG


// CGLKView message handlers


int CGLKView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


void CGLKView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CGLKView::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);	
}


BOOL CGLKView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CGLKView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}

void CGLKView::OnMButtonDown(UINT nFlags, CPoint point)
{
	CView::OnMButtonDown(nFlags, point);
}

void CGLKView::OnMButtonUp(UINT nFlags, CPoint point)
{
	CView::OnMButtonUp(nFlags, point);
}

void CGLKView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);

	m_dragging = true;
	m_lastPos = point;
}

void CGLKView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);


	m_dragging = false;
}

void CGLKView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);

	if (m_dragging)
	{
		int deltaX = point.x - m_lastPos.x;
		int deltaY = point.y - m_lastPos.y;

		double sensitivity = 1 / 5.0;

		m_glRenderer.RotateView(deltaY * sensitivity, deltaX * sensitivity);

		Invalidate();
	}
	m_lastPos = point;
}

BOOL CGLKView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_glRenderer.ZoomView(zDelta > 0);
	Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CGLKView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar)
	{
		case '1':
			if (m_glRenderer.arm1Angle <= -65) break;

			m_glRenderer.arm1Angle -= 3;
			break;
		case '2':
			if (m_glRenderer.arm1Angle >= 65) break;

			m_glRenderer.arm1Angle += 3;
			break;
		case '3':
			if (m_glRenderer.arm2Angle <= -80) break;

			m_glRenderer.arm2Angle -= 3;
			break;
		case '4':
			if (m_glRenderer.arm2Angle >= 120) break;

			m_glRenderer.arm2Angle += 3;
			break;
		case '5':
			if (m_glRenderer.headAngle <= -160) break;

			m_glRenderer.headAngle -= 3;
			break;
		case '6':
			if (m_glRenderer.headAngle >= -25) break;

			m_glRenderer.headAngle += 3;
			break;

		case VK_RIGHT:
			m_glRenderer.viewAngleXZ -= TO_DEG(0.1);
			break;

		case VK_LEFT:
			m_glRenderer.viewAngleXZ += TO_DEG(0.1);
			break;
	}
	m_glRenderer.CalculatePosition();
	Invalidate();
}
