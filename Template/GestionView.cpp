// GestionView.cpp : implementation file
//

#include "stdafx.h"
#include "Template.h"
#include "GestionView.h"
# include "IndexarDlg.h"
# include "ActualizadoPor.h"


// CGestionView

IMPLEMENT_DYNCREATE(CGestionView, CFormView)

CGestionView::CGestionView()
	: CFormView(CGestionView::IDD)
{

}

CGestionView::~CGestionView()
{
}

void CGestionView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EPKZ, m_EPKZ);
	DDX_Control(pDX, IDC_CACTUALIZADO, m_BActualizado);
	DDX_Control(pDX, IDC_CINVALIDO, m_BNoValidos);
	DDX_Control(pDX, IDC_EACTUALIZADOPOR, m_EActualizado);
	DDX_Control(pDX, IDC_ENOTAS, m_ENotas);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
	DDX_Control(pDX, IDC_BOK, m_BAceptar);
	DDX_Control(pDX, IDC_SFALLECIDO, m_SFallecido);
	DDX_Control(pDX, IDC_SACTUALIZACION, m_SActualizacion);
}

BEGIN_MESSAGE_MAP(CGestionView, CFormView)
	ON_EN_KILLFOCUS(IDC_EPKZ, &CGestionView::OnEnKillfocusEpkz)
	ON_BN_CLICKED(IDC_BCANCELAR, &CGestionView::OnBnClickedBcancelar)
	ON_BN_CLICKED(IDC_BOK, &CGestionView::OnBnClickedBok)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_INSERTPKZ, OnInsertPKZ)
	ON_WM_SIZE()
	ON_MESSAGE(WM_SHOWFALLECIDO, OnShowFallecido)
	ON_MESSAGE(WM_QUERYUSUARIO, OnQueryUsuario)
END_MESSAGE_MAP()


// CGestionView diagnostics

#ifdef _DEBUG
void CGestionView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CGestionView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGestionView message handlers

void CGestionView::OnEnKillfocusEpkz()
{
	CString temp= m_EPKZ.Text;
	if(temp.IsEmpty() || m_szOldPKZ==temp)
		return;
	m_szOldPKZ=temp;
	Clean();
	m_EPKZ.Text=temp;

	CQuery query("gestion");

	query+="actualizado";
	query+="actualizadopor";
	query+="ultimaactualizacion";
	query+="notas";

	m_pConector->Query(query.GetSelect("pkz='"+  m_EPKZ.Text + "'"), query_pkz, this);	

	((CMDIFrameWndXP *) AfxGetMainWnd())->SendMessageBroadCast(WM_INSERTPKZ, 0, (LPARAM)(const char *)m_EPKZ.Text);
}

void CGestionView::OnBnClickedBcancelar()
{
	m_EPKZ.Text="";
	m_szOldPKZ.Empty();
	Clean();
	SetErrorEx(&m_EPKZ, CLanguage::GetTraduccion(AFX_IDS_IDLEMESSAGE));
}

void CGestionView::OnBnClickedBok()
{
	CString temp;
	CQuery query("gestion");
	CMyTime tiempo;

	if(m_EPKZ.Text.IsEmpty()) {
		SetErrorEx(&m_EPKZ, CLanguage::GetTraduccion(IDS_INSERTPKZ), 2, "residencia");
		return;
	}
	if(m_szCurrentPKZ.IsEmpty())
		query["pkz"]= m_EPKZ.Text;

	temp.Format("%d", m_BActualizado.GetCheck());
	query["actualizado"]=temp;
	query["actualizadopor"]=m_EActualizado.Text;
	query["ultimaactualizacion"]= "SYSDATE()";
	query["notas"]=m_ENotas.Text;

	m_pConector->Query(m_szCurrentPKZ.IsEmpty() ? query.GetInsert() : query.GetUpdate("pkz='"+ m_szCurrentPKZ + "'"));
	CGestionView::Actualizar(m_pConector, m_EPKZ.Text);
	OnBnClickedBcancelar();
}

