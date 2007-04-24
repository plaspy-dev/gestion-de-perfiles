// DlgProgreso.cpp : implementation file
//

#include "stdafx.h"
#include "IOFile.h"
#include "DlgProgreso.h"
# include "resource.h"


// CDlgProgreso dialog

IMPLEMENT_DYNAMIC(CDlgProgreso, CDialog)

CDlgProgreso::CDlgProgreso(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProgreso::IDD, pParent)
{
	m_nMaximo=0;
	m_pConector=NULL;
}

CDlgProgreso::~CDlgProgreso()
{
}

void CDlgProgreso::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPROGRESO, m_SProgreso);
	DDX_Control(pDX, IDC_SMENSAJE, m_SMensaje);
	DDX_Control(pDX, IDC_PPROGRESO, m_PCProgreso);
}


BEGIN_MESSAGE_MAP(CDlgProgreso, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgProgreso::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgProgreso::OnBnClickedCancel)
	ON_MESSAGE(WM_MYSQL, OnMySQLMensaje)
END_MESSAGE_MAP()


// CDlgProgreso message handlers

void CDlgProgreso::OnBnClickedOk()
{
	// OnOK();
}

void CDlgProgreso::OnBnClickedCancel()
{
	// OnCancel();
}

BOOL CDlgProgreso::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT(m_pQuerys);
	ASSERT(m_pMensajes);
	ASSERT(m_pQuerys->GetCount()==m_pMensajes->GetCount());
	if(m_pMensajes->IsEmpty()) {
		
		OnOK();
		return FALSE;
	}

	if(!m_pConector)
		m_pConector= (CConector* )AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);

	m_nMaximo= m_pQuerys->GetCount();
	SetWindowText(m_szTitulo);

	CString text;
	text.Format("Paso 1 de %d", m_nMaximo);
	m_SProgreso.SetWindowText(text);
	m_SMensaje.SetWindowText(m_pMensajes->GetAt(0));
	
	m_PCProgreso.SetRange(0, m_nMaximo);
	m_PCProgreso.SetPos(0);

	for(int i=0; i<m_pQuerys->GetCount(); i++) 
		m_pConector->Query(m_pQuerys->GetAt(i), 1, this);
	return TRUE;
}

LRESULT CDlgProgreso::OnMySQLMensaje(WPARAM wParam, LPARAM lParam) {

	CMySQLMessage *mess=(CMySQLMessage *)lParam;
	if(wParam==THREAD_END) {
		int pos= m_PCProgreso.StepIt()+1;
		if(pos==m_nMaximo) {
			OnOK();
		} else {
			CString text;
			text.Format("Paso %d de %d",pos+1, m_nMaximo);
			m_SProgreso.SetWindowText(text);
			m_SMensaje.SetWindowText(m_pMensajes->GetAt(pos));
		}
	}
	return 0;
}