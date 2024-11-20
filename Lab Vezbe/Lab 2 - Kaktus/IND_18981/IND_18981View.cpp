
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

const int GUNIT = 25; // Grid Unit
const float PI = 3.141592653;
const int CENTER_X = 14 * GUNIT;
const int CENTER_Y = 10 * GUNIT;
#define TO_RAD(x) {x * PI / 180}


// CIND18981View

IMPLEMENT_DYNCREATE(CIND18981View, CView)

BEGIN_MESSAGE_MAP(CIND18981View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
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

// Funkcije transformacije
void CIND18981View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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
void CIND18981View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
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
void CIND18981View::Rotate(CDC* pDC, float angle, bool rightMultiply)
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
void CIND18981View::DrawBg(CDC* pDC)
{
	CBrush bgBrush(RGB(135, 206, 235));
	CBrush* oldBrush = pDC->SelectObject(&bgBrush);
	pDC->Rectangle(0, 0, 500, 500);

	pDC->SelectObject(oldBrush);
}

void CIND18981View::DrawGrid(CDC* pDC)
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

void CIND18981View::DrawPot(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush brush(RGB(222, 148, 0));

	CPen* oldPen = pDC->SelectObject(&pen);
	CBrush* oldBrush = pDC->SelectObject(&brush);

	POINT points[8] = {
		CPoint(17.1 * GUNIT, 12.2 * GUNIT),
		CPoint(18 * GUNIT, 12.2 * GUNIT),
		CPoint(18 * GUNIT, 12 * GUNIT),
		CPoint(20 * GUNIT, 11.7 * GUNIT),
		CPoint(20 * GUNIT, 8.3 * GUNIT),
		CPoint(18 * GUNIT, 8 * GUNIT),
		CPoint(18 * GUNIT, 7.8 * GUNIT),
		CPoint(17.1 * GUNIT, 7.8 * GUNIT)
	};

	pDC->Polygon(points, 8);

	pDC->MoveTo(18 * GUNIT, 12 * GUNIT);
	pDC->LineTo(18 * GUNIT, 8 * GUNIT);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	pDC->SetWorldTransform(&oldForm);
}

void CIND18981View::DrawText(CDC* pDC, int size, char* text, int x, int y, int degrees, COLORREF color)
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

void CIND18981View::DrawNode(CDC* pDC, float x, float y, float angleAroundPot)
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

void CIND18981View::DrawCactusPart(CDC* pDC, HENHMETAFILE* metaFile, float x, float y, int width, int height, float angleAroundPot, float angleFixed, float angleAroundNode)
{
	XFORM oldTransformation;
	pDC->GetWorldTransform(&oldTransformation);

	width >>= 1;
	height >>= 1;

	Scale(pDC, -width, height);

	if (angleAroundNode != 0)
	{
		Translate(pDC, 0, -height);
		Rotate(pDC, TO_RAD(angleAroundNode));
		Translate(pDC, 0, height);
	}

	Rotate(pDC, TO_RAD(angleFixed));
	Translate(pDC, x - CENTER_X, y - CENTER_Y);
	Rotate(pDC, TO_RAD(angleAroundPot));
	Translate(pDC, CENTER_X, CENTER_Y);


	pDC->PlayMetaFile(*metaFile, CRect(-1, -1, 1, 1));

	pDC->SetWorldTransform(&oldTransformation);
}

void CIND18981View::DrawFigure(CDC* pDC)
{
	int oldMode = pDC->SetGraphicsMode(GM_ADVANCED);

	HENHMETAFILE lightGreen = GetEnhMetaFile(_T("cactus_part_light.emf"));
	HENHMETAFILE darkGreen = GetEnhMetaFile(_T("cactus_part.emf"));
	if (lightGreen == NULL || darkGreen == NULL)
		return;

	DrawCactusPart(pDC, &darkGreen, 15.5 * GUNIT, 10 * GUNIT, 2.6 * GUNIT, 3 * GUNIT, 0, 90);
	DrawCactusPart(pDC, &darkGreen, 12.5 * GUNIT, 10 * GUNIT, 0.8 * GUNIT, 3 * GUNIT, 0, 90);
	DrawCactusPart(pDC, &lightGreen, 13 * GUNIT, 9 * GUNIT, 0.8 * GUNIT, 3.2 * GUNIT, angleAroundPot, -45);
	DrawCactusPart(pDC, &darkGreen, 13 * GUNIT, 11 * GUNIT, 0.8 * GUNIT, 3.2 * GUNIT, 0, 45);
	DrawCactusPart(pDC, &darkGreen, 10.4 * GUNIT, 7.8 * GUNIT, 1.6 * GUNIT, 3 * GUNIT, angleAroundPot, -90);
	DrawCactusPart(pDC, &darkGreen, 11.8 * GUNIT, 6.3 * GUNIT, 1.6 * GUNIT, 3 * GUNIT, angleAroundPot);
	DrawCactusPart(pDC, &lightGreen, 11.9 * GUNIT, 3.4 * GUNIT, 1.6 * GUNIT, 3 * GUNIT, angleAroundPot, 0, angleAroundNode - 45);
	DrawCactusPart(pDC, &darkGreen, 11 * GUNIT, 13.2 * GUNIT, 2.6 * GUNIT, 3 * GUNIT, 0, 45);
	DrawCactusPart(pDC, &darkGreen, 10 * GUNIT, 9 * GUNIT, 1.6 * GUNIT, 3 * GUNIT, 0, -45);
	DrawCactusPart(pDC, &darkGreen, 10 * GUNIT, 11 * GUNIT, 1.6 * GUNIT, 3 * GUNIT, 0, 45);
	DrawCactusPart(pDC, &darkGreen, 12.8 * GUNIT, 3.8 * GUNIT, 1.6 * GUNIT, 3 * GUNIT, angleAroundPot, 45);

	DrawNode(pDC, 17 * GUNIT, 10 * GUNIT, 0);
	DrawNode(pDC, 14 * GUNIT, 10 * GUNIT, 0);
	DrawNode(pDC, 11 * GUNIT, 10 * GUNIT, 0);
	DrawNode(pDC, 11.9 * GUNIT, 12.1 * GUNIT, 0);
	DrawNode(pDC, 11.9 * GUNIT, 7.9 * GUNIT, angleAroundPot);
	DrawNode(pDC, 11.9 * GUNIT, 4.9 * GUNIT, angleAroundPot);

	DrawPot(pDC);

	pDC->SetGraphicsMode(oldMode);
}

// CKaktusView drawing

void CIND18981View::OnDraw(CDC* pDC)
{
	CIND18981Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawBg(pDC);

	DrawFigure(pDC);

	if (gridEnabled)
		DrawGrid(pDC);
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


void CIND18981View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
	case 'G':
		gridEnabled = !gridEnabled;
		break;
	case 'W':
		angleAroundNode -= 10;
		break;
	case 'S':
		angleAroundNode += 10;
		break;
	case 'A':
		angleAroundPot -= 10;
		break;
	case 'D':
		angleAroundPot += 10;
		break;
	default:
		break;
	}
	Invalidate();
}
