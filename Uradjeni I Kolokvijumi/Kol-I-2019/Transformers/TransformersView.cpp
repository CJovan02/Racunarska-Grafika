
// TransformersView.cpp : implementation of the CTransformersView class
//

#include "pch.h"
#include "framework.h"
#include "DImage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Transformers.h"
#endif

using namespace std;

#include "TransformersDoc.h"
#include "TransformersView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const float PI = 3.141592653f;
#define TO_RAD(x) { x * PI/180 }
const int CENTERX = 30;
const int CENTERY = 125;


// CTransformersView

IMPLEMENT_DYNCREATE(CTransformersView, CView)

BEGIN_MESSAGE_MAP(CTransformersView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CTransformersView construction/destruction

CTransformersView::CTransformersView() noexcept
{
	body = new DImage();
	arm1 = new DImage();
	arm2 = new DImage();
	leg1 = new DImage();
	leg2 = new DImage();
	background = new DImage();

	body->Load(CString("./slike/body1.png"));
	arm1->Load(CString("./slike/arm1.png"));
	arm2->Load(CString("./slike/arm2.png"));
	leg1->Load(CString("./slike/leg1.png"));
	leg2->Load(CString("./slike/leg2.png"));
	background->Load(CString("./slike/background.jpg"));

	if (Ucitaj() == 0)
	{
		for (int i = 0; i < 5; i++)
		{
			iksovi[i] = 0;
			ipsiloni[i] = 0;
		}
	}
}

CTransformersView::~CTransformersView()
{
	delete body, arm1, arm2, leg1, leg2, background;
}

BOOL CTransformersView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CTransformersView::DrawBackground(CDC* pDC, CRect rc)
{
	int bgWidth = background->Width();
	int bgHeight = background->Height();

	CRect pozadina(0, 0, bgWidth, bgHeight);
	background->Draw(pDC, pozadina, rc);
}

// Nije nam data DrawTransparent funkciju u DImage pa radimo ovako
void CTransformersView::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	int w = pImage->Width();
	int h = pImage->Height();

	//Translate(pDC, -(w >> 1), -(h >> 1));

	CBitmap bmpImage, bmpMask;

	bmpImage.CreateCompatibleBitmap(pDC, w, h);
	bmpMask.CreateBitmap(w, h, 1, 1, NULL);

	CDC* pSrcDC = new CDC();
	pSrcDC->CreateCompatibleDC(pDC);
	pSrcDC->SelectObject(&bmpImage);
	CDC* pMaskDC = new CDC();
	pMaskDC->CreateCompatibleDC(pDC);
	pMaskDC->SelectObject(&bmpMask);

	pImage->Draw(pSrcDC, CRect(0, 0, w, h), CRect(0, 0, w, h));

	COLORREF pozadina = pSrcDC->GetPixel(0, 0);
	pSrcDC->SetBkColor(pozadina);
	pMaskDC->BitBlt(0, 0, w, h, pSrcDC, 0, 0, SRCCOPY);

	pSrcDC->SetTextColor(RGB(255, 255, 255));
	pSrcDC->SetBkColor(RGB(0, 0, 0));
	pSrcDC->BitBlt(0, 0, w, h, pMaskDC, 0, 0, SRCAND);


	pDC->BitBlt(0, 0, w, h, pMaskDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, w, h, pSrcDC, 0, 0, SRCPAINT);

	delete pSrcDC;
	delete pMaskDC;

	pDC->SetWorldTransform(&oldForm);
}

