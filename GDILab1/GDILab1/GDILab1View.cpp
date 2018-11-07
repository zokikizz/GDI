
// GDILab1View.cpp : implementation of the CGDILab1View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDILab1.h"
#endif

#include "GDILab1Doc.h"
#include "GDILab1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGDILab1View

IMPLEMENT_DYNCREATE(CGDILab1View, CView)

BEGIN_MESSAGE_MAP(CGDILab1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGDILab1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CGDILab1View construction/destruction

CGDILab1View::CGDILab1View() noexcept
{
	// TODO: add construction code here

	keyTaken = false;
	isWarderobeOpen = false;
	isDoorOpen = false;
	key2Taken = false;
	isSefOpen = false;
}

CGDILab1View::~CGDILab1View()
{
}

BOOL CGDILab1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGDILab1View drawing

void CGDILab1View::OnDraw(CDC* pDC)
{
	/*CGDILab1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	*/

	// TODO: add draw code for native data here

	CBrush wallBrush(RGB(255, 192, 128));
	CBrush* oldBrush = pDC->SelectObject(&wallBrush);


#pragma region Room

	// front wall
	CRect frontWallRect(200, 100, 1300, 700);
	pDC->Rectangle(frontWallRect);

	// aside walls

	CBrush asideWallBrush(RGB(192, 128, 64));
	pDC->SelectObject(&asideWallBrush);

	int numberOfPoints = 4;
	CPoint leftAsideWall[] = { {0,0},{201,99},{201,699},{0,900} };
	pDC->Polygon(leftAsideWall, 4);

	CPoint rightAsideWall[] = { {1299, 99},{1500,0},{1500,900},{1299,699} };
	pDC->Polygon(rightAsideWall, 4);

	//floor

	CBrush floorBrush(RGB(150, 150, 150));
	pDC->SelectObject(&floorBrush);
	CPoint floorPolygon[] = { {201, 699}, {1300, 699}, {1500, 900}, { 0, 900 } };
	pDC->Polygon(floorPolygon, numberOfPoints);


	// item container
	CBrush itemContainerBrush(RGB(150, 150, 150));

	pDC->SelectObject(&itemContainerBrush);
	pDC->RoundRect(10, 5, 90, 90, 10, 10);
	pDC->RoundRect(100, 5, 180, 90, 10, 10);


	if (keyTaken && !isDoorOpen)
	{
		// put key in item list
		HENHMETAFILE key = GetEnhMetaFile((CString)"Key.emf");
		PlayEnhMetaFile(pDC->m_hDC, key, CRect(15, 85, 85, 10));
	}

	if (key2Taken && !isDoorOpen)
	{
		// put key in item list
		HENHMETAFILE key = GetEnhMetaFile((CString)"Key.emf");
		PlayEnhMetaFile(pDC->m_hDC, key, CRect(105, 85, 175, 10));
	}





#pragma endregion


#pragma region Wardrobe

	CBrush wardrobeBrush(RGB(132, 99, 66));
	pDC->SelectObject(&wardrobeBrush);

	// main front rect
	pDC->Rectangle(CRect(150, 170, 350, 750));

	// aside polygone

	CPoint wardrobeAside[] = { {349,169}, {400, 229}, {400,699}, { 349, 749} };
	pDC->Polygon(wardrobeAside, numberOfPoints);

	// wardrobe elements

	pDC->Rectangle(CRect(160, 190, 250, 536));

	if (isWarderobeOpen)
	{
		CBrush blackBrush(RGB(0, 0, 0));
		pDC->SelectObject(blackBrush);

		pDC->Rectangle(CRect(160, 190, 250, 536));

		pDC->SelectObject(wardrobeBrush);


		CPoint wingPolygone[] = { {160, 190},{160, 536},{120, 556},{120, 170} };
		pDC->Polygon(wingPolygone, 4);
	}

	pDC->Rectangle(CRect(249, 190, 340, 536));

	//drowers, first upper, secund lower
	
	CRect upperDrower(160, 556, 340, 636);
	pDC->Rectangle(upperDrower);
	CRect lowerDrower(160, 656, 340, 736);
	pDC->Rectangle(lowerDrower);

	// wardrobe handles

	CBrush handlesBrush(RGB(234, 153, 0));
	pDC->SelectObject(&handlesBrush);

	
	if (!isWarderobeOpen)
	{
		pDC->Ellipse(225, 356, 245, 372);

	}
	else if(!keyTaken)
	{
		// put key in item list
		HENHMETAFILE key = GetEnhMetaFile((CString)"Key.emf");
		PlayEnhMetaFile(pDC->m_hDC, key, CRect(190, 526, 240, 476));
	}

	pDC->Ellipse(254, 356, 274, 372);

	// drower handles

	CPoint upperDrowerCenter = upperDrower.CenterPoint();
	CPoint lowerDrowerCenter= lowerDrower.CenterPoint();

	int deltaX = 20/2,  deltaY =16/2;

	pDC->Ellipse(upperDrowerCenter.x - deltaX, upperDrowerCenter.y - deltaY, upperDrowerCenter.x+deltaX, upperDrowerCenter.y+deltaY);
	pDC->Ellipse(lowerDrowerCenter.x - deltaX, lowerDrowerCenter.y - deltaY, lowerDrowerCenter.x + deltaX, lowerDrowerCenter.y + deltaY);


#pragma endregion

#pragma region Sef

	CBrush sefBrush(RGB(99, 99, 99));
	pDC->SelectObject(&sefBrush);

	// main front rect
	pDC->Rectangle(CRect(410, 460, 610, 750));

	// door

	CRect doorRect(420, 470, 600, 740);

	CPoint centerOfDoor = doorRect.CenterPoint();

	if (isSefOpen)
	{
		CBrush doorBlack(RGB(0, 0, 0));
		pDC->SelectObject(&doorBlack);

		pDC->Rectangle(doorRect);
/*
		pDC->SelectObject(&sefBrush);
		CRect door(420, 470, 240, 200);
		pDC->Rectangle(doorRect);*/


		HENHMETAFILE key = GetEnhMetaFile((CString)"Key.emf");
		PlayEnhMetaFile(pDC->m_hDC, key, CRect(centerOfDoor.x, centerOfDoor.y+50, centerOfDoor.x+50, centerOfDoor.y+100));

		CBrush gray(RGB(66, 66, 66));
		pDC->SelectObject(&gray);
		pDC->Rectangle(CRect(220, 470, 420, 740));

	}
	else
	{
		pDC->Rectangle(doorRect);
	}

	CBrush Sefhandles(RGB(0, 0, 0));
	pDC->SelectObject(&Sefhandles);

	pDC->Ellipse(centerOfDoor.x - 5, centerOfDoor.y - 5, centerOfDoor.x + 5, centerOfDoor.y + 5);



	CBrush sefAsideBrush(RGB(66, 66, 66));
	pDC->SelectObject(&sefAsideBrush);

	CPoint sefAsidePolygon[] = { {609, 461}, {630, 440}, {630, 680}, {609, 750} };
	pDC->Polygon(sefAsidePolygon, numberOfPoints);


	CPoint sefUpperPolygon[] = { {409, 461}, {430, 441}, {630, 441}, {609, 461} };
	pDC->Polygon(sefUpperPolygon, numberOfPoints);






#pragma endregion



#pragma region Door

	CBrush doorBrush(RGB(132, 99, 66));
	pDC->SelectObject(doorBrush);

	CPoint doorPolygone[] = { {1309, 159}, { 1409,109 }, { 1409,809 }, { 1309,709 } };

	if (isDoorOpen)
	{
		CBrush blackBrush(RGB(0, 0, 0));
		pDC->SelectObject(blackBrush);
		pDC->Polygon(doorPolygone, numberOfPoints);
		pDC->SelectObject(doorBrush);

		pDC->Rectangle(CRect(1309, 159, 1009, 709));
	}
	else
	{
		pDC->Polygon(doorPolygone, numberOfPoints);

		pDC->SelectObject(&handlesBrush);
		pDC->Ellipse(1373, 485, 1399, 500);
		pDC->Ellipse(1368, 480, 1395, 505);
	}
	

	// keyhole

	CBrush keyholeBrush(RGB(0, 0, 0));
	pDC->SelectObject(&keyholeBrush);
	pDC->Ellipse(1392,515, 1398, 530);
#pragma endregion

#pragma region Window

	//upper part of window


	CBrush windowBrush(RGB(132, 99, 66));
	pDC->SelectObject(&windowBrush);

	CPoint frontWallCenter = frontWallRect.CenterPoint();
	pDC->RoundRect(frontWallCenter.x - 140, frontWallCenter.y - 245, frontWallCenter.x + 200, frontWallCenter.y - 205, 50, 50);

	CRect window(frontWallCenter.x - 120, frontWallCenter.y - 205, frontWallCenter.x + 180, frontWallCenter.y + 100);
	pDC->Rectangle(window);

	// glass on window

	CBrush windowBlueBrush(RGB(0,186,219));
	pDC->SelectObject(windowBlueBrush);

	CPoint windowCenter = window.CenterPoint();
	CRect glass1(windowCenter.x - 140, windowCenter.y + 5, windowCenter.x - 5, windowCenter.y + 140);
	pDC->Rectangle(glass1);


	CRect glass2(windowCenter.x + 5, windowCenter.y + 5, windowCenter.x + 140, windowCenter.y + 140);
	pDC->Rectangle(glass2);

	CRect glass3(windowCenter.x - 140, windowCenter.y - 5, windowCenter.x - 5, windowCenter.y - 140);
	pDC->Rectangle(glass3);


	CRect glass4(windowCenter.x + 5, windowCenter.y - 5, windowCenter.x + 140, windowCenter.y - 140);
	pDC->Rectangle(glass4);

	// carpet

	CBrush carpetBrush(RGB(248,253,159));

	pDC->SelectObject(&carpetBrush);

	pDC->BeginPath();

	pDC->MoveTo(frontWallCenter.x - 125, frontWallCenter.y - 205);
	pDC->LineTo(frontWallCenter.x - 125, frontWallCenter.y + 50);

	CPoint controlPoints[] = { {windowCenter.x - 140, frontWallCenter.y }, {frontWallCenter.x + 170, frontWallCenter.y + 100} 
	,  {frontWallCenter.x + 185, frontWallCenter.y + 60} };
	pDC->PolyBezierTo(controlPoints, 3);

	
	pDC->LineTo(frontWallCenter.x + 185, frontWallCenter.y - 205);

	pDC->LineTo(frontWallCenter.x - 120, frontWallCenter.y - 205);

	pDC->EndPath();
	pDC->StrokeAndFillPath();

	


#pragma endregion


		// recover contex

	pDC->SelectObject(oldBrush);
}


// CGDILab1View printing


void CGDILab1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CGDILab1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDILab1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDILab1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CGDILab1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGDILab1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGDILab1View diagnostics

#ifdef _DEBUG
void CGDILab1View::AssertValid() const
{
	CView::AssertValid();
}

void CGDILab1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDILab1Doc* CGDILab1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDILab1Doc)));
	return (CGDILab1Doc*)m_pDocument;
}
#endif //_DEBUG


