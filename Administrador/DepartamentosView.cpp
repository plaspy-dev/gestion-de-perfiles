// DepartamentosView.cpp : implementation file
//

#include "stdafx.h"
#include "Administrador.h"
#include "DepartamentosView.h"


// CDepartamentosView

IMPLEMENT_DYNCREATE(CDepartamentosView, CFormView)

CDepartamentosView::CDepartamentosView()
	: CFormView(CDepartamentosView::IDD)
{

}

CDepartamentosView::~CDepartamentosView()
{
}

void CDepartamentosView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENOMBRE, m_ENombre);
	DDX_Control(pDX, IDC_COMBO1, m_CBPais);
	DDX_Control(pDX, IDC_BACEPTAR, m_BAceptar);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
	DDX_Control(pDX, IDC_LDEPARTAMENTO, m_LCDepartamentos);
}

BEGIN_MESSAGE_MAP(CDepartamentosView, CFormView)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
	ON_BN_CLICKED(IDC_BACEPTAR, &CDepartamentosView::OnBnClickedBaceptar)
	ON_BN_CLICKED(IDC_BCANCELAR, &CDepartamentosView::OnBnClickedBcancelar)
END_MESSAGE_MAP()


// CDepartamentosView diagnostics

#ifdef _DEBUG
void CDepartamentosView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDepartamentosView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDepartamentosView message handlers

void CDepartamentosView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);

	GetDocument()->SetTitle("Departamentos");
	m_LCDepartamentos.AddColumn("Departamento");
	m_LCDepartamentos.AddColumn("Pais");
	m_LCDepartamentos.SetOrdenable();

	m_pConector->Query("SELECT id,nombre FROM shared WHERE nReservado=3 AND (szReservado IS NULL OR szReservado!=-1) ORDER BY nombre", query_paises, this);
	m_pConector->Query("SELECT id,nombre,descripcion FROM shared WHERE nReservado=4 AND (szReservado IS NULL OR szReservado!=-1) ORDER BY nombre", query_departamentos, this);
}

LRESULT CDepartamentosView::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {
	CMySQLMessage mess(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(mess.GetID()==query_paises) {
				m_CBPais.SetItemData(m_CBPais.AddString(mess["nombre"]),
					atoi(mess["id"]));
			} else if(mess.GetID()==query_departamentos) {
				int pos=m_LCDepartamentos.AddItem(mess["nombre"]);
				m_LCDepartamentos.Items[pos][1]=mess["descripcion"];
				m_LCDepartamentos.SetItemData(pos, atoi(mess["id"]));
			} else if(mess.GetID()==insert_departamento) {
				for(int i=0;i<m_LCDepartamentos.GetItemCount(); i++) {
					if(!m_LCDepartamentos.GetItemData(i)) {
						m_LCDepartamentos.SetItemData(i, atoi(mess[0]));
						break;
					}
				}
			}
			break;
	}
	return 0;
}

void CDepartamentosView::OnBnClickedBaceptar()
{
	CString nombre= m_ENombre.Text;
	if(nombre.IsEmpty()) {
		SetErrorEx(&m_ENombre, "Por favor ingrese un nombre", 2, "departamento");
		return;
	}
	if(m_CBPais.SelectedIndex==-1) {
		SetErrorEx(&m_CBPais, "Por favor seleccione un pais", 2, "departamento");
		return;
	}
	CQuery query("shared");
	query["nombre"]=nombre;
	nombre.Format("%d", m_CBPais.SelectedData);	
	query["descripcion"]= nombre;
	query["nReservado"]= "4";

	int pos= m_LCDepartamentos.AddItem(m_ENombre.Text);
	m_LCDepartamentos.Text[pos][1]= m_CBPais.Text;
	m_pConector->Query(query.GetInsert(), insert_departamento, this);
	OnBnClickedBcancelar();
}

void CDepartamentosView::OnBnClickedBcancelar()
{
	m_ENombre.Text="";
	m_CBPais.Text="";
}
