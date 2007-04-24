// DlgExportar.cpp : implementation file
//

#include "stdafx.h"
#include "IOFile.h"
#include "DlgExportar.h"

// CDlgExportar dialog

IMPLEMENT_DYNAMIC(CDlgExportar, CDialog)
CDlgExportar::CDlgExportar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExportar::IDD, pParent)
{
	m_pExport= NULL;
	m_nTotal=0;
	m_nRealizados=0;
	m_nCurrent= 0;
	m_szQuery=NULL;
}

CDlgExportar::~CDlgExportar()
{
}

void CDlgExportar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICINFORMACION, m_SAyuda);
	DDX_Control(pDX, IDC_PROGRESS, m_PCEstatus);
	DDX_Control(pDX, IDCANCEL, m_BCancel);
}


BEGIN_MESSAGE_MAP(CDlgExportar, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgExportar message handlers

void CDlgExportar::OnBnClickedOk()
{
	// OnOK();
}

void CDlgExportar::OnBnClickedCancel()
{
	OnCancel();
}

LRESULT CDlgExportar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message== WM_MYSQL) {

		MySQLMessage	*message= (MySQLMessage *) lParam;
		if( wParam== QUERY_EMPTY) {

			m_pExport->Vacio();
		} else if( wParam==QUERY_RESULT) {

			m_pExport->Agregar( *message->GetMensaje());
			m_nRealizados++;
			if( m_nTotal) {
					
				CString atributo;
				atributo.Format( "Elemento %d de %d", m_nRealizados, m_nTotal);
				m_SAyuda.SetWindowText(atributo);
				m_PCEstatus.SetPos( m_nRealizados*100/m_nTotal);
			}
		} else if(wParam==QUERY_COUNT2) { //total de filas
            m_nTotal= message->GetCount();
		} else if( wParam==THREAD_END) {

			m_pExport->Terminar();
			m_nCurrent++;
			if( m_nCurrent<m_szNombre->GetCount()) {

				SetWindowText(m_szNombre->GetAt(m_nCurrent));
				m_PCEstatus.SetPos(0);
				m_nTotal=0;
				m_nRealizados=0;
				m_pExport->m_pConector->Query( m_szQuery->GetAt(m_nCurrent), ID_FIND_TERCEROS, this);
				return 0;
			}
			OnOK();
		} else if( wParam==QUERY_TITLE) {

			if( message->GetID()==ID_FIND_TERCEROS) 

				m_pExport->Inicio( *message->GetMensaje());
		}
		else if( wParam==QUERY_ERROR)

			MessageBox(message->GetAt(0));
		return 0;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CDlgExportar::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_PCEstatus.SetRange(0, 100);
	m_PCEstatus.SetPos(0);

	ASSERT(m_szNombre);
	ASSERT(m_pExport);

	SetWindowText("Exportando "+ m_szNombre->GetAt(m_nCurrent));
	m_pExport->m_pConector->Query( m_szQuery->GetAt(0), ID_FIND_TERCEROS, this);
	return TRUE;
}
