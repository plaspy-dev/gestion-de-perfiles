// DlgBases.cpp : implementation file
//

#include "stdafx.h"
#include "DlgBases.h"
# include "componentes/newmenu/newmenu.h"
# include "conector.h"
// CDlgBases dialog

IMPLEMENT_DYNAMIC(CDlgBases, CDialog)
CDlgBases::CDlgBases(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBases::IDD, pParent)
{
	m_nSeleccion= -1;
	m_bModificar= false;
}

CDlgBases::~CDlgBases()
{
}

void CDlgBases::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTDB, m_LCDB);
	DDX_Control(pDX, IDC_EDITSERVIDOR, m_ENombreServidor);
	DDX_Control(pDX, IDC_EDITNOMBREDB, m_ENombreDB);
	DDX_Control(pDX, IDC_EDITPUERTO, m_EPuerto);
	DDX_Control(pDX, IDOK, m_BOK);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDC_STATICPRIMERO, m_SPrimero);
	DDX_Control(pDX, IDC_STATICSEGUNDO, m_SSegundo);
	DDX_Control(pDX, IDC_EDITREINTENTOS, m_EReintentos);
	DDX_Control(pDX, IDC_EARCHIVO, m_EArchivo);
	DDX_Control(pDX, IDC_BEXAMINAR, m_BExaminar);
}


BEGIN_MESSAGE_MAP(CDlgBases, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_RCLICK, IDC_LISTDB, OnNMRclickListdb)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTDB, OnNMDblclkListdb)
	ON_EN_CHANGE(IDC_EDITSERVIDOR, OnEnChangeEditservidor)
	ON_COMMAND(32001, OnListasEditar)
	ON_COMMAND(32000, OnListasEliminar)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BEXAMINAR, &CDlgBases::OnBnClickedBexaminar)
END_MESSAGE_MAP()

void CDlgBases::OnBnClickedOk()
{
	CString nombre, servidor, puerto;
	bool autonomo;
	m_ENombreDB.GetWindowText(nombre);
	m_ENombreServidor.GetWindowText(servidor);
	m_EPuerto.GetWindowText(puerto);
	
	if( m_bModificar) 

		m_LCDB.DeleteItem(m_nSeleccion);
	else 
		m_nSeleccion= m_LCDB.GetItemCount();

	if( servidor.IsEmpty() || nombre.IsEmpty() || puerto.IsEmpty()) {

		if( !m_bModificar) {
		
			CConector	m_pConector;
			for( int i=0; i<m_LCDB.GetItemCount(); i++) {

				servidor= m_LCDB.GetItemText( i, 0);
				nombre= m_LCDB.GetItemText( i, 1);
				puerto= m_LCDB.GetItemText( i, 2);
				autonomo= m_LCDB.GetItemText(i, 3)=="Si";
				m_pConector.AddDataBase( nombre, "", "", servidor, atol(puerto), autonomo);
			}
			m_pConector.Guardar();
			m_EReintentos.GetWindowText(puerto);
			CMyRegKey::WriteProfileInt("control", "retry", atol(puerto));
			if(m_szArchivo!=m_EArchivo.Text) {

				CMyRegKey key(HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey());
				if(!key.WriteString("control", "database", m_EArchivo.Text)) {
					MessageBox("No se ha podido cambiar la ruta del almacén de datos, consulte a su administrador.", CApplicacion::GetAppName(), MB_OK | MB_ICONERROR);
				} else {
					MessageBox("Ha cambiado la ruta del almacén de datos, esto afectara a todos los usuarios.\r\nDebe reiniciar la aplicación para efectuar los cambios.", CApplicacion::GetAppName(), MB_OK | MB_ICONWARNING);
				}
			}
			OnOK();
		}
		return;
	}
	m_LCDB.InsertItem(m_nSeleccion, servidor);
	m_LCDB.SetItemText(m_nSeleccion, 1, nombre);
	m_LCDB.SetItemText(m_nSeleccion, 2, puerto);
	m_LCDB.SetItemText(m_nSeleccion, 3, "No");
	m_ENombreDB.SetWindowText("");
	m_ENombreServidor.SetWindowText("");
	puerto.Format("%d", MYSQL_PORT);
	m_EPuerto.SetWindowText(puerto);
	m_bModificar= false;
	m_BOK.SetWindowText("&Aceptar");
}

void CDlgBases::OnNMRclickListdb(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;

	m_nSeleccion= pNMListView->iItem;

	CNewMenu menu;

	menu.CreatePopupMenu();
	menu.InsertMenu(0, MF_BYPOSITION | MF_STRING, 32001, "&Editar");
	menu.InsertMenu(1, MF_BYPOSITION | MF_STRING, 32000, "&Eliminar");
	
	POINT point;
    GetCursorPos( &point );
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0;
}

void CDlgBases::OnNMDblclkListdb(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	m_nSeleccion= pNMListView->iItem;
	OnListasEditar();
	*pResult = 0;
}

