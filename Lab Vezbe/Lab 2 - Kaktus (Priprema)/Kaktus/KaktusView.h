
// KaktusView.h : interface of the CKaktusView class
//

#pragma once


class CKaktusView : public CView
{
protected: // create from serialization only
	CKaktusView() noexcept;
	DECLARE_DYNCREATE(CKaktusView)

// Attributes
public:
	CKaktusDoc* GetDocument() const;
	bool gridEnabled = false;
	float angleAroundNode = 0;
	float angleAroundPot = 0;

// Operations
public:

// Overrides
public:
	// Funkcije za iscrtavanje
	virtual void DrawBg(CDC* pDC);
	virtual void DrawGrid(CDC* pDC);
	virtual void DrawPot(CDC* pDC);
	virtual void DrawNode(CDC* pDC, float x, float y, float angleAroundPot);
	virtual void DrawCactusPart(CDC* pDC, HENHMETAFILE* metaFile, float x, float y, int width, int height, float angleAroundPot, float angleFixed = 0, float angleAroundNode = 0);
	virtual void DrawFigure(CDC* pDC);
	virtual void DrawText(CDC* pDC, int size, char* text, int x, int y, int degrees, COLORREF color);

	// Funkcije transformacije
	virtual void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = true);
	virtual void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = true);
	virtual void Rotate(CDC* pDC, float angle, bool rightMultiply = true);

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKaktusView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in KaktusView.cpp
inline CKaktusDoc* CKaktusView::GetDocument() const
   { return reinterpret_cast<CKaktusDoc*>(m_pDocument); }
#endif