void CTransformersView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xform;
	xform.eM11 = 1.0;
	xform.eM12 = 0.0;
	xform.eM21 = 0.0;
	xform.eM22 = 1.0;
	xform.eDx = dX;
	xform.eDy = dY;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CTransformersView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	angle = TO_RAD(angle);

	XFORM xform;
	xform.eM11 = cos(angle);
	xform.eM12 = sin(angle);
	xform.eM21 = -sin(angle);
	xform.eM22 = cos(angle);
	xform.eDx = 0.0;
	xform.eDy = 0.0;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CTransformersView::DrawArm1(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	// Rotacija oko sebe
	Translate(pDC, -34, -31);
	Rotate(pDC, (arm1Angle * 2.69) + 90);
	Translate(pDC, 34, 31);
	// Pomeraj figure
	Translate(pDC, iksovi[2], ipsiloni[2]);

	// Ruka mora da prati rotaciju tela i da rotira oko iste tacke
	Translate(pDC, -26 - iksovi[0], -133 - ipsiloni[0]);
	Rotate(pDC, bodyAngle);
	Translate(pDC, 26 + iksovi[0], 133 + ipsiloni[0]);
	
	// Takodje telo mora da prati rotaciju oko leg1 i da rotira oko iste tacke
	Translate(pDC, -CENTERX - iksovi[1], -CENTERY - ipsiloni[1]);
	Rotate(pDC, arm1Angle);
	Translate(pDC, CENTERX + iksovi[1], CENTERY + ipsiloni[1]);

	DrawImgTransparent(pDC, arm1);

	pDC->SetWorldTransform(&oldForm);
}

void CTransformersView::DrawArm2(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	// Rotacija oko sebe
	Translate(pDC, -23, -61);
	Rotate(pDC, arm2Angle);
	Translate(pDC, 23, 61);
	// Pomeraj figure
	Translate(pDC, iksovi[3], ipsiloni[3]);

	// Arm2 prati rotaciju arm1 i rotira oko iste tacke
	Translate(pDC, -34 - iksovi[2], -31 - ipsiloni[2]);
	Rotate(pDC, arm1Angle * 2.69);
	Translate(pDC, 34 + iksovi[2], 31 + ipsiloni[2]);

	// Takodje rotira oko tela
	Translate(pDC, -26 - iksovi[0], -133 - ipsiloni[0]);
	Rotate(pDC, bodyAngle);
	Translate(pDC, 26 + iksovi[0], 133 + ipsiloni[0]);

	// I na kraju rotira oko leg1
	Translate(pDC, -CENTERX - iksovi[1], -CENTERY - ipsiloni[1]);
	Rotate(pDC, arm1Angle);
	Translate(pDC, CENTERX + iksovi[1], CENTERY + ipsiloni[1]);

	DrawImgTransparent(pDC, arm2);

	pDC->SetWorldTransform(&oldForm);
}

void CTransformersView::DrawLeg1(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	// Rotira oko sebe
	Translate(pDC, -30, -125);
	Rotate(pDC, arm1Angle - 100);
	Translate(pDC, 30, 125);
	// Pomeraj figure
	Translate(pDC, iksovi[1], ipsiloni[1]);

	DrawImgTransparent(pDC, leg1);

	pDC->SetWorldTransform(&oldForm);
}

void CTransformersView::DrawLeg2(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	// Rotacija oko sebe
	Translate(pDC, -35, -60);
	Rotate(pDC, leg2Angle + 110);
	Translate(pDC, 35, 60);
	// Pomeraj figure
	Translate(pDC, iksovi[4], ipsiloni[4]);

	// Leg2 mora da prati rotaciju leg1 i da rotara u istoj tacki
	Translate(pDC, -CENTERX - iksovi[1], -CENTERY - ipsiloni[1]);
	Rotate(pDC, arm1Angle);
	Translate(pDC, CENTERX + iksovi[1], CENTERY + ipsiloni[1]);

	DrawImgTransparent(pDC, leg2);

	pDC->SetWorldTransform(&oldForm);
}

void CTransformersView::Body1(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	//Rotacije oko svoje ose
	Translate(pDC, -26, -133);
	Rotate(pDC, bodyAngle);
	Translate(pDC, 26, 133);
	// Pomeraj figure
	Translate(pDC, iksovi[0], ipsiloni[0]);

	// Telo mora da prati rotaciju oko leg1 i da rotira zajedno s njim
	Translate(pDC, -CENTERX - iksovi[1], -CENTERY - ipsiloni[1]);
	Rotate(pDC, arm1Angle);
	Translate(pDC, CENTERX + iksovi[1], CENTERY + ipsiloni[1]);

	DrawImgTransparent(pDC, body);

	pDC->SetWorldTransform(&oldForm);
}

