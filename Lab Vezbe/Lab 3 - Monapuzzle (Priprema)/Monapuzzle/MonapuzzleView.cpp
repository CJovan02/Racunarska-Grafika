
// MonapuzzleView.cpp : implementation of the CMonapuzzleView class
//

#include "pch.h"
#include "framework.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Monapuzzle.h"
#include "DImage.h"
#endif

#include "MonapuzzleDoc.h"
#include "MonapuzzleView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TO_RAD(x) {x * PI/180}
const float PI = 3.141592653f;
const int GUNIT = 25; // Grid Unit
const int N = 9;

using namespace std;

// CMonapuzzleView

IMPLEMENT_DYNCREATE(CMonapuzzleView, CView)

BEGIN_MESSAGE_MAP(CMonapuzzleView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMonapuzzleView construction/destruction

CMonapuzzleView::CMonapuzzleView() noexcept
{
	indeksSelekcije = 0;
	if (UcitajKoordinate() == 0)
	{
		for (int i = 0; i < N; i++)
		{
			if (i < 5)
			{
				dinamoX[i] = i * 256;
				dinamoY[i] = 0;
			}
			else
			{
				dinamoX[i] = (i - 5) * 256;
				dinamoY[i] = 256;
			}
			rotacije[i] = rotacije[i] = 0;
			miroriX[i] = miroriY[i] = false;
		}
	}
}

CMonapuzzleView::~CMonapuzzleView()
{

}

BOOL CMonapuzzleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

#pragma region Funkcije Transformacija
void CMonapuzzleView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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
void CMonapuzzleView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
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
void CMonapuzzleView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	angle = TO_RAD(angle);
	XFORM xform{};

	xform.eM11 = cos(angle);
	xform.eM12 = sin(angle);
	xform.eM21 = -sin(angle);
	xform.eM22 = cos(angle);
	xform.eDx = 0.0;
	xform.eDy = 0.0;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CMonapuzzleView::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM xform{};

	xform.eM11 = mx ? -1.0 : 1.0;
	xform.eM12 = 0.0;
	xform.eM21 = 0.0;
	xform.eM22 = my ? -1.0 : 1.0;
	xform.eDx = 0.0;
	xform.eDy = 0.0;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
#pragma endregion

void CMonapuzzleView::Sacuvaj()
{
	ofstream outFile("koordinate.txt");

	if (outFile.is_open())
	{
		for (int i = 0; i < N; i++)
		{
			string mirXForPrint = miroriX[i] ? "true" : "false";
			string mirYForPrint = miroriY[i] ? "true" : "false";

			outFile << dinamoX[i] << "|"
					<< dinamoY[i] << "|"
					<< rotacije[i] << "|"
					<< miroriX[i] << "|"
					<< miroriY[i] << endl;
		}
		
		//outFile << endl << "Ovo samo copy paste-ujes direktno u kodu. Moras sam da promenis format funkcije da bi lepo ispisalo." << endl;
		//for (int i = 0; i < N; i++)
		//{
		//	string mirXForPrint = miroriX[i] ? "true" : "false";
		//	string mirYForPrint = miroriY[i] ? "true" : "false";

		//	outFile << "PlacePuzzle(pDC, CString(\"./slike/" << i + 1 << ".bmp\"), " 
		//			<< dinamoX[i] << ", " 
		//			<< dinamoY[i] << ", " 
		//			<< rotacije[i] << ", " 
		//			<< mirXForPrint << ", "
		//			<< mirYForPrint << ");" << endl;
		//}
		
		outFile.close();
		cout << "Uspesno upisano u fajl 'koordinate.txt'" << endl;
	}
	else
	{
		cerr << "Greska pri otvaranju fajla" << endl;
	}
}

int CMonapuzzleView::UcitajKoordinate()
{
	ifstream inputFile("koordinate.txt");
	if (!inputFile) return 0;

	string line;
	int i = 0;
	while (getline(inputFile, line))
	{
		stringstream ss(line);
		vector<string> values;
		string token;

		while (getline(ss, token, '|'))
		{
			values.push_back(token);
		}

		dinamoX[i] = stoi(values[0]);
		dinamoY[i] = stoi(values[1]);
		rotacije[i] = stoi(values[2]);
		miroriX[i] = stoi(values[3]);
		miroriY[i] = stoi(values[4]);

		i++;
	}

	inputFile.close();
	return 1;
}

void CMonapuzzleView::DrawGrid(CDC* pDC)
{
	CPen gridPen(BS_SOLID, 1, RGB(230, 230, 230));
	CPen* oldPen = pDC->SelectObject(&gridPen);

	for (int x = 0; x <= 800; x += GUNIT)
	{
		// Horizontalne linije
		pDC->MoveTo(0, x);
		pDC->LineTo(800, x);

		// Vertikalne linije
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, 800);
	}

	pDC->SelectObject(oldPen);
}

