
// KaktusView.cpp : implementation of the CKaktusView class
//

#include "pch.h"
#include "framework.h"
#include "math.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kaktus.h"
#endif

#include "KaktusDoc.h"
#include "KaktusView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define TO_RAD(x) {x * PI/180}
#endif

const int GUNIT = 25; // Grid Unit
const float PI = 3.141592653;
const int CENTER_X = 10 * GUNIT;
const int CENTER_Y = 17 * GUNIT;


// CKaktusView

IMPLEMENT_DYNCREATE(CKaktusView, CView)

BEGIN_MESSAGE_MAP(CKaktusView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKaktusView construction/destruction

CKaktusView::CKaktusView() noexcept
{
	// TODO: add construction code here

}

CKaktusView::~CKaktusView()
{
}

BOOL CKaktusView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Funkcije transformacije
void CKaktusView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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
void CKaktusView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xform{};
	xform.eM11 = sX;
	xform.eM12 = 0.0;
	xform.eM21 = 0.0;
	xform.eM22 = sY;
	xform.eDx = 0.0;
	xform.eDy = 0.0;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CKaktusView::Rotate(CDC* pDC, float angle, bool rightMultiply)
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

// Crtanje kaktusa
void CKaktusView::DrawBg(CDC* pDC)
{
	CBrush bgBrush(RGB(135, 206, 235));
	CBrush* oldBrush = pDC->SelectObject(&bgBrush);
	pDC->Rectangle(0, 0, 500, 500);

	pDC->SelectObject(oldBrush);
}

void CKaktusView::DrawGrid(CDC* pDC)
{
	CPen gridPen(BS_SOLID, 1, RGB(230, 230, 230));
	CPen* oldPen = pDC->SelectObject(&gridPen);

	for (int x = 0; x <= 500; x += GUNIT)
	{
		// Horizontalne linije
		pDC->MoveTo(0, x);
		pDC->LineTo(500, x);

		// Vertikalne linije
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, 500);
	}

	pDC->SelectObject(oldPen);
}

void CKaktusView::DrawPot(CDC* pDC)
{
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush brush(RGB(222, 148, 0));

	CPen* oldPen = pDC->SelectObject(&pen);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	POINT points[8] = {
		CPoint(7.5 * GUNIT, 17.2 * GUNIT),
		CPoint(7.5 * GUNIT, 18 * GUNIT),
		CPoint(8 * GUNIT, 18 * GUNIT),
		CPoint(8.3 * GUNIT, 20 * GUNIT),
		CPoint(11.7 * GUNIT, 20 * GUNIT),
		CPoint(12 * GUNIT, 18 * GUNIT),
		CPoint(12.5 * GUNIT, 18 * GUNIT),
		CPoint(12.5 * GUNIT, 17.2 * GUNIT)
	};


	pDC->Polygon(points, 8);

	pDC->MoveTo(12.5 * GUNIT, 18 * GUNIT);
	pDC->LineTo(8 * GUNIT, 18 * GUNIT);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CKaktusView::DrawText(CDC* pDC, int size, char* text, int x, int y, int degrees, COLORREF color)
{

	CFont font1;
	font1.CreateFont(size, 0, degrees * (-10), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (CString)"Arial");
	CFont* oldFont = pDC->SelectObject(&font1);
	int oldFontColor = pDC->SetTextColor(color);
	int oldTextAlign = pDC->SetTextAlign(TA_LEFT);
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);

	pDC->TextOut(x, y - size / 2, (CString)text);

	pDC->SetTextColor(oldFontColor);
	pDC->SetTextAlign(oldTextAlign);
	pDC->SetBkMode(oldBkMode);
	pDC->SelectObject(oldFont);
	font1.DeleteObject();
}

void CKaktusView::DrawNode(CDC* pDC, float x, float y, float angleAroundPot)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	CPen pen(BS_SOLID, 1, RGB(0, 0, 0));
	CBrush greenBrush(RGB(0, 204, 0));

	CPen* oldPen = pDC->SelectObject(&pen);
	CBrush* oldBrush = pDC->SelectObject(&greenBrush);

	Translate(pDC, x - CENTER_X, y - CENTER_Y);
	Rotate(pDC, TO_RAD(angleAroundPot));
	Translate(pDC, CENTER_X, CENTER_Y);

	pDC->Ellipse(-10, -10, 10, 10);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	pDC->SetWorldTransform(&oldForm);
}

void CKaktusView::DrawCactusPart(CDC* pDC, HENHMETAFILE* metaFile, float x, float y, int width, int height,  float angleAroundPot, float angleFixed, float angleAroundNode)
{
	XFORM oldTransformation;
	pDC->GetWorldTransform(&oldTransformation);

	width >>= 1;
	height >>= 1;

	Rotate(pDC, TO_RAD(angleFixed));

	if (angleAroundNode != 0)
	{
		Translate(pDC, 0, -height);
		Rotate(pDC, TO_RAD(angleAroundNode));
		Translate(pDC, 0, height);
	}
	Translate(pDC, x - CENTER_X, y - CENTER_Y);
	Rotate(pDC, TO_RAD(angleAroundPot));
	Translate(pDC, CENTER_X, CENTER_Y);

	pDC->PlayMetaFile(*metaFile, CRect(-width, -height, width, height));

	pDC->SetWorldTransform(&oldTransformation);
}

