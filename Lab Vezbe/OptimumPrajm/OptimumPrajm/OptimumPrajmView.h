
// OptimumPrajmView.h : interface of the COptimumPrajmView class
//

#pragma once


class COptimumPrajmView : public CView
{
protected: // create from serialization only
	COptimumPrajmView() noexcept;
	DECLARE_DYNCREATE(COptimumPrajmView)

// Attributes
public:
	COptimumPrajmDoc* GetDocument() const;
	bool gridEnabled = false;
	bool betterCross = false;
	bool betterBg = false;
	bool betterEdges = false;
	bool evenBetterEdges = false;
	bool wave = false;

// Operations
public:

// Overrides
public:
	void Rotate(CDC* pDC, float angle, bool rightMultiply = true);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = true);

	void DrawGrid(CDC* pDC);

	void DrawBg(CDC* pDC, bool better);
	void DrawCircle(CDC* pDC, bool better);

	void DrawCross(CDC* pDC, bool better);

	void DrawEdge(CDC* pDC, int x, int y, int angle, bool leftCenter, bool betterCross, bool betterEdges, bool evenBetterEdges);

	void DrawMan(CDC* pDC, bool wave);

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~COptimumPrajmView();
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

#ifndef _DEBUG  // debug version in OptimumPrajmView.cpp
inline COptimumPrajmDoc* COptimumPrajmView::GetDocument() const
   { return reinterpret_cast<COptimumPrajmDoc*>(m_pDocument); }
#endif

