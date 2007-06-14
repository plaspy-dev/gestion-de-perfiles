// IndexarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Template.h"
#include "IndexarDlg.h"

CIndexarDlg	*CIndexarDlg::m_pDlg=NULL;

// CIndexarDlg dialog

IMPLEMENT_DYNAMIC(CIndexarDlg, CDialog)

CIndexarDlg::CIndexarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIndexarDlg::IDD, pParent)
{
	m_nTotal=0;
	m_bFirst=true;
	m_nCurrent=0;
	m_nMaximo=1;
}

CIndexarDlg::CIndexarDlg(const CString pkz)
: CDialog(CIndexarDlg::IDD, NULL)
{
	m_szPKZ= pkz;
	m_nTotal=0;
	m_bFirst=true;
	m_nCurrent=0;
	m_nMaximo=1;
	if(Create(CIndexarDlg::IDD, NULL)) 
		ShowWindow(SW_HIDE);
}

CIndexarDlg::~CIndexarDlg()
{
}

void CIndexarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PPROGRESOREAL, m_PCProgreso);
}


BEGIN_MESSAGE_MAP(CIndexarDlg, CDialog)
	ON_MESSAGE(WM_MYSQL, OnMySQLMessage)
END_MESSAGE_MAP()


// CIndexarDlg message handlers

void CIndexarDlg::SetPKZ(const CString &pkz) {

	if(m_pDlg==NULL) {
		m_pDlg= new CIndexarDlg(pkz);
	} else {
		m_pDlg->m_szPKZ=pkz;
		m_pDlg->m_pConector->Query("DELETE FROM busqueda WHERE pkz='" + m_pDlg->m_szPKZ + "'");
		m_pDlg->m_pConector->Query("SHOW TABLES;", query_tablas, m_pDlg);
	}
}

BOOL CIndexarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	if(m_szPKZ.IsEmpty()) {
		m_pConector->Query("TRUNCATE busqueda");
	} else {
		m_pConector->Query("DELETE FROM busqueda WHERE pkz='" + m_szPKZ + "'");
	}
	m_pConector->Query("SHOW TABLES;", query_tablas, this);
	return TRUE;
}

LRESULT CIndexarDlg::OnMySQLMessage(WPARAM wParam, LPARAM lParam) {

	CMySQLMessage mess(lParam);
	switch(wParam) {
		case QUERY_COUNT2:
			if(mess.GetID()==query_resultados) {
				if(m_bFirst) {
					m_bFirst=false;
					m_nMaximo= mess.GetCount()*m_nTotal;
					if(m_nMaximo==0)
						m_nMaximo=1;
					m_PCProgreso.SetRange32(0,m_nMaximo);
					m_PCProgreso.SetStep(1);
				}
			}
			break;
		case QUERY_RESULT:
			if(mess.GetID()==query_tablas) {
				if(mess[0]=="busqueda" || mess[0]=="shared")
					break;
				m_nTotal++;
				CString temp;
				temp= "SELECT * FROM " + mess[0];
				if(!m_szPKZ.IsEmpty()) 
					temp+=" WHERE pkz='" + m_szPKZ + "'";
				m_pConector->Query(temp,query_resultados, this);
			} else if(mess.GetID()==query_resultados) {
				CString texto;
				CString pkz;

				for(int i=0; i<mess.GetColumnCount(); i++) {

					if(mess.GetColumna(i)=="pkz" || mess.GetColumna(i)=="PKZ") 
						pkz= mess[i];
					else texto+= " " + mess[i];
				}
				if(!pkz.IsEmpty()) {

					CQuery query("busqueda");
					query["pkz"]=pkz;
					query["texto"]= texto;
					CString temp= query.GetInsert();
					temp.Replace("INSERT INTO ", "INSERT IGNORE INTO ");
					m_pConector->Find(temp);

					query.Clean();

					query["pkz"]=pkz;
					texto.Format("CONCAT(texto,%s)", query.GetValor(texto));
					query.AddCampo("texto", texto, false);
					m_pConector->Find(query.GetUpdate("pkz='" + pkz+"'"));
				}
				m_nCurrent++;
				m_PCProgreso.SetPos((m_nCurrent*100)/m_nMaximo);
			}
			break;
		case THREAD_END:
			if(mess.GetID()==query_resultados) {
				m_nTotal--;
				if(m_nTotal==0) {
					OnCancel();
				}
			}
			break;
	}
	return 0;
}
