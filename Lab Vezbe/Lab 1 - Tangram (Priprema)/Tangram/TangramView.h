
// TangramView.h : interface of the CTangramView class
//

#pragma once


class CTangramView : public CView
{
protected: // create from serialization only
	CTangramView() noexcept;
	DECLARE_DYNCREATE(CTangramView)

// Attributes
public:
	CTangramDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CTangramView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TangramView.cpp
inline CTangramDoc* CTangramView::GetDocument() const
   { return reinterpret_cast<CTangramDoc*>(m_pDocument); }
#endif

