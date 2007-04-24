// EstudiosView.cpp : implementation file
//

#include "stdafx.h"
#include "Template.h"
#include "EstudiosView.h"


// CEstudiosView

IMPLEMENT_DYNCREATE(CEstudiosView, CFormView)

CEstudiosView::CEstudiosView()
	: CFormView(CEstudiosView::IDD)
{

}

CEstudiosView::~CEstudiosView()
{
}

void CEstudiosView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EPKZ, m_EPKZ);
	DDX_Control(pDX, IDC_ECARRERA, m_ECarrera);
	DDX_Control(pDX, IDC_EUNIVERSIDAD, m_EUniversidad);
	DDX_Control(pDX, IDC_DTCOMIENZO1, m_DTComienzo1);
	DDX_Control(pDX, IDC_DPFIN1, m_DTFin1);
	DDX_Control(pDX, IDC_DPCOMIENZO2, m_EComienzo2);
	DDX_Control(pDX, IDC_DPFIN2, m_DTFin2);
	DDX_Control(pDX, IDC_EREVISTA, m_ERevista);
	DDX_Control(pDX, IDC_ETITULO, m_ETitulo);
	DDX_Control(pDX, IDC_EAPREGRADO, m_EAPregrado);
	DDX_Control(pDX, IDC_EAMAESTRIA, m_EAMaestria);
	DDX_Control(pDX, IDC_EADOCTORADO, m_EADoctorado);
	DDX_Control(pDX, IDC_EAOTROS, m_EAOtros);
	DDX_Control(pDX, IDC_EAUNIVERSIDAD, m_EAPUniversidad);
	DDX_Control(pDX, IDC_EAMUNIVERSIDAD, m_EAMUniversidad);
	DDX_Control(pDX, IDC_EADUNIVERSIDAD, m_EADUniversidad);
	DDX_Control(pDX, IDC_EAOUNIVERSIDAD, m_EAOUniversidad);
	DDX_Control(pDX, IDC_EFPREGRADO, m_EFPregrado);
	DDX_Control(pDX, IDC_EFMAESTRIA, m_EFMaestria);
	DDX_Control(pDX, IDC_EFDOCTORADO, m_EFDoctorado);
	DDX_Control(pDX, IDC_EFOTROS, m_EFOtros);
	DDX_Control(pDX, IDC_EFPUNIVERSIDAD, m_EFPUniversidad);
	DDX_Control(pDX, IDC_EFMUNIVERSIDAD, m_EFMUniversidad);
	DDX_Control(pDX, IDC_EFDUNIVERSIDAD, m_EFDUniversidad);
	DDX_Control(pDX, IDC_EFOUNIVERSIDAD, m_EFOUniversidad);
	DDX_Control(pDX, IDC_EFPPAIS, m_EFPPais);
	DDX_Control(pDX, IDC_EFMPAIS, m_EFMPais);
	DDX_Control(pDX, IDC_EFDPAIS, m_EFDPais);
	DDX_Control(pDX, IDC_EFOPAIS, m_EFOPais);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
}

BEGIN_MESSAGE_MAP(CEstudiosView, CFormView)
	ON_EN_KILLFOCUS(IDC_EPKZ, &CEstudiosView::OnEnKillfocusEpkz)
	ON_BN_CLICKED(IDC_BCANCELAR, &CEstudiosView::OnBnClickedButton1)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_INSERTPKZ,  OnInsertPKZ)
END_MESSAGE_MAP()


// CEstudiosView diagnostics

#ifdef _DEBUG
void CEstudiosView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEstudiosView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEstudiosView message handlers

void CEstudiosView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetDocument()->SetTitle("Estudios");
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
}

void CEstudiosView::OnEnKillfocusEpkz()
{
	CString temp= m_EPKZ.Text;
	if(temp.IsEmpty() || m_szOldPKZ==temp)
		return;
	m_szOldPKZ=temp;
	OnBnClickedButton1();
	m_EPKZ.Text=temp;
	((CMDIFrameWndXP *) AfxGetMainWnd())->SendMessageBroadCast(WM_INSERTPKZ, 0, (LPARAM)(const char *)temp);

	CQuery query("educacion");

	query+="carrera";
	query+="universidad";
	query+="comienzob1";
	query+="finb1";
	query+="comienzob2";
	query+="finb2";
	query+="revista";
	query+="titulos";

	m_pConector->Query(query.GetSelect("pkz='"+  m_EPKZ.Text + "'"), query_estudios, this);

	query.Clean();
	query.SetTabla("estudios");
	query+="pregrado";
	query+="puniversidad";
	query+="maestria";
	query+="muniversidad";
	query+="doctorado";
	query+="duniversidad";
	query+="otros";
	query+="ouniversidad";

	m_pConector->Query(query.GetSelect("pkz='"+  m_EPKZ.Text + "'"), query_alemania, this);


	query.Clean();
	query.SetTabla("estudiosfuera");
	query+="pregrado";
	query+="puniversidad";
	query+="ppais";
	query+="maestria";
	query+="muniversidad";
	query+="mpais";
	query+="doctorado";
	query+="duniversidad";
	query+="dpais";
	query+="otros";
	query+="ouniversidad";
	query+="opais";

	m_pConector->Query(query.GetSelect("pkz='"+  m_EPKZ.Text + "'"), query_fuera, this);
}

