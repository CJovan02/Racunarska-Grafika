
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

const int UNIT = 25;


// CIND18981View

IMPLEMENT_DYNCREATE(CIND18981View, CView)

BEGIN_MESSAGE_MAP(CIND18981View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONUP()
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

void CIND18981View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle) {
	POINT* points = new POINT[n];
	//da bi se dobio korak izmedju stranica
	float step = 2 * 3.14 / n;
	float angle = rotAngle;

	for (int i = 0; i < n; i++, angle += step) {
		points[i].x = cx + r * cos(angle);
		points[i].y = cy + r * sin(angle);
	}
	pDC->Polygon(points, n);
	delete[] points;
}

// CIND18981View drawing

void CIND18981View::OnDraw(CDC* pDC)
{
	CIND18981Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CPen penGrid(PS_SOLID, 1, RGB(255, 255, 255));
	CPen polyPen(PS_SOLID, 5, RGB(0, 0, 255));
	CPen polyPen2(PS_SOLID, 2, RGB(0, 0, 255));
	CBrush bgBrush(RGB(220, 220, 220));

	pDC->SelectObject(bgBrush);
	pDC->Rectangle(0, 0, 500, 500);

	// Roze trougao
	CBrush pinkBrush(0xFA4AFF);

	pDC->SelectObject(polyPen);
	pDC->SelectObject(pinkBrush);

	POINT pinkPts[3] = {
		CPoint(1 * UNIT, 1 * UNIT),
		CPoint(18 * UNIT, 1 * UNIT),
		CPoint(9.5 * UNIT, 9.5 * UNIT)
	};
	pDC->Polygon(pinkPts, 3);

	// Zuti Trougao
	CBrush yellowBrush(RGB(255, 255, 51));

	pDC->SelectObject(yellowBrush);
	pDC->SelectObject(polyPen);

	POINT yellowPts[3] = {
		CPoint(1 * UNIT, 1 * UNIT),
		CPoint(1 * UNIT, 13 * UNIT),
		CPoint(7 * UNIT, 7 * UNIT)
	};
	pDC->Polygon(yellowPts, 3);

	// Zeleni Trougao
	CBrush greenBrush(RGB(17, 255, 0));

	pDC->SelectObject(greenBrush);

	POINT greenPts[3] = {
		CPoint(1 * UNIT, 13 * UNIT),
		CPoint(7 * UNIT, 7 * UNIT),
		CPoint(7 * UNIT, 13 * UNIT)
	};

	pDC->Polygon(greenPts, 3);


	// Crveni kvardrat
	CBrush redBrush(RGB(255, 0, 0));

	pDC->SelectObject(redBrush);

	POINT redPts[4] = {
		CPoint(1 * UNIT, 13 * UNIT),
		CPoint(7 * UNIT, 13 * UNIT),
		CPoint(7*UNIT, 19*UNIT),
		CPoint(1 * UNIT, 19 * UNIT)
	};

	pDC->Polygon(redPts, 4);

	// narandzast trougao
	CBrush orangeBrush(RGB(255, 190, 0));

	pDC->SelectObject(orangeBrush);

	POINT orangePts[3] = {
		CPoint(7 * UNIT, 13 * UNIT),
		CPoint(7 * UNIT, 19 * UNIT),
		CPoint(13*UNIT, 19*UNIT)
	};

	pDC->Polygon(orangePts, 3);

	//paralelogram
	CBrush violet(RGB(153, 51, 255));

	pDC->SelectObject(violet);

	POINT paralelogram[4] = {
		CPoint(13 * UNIT, 19 * UNIT),
		CPoint(7 * UNIT, 13 * UNIT),
		CPoint(13 * UNIT, 13 * UNIT),
		CPoint(19 * UNIT, 19 * UNIT)
	};

	pDC->Polygon(paralelogram, 4);

	//srafiran trougao
	CBrush srafiranje(HS_HORIZONTAL, RGB(0, 0, 255));

	pDC->SelectObject(srafiranje);

	POINT srafiran[3] = {
		CPoint(19 * UNIT, 2 * UNIT),
		CPoint(10.5 * UNIT, 10.5 * UNIT),
		CPoint(19 * UNIT, 19 * UNIT)
	};

	pDC->Polygon(srafiran, 3);

	// pravilni mnogouglovi
	CBrush nullBrush(NULL_BRUSH);
	pDC->SelectObject(polyPen2);

	pDC->SelectObject(pinkBrush);
	DrawRegularPolygon(pDC, 9.5 * UNIT, 4.5 * UNIT, 1.7 * UNIT, 5, 0);
	pDC->SelectObject(yellowBrush);
	DrawRegularPolygon(pDC, 3.5 * UNIT, 7 * UNIT, 1.5 * UNIT, 6, 0);
	pDC->SelectObject(greenBrush);
	DrawRegularPolygon(pDC, 5.3 * UNIT, 11.3 * UNIT, 1 * UNIT, 8, 0);
	pDC->SelectObject(orangeBrush);
	DrawRegularPolygon(pDC, 8.8 * UNIT, 17.2 * UNIT, 0.8 * UNIT, 7, 0);
	pDC->SelectObject(srafiranje);
	DrawRegularPolygon(pDC, 15.5 * UNIT, 10.5 * UNIT, 1.5 * UNIT, 4, 0);

	// GRID
	if (gridEnabled) 
	{
		pDC->SelectObject(penGrid);
		for (int i = UNIT; i <= 500; i += UNIT) {
			pDC->MoveTo(CPoint(i, 0));
			pDC->LineTo(CPoint(i, 500));
			pDC->MoveTo(CPoint(0, i));
			pDC->LineTo(CPoint(500, i));
		}
	}
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


void CIND18981View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonUp(nFlags, point);
	gridEnabled = !gridEnabled;
	Invalidate();
}
