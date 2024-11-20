
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
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in IND_18981View.cpp
inline CIND18981Doc* CIND18981View::GetDocument() const
   { return reinterpret_cast<CIND18981Doc*>(m_pDocument); }
#endif