BOOL CDlgBases::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString temp;

	m_SPrimero.SetNextSectionId( IDC_STATICSEGUNDO);
	m_SPrimero.Collapse();

	temp.Format("%d", MYSQL_PORT);
	m_EPuerto.SetWindowText(temp);
	temp.Format("%d", CMyRegKey::GetProfileInt("control", "retry", 3));
	m_EReintentos.SetWindowText(temp);
	m_LCDB.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_LCDB.InsertColumn(0, "Servidor", LVCFMT_LEFT, 100);
	m_LCDB.InsertColumn(1, "Base de datos", LVCFMT_LEFT, 100);
	m_LCDB.InsertColumn(2, "Puerto", LVCFMT_LEFT, 50);
	m_LCDB.InsertColumn(3, "Autonomo", LVCFMT_LEFT, 50);

	// cargamos la ruta para mysql incrutado.
	TCHAR szPath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA | CSIDL_FLAG_CREATE,NULL, 0, szPath);
	CString ruta= szPath;
	ruta+="\\";
	ruta+= CApplicacion::GetCompany() + "\\";
	CreateDirectory(ruta, NULL);
	ruta+= CApplicacion::GetAppName() + "\\";
	CreateDirectory(ruta, NULL);
	ruta+="data\\";
	CreateDirectory(ruta, NULL);
	CMyRegKey key(HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey());
	m_szArchivo= key.GetString("control", "database", ruta);
	m_EArchivo.Text= m_szArchivo;

	CConector	m_pConector;
	m_pConector.Cargar();
	CArrayDB *info=NULL;
	for( int i=0; i<m_pConector.GetDatabasesCount(); i++) {

		info= m_pConector.GetDatabaseAt(i);
		m_LCDB.InsertItem( i, info->GetHost());
		m_LCDB.SetItemText( i, 1, info->GetDB());
		temp.Format("%d", info->GetPort());
		m_LCDB.SetItemText( i, 2, temp);
		m_LCDB.SetItemText(i, 3, info->EsIncrustado() ? "Si" : "No");
	}
	return TRUE;
}

void CDlgBases::OnEnChangeEditservidor()
{
	if( m_bModificar) 
		return;
	if( m_ENombreServidor.GetWindowTextLength()) {

		m_BOK.SetWindowText("&Agregar");
		if( !m_ENombreDB.GetWindowTextLength()) {
			CString db= CApplicacion::GetAppName();
			db.MakeLower();
			m_ENombreDB.SetWindowText(db);
		}
	}
	else m_BOK.SetWindowText("&Aceptar");
}

void CDlgBases::OnListasEditar()
{
	m_bModificar= true;
	m_nSeleccion= m_LCDB.GetNextItem(-1, LVNI_SELECTED);
	m_BOK.SetWindowText("&Modificar");
	m_ENombreServidor.SetWindowText( m_LCDB.GetItemText( m_nSeleccion, 0));
	m_ENombreDB.SetWindowText( m_LCDB.GetItemText(m_nSeleccion, 1));
	m_EPuerto.SetWindowText( m_LCDB.GetItemText( m_nSeleccion, 2));
}

void CDlgBases::OnListasEliminar()
{
	m_nSeleccion= m_LCDB.GetNextItem(-1, LVNI_SELECTED);
	if( MessageBox("Esta Seguro de elminar a " + m_LCDB.GetItemText( m_nSeleccion, 0),NULL, MB_ICONQUESTION | MB_YESNO)==IDYES) {

		m_LCDB.DeleteItem(m_nSeleccion);
		m_bModificar= true;
		OnBnClickedCancel();
	}
}

void CDlgBases::OnBnClickedCancel()
{
	if( m_bModificar) {

		m_ENombreDB.SetWindowText("");
		CString temp;
		temp.Format("%d", MYSQL_PORT);
		m_EPuerto.SetWindowText(temp);
		m_ENombreServidor.SetWindowText("");
		m_nSeleccion= -1;
		m_bModificar= false;
		m_BOK.SetWindowText("&Aceptar");
	} else 	
		OnCancel();
}

void CDlgBases::OnBnClickedBexaminar()
{

	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));

	bi.hwndOwner = m_hWnd;
	bi.ulFlags   = BIF_RETURNONLYFSDIRS;
	bi.lpszTitle="Directorio de almacenamiento.";
	

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	BOOL bRet = FALSE;

	TCHAR szFolder[MAX_PATH*2];
	szFolder[0] = _T('\0');

	if (pidl)
	{
		if (SHGetPathFromIDList(pidl, szFolder))
		{
			bRet = TRUE;
		}

		IMalloc *pMalloc = NULL; 
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc) 
		{  
			pMalloc->Free(pidl);  
			pMalloc->Release(); 
		}
	}
	if(bRet)
		m_EArchivo.Text=szFolder;
}
