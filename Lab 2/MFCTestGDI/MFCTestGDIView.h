
// MFCTestGDIView.h : interface of the CMFCTestGDIView class
//

#pragma once


class CMFCTestGDIView : public CView
{

#pragma region k|33

protected:
	double widthStart = 0, heightStart = 0, widthEnd = 1920, heightEnd = 1080;

	COLORREF wallColor;
	COLORREF asideWallColor;
	COLORREF floorColor;
	COLORREF woodColor;
	COLORREF wardrobePenColor;
	COLORREF wardrobeAsideColor;
	COLORREF glassColor;
	COLORREF carpetColor;
	COLORREF handelsColor;
	COLORREF blackColor;

	COLORREF goldenDarker;
	COLORREF goldenLighter;
	COLORREF goldenDarkForWallClockNeck;

	CRect frontWallDraw(CDC *pDC);
	CPoint* floorDraw(CDC *pDC, CRect frontWall);
	CPoint* leftWallDraw(CDC *pDC, CRect frontWall);
	CPoint* rightWallDraw(CDC *pDC, CRect frontWall);
	void inventoryDraw(CDC *pDC);
	void wardrobeDraw(CDC *pDC, CRect frontWall);
	void doorDraw(CDC *pDC, CPoint *rightAsideWall);
	void doorKeyholeAndHandleDraw(CDC *pDC, CPoint *doorPolygon);
	CRect windowDraw(CDC *pDC, CRect frontWallRect);
	void drawCarpet(CDC *pDC, CRect windowRect);

	bool isKeyTaken;
	bool isDoorOpen;
	bool isWardrobeOpen;

	CRect wardrobeLeft;
	CRect inventoryRect;
	CPoint *doorPolygon;

	// init there is no item
	CRect itemInWardrobe;

	// draw clock
	void drawClock(CDC *pDC, CRect frontWallRect);
	void changeWorldTransformation(CDC *pDC, float cx, float cy, float rx, float ry, float alfa, XFORM *oldForm);
	CString castIntNumberToCString(int number);

	double radianiZaMaluKazaljku;
	double radijaniZaVelikuKazaljku;
	double radijaniZaKlatno;
	double dodajRadijaneZaKlatno;
	
#pragma endregion


protected: // create from serialization only
	CMFCTestGDIView() noexcept;
	DECLARE_DYNCREATE(CMFCTestGDIView)

// Attributes
public:
	CMFCTestGDIDoc* GetDocument() const;

// Operations
public:

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
	virtual ~CMFCTestGDIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in MFCTestGDIView.cpp
inline CMFCTestGDIDoc* CMFCTestGDIView::GetDocument() const
   { return reinterpret_cast<CMFCTestGDIDoc*>(m_pDocument); }
#endif

