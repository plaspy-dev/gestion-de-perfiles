// EstudiosView.cpp : implementation file
//

#include "stdafx.h"
#include "Template.h"
#include "EstudiosView.h"
# include "GestionView.h"


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
	DDX_Control(pDX, IDC_BOK, m_BAceptar);
	DDX_Control(pDX, IDC_CBECARIO, m_BBecario);
	DDX_Control(pDX, IDC_SFALLECIDO, m_SFallecido);
	DDX_Control(pDX, IDC_ETIPO1, m_ETipo1);
	DDX_Control(pDX, IDC_ETIPO2, m_ETipo2);
	DDX_Control(pDX, IDC_EBECARIOOTRA, m_EBecarioOtra);
	DDX_Control(pDX, IDC_EAAPOSTGRADO, m_EAPostGrado);
	DDX_Control(pDX, IDC_EAPOSTUNIVERSIDAD, m_EAPostUniversidad);
	DDX_Control(pDX, IDC_EFPOSTGRADO, m_EOPostgrado);
	DDX_Control(pDX, IDC_EFPOSTUNIVERSIDAD, m_EOPostUniversidad);
	DDX_Control(pDX, IDC_EFPOSTPAIS, m_EOPostPais);
	DDX_Control(pDX, IDC_CAPAREA, m_CBArea);
}

BEGIN_MESSAGE_MAP(CEstudiosView, CFormView)
	ON_EN_KILLFOCUS(IDC_EPKZ, &CEstudiosView::OnEnKillfocusEpkz)
	ON_BN_CLICKED(IDC_BCANCELAR, &CEstudiosView::OnBnClickedButton1)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_INSERTPKZ,  OnInsertPKZ)
	ON_BN_CLICKED(IDC_BOK, &CEstudiosView::OnBnClickedBok)
	ON_WM_SIZE()
	ON_MESSAGE(WM_SHOWFALLECIDO, OnShowFallecido)
	ON_MESSAGE(WM_QUERYUSUARIO, OnQueryUsuario)
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
	GetDocument()->SetTitle(CLanguage::GetTraduccion(IDS_EDUCACION));
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	m_pUsuario= (CUsuario *)AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO);

	CMyTime tiempo(true);
	tiempo.m_status= COleDateTime::error;
	m_DTComienzo1.SetTime(tiempo);
	m_DTFin1.SetTime(tiempo);
	m_EComienzo2.SetTime(tiempo);
	m_DTFin2.SetTime(tiempo);

	m_SFallecido.SetFontColor(RGB(255,0,0));
	m_SFallecido.ShowWindow(SW_HIDE);

	m_EAPregrado.Mensaje=CLanguage::GetTraduccion(IDS_ESTUDIOS);
	m_EAPUniversidad.Mensaje=CLanguage::GetTraduccion(IDS_UNIVERSIDAD);
	m_EAMaestria.Mensaje=CLanguage::GetTraduccion(IDS_ESTUDIOS);
	m_EAMUniversidad.Mensaje=CLanguage::GetTraduccion(IDS_UNIVERSIDAD);
	m_EADoctorado.Mensaje=CLanguage::GetTraduccion(IDS_ESTUDIOS);
	m_EADUniversidad.Mensaje=CLanguage::GetTraduccion(IDS_UNIVERSIDAD);
	m_EAOtros.Mensaje=CLanguage::GetTraduccion(IDS_ESTUDIOS);
	m_EAOUniversidad.Mensaje=CLanguage::GetTraduccion(IDS_UNIVERSIDAD);
	m_EAPostGrado.Mensaje=CLanguage::GetTraduccion(IDS_POSTGRADO);
	m_EAPostUniversidad.Mensaje= CLanguage::GetTraduccion(IDS_UNIVERSIDAD);

	m_EFPregrado.Mensaje=CLanguage::GetTraduccion(IDS_ESTUDIOS);
	m_EFPUniversidad.Mensaje=CLanguage::GetTraduccion(IDS_UNIVERSIDAD);
	m_EFPPais.Mensaje=CLanguage::GetTraduccion(IDS_PAIS);
	m_EFMaestria.Mensaje=CLanguage::GetTraduccion(IDS_ESTUDIOS);
	m_EFMUniversidad.Mensaje=CLanguage::GetTraduccion(IDS_UNIVERSIDAD);
	m_EFMPais.Mensaje=CLanguage::GetTraduccion(IDS_PAIS);
	m_EFDoctorado.Mensaje=CLanguage::GetTraduccion(IDS_ESTUDIOS);
	m_EFDUniversidad.Mensaje=CLanguage::GetTraduccion(IDS_UNIVERSIDAD);
	m_EFDPais.Mensaje=CLanguage::GetTraduccion(IDS_PAIS);
	m_EFOtros.Mensaje=CLanguage::GetTraduccion(IDS_ESTUDIOS);
	m_EFOUniversidad.Mensaje=CLanguage::GetTraduccion(IDS_UNIVERSIDAD);
	m_EFOPais.Mensaje=CLanguage::GetTraduccion(IDS_PAIS);
	m_EOPostgrado.Mensaje= CLanguage::GetTraduccion(IDS_POSTGRADO);
	m_EOPostUniversidad.Mensaje=CLanguage::GetTraduccion(IDS_UNIVERSIDAD);
	m_EOPostPais.Mensaje= CLanguage::GetTraduccion(IDS_PAIS);

	m_pConector->Query("SELECT id, nombre FROM shared WHERE nReservado=3 AND (szReservado IS NULL OR szReservado!=-1)" ,query_areas, this);

	m_BAceptar.ShowWindow(m_pUsuario->GetGrupo()==1 ? SW_NORMAL : SW_HIDE);
	m_EPKZ.InitAutoWidth();
}

