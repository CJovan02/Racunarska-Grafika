
// OptimumPrajmView.cpp : implementation of the COptimumPrajmView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OptimumPrajm.h"
#endif

#include "OptimumPrajmDoc.h"
#include "OptimumPrajmView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TO_RAD(x) {x * PI/180}
const int GUNIT = 25; // Grid Unit
const float PI = 3.141592653;


// COptimumPrajmView

IMPLEMENT_DYNCREATE(COptimumPrajmView, CView)

BEGIN_MESSAGE_MAP(COptimumPrajmView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// COptimumPrajmView construction/destruction

COptimumPrajmView::COptimumPrajmView() noexcept
{
	// TODO: add construction code here

}

COptimumPrajmView::~COptimumPrajmView()
{
}

BOOL COptimumPrajmView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

#pragma region Funkcije Transformacije
void COptimumPrajmView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xform{};
	xform.eM11 = 1.0;
	xform.eM12 = 0.0;
	xform.eM21 = 0.0;
	xform.eM22 = 1.0;
	xform.eDx = dX;
	xform.eDy = dY;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void COptimumPrajmView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM xform{};
	xform.eM11 = cos(angle);
	xform.eM12 = sin(angle);
	xform.eM21 = -sin(angle);
	xform.eM22 = cos(angle);
	xform.eDx = 0.0;
	xform.eDy = 0.0;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
#pragma endregion

#pragma region Funkcije za crtanje transformersa
void COptimumPrajmView::DrawBg(CDC* pDC, bool better)
{
	CBrush bgBrush(better ? RGB(255, 0, 0) : RGB(255, 255, 255));
	CBrush* old = pDC->SelectObject(&bgBrush);

	pDC->Rectangle(0, 0, 1000, 600);

	pDC->SelectObject(old);
}

void COptimumPrajmView::DrawCircle(CDC* pDC, bool better)
{
	CPen whitePen(PS_SOLID, 1, better ? RGB(255, 0, 0) : RGB(255, 255, 255));
	CPen* old = pDC->SelectObject(&whitePen);

	pDC->Ellipse(9 * GUNIT, 3 * GUNIT, 27 * GUNIT, 21 * GUNIT);

	pDC->SelectObject(old);
}

void COptimumPrajmView::DrawCross(CDC* pDC, bool better)
{
	XFORM oldTrans;
	pDC->GetWorldTransform(&oldTrans);
	CBrush black(RGB(0, 0, 0));
	CBrush* old = pDC->SelectObject(&black);
	
	float angle = better ? 45 : 90;
	Rotate(pDC, TO_RAD(angle));
	Translate(pDC, 450, 300);
	pDC->Rectangle(-6 * GUNIT, -1.15 * GUNIT, 6 * GUNIT, 1.15 * GUNIT);

	Translate(pDC, -450, -300);
	Rotate(pDC, TO_RAD(-90));
	Translate(pDC, 450, 300);
	pDC->Rectangle(-6 * GUNIT, -1.15 * GUNIT, 6 * GUNIT, 1.15 * GUNIT);

	pDC->SelectObject(old);
	pDC->SetWorldTransform(&oldTrans);
}

void COptimumPrajmView::DrawEdge(CDC* pDC, int x, int y, int angle, bool leftCenter, bool betterCross, bool betterEdges, bool evenBetterEdges)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);
	CBrush black(RGB(0, 0, 0));
	CBrush* old = pDC->SelectObject(&black);

	float offsetX;
	leftCenter ? offsetX = 2.39: offsetX = -2.39;

	if (betterEdges || evenBetterEdges)
	{
		int angle2;
		(betterEdges && evenBetterEdges) ? angle2 = -90 : angle2 = -45;
		Translate(pDC, offsetX * GUNIT, 0);
		Rotate(pDC, TO_RAD(angle2));
		Translate(pDC, -offsetX * GUNIT, 0);
	}
	Rotate(pDC, TO_RAD(angle));
	Translate(pDC, x, y);
	if (betterCross)
	{
		Translate(pDC, -18 * GUNIT, -12 * GUNIT);
		Rotate(pDC, TO_RAD(45));
		Translate(pDC, 18 * GUNIT, 12 * GUNIT);
	}
	pDC->Rectangle(-3.5 * GUNIT, -1.15 * GUNIT, 3.5 * GUNIT, 1.15 * GUNIT);
	 
	pDC->SelectObject(old);
	pDC->SetWorldTransform(&oldForm);
}
void COptimumPrajmView::DrawMan(CDC* pDC, bool wave)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);
	
	CBrush blackBrush(RGB(0, 0, 0));
	CBrush redBrush(RGB(255, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&blackBrush);

	int angle;
	wave ? angle = 135 : angle = 90;

	float figurePositionX = 30 * GUNIT;
	float fiturePositionY = 8 * GUNIT;

	Translate(pDC, 0, 1 * GUNIT);
	Rotate(pDC, TO_RAD(angle));
	Translate(pDC, figurePositionX, fiturePositionY + 2 * GUNIT);
	pDC->Rectangle(-0.25 * GUNIT, -1 * GUNIT, 0.25 * GUNIT, 1 * GUNIT);
	pDC->SelectObject(redBrush);
	Translate(pDC, 0.3 * GUNIT, 0);
	pDC->Rectangle(-0.25 * GUNIT, -0.2 * GUNIT, 0.25 * GUNIT, 0.2 * GUNIT);
	Translate(pDC, -0.3 * GUNIT, 0);
	pDC->SelectObject(blackBrush);
	Translate(pDC, -figurePositionX, -fiturePositionY - 2 * GUNIT);
	Rotate(pDC, TO_RAD(-angle));

	Translate(pDC, figurePositionX, fiturePositionY);
	pDC->Ellipse(-0.65 * GUNIT, -0.65 * GUNIT, 0.65 * GUNIT, 0.65 * GUNIT);

	Translate(pDC, 0, 2.5 * GUNIT);
	pDC->Rectangle(-0.25 * GUNIT, -2.5 * GUNIT, 0.25 * GUNIT, 2.5 * GUNIT);
	Translate(pDC, 0, -2.5 * GUNIT);

	pDC->SelectObject(oldBrush);
	pDC->SetWorldTransform(&oldForm);
}
#pragma endregion

