
// MFCTestGDIView.cpp : implementation of the CMFCTestGDIView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCTestGDI.h"
#endif

#include "MFCTestGDIDoc.h"
#include "MFCTestGDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCTestGDIView

IMPLEMENT_DYNCREATE(CMFCTestGDIView, CView)

BEGIN_MESSAGE_MAP(CMFCTestGDIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCTestGDIView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCTestGDIView construction/destruction

CRect CMFCTestGDIView::frontWallDraw(CDC * pDC)
{
	CBrush wallBrush(this->wallColor);

	CBrush *oldBrush = pDC->SelectObject(&wallBrush);

	CRect frontWall((int)widthEnd*0.15, (int)(heightEnd*0.1), (int)widthEnd*0.85, (int)(heightEnd*0.75));

	pDC->Rectangle(frontWall);

	pDC->SelectObject(oldBrush);

	return frontWall;
}

CPoint* CMFCTestGDIView::floorDraw(CDC * pDC, CRect frontWall)
{
	CBrush floorBrush(this->floorColor);

	CBrush *oldBrush = pDC->SelectObject(&floorBrush);

	CPoint floorPolygon[] = { {frontWall.TopLeft().x, frontWall.BottomRight().y }, {frontWall.BottomRight()},
	{(int)this->widthEnd, (int)this->heightEnd}, { (int)this->widthStart, (int)this->heightEnd} };

	pDC->Polygon(floorPolygon, 4);
	
	pDC->SelectObject(oldBrush);
	return floorPolygon;
}

CPoint * CMFCTestGDIView::leftWallDraw(CDC *pDC, CRect frontWall)
{

	CBrush asideWallBrush(this->asideWallColor);

	CBrush *oldBrush = pDC->SelectObject(&asideWallBrush);

	CPoint leftWallPolygon[] = { { frontWall.TopLeft() }, {frontWall.TopLeft().x, frontWall.BottomRight().y},
	{(int)this->widthStart, (int)this->heightEnd}, { (int)this->widthStart, (int)this->heightStart} };

	pDC->Polygon(leftWallPolygon, 4);
	
	pDC->SelectObject(oldBrush);
	return leftWallPolygon;
}

CPoint * CMFCTestGDIView::rightWallDraw(CDC *pDC, CRect frontWall)
{
	CBrush asideWallBrush(asideWallColor);

	CBrush *oldBrush = pDC->SelectObject(&asideWallBrush);

	CPoint *rightWallPolygon = new CPoint[4];
	rightWallPolygon[0] = { frontWall.BottomRight() };
	rightWallPolygon[1] = { frontWall.BottomRight().x, frontWall.TopLeft().y };
	rightWallPolygon[2] = {(int)this->widthEnd, (int)this->heightStart};
	rightWallPolygon[3] = { (int)this->widthEnd, (int)this->heightEnd};


	pDC->Polygon(rightWallPolygon, 4);
	
	pDC->SelectObject(oldBrush);
	return rightWallPolygon;
}

void CMFCTestGDIView::inventoryDraw(CDC * pDC)
{
	CBrush invBrush(this->floorColor);
	CBrush *oldBrush = pDC->SelectObject(&invBrush);
	
	this->inventoryRect = CRect(this->widthEnd*0.03, this->heightEnd*0.01, this->widthEnd*0.06, this->heightEnd*0.06);
	pDC->RoundRect(this->widthEnd*0.03, this->heightEnd*0.01, this->widthEnd*0.06, this->heightEnd*0.06, 10, 10);

	if (isKeyTaken & !isDoorOpen)
	{
		HENHMETAFILE key = GetEnhMetaFile(CString("key.emf"));

		CRect keyPositonRect(this->widthEnd*0.03, this->heightEnd*0.06, this->widthEnd*0.06, this->heightEnd*0.01);

		PlayEnhMetaFile(pDC->m_hDC, key, keyPositonRect);
	}

	pDC->SelectObject(oldBrush);
}

void CMFCTestGDIView::wardrobeDraw(CDC * pDC, CRect frontWall)
{
	CBrush wardrobeBrush(this->woodColor);
	CPen wardrobePen(PS_SOLID,3,this->wardrobePenColor);
	CBrush *oldBrush = pDC->SelectObject(&wardrobeBrush);
	CPen *oldPen = pDC->SelectObject(&wardrobePen);

	CRect frontWardrobeRect(frontWall.TopLeft().x - (frontWall.TopLeft().x * 0.2), frontWall.TopLeft().y + (frontWall.BottomRight().y * 0.15),
		frontWall.TopLeft().x + (frontWall.BottomRight().x * 0.15), frontWall.BottomRight().y + (frontWall.BottomRight().y * 0.1));

	pDC->Rectangle(frontWardrobeRect);


	// elements
	CRect leftSide(frontWardrobeRect.TopLeft().x + ( frontWardrobeRect.TopLeft().x * 0.1),
		frontWardrobeRect.TopLeft().y + (frontWardrobeRect.TopLeft().y * 0.1),
		frontWardrobeRect.TopLeft().x + ((frontWardrobeRect.BottomRight().x - frontWardrobeRect.TopLeft().x) * 0.5),
		frontWardrobeRect.TopLeft().y + ((frontWardrobeRect.BottomRight().y - frontWardrobeRect.TopLeft().y) * 0.6)
		);

	this->wardrobeLeft = leftSide;

	CBrush *leftBrush;
	if (isWardrobeOpen)
	{
		leftBrush = new CBrush(this->blackColor);
		pDC->SelectObject(leftBrush);

	}

	pDC->Rectangle(leftSide);

	if (isWardrobeOpen)
	{
		pDC->SelectObject(&wardrobeBrush);
		delete leftBrush;
		leftBrush = NULL;

		CPoint openWardrobePart[] = 
		{
			{leftSide.TopLeft()},
			{ leftSide.TopLeft().x - 50, leftSide.TopLeft().y -30 },
			{leftSide.TopLeft().x - 50, leftSide.TopLeft().y +(leftSide.BottomRight().y - leftSide.TopLeft().y) + 30 },
			{leftSide.TopLeft().x, leftSide.BottomRight().y}
		};

		pDC->Polygon(openWardrobePart, 4);

		if (!isKeyTaken)
		{
			// key
			HENHMETAFILE key = GetEnhMetaFile(CString("key.emf"));

			CRect keyRect(leftSide.BottomRight().x - 60, leftSide.BottomRight().y - 10,
				leftSide.BottomRight().x - 10, leftSide.BottomRight().y - 60);

			this->itemInWardrobe = keyRect;

			PlayEnhMetaFile(pDC->m_hDC, key, keyRect);

		}
	}


	CRect rightSide(leftSide.BottomRight().x,
		leftSide.TopLeft().y,
		leftSide.BottomRight().x + (leftSide.BottomRight().x - leftSide.TopLeft().x),
		leftSide.BottomRight().y
	); 

	pDC->Rectangle(rightSide);


	// handles
	CBrush handelsBrush(this->handelsColor);
	pDC->SelectObject(&handelsBrush);

	if (!isWardrobeOpen)
	{
		pDC->Ellipse(
			(int)(leftSide.TopLeft().x + ((leftSide.BottomRight().x - leftSide.TopLeft().x) * 0.76)),
			(int)(leftSide.TopLeft().y + ((leftSide.BottomRight().y - leftSide.TopLeft().y) * 0.49)),
			(int)(leftSide.TopLeft().x + ((leftSide.BottomRight().x - leftSide.TopLeft().x) * 0.88)),
			(int)(leftSide.TopLeft().y + ((leftSide.BottomRight().y - leftSide.TopLeft().y) * 0.53))
		);
	}

	pDC->Ellipse(
		(int)(rightSide.TopLeft().x + ((rightSide.BottomRight().x - rightSide.TopLeft().x) * 0.12)),
		(int)(rightSide.TopLeft().y + ((rightSide.BottomRight().y - rightSide.TopLeft().y) * 0.49)),
		(int)(rightSide.TopLeft().x + ((rightSide.BottomRight().x - rightSide.TopLeft().x) * 0.24)),
		(int)(rightSide.TopLeft().y + ((rightSide.BottomRight().y - rightSide.TopLeft().y) * 0.53))
	);

	// drowers

	pDC->SelectObject(wardrobeBrush);

	CRect upperDworer(
		leftSide.TopLeft().x,
		leftSide.BottomRight().y + ((frontWardrobeRect.BottomRight().y - frontWardrobeRect.TopLeft().y) * 0.05), 
		rightSide.BottomRight().x,
		leftSide.BottomRight().y + ((frontWardrobeRect.BottomRight().y - frontWardrobeRect.TopLeft().y) * 0.15)
		);
	pDC->Rectangle(upperDworer);


	CRect lowerDworer(leftSide.TopLeft().x,
		upperDworer.BottomRight().y + ((frontWardrobeRect.BottomRight().y - frontWardrobeRect.TopLeft().y) * 0.05),
		rightSide.BottomRight().x, 
		frontWardrobeRect.BottomRight().y - ((frontWardrobeRect.BottomRight().y - frontWardrobeRect.TopLeft().y) * 0.05)
		);

	pDC->Rectangle(lowerDworer);

	pDC->SelectObject(&handelsBrush);

	int dX = upperDworer.BottomRight().x - upperDworer.TopLeft().x;
	int dY = upperDworer.BottomRight().y - upperDworer.TopLeft().y;
	pDC->Ellipse(upperDworer.CenterPoint().x - (dX  * 0.03),
		upperDworer.CenterPoint().y - (dY  * 0.1),
		upperDworer.CenterPoint().x + (dX  * 0.03),
		upperDworer.CenterPoint().y + (dY * 0.1)
	);

	int dX1 = lowerDworer.BottomRight().x - lowerDworer.TopLeft().x;
	int dY1 = lowerDworer.BottomRight().y - lowerDworer.TopLeft().y;
	pDC->Ellipse(lowerDworer.CenterPoint().x - (dX1  * 0.03),
		lowerDworer.CenterPoint().y - (dY1  * 0.08),
		lowerDworer.CenterPoint().x + (dX1  * 0.03),
		lowerDworer.CenterPoint().y + (dY1 * 0.08)
	);

	// wardrobe aside

	CBrush wardrobeAsideBrush(this->wardrobeAsideColor);

	pDC->SelectObject(wardrobeAsideBrush);

	CPoint wardrobeAside[] = { 
		{ frontWardrobeRect.BottomRight().x, frontWardrobeRect.TopLeft().y },
		{frontWardrobeRect.BottomRight().x + 40, frontWardrobeRect.TopLeft().y + 40 }, 
		{ frontWardrobeRect.BottomRight().x + 40, frontWardrobeRect.BottomRight().y - 80},
		{ frontWardrobeRect.BottomRight().x , frontWardrobeRect.BottomRight().y}
	};

	pDC->Polygon(wardrobeAside, 4);

	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}

void CMFCTestGDIView::doorDraw(CDC * pDC, CPoint * rightAsideWall)
{
	CBrush doorBrush(this->woodColor);
	CPen doorPen(PS_SOLID, 2, this->wardrobePenColor);
	CBrush *oldBrush = pDC->SelectObject(&doorBrush);
	CPen *oldPen = pDC->SelectObject(&doorPen);


	this->doorPolygon = new CPoint[4];

	doorPolygon[0] = { rightAsideWall[0].x + 10, rightAsideWall[0].y + 10 };
	doorPolygon[1] = { rightAsideWall[1].x + 10, rightAsideWall[1].y + 40 };
	doorPolygon[2] = { rightAsideWall[2].x - 130, rightAsideWall[2].y + 100 };
	doorPolygon[3] = { rightAsideWall[3].x - 130, rightAsideWall[3].y - 120 };


	if (isDoorOpen)
	{
		CBrush blackBrush(this->blackColor);
		pDC->SelectObject(blackBrush);

		pDC->Polygon(doorPolygon, 4);

		CRect openedDoor(
			doorPolygon[1].x - 300,
			doorPolygon[1].y,
			doorPolygon[0].x,
			doorPolygon[0].y
			);

		pDC->SelectObject(doorBrush);

		pDC->Rectangle(openedDoor);
	}
	else
	{
		pDC->Polygon(doorPolygon, 4);
		doorKeyholeAndHandleDraw(pDC, doorPolygon);
	}



	pDC->SelectObject(oldBrush);
	pDC->SelectObject(oldPen);
}

void CMFCTestGDIView::doorKeyholeAndHandleDraw(CDC * pDC, CPoint *doorPolygon)
{
	CBrush blackBrush(this->blackColor);
	CBrush *oldBrush = pDC->SelectObject(&blackBrush);


	// keyhole
	pDC->Ellipse(
		doorPolygon->x + 110, doorPolygon->y * 0.5 + 150 ,
		doorPolygon->x + 120, doorPolygon->y * 0.5 + 165
	);

	CBrush handleBrush(this->handelsColor);
	pDC->SelectObject(handleBrush);

	pDC->Ellipse(
		doorPolygon->x + 100, doorPolygon->y * 0.5 + 120,
		doorPolygon->x + 120, doorPolygon->y * 0.5 + 135
	);

	pDC->Ellipse(
		doorPolygon->x + 85, doorPolygon->y * 0.5 + 115,
		doorPolygon->x + 110, doorPolygon->y * 0.5 + 140
	);


	pDC->SelectObject(oldBrush);

}

CRect CMFCTestGDIView::windowDraw(CDC * pDC, CRect frontWallRect)
{
	CBrush woodBrush(this->woodColor);
	CBrush * oldBrush = pDC->SelectObject(&woodBrush);

	CRect window(
		frontWallRect.CenterPoint().x - 160, frontWallRect.CenterPoint().y - 240,
		frontWallRect.CenterPoint().x + 160, frontWallRect.CenterPoint().y + 180

	);

	pDC->Rectangle(window);

	CBrush windowBrush(this->glassColor);

	pDC->SelectObject(windowBrush);
	CRect* newWindow = NULL;
	CRect *temp = NULL;
	bool first = true;

	newWindow = new CRect(
		window.TopLeft().x + 10, window.CenterPoint().y - 200,
		window.CenterPoint().x - 5, window.CenterPoint().y - 5
	);

	pDC->Rectangle(newWindow);

	newWindow = new CRect(
		window.CenterPoint().x + 5, window.CenterPoint().y - 200,
		window.BottomRight().x - 10, window.CenterPoint().y - 5
	);

	pDC->Rectangle(newWindow);

	newWindow = new CRect(
		window.TopLeft().x + 10, window.CenterPoint().y + 5,
		window.CenterPoint().x - 5, window.BottomRight().y - 10
	);

	pDC->Rectangle(newWindow);

	newWindow = new CRect(
		window.CenterPoint().x + 5, window.CenterPoint().y + 5,
		window.BottomRight().x - 10, window.BottomRight().y - 10
	);

	pDC->Rectangle(newWindow);

	pDC->SelectObject(&woodBrush);

	pDC->RoundRect(window.TopLeft().x-10, window.TopLeft().y - 10, window.BottomRight().x + 10, window.TopLeft().y, 5, 5);

	// carpet
	this->drawCarpet(pDC, window);

	pDC->SelectObject(oldBrush);

	return window;
}

void CMFCTestGDIView::drawCarpet(CDC * pDC, CRect windowRect)
{
	CBrush carpetBrush(this->carpetColor);
	CBrush *oldBrush = pDC->SelectObject(&carpetBrush);

	pDC->BeginPath();

	pDC->MoveTo(windowRect.TopLeft());

	pDC->LineTo(windowRect.TopLeft().x, windowRect.BottomRight().y - 50);

	CPoint curve[] = { 
		{windowRect.CenterPoint().x - 150, windowRect.CenterPoint().y + 60},
		{windowRect.CenterPoint().x + 150, windowRect.CenterPoint().y + 200},
		{windowRect.BottomRight().x, windowRect.BottomRight().y - 50}
	};
	pDC->PolyBezierTo(curve, 3);

	pDC->LineTo(windowRect.BottomRight().x, windowRect.TopLeft().y);
	pDC->LineTo(windowRect.TopLeft());

	pDC->EndPath();
	pDC->StrokeAndFillPath();

	pDC->SelectObject(oldBrush);
}

void CMFCTestGDIView::drawClock(CDC * pDC, CRect frontWallRect)
{
	int wallStartX = frontWallRect.TopLeft().x, wallStartY = frontWallRect.TopLeft().y;
	int wallEndX = frontWallRect.BottomRight().x, wallEndY = frontWallRect.BottomRight().y;
	int wallXLenth = wallEndX - wallStartX;
	int wallYLenth = wallEndY - wallStartY;

	CBrush woodBrush(this->woodColor);

	CBrush *oldBrush = pDC->SelectObject(&woodBrush);


	// clock neck

	pDC->RoundRect(frontWallRect.TopLeft().x + (wallXLenth * 0.75),
		frontWallRect.TopLeft().y + (wallYLenth * 0.3),
		frontWallRect.TopLeft().x + (wallXLenth * 0.85),
		frontWallRect.TopLeft().y + (wallYLenth * 0.7), 20, 20);


	// black neck of clock
	CBrush blackBrush(this->blackColor);
	pDC->SelectObject(blackBrush);
		
	pDC->RoundRect(frontWallRect.TopLeft().x + (wallXLenth * 0.75) + 10,
		frontWallRect.TopLeft().y + (wallYLenth * 0.3) + 10,
		frontWallRect.TopLeft().x + (wallXLenth * 0.85) - 10,
		frontWallRect.TopLeft().y + (wallYLenth * 0.7) - 10, 20, 20);

	CBrush nackGoldenBrush(this->goldenDarkForWallClockNeck);
	pDC->SelectObject(nackGoldenBrush);


	CRect nackStaffRect(frontWallRect.TopLeft().x + (wallXLenth * 0.75) + 60,
		frontWallRect.TopLeft().y + (wallYLenth * 0.3) + 10,
		frontWallRect.TopLeft().x + (wallXLenth * 0.85) - 60,
		frontWallRect.TopLeft().y + (wallYLenth * 0.7) - 70);

	pDC->Rectangle(nackStaffRect);

	pDC->Ellipse(
		frontWallRect.TopLeft().x + (wallXLenth * 0.8) - 15,
		frontWallRect.TopLeft().y + (wallYLenth * 0.1) + 335,
		frontWallRect.TopLeft().x + (wallXLenth * 0.8) + 15,
		frontWallRect.TopLeft().y + (wallYLenth * 0.1) + 365);

	// upper part of clock
	pDC->SelectObject(woodBrush);

	pDC->Ellipse(frontWallRect.TopLeft().x + (wallXLenth * 0.72),
		frontWallRect.TopLeft().y + (wallYLenth * 0.1),
		frontWallRect.TopLeft().x + (wallXLenth * 0.88),
		frontWallRect.TopLeft().y + (wallYLenth * 0.4));

	CBrush goldenDarkerBrush(this->goldenDarker);

	pDC->SelectObject(goldenDarkerBrush);
	pDC->Ellipse(frontWallRect.TopLeft().x + (wallXLenth * 0.72) + 10,
		frontWallRect.TopLeft().y + (wallYLenth * 0.1) + 10,
		frontWallRect.TopLeft().x + (wallXLenth * 0.88) - 10,
		frontWallRect.TopLeft().y + (wallYLenth * 0.4) - 10);

	CBrush whiteBrush(RGB(255, 255, 255));
	pDC->SelectObject(whiteBrush);

	pDC->Ellipse(frontWallRect.TopLeft().x + (wallXLenth * 0.72) + 35,
		frontWallRect.TopLeft().y + (wallYLenth * 0.1) + 35,
		frontWallRect.TopLeft().x + (wallXLenth * 0.88) - 35,
		frontWallRect.TopLeft().y + (wallYLenth * 0.4) - 35);

	pDC->Ellipse(
		frontWallRect.TopLeft().x + (wallXLenth * 0.88) - 45,
		frontWallRect.TopLeft().y + (wallYLenth * 0.4) - 45,
		frontWallRect.TopLeft().x + (wallXLenth * 0.72) + 45,
		frontWallRect.TopLeft().y + (wallYLenth * 0.1) + 45);




	pDC->SelectObject(oldBrush);
}

void CMFCTestGDIView::changeWorldTransformation(CDC * pDC, int cx, int cy, int rx, int ry, int alfa, XFORM * oldForm)
{
	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	DWORD dw = GetLastError();

	bool b = GetWorldTransform(pDC->m_hDC, oldForm);
	XFORM xForm;


}

CMFCTestGDIView::CMFCTestGDIView() noexcept
{
	// TODO: add construction code here

	// color init
	this->wallColor = RGB(255, 192, 128);
	this->asideWallColor = RGB(194, 146, 97);
	this->floorColor = RGB(190, 190, 190);
	this->carpetColor = RGB(248, 253, 159);
	this->glassColor = RGB(0, 186, 219);
	this->handelsColor = RGB(233, 153, 0);
	this->woodColor = RGB(132, 99, 66);
	this->wardrobePenColor = RGB(88,42,26);
	this->blackColor = RGB(0, 0, 0);
	this->wardrobeAsideColor = RGB(110, 77, 44,);

	this->isKeyTaken = false;
	this->isDoorOpen = false;
	this->isWardrobeOpen = false;

	this->goldenDarker = RGB(240, 188, 22);
	this->goldenLighter = RGB(253, 216,110);
	this->goldenDarkForWallClockNeck = RGB(242, 190, 19);
}

CMFCTestGDIView::~CMFCTestGDIView()
{
}

BOOL CMFCTestGDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMFCTestGDIView drawing

void CMFCTestGDIView::OnDraw(CDC* pDC)
{
	CMFCTestGDIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;



	// TODO: add draw code for native data here

	// walls and floor
	CRect mainWallRectangle = this->frontWallDraw(pDC);
	CPoint *florPolygon = this->floorDraw(pDC, mainWallRectangle);
	CPoint *leftWall = this->leftWallDraw(pDC, mainWallRectangle);
	CPoint *rightWall = this->rightWallDraw(pDC, mainWallRectangle);

	this->inventoryDraw(pDC);
	// wardrobe
	this->wardrobeDraw(pDC, mainWallRectangle);

	// door
	this->doorDraw(pDC, rightWall);

	// window
	CRect windowRect = this->windowDraw(pDC, mainWallRectangle);

	// clock 
	this->drawClock(pDC, mainWallRectangle);

}


// CMFCTestGDIView printing


void CMFCTestGDIView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCTestGDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCTestGDIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCTestGDIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMFCTestGDIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCTestGDIView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCTestGDIView diagnostics

#ifdef _DEBUG
void CMFCTestGDIView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCTestGDIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCTestGDIDoc* CMFCTestGDIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCTestGDIDoc)));
	return (CMFCTestGDIDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCTestGDIView message handlers


void CMFCTestGDIView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	bool rerender = false;

	CRgn wardrobLeftRgn;
	wardrobLeftRgn.CreateRectRgn(this->wardrobeLeft.TopLeft().x, this->wardrobeLeft.TopLeft().y,
		this->wardrobeLeft.BottomRight().x, this->wardrobeLeft.BottomRight().y);

	CRgn keyRgn;
	keyRgn.CreateRectRgn(this->itemInWardrobe.TopLeft().x, this->itemInWardrobe.TopLeft().y,
		this->itemInWardrobe.BottomRight().x, this->itemInWardrobe.BottomRight().y);

	CRgn doorRgn;
	doorRgn.CreatePolygonRgn(this->doorPolygon, 4, WINDING);

	if (isWardrobeOpen && keyRgn.PtInRegion(point))
	{
		isKeyTaken = true;
		rerender = true;
	}
	else if (!isWardrobeOpen && wardrobLeftRgn.PtInRegion(point))
	{
		this->isWardrobeOpen = true;
		rerender = true;
	}
	else if (isKeyTaken && doorRgn.PtInRegion(point))
	{
		isDoorOpen = true;
		rerender = true;
	}


	if (rerender)
	{
		Invalidate();
	}


	CView::OnLButtonDown(nFlags, point);
}
