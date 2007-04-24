// CiudadesView.cpp : implementation file
//

#include "stdafx.h"
#include "Administrador.h"
#include "CiudadesView.h"


// CCiudadesView

IMPLEMENT_DYNCREATE(CCiudadesView, CFormView)

CCiudadesView::CCiudadesView()
	: CFormView(CCiudadesView::IDD)
{

}

CCiudadesView::~CCiudadesView()
{
}

void CCiudadesView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENOMBRE, m_ENombre);
	DDX_Control(pDX, IDC_CPAIS, m_CBPais);
	DDX_Control(pDX, IDC_CCIUDAD, m_CBDepartamento);
	DDX_Control(pDX, IDC_LCIUDADES, m_LCCiudades);
	DDX_Control(pDX, IDC_BACEPTAR, m_BAceptar);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
}

BEGIN_MESSAGE_MAP(CCiudadesView, CFormView)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
	ON_BN_CLICKED(IDC_BACEPTAR, OnBnClickedBaceptar)
	ON_BN_CLICKED(IDC_BCANCELAR, OnBnClickedBcancelar)
	ON_CBN_SELCHANGE(IDC_CPAIS, &CCiudadesView::OnCbnSelchangeCpais)
END_MESSAGE_MAP()


// CCiudadesView diagnostics

#ifdef _DEBUG
void CCiudadesView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCiudadesView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCiudadesView message handlers

void CCiudadesView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);

	GetDocument()->SetTitle("Ciudades");
	m_LCCiudades.AddColumn("Ciudad");
	m_LCCiudades.AddColumn("Departamento");
	m_LCCiudades.AddColumn("Pais");
	m_LCCiudades.SetOrdenable();

	m_pConector->Query("SELECT id,nombre FROM shared WHERE nReservado=3 AND (szReservado IS NULL OR szReservado!=-1) ORDER BY nombre", query_paises, this);
	m_pConector->Query("SELECT id,nombre,descripcion FROM shared WHERE nReservado=5 AND (szReservado IS NULL OR szReservado!=-1) ORDER BY nombre", query_ciudades, this);
}

LRESULT CCiudadesView::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {
	CMySQLMessage mess(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(mess.GetID()==query_paises) {
				m_CBPais.SetItemData(m_CBPais.AddString(mess["nombre"]),
					atoi(mess["id"]));
			} else if(mess.GetID()==query_departamentos) {
				m_CBDepartamento.SetItemData(m_CBDepartamento.AddString(mess["nombre"]),
					atoi(mess["id"]));
			} else if(mess.GetID()==query_ciudades) {
				int pos=m_LCCiudades.AddItem(mess["nombre"]);
				m_LCCiudades.Items[pos][1]=mess["descripcion"];
				m_LCCiudades.SetItemData(pos, atoi(mess["id"]));
			} else if(mess.GetID()==insert_ciudad) {
				for(int i=0;i<m_LCCiudades.GetItemCount(); i++) {
					if(!m_LCCiudades.GetItemData(i)) {
						m_LCCiudades.SetItemData(i, atoi(mess[0]));
						break;
					}
				}
			}
			break;
	}
	return 0;
}

void CCiudadesView::OnBnClickedBaceptar()
{
	CString nombre= m_ENombre.Text;
	if(nombre.IsEmpty()) {
		SetErrorEx(&m_ENombre, "Por favor ingrese un nombre", 2, "departamento");
		return;
	}
	if(m_CBDepartamento.SelectedIndex==-1) {
		SetErrorEx(&m_CBDepartamento, "Por favor seleccione un departamento", 2, "departamento");
		return;
	}
	CQuery query("shared");
	query["nombre"]=nombre;
	nombre.Format("%d", m_CBDepartamento.SelectedData);	
	query["descripcion"]= nombre;
	query["nReservado"]= "5";

	int pos= m_LCCiudades.AddItem(m_ENombre.Text);
	m_LCCiudades.Text[pos][1]= m_CBDepartamento.Text;
	m_LCCiudades.Text[pos][2]= m_CBPais.Text;
	m_pConector->Query(query.GetInsert(), insert_ciudad, this);
	OnBnClickedBcancelar();
}

void CCiudadesView::OnBnClickedBcancelar()
{
	m_ENombre.Text="";
	m_CBPais.Text="";
	m_CBDepartamento.Text="";
}

void CCiudadesView::OnCbnSelchangeCpais()
{
	int pos= m_CBPais.SelectedIndex;
	m_pConector->KillQuery(query_departamentos, this);
	if(pos==-1) {
		m_CBDepartamento.ResetContent(false);
	} else {
		m_CBDepartamento.ResetContent(false);
		CString query;
		query.Format("SELECT id,nombre FROM shared WHERE nReservado=4 AND descripcion='%d' AND (szReservado IS NULL OR szReservado!=-1) ORDER BY nombre", m_CBPais.SelectedData);
		m_pConector->Query(query, query_departamentos, this);
	}
}