LRESULT CEstudiosView::OnQueryUsuario(WPARAM wParam, LPARAM lParam) {

	m_pUsuario= (CUsuario *)lParam;
	m_BAceptar.ShowWindow(m_pUsuario->GetGrupo()==1 ? SW_NORMAL : SW_HIDE);
	return 0;
}

void CEstudiosView::OnEnKillfocusEpkz()
{
	CString temp= m_EPKZ.Text;
	if(temp.IsEmpty() || m_szOldPKZ==temp)
		return;
	m_szOldPKZ=temp;
	Clean();
	m_EPKZ.Text=temp;

	CQuery query("educacion");

	query+="carrera";
	query+="universidad";
	query+="comienzob1";
	query+="finb1";
	query+="tipo1";
	query+="comienzob2";
	query+="finb2";
	query+="tipo2";
	query+="revista";
	query+="becario";
	query+="becariootra";
	query+="area";

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
	query+="postgrado";
	query+="postuniversidad";

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
	query+="postgrado";
	query+="postuniversidad";
	query+="postpais";

	m_pConector->Query(query.GetSelect("pkz='"+  m_EPKZ.Text + "'"), query_fuera, this);

	((CMDIFrameWndXP *) AfxGetMainWnd())->SendMessageBroadCast(WM_INSERTPKZ, 0, (LPARAM)(const char *)m_EPKZ.Text);
}

void CEstudiosView::Clean() {


	m_szCurrentPKZ1.Empty();
	m_szCurrentPKZ2.Empty();
	m_szCurrentPKZ3.Empty();
	//estudios
	m_EPKZ.Text="";
	m_ECarrera.Text="";
	m_EUniversidad.Text="";
	CMyTime tiempo(true);
	tiempo.m_status= COleDateTime::error;
	m_DTComienzo1.SetTime(tiempo);
	m_DTFin1.SetTime(tiempo);
	m_EComienzo2.SetTime(tiempo);
	m_DTFin2.SetTime(tiempo);
	m_ERevista.Text="";
	m_ETipo1.Text="";
	m_ETipo2.Text="";
	m_EBecarioOtra.Text="";

	// estudios en alemania.
	m_EAPregrado.Text="";
	m_EAMaestria.Text="";
	m_EADoctorado.Text="";
	m_EAOtros.Text="";
	m_EAPUniversidad.Text="";
	m_EAMUniversidad.Text="";
	m_EADUniversidad.Text="";
	m_EAOUniversidad.Text="";
	m_EAPostGrado.Text="";
	m_EAPostUniversidad.Text="";

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
	m_EOPostPais.Text="";
	m_EOPostgrado.Text="";
	m_EOPostUniversidad.Text="";
	// becario
	m_BBecario.SetCheck(0);
	m_SFallecido.ShowWindow(SW_HIDE);

	// combobox

	m_CBArea.SelectedIndex=-1;
}

void CEstudiosView::OnBnClickedButton1()
{
	Clean();
	m_szOldPKZ.Empty();
	SetErrorEx(&m_EPKZ, CLanguage::GetTraduccion(AFX_IDS_IDLEMESSAGE));
}