void COptimumPrajmView::DrawGrid(CDC* pDC)
{ 
	CPen gridPen(BS_SOLID, 1, RGB(230, 230, 230));
	CPen* oldPen = pDC->SelectObject(&gridPen);

	for (int x = 0; x <= 1000; x += GUNIT)
	{
		// Horizontalne linije
		pDC->MoveTo(0, x);
		pDC->LineTo(1000, x);

		// Vertikalne linije
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, 600);
	}

	pDC->SelectObject(oldPen);
}

void COptimumPrajmView::OnDraw(CDC* pDC)
{
	int oldMode = pDC->SetGraphicsMode(GM_ADVANCED);

	COptimumPrajmDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawBg(pDC, betterBg);
	DrawCircle(pDC, betterBg);
	DrawCross(pDC, betterCross);
	DrawEdge(pDC, 18 * GUNIT, 9.5 * GUNIT, 90,true, betterCross, betterEdges, evenBetterEdges);
	DrawEdge(pDC, 20.5 * GUNIT, 12 * GUNIT, 0, false, betterCross, betterEdges, evenBetterEdges);
	DrawEdge(pDC, 18 * GUNIT, 14.5 * GUNIT, 90, false, betterCross, betterEdges, evenBetterEdges);
	DrawEdge(pDC, 15.5 * GUNIT, 12 * GUNIT, 0, true, betterCross, betterEdges, evenBetterEdges);

	if (betterBg && betterCross && betterEdges && evenBetterEdges) DrawMan(pDC, wave);

	gridEnabled ? DrawGrid(pDC) : NULL;

	pDC->SetGraphicsMode(oldMode);
}


// COptimumPrajmView printing

BOOL COptimumPrajmView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COptimumPrajmView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COptimumPrajmView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// COptimumPrajmView diagnostics

#ifdef _DEBUG
void COptimumPrajmView::AssertValid() const
{
	CView::AssertValid();
}

void COptimumPrajmView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COptimumPrajmDoc* COptimumPrajmView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COptimumPrajmDoc)));
	return (COptimumPrajmDoc*)m_pDocument;
}
#endif //_DEBUG


// COptimumPrajmView message handlers


void COptimumPrajmView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	
	switch (nChar)
	{
		case 'G':
			gridEnabled = !gridEnabled;
			break;
		case 'N':
			betterCross = !betterCross;
			break;
		case 'A':
			betterEdges = !betterEdges;
			break;
		case 'Z':
			betterBg = !betterBg;
			break;
		case 'I':
			evenBetterEdges = !evenBetterEdges;
			break;
		case 'H':
			wave = !wave;
			break;
		default:
			break;
	}
	Invalidate();
}
