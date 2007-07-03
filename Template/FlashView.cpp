// FlashView.cpp : implementation file
//

#include "stdafx.h"
#include "Template.h"
#include "FlashView.h"


// CFlashView

IMPLEMENT_DYNCREATE(CFlashView, CFormView)

CFlashView::CFlashView()
	: CFormView(CFlashView::IDD)
{

}

CFlashView::~CFlashView()
{
}

void CFlashView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CNOMOSTRAR, m_BMostrar);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH1, m_SFlash);
}

BEGIN_MESSAGE_MAP(CFlashView, CFormView)
	ON_BN_CLICKED(IDC_CNOMOSTRAR, &CFlashView::OnBnClickedCnomostrar)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CFlashView diagnostics

#ifdef _DEBUG
void CFlashView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFlashView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFlashView message handlers

void CFlashView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	GetDocument()->SetTitle(CLanguage::GetTraduccion(IDS_BIENVENIDO));
	m_BMostrar.SetCheck(AfxGetApp()->GetProfileInt("bienvenido", "noinicio", 0));

	m_SFlash.put_Movie(CApplicacion::GetAppPath("logo1.swf"));
}

void CFlashView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	if(bActivate) {
		KillTimer(1);
		m_SFlash.ShowWindow(SW_SHOW);
	} else {
		SetTimer(1, 1000, NULL);
	}
}

void CFlashView::OnBnClickedCnomostrar()
{
	AfxGetApp()->WriteProfileInt("bienvenido", "noinicio", m_BMostrar.GetCheck());
}

# define DELTA 3
void CFlashView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if(::IsWindow(m_SFlash)) {

		CRect rect;

		m_BMostrar.GetWindowRect(rect);

		int ancho=rect.Width();
		int alto=rect.Height();

		rect.bottom= cy-DELTA;
		rect.top= rect.bottom-alto;
		rect.left= DELTA;
		rect.right= rect.left+ ancho;

		m_BMostrar.MoveWindow(rect);

		rect.bottom= rect.top-DELTA;
		rect.top= DELTA;
		rect.left= DELTA;
		rect.right= cx-DELTA;
		
		m_SFlash.MoveWindow(rect);
	}
}

void CFlashView::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==1) {
		CMDIFrameWndXP *pFrame= (CMDIFrameWndXP *)AfxGetMainWnd();
		CMDIChildWndXP *pChild= (CMDIChildWndXP *)pFrame->GetActiveFrame();
		if(pChild->GetActiveView()!=this) {
			KillTimer(1);
			m_SFlash.ShowWindow(SW_HIDE);
		}
	}
	CFormView::OnTimer(nIDEvent);
}

void CFlashView::OnDestroy()
{
	KillTimer(1);
	CFormView::OnDestroy();
}
