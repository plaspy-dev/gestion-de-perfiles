// COficinaView message handlers


// OficinaView.cpp : implementation file
//

#include "stdafx.h"
#include "Template.h"
#include "OficinaView.h"


// COficinaView

IMPLEMENT_DYNCREATE(COficinaView, CFormView)

COficinaView::COficinaView()
	: CFormView(COficinaView::IDD)
{

}

COficinaView::~COficinaView()
{
}

void COficinaView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EPKZ1, m_EPKZ);
	DDX_Control(pDX, IDC_EDIRECCION, m_EDireccion);
	DDX_Control(pDX, IDC_ECPOSTAL, m_ECPostal);
	DDX_Control(pDX, IDC_EAAEREO, m_EAAereo);
	DDX_Control(pDX, IDC_ETELEFONO, m_ETelefono);
	DDX_Control(pDX, IDC_EFAX, m_EFax);
	DDX_Control(pDX, IDC_EEMAIL, m_EEmail);
	DDX_Control(pDX, IDC_EWEBSITE, m_EWebSite);
	DDX_Control(pDX, IDC_BCANCEL, m_BCancelar);
	DDX_Control(pDX, IDC_ENOMBRE, m_ENombre);
	DDX_Control(pDX, IDC_EPAIS, m_EPais);
	DDX_Control(pDX, IDC_EDEPARTAMENTO1, m_EDepartamento);
	DDX_Control(pDX, IDC_ECIUDAD1, m_ECiudad);
}

BEGIN_MESSAGE_MAP(COficinaView, CFormView)
	ON_EN_KILLFOCUS(IDC_EPKZ1, &COficinaView::OnEnKillfocusEpkz1)
	ON_BN_CLICKED(IDC_BCANCEL, &COficinaView::OnBnClickedBcancel)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_INSERTPKZ,  OnInsertPKZ)
END_MESSAGE_MAP()


// COficinaView diagnostics

#ifdef _DEBUG
void COficinaView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void COficinaView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// COficinaView message handlers
void COficinaView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	GetDocument()->SetTitle("Oficina");
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
}

void COficinaView::OnEnKillfocusEpkz1()
{
	CString temp= m_EPKZ.Text;
	if(temp.IsEmpty() || m_szOldPKZ==temp)
		return;
	m_szOldPKZ=temp;
	Clean();
	((CMDIFrameWndXP *) AfxGetMainWnd())->SendMessageBroadCast(WM_INSERTPKZ, 0, (LPARAM)(const char *)temp);

	CQuery query("empresa");

	query["nombre"]="nombre";
	query["ciudad"]= "ciudad";
	query["direccion"]= "direccion";
	query["cpostal"]= "cpostal";
	query["aaereo"]= "aaereo";
	query["telefono"]= "telefono";
	query["fax"]= "fax";
	query["email"]= "email";
	query["website"]= "website";
	query+="pais";
	query+="departamento";

	m_pConector->Query(query.GetSelect("pkz='"+  m_EPKZ.Text + "'"), query_pkz, this);
}

void COficinaView::OnBnClickedBok()
{
	CString temp;
	CQuery query("empresa");

	if(m_EPKZ.Text.IsEmpty()) {
		SetErrorEx(&m_EPKZ, "Por favor ingrese un PKZ.", 2, "residencia");
		return;
	}
	
	query["pkz"]= m_EPKZ.Text;
	query["nombre"]=m_ENombre.Text;
	query["ciudad"]= m_ECiudad.Text;
	query["direccion"]= m_EDireccion.Text;
	query["cpostal"]= m_ECPostal.Text;
	query["aaereo"]= m_EAAereo.Text;
	query["telefono"]= m_ETelefono.Text;
	query["fax"]= m_EFax.Text;
	query["email"]= m_EEmail.Text;
	query["website"]= m_EWebSite.Text;

	m_pConector->Query(query.GetInsert());
	OnBnClickedBcancel();	
}

void COficinaView::Clean() {

	m_EPais.Text="";
	m_EDepartamento.Text="";
	m_ECiudad.Text="";
	m_EDireccion.Text="";
	m_ECPostal.Text="";
	m_EAAereo.Text="";
	m_ETelefono.Text="";
	m_EFax.Text="";
	m_EEmail.Text="";
	m_EWebSite.Text="";
}

void COficinaView::OnBnClickedBcancel()
{
	m_EPKZ.Text="";
	Clean();
	SetErrorEx(&m_EPKZ, "Listo");
}

LRESULT COficinaView::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {
	CMySQLMessage mess(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(mess.GetID()==query_pkz) {

				m_ENombre.Text=mess["nombre"];
				m_ECiudad.Text =mess["ciudad"];
				m_EDireccion.Text =mess["direccion"];
				m_ECPostal.Text =mess["cpostal"];
				m_EAAereo.Text =mess["aaereo"];
				m_ETelefono.Text =mess["telefono"];
				m_EFax.Text =mess["fax"];
				m_EEmail.Text =mess["email"];
				m_EWebSite.Text =mess["website"];
				m_EDepartamento.Text=mess["departamento"];
				m_EPais.Text=mess["pais"];
			}
			break;
	}
	return 0;
}

void COficinaView::OnDestroy()
{
	m_EPKZ.Text="";
	CFormView::OnDestroy();
}

BOOL COficinaView::PreTranslateMessage(MSG* pMsg)
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

LRESULT COficinaView::OnInsertPKZ(WPARAM wParam, LPARAM lParam) {

	CString temp=(const char *)lParam;
	m_EPKZ.Text=temp;
	OnEnKillfocusEpkz1();
	return 0;
}