void CKaktusView::DrawFigure(CDC* pDC)
{
	int oldMode = pDC->SetGraphicsMode(GM_ADVANCED);

	HENHMETAFILE lightGreen = GetEnhMetaFile(_T("cactus_part_light.emf"));	
	HENHMETAFILE darkGreen = GetEnhMetaFile(_T("cactus_part.emf"));
	if (lightGreen == NULL || darkGreen == NULL)
		return;

	DrawCactusPart(pDC, &lightGreen, 10 * GUNIT, 15.5 * GUNIT, 2.6 * GUNIT, 3 * GUNIT, angleAroundPot);
	DrawCactusPart(pDC, &darkGreen, 10 * GUNIT, 12.5 * GUNIT, 0.8 * GUNIT, 3 * GUNIT, angleAroundPot);
	DrawCactusPart(pDC, &darkGreen, 9 * GUNIT, 13 * GUNIT, 0.8 * GUNIT, 3.2 * GUNIT, angleAroundPot, -45);
	DrawCactusPart(pDC, &darkGreen, 11 * GUNIT, 13 * GUNIT, 0.8 * GUNIT, 3.2 * GUNIT, angleAroundPot, 45);
	DrawCactusPart(pDC, &darkGreen, 6.4 * GUNIT, 11.9 * GUNIT, 1.6 * GUNIT, 3 * GUNIT, angleAroundPot, - 90);
	DrawCactusPart(pDC, &darkGreen, 7.9 * GUNIT, 10.4 * GUNIT, 1.6 * GUNIT, 3 * GUNIT, angleAroundPot);
	DrawCactusPart(pDC, &darkGreen, 7.9 * GUNIT, 7.4 * GUNIT, 2.4 * GUNIT, 3 * GUNIT, angleAroundPot);
	DrawCactusPart(pDC, &darkGreen, 13.6 * GUNIT, 11.9 * GUNIT, 1.6 * GUNIT, 3 * GUNIT, angleAroundPot, - 90);
	DrawCactusPart(pDC, &darkGreen, 16.2 * GUNIT, 10.9 * GUNIT, 1.6 * GUNIT, 3 * GUNIT, angleAroundPot, 45);
	DrawCactusPart(pDC, &darkGreen, 16.2 * GUNIT, 12.9 * GUNIT, 1.6 * GUNIT, 3 * GUNIT, angleAroundPot, 135);
	DrawCactusPart(pDC, &lightGreen, 12.1 * GUNIT, 10.5 * GUNIT, 1.6 * GUNIT, 3 * GUNIT, angleAroundPot, 0, angleAroundNode);

	DrawNode(pDC, 10 * GUNIT, 17 * GUNIT, angleAroundPot);
	DrawNode(pDC, 10 * GUNIT, 14 * GUNIT, angleAroundPot);
	DrawNode(pDC, 7.9 * GUNIT, 11.9 * GUNIT, angleAroundPot);
	DrawNode(pDC, 7.9 * GUNIT, 8.9 * GUNIT, angleAroundPot);
	DrawNode(pDC, 12.1 * GUNIT, 11.9 * GUNIT, angleAroundPot);
	DrawNode(pDC, 15.1 * GUNIT, 11.9 * GUNIT, angleAroundPot);
	DrawPot(pDC);

	pDC->SetGraphicsMode(oldMode);
}

// CKaktusView drawing

void CKaktusView::OnDraw(CDC* pDC)
{
	CKaktusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawBg(pDC);

	DrawText(pDC, 30, "Jovan Cvetkovic 18981", 477, 152, 90, RGB(0, 0, 0));
	DrawText(pDC, 30, "Jovan Cvetkovic 18981", 475, 150, 90, RGB(255, 255, 0));

	DrawFigure(pDC);

	if(gridEnabled)
		DrawGrid(pDC);
}


// CKaktusView printing

BOOL CKaktusView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKaktusView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKaktusView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKaktusView diagnostics

#ifdef _DEBUG
void CKaktusView::AssertValid() const
{
	CView::AssertValid();
}

void CKaktusView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKaktusDoc* CKaktusView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKaktusDoc)));
	return (CKaktusDoc*)m_pDocument;
}
#endif //_DEBUG


// CKaktusView message handlers


void CKaktusView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar)
	{
		case 'G':
			gridEnabled = !gridEnabled;
			break;
		case 'W':
			angleAroundNode -= 6;
			break;
		case 'S':
			angleAroundNode += 6;
			break;
		case 'A':
			angleAroundPot -= 6;
			break;
		case 'D':
			angleAroundPot += 6;
			break;
		default:
			break;
	}
	Invalidate();
}