// CGDILab1View message handlers


void CGDILab1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	bool rerenderSceen = false;

	CRgn keyRgn;
	keyRgn.CreateRectRgn(190, 476, 240, 526);

	CRgn wardrobeRgn;
	wardrobeRgn.CreateRectRgn(160, 190, 250, 536);

	CRgn doorRgn;
	CPoint doorPolygon[] = {{ 1309, 159 }, { 1409,109 }, { 1409,809 }, { 1309,709 }};
	doorRgn.CreatePolygonRgn(doorPolygon, 4, WINDING);

	CRgn sefRgn;
	sefRgn.CreateRectRgn(420, 470, 600, 740);

	//420, 470, 600, 74

	CRgn key2Rgn;
	key2Rgn.CreateRectRgn(420, 470 + 50, 600 + 50, 740 + 100);

	
/*
	CRgn keyRgn;
	keyRgn.CreateRoundRectRgn(10, 5, 90, 90, 10, 10);
	if (!keyTaken && keyRgn.PtInRegion(point))
	{
		keyTaken = true;
		rerenderSceen = true;
	}else*/

	if (!isWarderobeOpen && wardrobeRgn.PtInRegion(point))
	{
		isWarderobeOpen = true;
		rerenderSceen = true;
	}
	else if (isWarderobeOpen && !keyTaken && keyRgn.PtInRegion(point))
	{
		keyTaken = true;
		rerenderSceen = true;
	}
	else if (isSefOpen && key2Rgn.PtInRegion(point))
	{
		key2Taken = true;
		rerenderSceen = true;
	}
	else if (keyTaken && sefRgn.PtInRegion(point))
	{
		isSefOpen = true;
		rerenderSceen = true;
	}
	else if (key2Taken && doorRgn.PtInRegion(point))
	{
		isDoorOpen = true;
		rerenderSceen = true;
	}
	

	if (rerenderSceen)
	{
		Invalidate();
	}
	CView::OnLButtonDown(nFlags, point);
}
