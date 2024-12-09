
// IND_18981View.cpp : implementation of the CIND18981View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_18981.h"
#endif

#include "IND_18981Doc.h"
#include "IND_18981View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIND18981View

IMPLEMENT_DYNCREATE(CIND18981View, CView)

BEGIN_MESSAGE_MAP(CIND18981View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//	ON_WM_MBUTTONDOWN()
	//	ON_WM_MBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND18981View construction/destruction

CIND18981View::CIND18981View() noexcept
{
	// TODO: add construction code here

}

CIND18981View::~CIND18981View()
{
}

BOOL CIND18981View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND18981View drawing

void CIND18981View::OnDraw(CDC* pDC)
{
	CIND18981Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}


// CIND18981View printing

BOOL CIND18981View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND18981View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND18981View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND18981View diagnostics

#ifdef _DEBUG
void CIND18981View::AssertValid() const
{
	CView::AssertValid();
}

void CIND18981View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND18981Doc* CIND18981View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND18981Doc)));
	return (CIND18981Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND18981View message handlers


int CIND18981View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


void CIND18981View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CIND18981View::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


BOOL CIND18981View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CIND18981View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


void CIND18981View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);

	m_lastPos = point;
	m_dragging = true;
}


void CIND18981View::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);

	m_dragging = false;
}


void CIND18981View::OnMouseMove(UINT nFlags, CPoint point)
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


BOOL CIND18981View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	float zoomSensitivity = 0.5;
	// Scroll Up
	if (zDelta > 0)
	{
		m_glRenderer.viewR = max(1, m_glRenderer.viewR - zoomSensitivity);
	}
	else
	{
		// Scroll down
		m_glRenderer.viewR = min(100.0, m_glRenderer.viewR + zoomSensitivity); // Zoom out
	}

	m_glRenderer.CalculatePosition();
	Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CIND18981View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	float moveSensitivity = 0.5;
	float rotationSensitivity = 5;

	switch (nChar)
	{
	case 'W': m_glRenderer.centery += moveSensitivity;
		break;
	case 'S': m_glRenderer.centery -= moveSensitivity;
		break;
	case 'A': m_glRenderer.centerx -= moveSensitivity;
		break;
	case 'D': m_glRenderer.centerx += moveSensitivity;
		break;
	case VK_LEFT: m_glRenderer.cactusPartAngle += rotationSensitivity;
		break;
	case VK_RIGHT: m_glRenderer.cactusPartAngle -= rotationSensitivity;
		break;
	default:
		break;
	}
	m_glRenderer.CalculatePosition();
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
