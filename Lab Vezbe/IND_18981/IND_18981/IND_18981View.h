
// IND_18981View.h : interface of the CIND18981View class
//
#include "GLRenderer.h"
#pragma once


class CIND18981View : public CView
{
protected: // create from serialization only
	CIND18981View() noexcept;
	DECLARE_DYNCREATE(CIND18981View)

	// Attributes
public:
	CIND18981Doc* GetDocument() const;

protected:
	CGLRenderer m_glRenderer;

	// Operations
public:
	bool m_dragging = false;
	CPoint m_lastPos{};

	// Overrides
public:
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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	//	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in GLKView.cpp
inline CGLKDoc* CGLKView::GetDocument() const
{
	return reinterpret_cast<CGLKDoc*>(m_pDocument);
}
#endif