LRESULT CEstudiosView::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {

	CMySQLMessage query(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(query.GetID()==query_estudios) {

				CMyTime tiempo(true);

				m_szCurrentPKZ1= m_szOldPKZ;
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
				m_BBecario.SetCheck(atoi(query["becario"]));
				m_ETipo1.Text= query["tipo1"];
				m_ETipo2.Text= query["tipo2"];
				m_EBecarioOtra.Text=query["becariootra"];
				m_CBArea.SelectedData= atoi(query["area"]);

			} else if(query.GetID()==query_alemania) {

				m_szCurrentPKZ2= m_szOldPKZ;
				m_EAPregrado.Text=query["pregrado"];
				m_EAPUniversidad.Text=query["puniversidad"];
				m_EAMaestria.Text=query["maestria"];
				m_EAMUniversidad.Text=query["muniversidad"];
				m_EADoctorado.Text=query["doctorado"];
				m_EADUniversidad.Text=query["duniversidad"];
				m_EAOtros.Text=query["otros"];
				m_EAOUniversidad.Text=query["ouniversidad"];
				m_EAPostGrado.Text=query["postgrado"];
				m_EAPostUniversidad.Text=query["postuniversidad"];

			} else if(query.GetID()==query_fuera) {

				m_szCurrentPKZ3= m_szOldPKZ;
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
				m_EOPostgrado.Text=query["postgrado"];
				m_EOPostUniversidad.Text=query["postuniversidad"];
				m_EOPostPais.Text=query["postpais"];
			} else if(query.GetID()==query_areas) {

				int pos=atoi(query["id"]);
				m_CBArea.SetItemData(m_CBArea.AddString(query["nombre"]), pos);
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

void CEstudiosView::OnBnClickedBok()
{
	CString temp;
	if(m_EPKZ.Text.IsEmpty()) {
		SetErrorEx(&m_EPKZ, CLanguage::GetTraduccion(IDS_INSERTPKZ), 2, "residencia");
		return;
	}

	CQuery query("educacion");
	CMyTime tiempo;

	if(m_szCurrentPKZ1.IsEmpty())
		query["pkz"]= m_EPKZ.Text;

	tiempo= query["comienzob1"];
	
	query["carrera"]=m_ECarrera.Text;
	query["universidad"]=m_EUniversidad.Text;
	m_DTComienzo1.GetTime(tiempo);
	query["comienzob1"]=tiempo.GetFormatoMysql();
	m_DTFin1.GetTime(tiempo);
	query["finb1"]=tiempo.GetFormatoMysql();
	query["tipo1"]= m_ETipo1.Text;
	m_EComienzo2.GetTime(tiempo);
	query["comienzob2"]=tiempo.GetFormatoMysql();
	m_DTFin2.GetTime(tiempo);
	query["finb2"]=tiempo.GetFormatoMysql();
	query["tipo2"]= m_ETipo2.Text;
	query["revista"]=m_ERevista.Text;
	temp.Format("%d", m_BBecario.GetCheck());
	query["becario"]=temp;
	query["becariootra"]=m_EBecarioOtra.Text;
	temp.Format("%d", m_CBArea.SelectedData);
	query["area"]= temp;

	m_pConector->Query(m_szCurrentPKZ1.IsEmpty() ? query.GetInsert() : query.GetUpdate("pkz='"+ m_szCurrentPKZ1 + "'"));

	query.Clean();
	query.SetTabla("estudios");
	
	if(m_szCurrentPKZ2.IsEmpty())
		query["pkz"]= m_EPKZ.Text;

	query["pregrado"]=m_EAPregrado.Text;
	query["puniversidad"]=m_EAPUniversidad.Text;
	query["maestria"]=m_EAMaestria.Text;
	query["muniversidad"]=m_EAMUniversidad.Text;
	query["doctorado"]=m_EADoctorado.Text;
	query["duniversidad"]=m_EADUniversidad.Text;
	query["otros"]=m_EAOtros.Text;
	query["ouniversidad"]=m_EAOUniversidad.Text;
	query["postgrado"]= m_EAPostGrado.Text;
	query["postuniversidad"]= m_EAPostUniversidad.Text;

	m_pConector->Query(m_szCurrentPKZ2.IsEmpty() ? query.GetInsert() : query.GetUpdate("pkz='"+ m_szCurrentPKZ2 + "'"));


	query.Clean();
	query.SetTabla("estudiosfuera");
	
	if(m_szCurrentPKZ3.IsEmpty())
		query["pkz"]= m_EPKZ.Text;

	query["pregrado"]=m_EFPregrado.Text;
	query["puniversidad"]=m_EFPUniversidad.Text;
	query["ppais"]=m_EFPPais.Text;
	query["maestria"]=m_EFMaestria.Text;
	query["muniversidad"]=m_EFMUniversidad.Text;
	query["mpais"]=m_EFMPais.Text;
	query["doctorado"]=m_EFDoctorado.Text;
	query["duniversidad"]=m_EFDUniversidad.Text;
	query["dpais"]=m_EFDPais.Text;
	query["otros"]=m_EFOtros.Text;
	query["ouniversidad"]=m_EFOUniversidad.Text;
	query["opais"]=m_EFOPais.Text;
	query["postgrado"]= m_EOPostgrado.Text;
	query["postpais"]= m_EOPostPais.Text;
	query["postuniversidad"]= m_EOPostUniversidad.Text;

	m_pConector->Query(m_szCurrentPKZ3.IsEmpty() ? query.GetInsert() : query.GetUpdate("pkz='"+ m_szCurrentPKZ3 + "'"));

	CGestionView::Actualizar(m_pConector, m_EPKZ.Text);

	OnBnClickedButton1();
}

void CEstudiosView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if(::IsWindow(m_SFallecido)) {
		CRect rect;
		m_SFallecido.GetWindowRect(rect);
		m_SFallecido.MoveWindow(0,0,cx, rect.Height());
	}
}

LRESULT CEstudiosView::OnShowFallecido(WPARAM wParam, LPARAM lParam) {

	m_SFallecido.ShowWindow(wParam ? SW_NORMAL : SW_HIDE);
	return 0;
}