void CTransformersView::DrawPoint(CDC* pDC, int x, int y)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, x, y);
	pDC->Ellipse(-5, -5, 5, 5);

	pDC->SetWorldTransform(&oldForm);
}

void CTransformersView::DrawTransformer(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	DrawArm2(pDC);
	Body1(pDC);
	DrawLeg2(pDC);
	DrawLeg1(pDC);
	DrawArm1(pDC);

	pDC->SetWorldTransform(&oldForm);
}

void CTransformersView::Upisi()
{
	ofstream outputFile("koordinate.txt");

	if (outputFile.is_open())
	{
		for (int i = 0; i < 5; i++)
		{
			outputFile << iksovi[i] << '|'
						<< ipsiloni[i] << endl;
		}
	}

	outputFile.close();
}

int CTransformersView::Ucitaj()
{
	ifstream file("koordinate.txt");

	if (!file) return 0;

	string line;
	int i = 0;
	while (getline(file, line))
	{
		stringstream ss(line);
		vector<string> values;
		string token;

		while (getline(ss, token, '|'))
		{
			values.push_back(token);
		}

		iksovi[i] = stoi(values[0]);
		ipsiloni[i] = stoi(values[1]);

		i++;
	}

	file.close();
	return 1;
}

void CTransformersView::OnDraw(CDC* pDC)
{
	CTransformersDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int oldDCMode = pDC->SetGraphicsMode(GM_ADVANCED);

	CRect clientRect;
	GetClientRect(&clientRect);
	CDC* pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);

	int oldMode = pMemDC->SetGraphicsMode(GM_ADVANCED);

	CBitmap memBitmap;
	memBitmap.CreateBitmap(clientRect.Width(), clientRect.Height(), 4, 8, NULL);
	CBitmap* oldBitMap = pMemDC->SelectObject(&memBitmap);

	// zoves funkcije za crtanje i prosledis pMemDc njima
	DrawBackground(pMemDC, clientRect);

	DrawTransformer(pMemDC);
	//DrawPoint(pMemDC, 34, 31);
	//DrawPoint(pMemDC, 26, 133);

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), pMemDC, 0, 0, SRCCOPY);

	pMemDC->SetGraphicsMode(oldMode);
	pMemDC->SelectObject(oldBitMap);
	delete pMemDC;
}


// CTransformersView printing

BOOL CTransformersView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTransformersView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTransformersView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTransformersView diagnostics

#ifdef _DEBUG
void CTransformersView::AssertValid() const
{
	CView::AssertValid();
}

void CTransformersView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTransformersDoc* CTransformersView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTransformersDoc)));
	return (CTransformersDoc*)m_pDocument;
}
#endif //_DEBUG


// CTransformersView message handlers


void CTransformersView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	switch (nChar)
	{
	case 'Q': 
		arm1Angle += 5; break;
	case 'A':
		arm1Angle -= 5; break;
	case 'R':
		leg2Angle += 5; break;
	case 'F':
		leg2Angle -= 5; break;
	case 'T':
		arm2Angle += 5; break;
	case 'G':
		arm2Angle -= 5; break;
	case 'W':
		bodyAngle += 5; break;
	case 'S':
		bodyAngle -= 5; break;
	case 'J':
		iksovi[indeks] -= pomeraj; break;
	case 'L':
		iksovi[indeks] += pomeraj; break;
	case 'I':
		ipsiloni[indeks] -= pomeraj; break;
	case 'K':
		ipsiloni[indeks] += pomeraj; break;
	case '1':
		indeks = 0; break;
	case '2':
		indeks = 1; break;
	case '3':
		indeks = 2; break;
	case '4':
		indeks = 3; break;
	case '5':
		indeks = 4; break;
	case '6':
		pomeraj = 1; break;
	case '7':
		pomeraj = 20; break;
	case 'P':
		Upisi(); break;
	default:
		break;
	}
	Invalidate();
}


BOOL CTransformersView::OnEraseBkgnd(CDC* pDC)
{
	return 1;
}
