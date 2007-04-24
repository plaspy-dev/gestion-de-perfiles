// SeguimientoView.cpp : implementation file
//

#include "stdafx.h"
#include "Template.h"
#include "SeguimientoView.h"


// CSeguimientoView

IMPLEMENT_DYNCREATE(CSeguimientoView, CFormView)

CSeguimientoView::CSeguimientoView()
	: CFormView(CSeguimientoView::IDD)
{

}

CSeguimientoView::~CSeguimientoView()
{
}

void CSeguimientoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EPKZ, m_EPKZ);
	DDX_Control(pDX, IDC_DPRIMERO, m_DTPrimero);
	DDX_Control(pDX, IDC_DSEGUNDO, m_DTSegundo);
	DDX_Control(pDX, IDC_DTERCERO, m_DTTercero);
	DDX_Control(pDX, IDC_DENCUESTA, m_DTEncuesta);
	DDX_Control(pDX, IDC_ENOTAS, m_ENotas);
	DDX_Control(pDX, IDC_EMPRIMERO, m_EPrimero);
	DDX_Control(pDX, IDC_EMSEGUNDO, m_ESegundo);
	DDX_Control(pDX, IDC_EMTERCERO, m_ETercero);
	DDX_Control(pDX, IDC_ERESPONDIO, m_ERespondio);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
}

BEGIN_MESSAGE_MAP(CSeguimientoView, CFormView)
	ON_EN_KILLFOCUS(IDC_EPKZ, &CSeguimientoView::OnEnKillfocusEpkz)
	ON_BN_CLICKED(IDC_BCANCELAR, &CSeguimientoView::OnBnClickedBcancelar)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_INSERTPKZ,  OnInsertPKZ)
END_MESSAGE_MAP()


// CSeguimientoView diagnostics

#ifdef _DEBUG
void CSeguimientoView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSeguimientoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSeguimientoView message handlers

void CSeguimientoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetDocument()->SetTitle("Seguimiento");
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	m_ENotas.InitMultiline();
}

void CSeguimientoView::OnEnKillfocusEpkz()
{
	CString temp= m_EPKZ.Text;
	if(temp.IsEmpty() || m_szOldPKZ==temp)
		return;
	m_szOldPKZ=temp;
	OnBnClickedBcancelar();
	m_EPKZ.Text=temp;
	((CMDIFrameWndXP *) AfxGetMainWnd())->SendMessageBroadCast(WM_INSERTPKZ, 0, (LPARAM)(const char *)temp);

	CQuery query("seguimiento");
	
	query+="primero";
	query+="mmedio";
	query+="segundo";
	query+="smedio";
	query+="tercero";
	query+="tmedio";
	query+="encuesta";
	query+="respondio";
	query+="notas";

	m_pConector->Query(query.GetSelect("pkz='"+  m_EPKZ.Text + "'"), query_pkz, this);
}

void CSeguimientoView::OnBnClickedBcancelar()
{
	m_EPKZ.Text="";
	CMyTime tiempo(true);
	m_DTPrimero.SetTime(tiempo);
	m_DTSegundo.SetTime(tiempo);
	m_DTTercero.SetTime(tiempo);
	m_DTEncuesta.SetTime(tiempo);
	m_ENotas.Text="";
	m_EPrimero.Text="";
	m_ESegundo.Text="";
	m_ETercero.Text="";
	m_ERespondio.Text="";
	m_ENotas.ValidarTamano();
	SetErrorEx(&m_EPKZ, "Listo");
}

LRESULT CSeguimientoView::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {

	CMySQLMessage query(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(query.GetID()==query_pkz) {

				CMyTime tiempo;

				tiempo= query["primero"];
				m_DTPrimero.SetTime(tiempo);
				tiempo= query["segundo"];
				m_DTSegundo.SetTime(tiempo);
				tiempo= query["tercero"];
				m_DTTercero.SetTime(tiempo);
				tiempo= query["encuesta"];
				m_DTEncuesta.SetTime(tiempo);

				m_ENotas.Text=query["notas"];
				m_EPrimero.Text=query["mmedio"];
				m_ESegundo.Text=query["smedio"];
				m_ETercero.Text=query["tmedio"];
				m_ERespondio.Text=query["respondio"];

				m_ENotas.ValidarTamano();
			}
			break;
	}
	return 0;
}

BOOL CSeguimientoView::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
		if( pMsg->wParam== VK_RETURN) {

			CWnd *foco= GetFocus();
			if( foco)
				if( !foco->IsKindOf( RUNTIME_CLASS(CButtonXP))) 

					pMsg->wParam= VK_TAB;
		}
		return CFormView::PreTranslateMessage(pMsg);
}

void CSeguimientoView::OnDestroy()
{
	m_EPKZ.Text="";
	CFormView::OnDestroy();
}

LRESULT CSeguimientoView::OnInsertPKZ(WPARAM wParam, LPARAM lParam) {

	CString temp=(const char *)lParam;
	m_EPKZ.Text=temp;
	OnEnKillfocusEpkz();
	return 0;
}
