
// IND_18981View.h : interface of the CIND18981View class
//

#pragma once


class CIND18981View : public CView
{
protected: // create from serialization only
	CIND18981View() noexcept;
	DECLARE_DYNCREATE(CIND18981View)

// Attributes
public:
	CIND18981Doc* GetDocument() const;
	bool gridEnabled = false;
	int dinamoX[9];
	int dinamoY[9];
	int rotacije[9];
	bool miroriX[9];
	bool miroriY[9];
	int indeksSelekcije = 0;

// Operations
public:

// Overrides
public:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = true);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = true);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = true);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply = true);

	void Sacuvaj();
	int UcitajKoordinate();
	void DrawGrid(CDC* pDC);
	void PlacePuzzle(CDC* pDC, CString imeSlike, int x, int y, int angle = 0, bool mirrorX = false, bool mirrorY = false, bool samoPlava = false);
	void Filtriraj(CBitmap* bmpImage, bool boja);
	void DrawWholePuzzle(CDC* pDC);
	
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIND18981View();
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

#ifndef _DEBUG  // debug version in IND_18981View.cpp
inline CIND18981Doc* CIND18981View::GetDocument() const
   { return reinterpret_cast<CIND18981Doc*>(m_pDocument); }
#endif

