// SeguimientoView.cpp : implementation file
//

#include "stdafx.h"
#include "Template.h"
#include "SeguimientoView.h"
# include "GestionView.h"

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
	DDX_Control(pDX, IDC_EMPRIMERO, m_EPrimero);
	DDX_Control(pDX, IDC_EMSEGUNDO, m_ESegundo);
	DDX_Control(pDX, IDC_EMTERCERO, m_ETercero);
	DDX_Control(pDX, IDC_ERESPONDIO, m_ERespondio);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
	DDX_Control(pDX, IDC_BOK, m_BAceptar);
	DDX_Control(pDX, IDC_SFALLECIDO, m_SFallecido);
}

BEGIN_MESSAGE_MAP(CSeguimientoView, CFormView)
	ON_EN_KILLFOCUS(IDC_EPKZ, &CSeguimientoView::OnEnKillfocusEpkz)
	ON_BN_CLICKED(IDC_BCANCELAR, &CSeguimientoView::OnBnClickedBcancelar)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_INSERTPKZ,  OnInsertPKZ)
	ON_BN_CLICKED(IDC_BOK, &CSeguimientoView::OnBnClickedBok)
	ON_WM_SIZE()
	ON_MESSAGE(WM_SHOWFALLECIDO, OnShowFallecido)
	ON_MESSAGE(WM_QUERYUSUARIO, OnQueryUsuario)
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
	GetDocument()->SetTitle(CLanguage::GetTraduccion(IDS_SEGUIMIENTO));
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	m_pUsuario= (CUsuario *)AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO);

	CMyTime tiempo(true);
	tiempo.m_status= COleDateTime::error;
	m_DTPrimero.SetTime(tiempo);
	m_DTSegundo.SetTime(tiempo);
	m_DTTercero.SetTime(tiempo);
	m_DTEncuesta.SetTime(tiempo);

	m_SFallecido.SetFontColor(RGB(255,0,0));
	m_SFallecido.ShowWindow(SW_HIDE);
	m_BAceptar.ShowWindow(m_pUsuario->GetGrupo()==1 ? SW_NORMAL : SW_HIDE);
	m_EPKZ.InitAutoWidth();
}

LRESULT CSeguimientoView::OnQueryUsuario(WPARAM wParam, LPARAM lParam) {

	m_pUsuario= (CUsuario *)lParam;
	m_BAceptar.ShowWindow(m_pUsuario->GetGrupo()==1 ? SW_NORMAL : SW_HIDE);
	return 0;
}

void CSeguimientoView::OnEnKillfocusEpkz()
{
	CString temp= m_EPKZ.Text;
	if(temp.IsEmpty() || m_szOldPKZ==temp)
		return;
	m_szOldPKZ=temp;
	Clean();
	m_EPKZ.Text=temp;

	CQuery query("seguimiento");
	
	query+="primero";
	query+="mmedio";
	query+="segundo";
	query+="smedio";
	query+="tercero";
	query+="tmedio";
	query+="encuesta";
	query+="respondio";

	m_pConector->Query(query.GetSelect("pkz='"+  m_EPKZ.Text + "'"), query_pkz, this);

	((CMDIFrameWndXP *) AfxGetMainWnd())->SendMessageBroadCast(WM_INSERTPKZ, 0, (LPARAM)(const char *)m_EPKZ.Text);
}

void CSeguimientoView::Clean() {

	m_szCurrentPKZ.Empty();
	CMyTime tiempo(true);
	tiempo.m_status= COleDateTime::error;
	m_DTPrimero.SetTime(tiempo);
	m_DTSegundo.SetTime(tiempo);
	m_DTTercero.SetTime(tiempo);
	m_DTEncuesta.SetTime(tiempo);
	m_EPrimero.Text="";
	m_ESegundo.Text="";
	m_ETercero.Text="";
	m_ERespondio.Text="";
	m_SFallecido.ShowWindow(SW_HIDE);
}

void CSeguimientoView::OnBnClickedBcancelar()
{
	m_EPKZ.Text="";
	m_szOldPKZ.Empty();
	Clean();
	SetErrorEx(&m_EPKZ, CLanguage::GetTraduccion(AFX_IDS_IDLEMESSAGE));
}

LRESULT CSeguimientoView::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {

	CMySQLMessage query(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(query.GetID()==query_pkz) {

				m_szCurrentPKZ= m_szOldPKZ;

				CMyTime tiempo;

				tiempo= query["primero"];
				m_DTPrimero.SetTime(tiempo);
				tiempo= query["segundo"];
				m_DTSegundo.SetTime(tiempo);
				tiempo= query["tercero"];
				m_DTTercero.SetTime(tiempo);
				tiempo= query["encuesta"];
				m_DTEncuesta.SetTime(tiempo);

				m_EPrimero.Text=query["mmedio"];
				m_ESegundo.Text=query["smedio"];
				m_ETercero.Text=query["tmedio"];
				m_ERespondio.Text=query["respondio"];

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

void CSeguimientoView::OnBnClickedBok()
{
	CString temp;
	CQuery query("seguimiento");
	CMyTime tiempo;

	if(m_EPKZ.Text.IsEmpty()) {
		SetErrorEx(&m_EPKZ, CLanguage::GetTraduccion(IDS_INSERTPKZ), 2, "residencia");
		return;
	}
	if(m_szCurrentPKZ.IsEmpty())
		query["pkz"]= m_EPKZ.Text;

	m_DTPrimero.GetTime(tiempo);
	query["primero"]=tiempo.GetFormatoMysql();
	query["mmedio"]=m_EPrimero.Text;
	m_DTSegundo.GetTime(tiempo);
	query["segundo"]=tiempo.GetFormatoMysql();
	query["smedio"]=m_ESegundo.Text;
	m_DTTercero.GetTime(tiempo);
	query["tercero"]=tiempo.GetFormatoMysql();
	query["tmedio"]=m_ETercero.Text;
	m_DTEncuesta.GetTime(tiempo);
	query["encuesta"]=tiempo.GetFormatoMysql();
	query["respondio"]=m_ERespondio.Text;

	m_pConector->Query(m_szCurrentPKZ.IsEmpty() ? query.GetInsert() : query.GetUpdate("pkz='"+ m_szCurrentPKZ + "'"));
	CGestionView::Actualizar(m_pConector, m_EPKZ.Text);
	OnBnClickedBcancelar();
}

void CSeguimientoView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if(::IsWindow(m_SFallecido)) {
		CRect rect;
		m_SFallecido.GetWindowRect(rect);
		m_SFallecido.MoveWindow(0,0,cx, rect.Height());
	}
}

LRESULT CSeguimientoView::OnShowFallecido(WPARAM wParam, LPARAM lParam) {

	m_SFallecido.ShowWindow(wParam ? SW_NORMAL : SW_HIDE);
	return 0;
}