void CGestionView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	GetDocument()->SetTitle(CLanguage::GetTraduccion(IDS_GESTIONBASE));
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	m_pUsuario= (CUsuario *)AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO);
	
	m_ENotas.InitMultiline();
	m_SFallecido.SetFontColor(RGB(255,0,0));
	m_SFallecido.ShowWindow(SW_HIDE);
	m_BAceptar.ShowWindow(m_pUsuario->GetGrupo()==1 ? SW_NORMAL : SW_HIDE);
	m_EPKZ.InitAutoWidth();
}

LRESULT CGestionView::OnQueryUsuario(WPARAM wParam, LPARAM lParam) {

	m_pUsuario= (CUsuario *)lParam;
	m_BAceptar.ShowWindow(m_pUsuario->GetGrupo()==1 ? SW_NORMAL : SW_HIDE);
	return 0;
}

BOOL CGestionView::PreTranslateMessage(MSG* pMsg)
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

void CGestionView::Clean() {

	m_szCurrentPKZ.Empty();
	
	m_SActualizacion.SetWindowText("");

	m_BActualizado.SetCheck(0);
	m_EActualizado.Text="";
	m_ENotas.Text="";
	m_ENotas.ValidarTamano();
	m_SFallecido.ShowWindow(SW_HIDE);
}

LRESULT CGestionView::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {

	CMySQLMessage query(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(query.GetID()==query_pkz) {

				m_szCurrentPKZ= m_szOldPKZ;

				CMyTime tiempo;

				m_BActualizado.SetCheck(atoi(query["actualizado"]));
				m_EActualizado.Text= query["actualizadopor"];
				tiempo= query["ultimaactualizacion"];

				CString temp;
				temp.Format(CLanguage::GetTraduccion(IDS_SULTIMAACTUALIZACION), tiempo.GetFechaHora());
				m_SActualizacion.SetWindowText(temp);

				m_ENotas.Text=query["notas"];

				m_ENotas.ValidarTamano();
			}
			break;
	}
	return 0;
}

void CGestionView::OnDestroy()
{
	m_EPKZ.Text="";
	CFormView::OnDestroy();
}

LRESULT CGestionView::OnInsertPKZ(WPARAM wParam, LPARAM lParam) {

	CString temp=(const char *)lParam;
	m_EPKZ.Text=temp;
	OnEnKillfocusEpkz();
	return 0;
}

void CGestionView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if(::IsWindow(m_SFallecido)) {
		CRect rect;
		m_SFallecido.GetWindowRect(rect);
		m_SFallecido.MoveWindow(0,0,cx, rect.Height());
	}
}

LRESULT CGestionView::OnShowFallecido(WPARAM wParam, LPARAM lParam) {

	m_SFallecido.ShowWindow(wParam ? SW_NORMAL : SW_HIDE);
	return 0;
}

CString CGestionView::GetActualizacionI(const CString pkz, bool actualizado/*=true*/,const CString &actualizadopor/*=""*/) {

	CQuery query("gestion");
	query["pkz"]=pkz;
	query["ultimaactualizacion"]="SYSDATE()";
	if(actualizado)
		query["actualizado"]="1";
	if(!actualizadopor.IsEmpty())
		query["actualizadopor"]= actualizadopor;

	CString temp=query.GetInsert();
	temp.Replace("INSERT INTO ", "INSERT IGNORE INTO ");
	return temp;
}

CString CGestionView::GetActualizacionU(const CString pkz, bool actualizado/*=true*/,const CString &actualizadopor/*=""*/) {

	CQuery query("gestion");
	query["ultimaactualizacion"]="SYSDATE()";
	if(actualizado)
		query["actualizado"]="1";
	if(!actualizadopor.IsEmpty())
		query["actualizadopor"]= actualizadopor;

	return query.GetUpdate("pkz='"+ pkz + "'");
}

void CGestionView::Actualizar(CConector *conector, const CString pkz, bool actualizado/*=true*/) {

	ASSERT(conector);
	if(actualizado) {
		// aqui va el dlg de usuario.
		CActualizadoPor dlg1;
		if(dlg1.DoModal()==IDOK) {

			conector->Query(GetActualizacionU(pkz, actualizado, dlg1.m_szUsuario));
			conector->Query(GetActualizacionI(pkz, actualizado, dlg1.m_szUsuario));
			CIndexarDlg::SetPKZ(pkz);
		}
	} else {
		conector->Find(GetActualizacionU(pkz, actualizado));
		conector->Find(GetActualizacionI(pkz, actualizado));
	}
}
