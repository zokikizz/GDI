
// GDITemplateView.cpp : implementation of the CGDITemplateView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDITemplate.h"
#endif

#include "GDITemplateDoc.h"
#include "GDITemplateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGDITemplateView

IMPLEMENT_DYNCREATE(CGDITemplateView, CView)

BEGIN_MESSAGE_MAP(CGDITemplateView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGDITemplateView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CGDITemplateView construction/destruction

CGDITemplateView::CGDITemplateView() noexcept
{
	// TODO: add construction code here

}

CGDITemplateView::~CGDITemplateView()
{
}

BOOL CGDITemplateView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGDITemplateView drawing

void CGDITemplateView::OnDraw(CDC* pDC)
{
	// TODO: add draw code for native data here

	pDC->Rectangle(CRect(100, 100, 200, 200));
}


// CGDITemplateView printing


void CGDITemplateView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CGDITemplateView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDITemplateView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDITemplateView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CGDITemplateView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGDITemplateView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGDITemplateView diagnostics

#ifdef _DEBUG
void CGDITemplateView::AssertValid() const
{
	CView::AssertValid();
}

void CGDITemplateView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDITemplateDoc* CGDITemplateView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDITemplateDoc)));
	return (CGDITemplateDoc*)m_pDocument;
}
#endif //_DEBUG


// CGDITemplateView message handlers