void CMonapuzzleView::PlacePuzzle(CDC* pDC, CString imeSlike, int x, int y, int angle, bool mirrorX, bool mirrorY, bool samoPlava)
{
	XFORM oldTransform;
	pDC->GetWorldTransform(&oldTransform);

	Translate(pDC, -128, -128);
	Rotate(pDC, angle);
	Mirror(pDC, mirrorX, mirrorY);
	Translate(pDC, 128, 128);

	Translate(pDC, x, y);

	DImage img;
	img.Load(imeSlike);
	int w = img.Width();
	int h = img.Height();
	
	CBitmap bmpImage, bmpMask;

	bmpImage.CreateCompatibleBitmap(pDC, w, h);
	bmpMask.CreateBitmap(w, h, 1, 1, NULL);

	CDC* pSrcDC = new CDC();
	pSrcDC->CreateCompatibleDC(pDC);
	pSrcDC->SelectObject(&bmpImage);
	CDC* pMaskDC = new CDC();
	pMaskDC->CreateCompatibleDC(pDC);
	pMaskDC->SelectObject(&bmpMask);

	img.Draw(pSrcDC, CRect(0, 0, w, h), CRect(0, 0, w, h));

	COLORREF pozadina = pSrcDC->GetPixel(0, 0);
	pSrcDC->SetBkColor(pozadina);
	pMaskDC->BitBlt(0, 0, w, h, pSrcDC, 0, 0, SRCCOPY);

	pSrcDC->SetTextColor(RGB(255, 255, 255));
	pSrcDC->SetBkColor(RGB(0, 0, 0));
	pSrcDC->BitBlt(0, 0, w, h, pMaskDC, 0, 0, SRCAND);

	Filtriraj(&bmpImage, samoPlava);

	pDC->BitBlt(0, 0, w, h, pMaskDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, w, h, pSrcDC, 0, 0, SRCPAINT);

	delete pSrcDC;
	delete pMaskDC;

	pDC->SetWorldTransform(&oldTransform);
}

void CMonapuzzleView::Filtriraj(CBitmap* bmpImage, bool boja)
{
	BITMAP bitmapStruct;
	bmpImage->GetBitmap(&bitmapStruct);

	DWORD dwCount = bitmapStruct.bmWidthBytes * bitmapStruct.bmHeight;
	vector<BYTE> imageBits(dwCount);

	if (bmpImage->GetBitmapBits(dwCount, imageBits.data()) == 0) return;

	BITMAPINFO* bmpInfo = reinterpret_cast<BITMAPINFO*>(&bitmapStruct);
	WORD bpp = bmpInfo->bmiHeader.biBitCount;

	for (size_t i = 0; i < imageBits.size() - 3; i += 4)
	{
		BYTE b = imageBits[i];
		BYTE g = imageBits[i + 1];
		BYTE r = imageBits[i + 2];
		
		int grayedBit = 64 + (r + g + b) / 3;
		if (grayedBit > 255) grayedBit = 255;

		r = g = boja ? 0 : grayedBit;
		b = grayedBit;

		imageBits[i] = b;
		imageBits[i + 1] = g;
		imageBits[i + 2] = r;
	}
	if (bmpImage->SetBitmapBits(dwCount, imageBits.data()));
}

