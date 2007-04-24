// LicenceDlg.cpp: archivo de implementación
//

#include "stdafx.h"
# include "iofile.h"

# include "resource.h"
#include "LicenceDlg.h"
# include "defender.h"

// Cuadro de diálogo de CLicenceDlg

IMPLEMENT_DYNAMIC(CLicenceDlg, CDialogXP)
CLicenceDlg::CLicenceDlg(CWnd* pParent /*=NULL*/)
	: CDialogXP(CLicenceDlg::IDD, pParent)
{
	m_nCurrentPos=0;
}

CLicenceDlg::~CLicenceDlg()
{
}

void CLicenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogXP::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ESEMILLA, m_ESemilla);
	DDX_Control(pDX, IDC_ESERIAL, m_ESerial);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDC_BPROBAR, m_BProbar);
	DDX_Control(pDX, IDC_STITULO, m_STitulo);
	DDX_Control(pDX, IDC_SLINK, m_SLicencia);
	DDX_Control(pDX, IDC_SLOGO, m_SLogo);
	DDX_Control(pDX, IDC_BNEXT, m_BNext);
}


BEGIN_MESSAGE_MAP(CLicenceDlg, CDialogXP)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BPROBAR, &CLicenceDlg::OnBnClickedBprobar)
	ON_BN_CLICKED(IDC_BNEXT, &CLicenceDlg::OnBnClickedBnext)
END_MESSAGE_MAP()


// Controladores de mensajes de CLicenceDlg

void CLicenceDlg::OnBnClickedOk()
{
	CString temp;
	m_ESerial.GetWindowText(temp);
	if(temp.IsEmpty()) {
		MessageBeep(MB_OK);
		return;
	}
	temp= temp.MakeUpper();
	temp.Trim();
	temp.Remove('-');
	temp.Remove(' ');
	temp.Remove('\t');
	CDefender info(temp);
	if(info.IsValidSerial()==&info) {
		CMyRegKey key;
		key.MyOpen(HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey());
		key.WriteString("control", "licencia", temp);
		key.WriteString("control\\licencia", "", temp);
		key.MyOpen(HKEY_CURRENT_USER, CApplicacion::GetAppKey());
		key.WriteString("control\\licencia", "", temp);
		key.WriteString("control", "licencia", temp);
		MessageBox("Gracias por utilizar " + CApplicacion::GetLongName() + ".", CApplicacion::GetAppName());
		OnOK();
	} else {
		CWaitCursor wait;
		Sleep(3000);
		MessageBox("El serial introducido no es valido.\nPor favor consulte a su proveedor.", CApplicacion::GetAppName(), MB_ICONERROR);
	}
}

void CLicenceDlg::OnBnClickedCancel()
{
	OnCancel();
}

BOOL CLicenceDlg::OnInitDialog()
{
	CDialogXP::OnInitDialog();

	CString temp;
	m_SLogo.SetBitmap(IDB_BLOGO);

	CDefender info;
	m_ESemilla.SetWindowText(info.GetKernel());

	CDiskID *disco=info.GetDiskID();
	for(int i=0; i<disco->GetDiscosDuros(); i++) {

		temp.Format("%u", disco->GetSerialNumber(i));
		m_saSeriales.Add(temp);
	}

	CTrialInfo trial;
	m_BProbar.EnableWindow(trial.GetLeftDays()>0);

	temp.Format("Bienvenido a %s.\nA continuación ingrese el número de serial del producto\n si no conoce este número consulte a su proveedor.",CApplicacion::GetLongName());
	m_STitulo.SetWindowText(temp);

	m_SLicencia.SetURL(CApplicacion::GetURL());
	m_SLicencia.SetUnderline();

	return TRUE;
}

void CLicenceDlg::OnBnClickedBprobar()
{
	CString texto;
	CTrialInfo info;
	texto.Format("Gracias por probar " + CApplicacion::GetLongName() + ".\nLe quedan %d Dias.", info.GetLeftDays());
	MessageBox(texto, CApplicacion::GetAppName());
	OnOK();
}

void CLicenceDlg::OnBnClickedBnext()
{
	m_nCurrentPos++;
	if(m_nCurrentPos>=m_saSeriales.GetCount())
		m_nCurrentPos=0;
	m_ESemilla.Text= m_saSeriales[m_nCurrentPos];
}
