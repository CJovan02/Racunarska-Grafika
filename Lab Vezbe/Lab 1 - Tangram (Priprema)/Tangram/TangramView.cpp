
// TangramView.cpp : implementation of the CTangramView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Tangram.h"
#endif

#include "TangramDoc.h"
#include "TangramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const float PI = 3.14159265f;
const float UNIT = 25;


// CTangramView

IMPLEMENT_DYNCREATE(CTangramView, CView)

BEGIN_MESSAGE_MAP(CTangramView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTangramView construction/destruction

CTangramView::CTangramView() noexcept
{
	// TODO: add construction code here

}

CTangramView::~CTangramView()
{
}

BOOL CTangramView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTangramView drawing

void CTangramView::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
{
	/*
	Implementirati funckciju DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle) koja crta pravilni mnogougao zarotiran za ugao rotAngle, 
	sa n stranica, poluprecnikom opisane kruznice r i centrom u (cx,cy). 
	Za izracunavanje koordinata temena pravilnog mnogougla koristiti jednačine: x=r*cos(alfa); y=r*sin(alfa).
	*/

	POINT* points = new POINT[n];
	float unutrasnjiUgao = 2 * PI / n;
	float ugao = rotAngle;

	for (int i = 0; i < n; i++, ugao += unutrasnjiUgao)
	{
		points[i].x = cx + r * cos(ugao);
		points[i].y = cy + r * sin(ugao);	
	}

	pDC->Polygon(points, n);

	delete[] points;
}

void CTangramView::OnDraw(CDC* pDC)
{
	CTangramDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CPen gridPen(PS_SOLID, 2, RGB(230, 230, 230));
	CPen polyPen(PS_SOLID, 5, RGB(255, 0, 0));
	CPen polyPen2(PS_SOLID, 3, RGB(255, 0, 0));

	CBrush bgBrush(RGB(200, 200, 200));
	
	pDC->SelectObject(bgBrush);
	pDC->Rectangle(0, 0, 500, 500);

	//roze trougao
	CBrush pinkBrush(RGB(255, 192, 203));
	pDC->SelectObject(polyPen);
	pDC->SelectObject(pinkBrush);
	POINT pinkPoints[3] = {
		CPoint(1 * UNIT, 7 * UNIT),
		CPoint(13 * UNIT, 7 * UNIT),
		CPoint(7 * UNIT, 1 * UNIT)
	};
	pDC->Polygon(pinkPoints, 3);

	//mnogougao u roze trougao
	pDC->SelectObject(polyPen2);
	DrawRegularPolygon(pDC, 7 * UNIT, 4.5 * UNIT, 1 * UNIT, 7, 0);

	CRect clientRect;
	GetClientRect(&clientRect);

	//srafiran trougao
	CBrush srafiranje(HS_CROSS, RGB(0, 0, 255));
	pDC->SelectObject(srafiranje);
	POINT srafTrougao[3] = {
		CPoint(1 * UNIT, 19 * UNIT),
		CPoint(13 * UNIT, 19 * UNIT),
		CPoint(13 * UNIT, 7 * UNIT)
	};
	pDC->Polygon(srafTrougao, 3);

	DrawRegularPolygon(pDC, 9.5 * UNIT, 16 * UNIT, 1.5*UNIT, 5, 0);


	pDC->SelectObject(gridPen);

	// vertikalne linije
	for (int x = clientRect.left; x < clientRect.right; x += UNIT)
	{
		pDC->MoveTo(x, clientRect.top);
		pDC->LineTo(x, clientRect.bottom);
	}

	// horizonatalne linije
	for (int y = clientRect.top; y < clientRect.bottom; y += UNIT)
	{
		pDC->MoveTo(clientRect.left, y);
		pDC->LineTo(clientRect.right, y);
	}
}



// CTangramView printing

BOOL CTangramView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTangramView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTangramView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTangramView diagnostics

#ifdef _DEBUG
void CTangramView::AssertValid() const
{
	CView::AssertValid();
}

void CTangramView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTangramDoc* CTangramView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTangramDoc)));
	return (CTangramDoc*)m_pDocument;
}
#endif //_DEBUG


// CTangramView message handlers