void CMonapuzzleView::DrawWholePuzzle(CDC* pDC)
{
	for (int i = 0; i < N; i++)
	{
		string path = "./slike/" + to_string(i + 1) + ".bmp";
		if (i == 5)
			PlacePuzzle(pDC, CString(path.c_str()), dinamoX[i], dinamoY[i], rotacije[i], miroriX[i], miroriY[i], true);
		else
			PlacePuzzle(pDC, CString(path.c_str()), dinamoX[i], dinamoY[i], rotacije[i], miroriX[i], miroriY[i]);
	}

	//PlacePuzzle(pDC, CString("./slike/1.bmp"), dinamoX[0], dinamoY[0], rotacije[0], miroriX[0], miroriY[0]);
	//PlacePuzzle(pDC, CString("./slike/2.bmp"), dinamoX[1], dinamoY[1], rotacije[1], miroriX[1], miroriY[1]);
	//PlacePuzzle(pDC, CString("./slike/3.bmp"), dinamoX[2], dinamoY[2], rotacije[2], miroriX[2], miroriY[2]);
	//PlacePuzzle(pDC, CString("./slike/4.bmp"), dinamoX[3], dinamoY[3], rotacije[3], miroriX[3], miroriY[3]);
	//PlacePuzzle(pDC, CString("./slike/5.bmp"), dinamoX[4], dinamoY[4], rotacije[4], miroriX[4], miroriY[4]);
	//PlacePuzzle(pDC, CString("./slike/6.bmp"), dinamoX[5], dinamoY[5], rotacije[5], miroriX[5], miroriY[5], true);
	//PlacePuzzle(pDC, CString("./slike/7.bmp"), dinamoX[6], dinamoY[6], rotacije[6], miroriX[6], miroriY[6]);
	//PlacePuzzle(pDC, CString("./slike/8.bmp"), dinamoX[7], dinamoY[7], rotacije[7], miroriX[7], miroriY[7]);
	//PlacePuzzle(pDC, CString("./slike/9.bmp"), dinamoX[8], dinamoY[8], rotacije[8], miroriX[8], miroriY[8]);
}


// CMonapuzzleView drawing

void CMonapuzzleView::OnDraw(CDC* pDC)
{
	CMonapuzzleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Kreiranje memoryDC-a jer ce sve na njemu da se iscrtava
	CRect clientRect;
	GetClientRect(&clientRect);
	CDC* pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);

	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());
	pMemDC->SelectObject(&memBitmap);

	// MemoryDc ima crnu pozadinu i mora da se sinhronizuje sa pDC-om
	pMemDC->FillSolidRect(clientRect, pDC->GetBkColor());

	int oldMode = pMemDC->SetGraphicsMode(GM_ADVANCED);

	DrawWholePuzzle(pMemDC);

	if (gridEnabled) DrawGrid(pMemDC);
	
	pMemDC->SetGraphicsMode(oldMode);

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), pMemDC, 0, 0, SRCCOPY);
	
	delete pMemDC;
}


// CMonapuzzleView printing

BOOL CMonapuzzleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMonapuzzleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMonapuzzleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMonapuzzleView diagnostics

#ifdef _DEBUG
void CMonapuzzleView::AssertValid() const
{
	CView::AssertValid();
}

void CMonapuzzleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMonapuzzleDoc* CMonapuzzleView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMonapuzzleDoc)));
	return (CMonapuzzleDoc*)m_pDocument;
}
#endif //_DEBUG


// CMonapuzzleView message handlers


void CMonapuzzleView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	
	switch (nChar)
	{
		// Selekcija slike
		case '1': indeksSelekcije = 0; break;
		case '2': indeksSelekcije = 1; break;
		case '3': indeksSelekcije = 2; break;
		case '4': indeksSelekcije = 3; break;
		case '5': indeksSelekcije = 4; break;
		case '6': indeksSelekcije = 5; break;
		case '7': indeksSelekcije = 6; break;
		case '8': indeksSelekcije = 7; break;
		case '9': indeksSelekcije = 8; break;

		// Finije kontrole
		case 'W': dinamoY[indeksSelekcije] -= 2; break;
		case 'A': dinamoX[indeksSelekcije] -= 2; break;
		case 'S': dinamoY[indeksSelekcije] += 2; break;
		case 'D': dinamoX[indeksSelekcije] += 2; break;
		case 'Q': rotacije[indeksSelekcije] -= 1; break;
		case 'E': rotacije[indeksSelekcije] += 1; break;
		case 'Z': miroriX[indeksSelekcije] = !miroriX[indeksSelekcije]; break;
		case 'X': miroriY[indeksSelekcije] = !miroriY[indeksSelekcije]; break;

		// Grublje kontrole
		case 'I': dinamoY[indeksSelekcije] -= 20; break;
		case 'J': dinamoX[indeksSelekcije] -= 20; break;
		case 'K': dinamoY[indeksSelekcije] += 20; break;
		case 'L': dinamoX[indeksSelekcije] += 20; break;
		case 'U': rotacije[indeksSelekcije] -= 10; break;
		case 'O': rotacije[indeksSelekcije] += 10; break;

		// Save
		case 'P': Sacuvaj(); break;
		default: break;
	}
	Invalidate();
}