void CEstudiosView::OnBnClickedButton1()
{
	//estudios
	m_EPKZ.Text="";
	m_ECarrera.Text="";
	m_EUniversidad.Text="";
	CMyTime tiempo(true);
	m_DTComienzo1.SetTime(tiempo);
	m_DTFin1.SetTime(tiempo);
	m_EComienzo2.SetTime(tiempo);
	m_DTFin2.SetTime(tiempo);
	m_ERevista.Text="";
	m_ETitulo.Text="";

	// estudios en alemania.
	m_EAPregrado.Text="";
	m_EAMaestria.Text="";
	m_EADoctorado.Text="";
	m_EAOtros.Text="";
	m_EAPUniversidad.Text="";
	m_EAMUniversidad.Text="";
	m_EADUniversidad.Text="";
	m_EAOUniversidad.Text="";
	// estudios fuera de alemania
	m_EFPregrado.Text="";
	m_EFMaestria.Text="";
	m_EFDoctorado.Text="";
	m_EFOtros.Text="";
	m_EFPUniversidad.Text="";
	m_EFMUniversidad.Text="";
	m_EFDUniversidad.Text="";
	m_EFOUniversidad.Text="";
	m_EFPPais.Text="";
	m_EFMPais.Text="";
	m_EFDPais.Text="";
	m_EFOPais.Text="";

	SetErrorEx(&m_EPKZ, "Listo");
}

LRESULT CEstudiosView::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {

	CMySQLMessage query(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(query.GetID()==query_estudios) {

				CMyTime tiempo(true);

				m_ECarrera.Text=query["carrera"];
				m_EUniversidad.Text=query["universidad"];
				tiempo= query["comienzob1"];
				m_DTComienzo1.SetTime(tiempo);
				tiempo= query["finb1"];
				m_DTFin1.SetTime(tiempo);
				tiempo= query["comienzob2"];
				m_EComienzo2.SetTime(tiempo);
				tiempo= query["finb2"];
				m_DTFin2.SetTime(tiempo);
				m_ERevista.Text=query["revista"];
				m_ETitulo.Text=query["titulos"];

			} else if(query.GetID()==query_alemania) {

				m_EAPregrado.Text=query["pregrado"];
				m_EAMaestria.Text=query["puniversidad"];
				m_EADoctorado.Text=query["maestria"];
				m_EAOtros.Text=query["muniversidad"];
				m_EAPUniversidad.Text=query["doctorado"];
				m_EAMUniversidad.Text=query["duniversidad"];
				m_EADUniversidad.Text=query["otros"];
				m_EAOUniversidad.Text=query["ouniversidad"];
			} else if(query.GetID()==query_fuera) {

				m_EFPregrado.Text=query["pregrado"];
				m_EFMaestria.Text=query["maestria"];
				m_EFDoctorado.Text=query["doctorado"];
				m_EFOtros.Text=query["otros"];
				m_EFPUniversidad.Text=query["puniversidad"];
				m_EFMUniversidad.Text=query["muniversidad"];
				m_EFDUniversidad.Text=query["duniversidad"];
				m_EFOUniversidad.Text=query["ouniversidad"];
				m_EFPPais.Text=query["ppais"];
				m_EFMPais.Text=query["mpais"];
				m_EFDPais.Text=query["dpais"];
				m_EFOPais.Text=query["opais"];
			}
			break;
	}
	return 0;
}

BOOL CEstudiosView::PreTranslateMessage(MSG* pMsg)
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

void CEstudiosView::OnDestroy()
{
	m_EPKZ.Text="";
	CFormView::OnDestroy();
}

LRESULT CEstudiosView::OnInsertPKZ(WPARAM wParam, LPARAM lParam) {

	CString temp=(const char *)lParam;
	m_EPKZ.Text=temp;
	OnEnKillfocusEpkz();
	return 0;
}
