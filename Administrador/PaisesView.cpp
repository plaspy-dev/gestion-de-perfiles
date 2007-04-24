// PaisesView.cpp : implementation file
//

#include "stdafx.h"
#include "Administrador.h"
#include "PaisesView.h"
# include "resource.h"

// CPaisesView

IMPLEMENT_DYNCREATE(CPaisesView, CFormView)

CPaisesView::CPaisesView()
	: CFormView(CPaisesView::IDD)
{

}

CPaisesView::~CPaisesView()
{
}

void CPaisesView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EPAIS, m_EPais);
	DDX_Control(pDX, IDC_BACEPTAR, m_BAceptar);
	DDX_Control(pDX, IDC_LPAISES, m_LCPaises);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
}

BEGIN_MESSAGE_MAP(CPaisesView, CFormView)
	ON_BN_CLICKED(IDC_BACEPTAR, &CPaisesView::OnBnClickedBaceptar)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
	ON_BN_CLICKED(IDC_BCANCELAR, &CPaisesView::OnBnClickedBcancelar)
END_MESSAGE_MAP()


// CPaisesView diagnostics

#ifdef _DEBUG
void CPaisesView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPaisesView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPaisesView message handlers

void CPaisesView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);

	GetDocument()->SetTitle("Paises");
	m_LCPaises.AddColumn("Pais");
	m_LCPaises.SetOrdenable();

	m_pConector->Query("SELECT id,nombre FROM shared WHERE nReservado=3 AND (szReservado IS NULL OR szReservado!=-1) ORDER BY nombre", 1, this);
}

void CPaisesView::OnBnClickedBaceptar()
{
	CString pais= m_EPais.Text;
	if(pais.IsEmpty()) {
		SetErrorEx(&m_EPais, "Por favor ingrese un pais", 2, "pais");
		return;
	}
	CQuery query("shared");
	query["nombre"]=pais;
	query["nReservado"]="3";
	m_LCPaises.AddItem(pais);
	m_pConector->Query(query.GetInsert(), 2, this);
	OnBnClickedBcancelar();
}

LRESULT CPaisesView::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {

	CMySQLMessage mess(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(mess.GetID()==1) {
				m_LCPaises.SetItemData(
					m_LCPaises.AddItem(mess["nombre"]),
					atoi(mess["id"]));
			} else if(mess.GetID()==2) {
				for(int i=0; i<m_LCPaises.GetItemCount(); i++) {
					if(!m_LCPaises.GetItemData(i)) {
						m_LCPaises.SetItemData(i, atoi(mess[0]));
						break;
					}
				}
			}
			break;
	}
	return 0;
}

void CPaisesView::OnBnClickedBcancelar()
{
	m_EPais.Text="";
}
