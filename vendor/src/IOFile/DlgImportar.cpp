// DlgImportar.cpp : implementation file
//

#include "stdafx.h"
#include "IOFile.h"
#include "DlgImportar.h"


// CDlgImportar dialog

IMPLEMENT_DYNAMIC(CDlgImportar, CDialog)
CDlgImportar::CDlgImportar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImportar::IDD, pParent)
{
	m_pImportThread= NULL;
	m_pImport= NULL;
	m_nTotal= 0;
	m_nCurrent= -1;
	m_nCompletado= 0;
}

CDlgImportar::~CDlgImportar()
{
	if( m_pImportThread)
		delete m_pImportThread;
}

void CDlgImportar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICINFORMACION, m_SAyuda);
	DDX_Control(pDX, IDC_PROGRESS, m_PCStatus);
	DDX_Control(pDX, IDCANCEL, m_BCancel);
}


BEGIN_MESSAGE_MAP(CDlgImportar, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgImportar message handlers
void CDlgImportar::OnBnClickedOk()
{
	// OnOK();
}

void CDlgImportar::OnBnClickedCancel()
{
	CWaitCursor wait;
	if( m_pImportThread) 
	{
		delete m_pImportThread;
		m_pImportThread=NULL;
	}
	OnCancel();
}

LRESULT CDlgImportar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message== WM_IMPORTFILE) { // mensajes del hilo

		if( wParam== 0) { // inicio
			m_nTotal= lParam;
			m_nCompletado= 0;
			m_PCStatus.SetPos(0);
			m_nCurrent++;
			SetWindowText("Importando "+ m_pImport->m_pNombres.GetAt(m_nCurrent));
		} else if(wParam==1) { // comenzo a importar uno

			m_nCompletado= lParam;
			if( m_nTotal) {
				
				CString temp;
				temp.Format("Completado %d de %d", m_nCompletado, m_nTotal);
				m_SAyuda.SetWindowText(temp);
				m_PCStatus.SetPos( (m_nCompletado*100)/m_nTotal);
			}
		} else if(wParam== 3)	{ // acabo
			CWaitCursor wait;
			if( m_pImportThread) 
			{
				delete m_pImportThread;
				m_pImportThread=NULL;
			}
			OnOK();
		}
		return 0;
	} else if( message== WM_MYSQL) {

		MySQLMessage	*message= (MySQLMessage *) lParam;
		if( wParam==QUERY_ERROR)
			if( MessageBox("Ocurrio el siguiente error:\r\n" + message->GetAt(0),message->GetAt(1), MB_OKCANCEL | MB_ICONERROR)==IDCANCEL)
				OnBnClickedCancel();
		return 0;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgImportar::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_PCStatus.SetRange(0, 100);
	m_PCStatus.SetPos(0);

	ASSERT(m_pImport);

	m_pImportThread= new CImportThread();
	m_pImportThread->m_pParent= this;
	m_pImportThread->m_pImport= m_pImport;
	m_pImport->m_pParent= this;
	m_pImportThread->Run();
	return TRUE;
}