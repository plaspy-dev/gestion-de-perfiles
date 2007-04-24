// FormUser.cpp : implementation file
//

#include "stdafx.h"
# include "administrador.h"
#include "FormUser.h"
# include "iofile/iofile.h"
#include "DlgEditar.h"
# include "DlgPasswd.h"
// CFormUser

IMPLEMENT_DYNCREATE(CFormUser, CFormView)

CFormUser::CFormUser()
	: CFormView(CFormUser::IDD)
{
	m_nCurrentGroup= -1;
}

CFormUser::~CFormUser()
{
}

void CFormUser::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTGRUPOS, m_LCGrupos);
	DDX_Control(pDX, IDC_LISTUSUARIOS, m_LCUser);
	DDX_Control(pDX, IDC_BUTTONNUEVO, m_BNuevo);
}

BEGIN_MESSAGE_MAP(CFormUser, CFormView)
	ON_BN_CLICKED(IDC_BUTTONNUEVO, OnBnClickedButtonnuevo)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTGRUPOS, OnNMDblclkListgrupos)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTUSUARIOS, OnNMDblclkListusuarios)
	ON_NOTIFY(NM_RCLICK, IDC_LISTUSUARIOS, OnNMRclickListusuarios)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LISTGRUPOS, OnNMRclickListgrupos)
	ON_COMMAND( 32000,OnVer)
	ON_COMMAND( 32001,OnBnClickedButtonnuevo)
	ON_COMMAND( 22000,OnEditar)
	ON_COMMAND( 22001,OnContrasena)
	ON_COMMAND( 22002,OnEliminar)
END_MESSAGE_MAP()


// CFormUser diagnostics

#ifdef _DEBUG
void CFormUser::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormUser::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CFormUser message handlers

void CFormUser::OnBnClickedButtonnuevo()
{
	int sel;
	sel= m_LCGrupos.GetNextItem(-1,LVIS_SELECTED);
	if( sel==-1)
		sel= m_nCurrentGroup;
	Editar(NULL, m_uaIDS.GetAt(sel));
}

void CFormUser::OnVer() 
{
	FillLista(m_LCGrupos.GetNextItem(-1, LVIS_SELECTED));
}

void CFormUser::OnEditar()
{
	int pos= m_LCUser.GetNextItem(-1, LVIS_SELECTED);
	if( pos!=-1) 
		Editar(m_pUsuarios.FindUsuario(m_LCUser.GetItemData(pos)));
}

void CFormUser::OnEliminar()
{
	int pos= m_LCUser.GetNextItem(-1, LVIS_SELECTED);
	if( pos!=-1) { // eliminar usuario
		CUsuario *info= m_pUsuarios.FindUsuario(m_LCUser.GetItemData(pos));
		if( info) {
			if( MessageBox("Esta seguro de eliminar a\r\n" + info->GetNombre(), NULL, MB_ICONQUESTION | MB_YESNO)==IDYES) {

				EliminarUsuario(info->GetNombre());
				CString temp;
				temp.Format("UPDATE shared SET szReservado=\"-1\" WHERE id=%d", info->GetID());
				m_pConector->Query(temp, 0, this);
				info->SetId(0);
				m_LCUser.DeleteItem(pos);
			}
		}
	}
}

void CFormUser::CambiarContrasena(CString usuario, CString newpasswd) {

	CString encriptado= CUsuario::GetCodificado(usuario);
	CString query;

	query.Format("SET PASSWORD FOR '%s'=PASSWORD('%s')", encriptado, newpasswd);
	m_pConector->Query(query, 0, this);
	m_pConector->Query("FLUSH PRIVILEGES", 0, this);
}

void CFormUser::EliminarUsuario(CString usuario) {
	
	CString temp;
	CString encriptado;
	// usuario encriptado en md5
	encriptado= CUsuario::GetCodificado(usuario);
	// borramos de la tabla de usuarios
	temp.Format( "DELETE FROM mysql.user WHERE User=\"%s\" OR User=\"%s\"", usuario, encriptado);
	m_pConector->Query( temp, 0, this);
	// borramos de la tabla de bases de datos al usuario
	temp.Format( "DELETE FROM mysql.db WHERE User=\"%s\" OR User=\"%s\"", usuario, encriptado);
	m_pConector->Query( temp, 0, this);
	m_pConector->Query("FLUSH PRIVILEGES", 0, this);
}

void CFormUser::AgregarUsuario(CUsuario *usuario, CString passwd) {

	CString metodo,realpasswd;
	for( int i=0; i<m_uaIDS.GetCount(); i++) {
		if( usuario->GetGrupo()==m_uaIDS.GetAt(i)) {
			metodo= m_saMetodos.GetAt(i);
			break;
		}
	}
	if( metodo.IsEmpty()) {
		for( int i=0; i<m_uaIDS.GetCount(); i++) {
			if( usuario->Pertenece(m_uaIDS.GetAt(i))) {
				metodo= m_saMetodos.GetAt(i);
				break;
			}
		}
	}
	if( metodo.IsEmpty())
		return;
	if( !passwd.IsEmpty()) // realizar grant con contrasena
		realpasswd.Format(" IDENTIFIED BY \"%s\" ", passwd);
	// o simplemente grant sencillo

	CString encriptado= usuario->GetCodificado();

	metodo.Replace("%0", encriptado);
	metodo.Replace("%1", realpasswd);
	metodo.Replace("%2", m_pConector->GetNombreDatabase());
	m_pConector->Query(metodo, 0, this);
}

void CFormUser::OnContrasena()
{
	CDlgPasswd dlg;
	CString temp, query;

	if( dlg.DoModal()==IDOK) {
		temp= dlg.m_szContrasena;
		int pos= m_LCUser.GetNextItem(-1, LVIS_SELECTED);
		if( pos!=-1) {
			CUsuario *info= m_pUsuarios.FindUsuario(m_LCUser.GetItemData(pos));
			CambiarContrasena(info->GetNombre(), temp);
		}
	}
}

void CFormUser::Editar(CUsuario *usuario, int grupo) {
	
	CDlgEditar dlg;
	dlg.m_pActual= usuario;
	CString temp,final;
	if( usuario) 
		grupo= usuario->GetGrupo();
	temp.Format("%d",grupo);
	final= NumerosToMarcas(m_saGrupos, m_uaIDS, temp);
	if( final==temp) {
		temp= usuario->GetGrupos(grupo);
		final= NumerosToMarcas(m_saGrupos, m_uaIDS, temp);
	}
	dlg.m_szGrupo= final;
	if( usuario)
		dlg.m_szContrasena="-1";
	dlg.m_saDescripciones= &m_saDescripciones;
	dlg.m_saNombres= &m_saGrupos;
	dlg.m_uaIDS= &m_uaIDS;

	if(dlg.DoModal()==IDOK) {

		CStringArray campos, valores;
		int posicion;

		if( usuario) { // actualizacion de datos
			
			if(dlg.m_szNombre!=usuario->GetNombre()) { // cambio el nombre
				EliminarUsuario(usuario->GetNombre());
				campos.Add("nombre");
				usuario->SetUser(dlg.m_szNombre);
				campos.Add("descripcion");
				usuario->SetDescripcion(dlg.m_szVendedor);
				campos.Add("szReservado");
				usuario->SetGrupo(dlg.m_dwGrupo);
				valores.Add(usuario->GetNombre());
				final = usuario->GetDescripcion();
				valores.Add(final);
				final.Format("%d", usuario->GetGrupo());
				valores.Add(final);
				final.Format("id=%d", usuario->GetID());
				m_pConector->Query(CQuery::CreateUpdate("shared", campos, valores, final), 0, this);
				// realizar un grant 
				AgregarUsuario(usuario, dlg.m_szContrasena);
				posicion= m_nCurrentGroup;
				m_nCurrentGroup=-1;
				FillLista(posicion);
				return;
			} else if( dlg.m_dwGrupo!= usuario->GetGrupo()) { // cambio algo

				usuario->SetGrupo(dlg.m_dwGrupo);
				campos.Add("descripcion");
				usuario->SetDescripcion(dlg.m_szVendedor);
				campos.Add("szReservado");
				final=usuario->GetDescripcion();
				valores.Add(final);
				final.Format("%d", usuario->GetGrupo());
				valores.Add(final);
				final.Format("id=%d", usuario->GetID());
				m_pConector->Query(CQuery::CreateUpdate("shared", campos, valores, final), 0, this);
				// realizar un grant simple
				AgregarUsuario(usuario, "");
				posicion= m_nCurrentGroup;
				m_nCurrentGroup=-1;
				FillLista(posicion);
			} else if(dlg.m_szVendedor!= usuario->GetDescripcion()) { // cambio solo el cac

				campos.Add("descripcion");
				usuario->SetDescripcion(dlg.m_szVendedor);
				final= usuario->GetDescripcion();
				valores.Add(final);
				final.Format("id=%d", usuario->GetID());
				// actualizacion simple de cac
				m_pConector->Query(CQuery::CreateUpdate("shared", campos, valores, final), 0, this);
				posicion= m_nCurrentGroup;
				m_nCurrentGroup=-1;
				FillLista(posicion);
			}
			if( dlg.m_szContrasena!="-1") { // cambio contrasena
				CambiarContrasena(usuario->GetNombre(), dlg.m_szContrasena);
			}
		} else { // nuevo usuario
			CUsuario nuevo;
			nuevo.SetUser(dlg.m_szNombre);
			nuevo.SetDescripcion(dlg.m_szVendedor);
			nuevo.SetGrupo(dlg.m_dwGrupo);
			// id
			campos.Add("");
			// nombre
			campos.Add(nuevo.GetNombre());
			// descripcion
			final= nuevo.GetDescripcion();
			campos.Add(final);
			// nReservado
			campos.Add("2");
			// szReservado
			final.Format("%d", nuevo.GetGrupo());
			campos.Add(final);
			m_pConector->Find(CQuery::CreateInsert("shared", campos), this, NULL);
			if( m_pConector->Find("SELECT LAST_INSERT_ID()", this, &valores)) {

				nuevo.SetId(atol(valores.GetAt(0)));
				// realizamos el grant
				AgregarUsuario(&nuevo, dlg.m_szContrasena);
				m_pUsuarios.AddUsuario(nuevo.GetNombre(), nuevo.GetID(), nuevo.GetGrupo());
				m_pUsuarios.FindUsuario(nuevo.GetID())->SetDescripcion(nuevo.GetDescripcion());
				posicion= m_nCurrentGroup;
				m_nCurrentGroup=-1;
				FillLista(posicion);
			}
		}
	}
}

LRESULT CFormUser::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch ( message) {

		case WM_MYSQL:
			{
				MySQLMessage	*message= (MySQLMessage *) lParam;
				if( wParam==QUERY_RESULT) {

					if(message->GetID()== 1) {
						CString nombre= message->GetAt(1);
						DWORD	id= atol(message->GetAt(0));
						m_pUsuarios.AddUsuario( nombre, id, atol(message->GetAt(2)));
						CUsuario *info= m_pUsuarios.FindUsuario( id);
						ASSERT(info);
						info->SetDescripcion(message->GetAt(3));
						// ya tengo todo en info
					}
				} else if( wParam==THREAD_END) {
					if( m_nCurrentGroup==-1) {
						m_LCUser.SetMessageLista("No hay Usuarios");
						FillLista(0);
					}
					// invalidar toda la seleccion
				}
			}
			return 0;
	}
	return CFormView::WindowProc(message, wParam, lParam);
}

void CFormUser::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage( WM_QUERYCONECTOR);
	GetDocument()->SetTitle("Usuarios");
	CUsuario::GetNombresGrupos( &m_saGrupos, &m_uaIDS, &m_saMetodos,&m_saDescripciones);
	m_LCGrupos.InsertColumn(0, "Grupos");
	for( int i=0; i<m_saGrupos.GetCount(); i++) 
		m_LCGrupos.InsertItem(i, m_saGrupos.GetAt(i));
	m_LCUser.InsertColumn(0, "Nombre", LVCFMT_LEFT, 100);
	m_LCUser.InsertColumn(1, "Grupos", LVCFMT_LEFT, 100);
	m_LCUser.SetMessageLista("Cargando Usuarios");
	m_LCUser.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_LCUser.SetOrdenable();
	m_LCGrupos.AutoSizeColumns();
	m_pConector->Query("SELECT id,nombre,szReservado,descripcion FROM shared WHERE nReservado=2 AND (szReservado!=\"-1\")", 1, this);
}

void CFormUser::AddLista(CUsuario *Usuario) {

	int pos= m_LCUser.InsertItem(m_LCUser.GetItemCount(), Usuario->GetNombre());
	
	CString temp,final;
	temp.Format("%d", Usuario->GetGrupo());
	final= NumerosToMarcas(m_saGrupos, m_uaIDS, temp);
	if( final==temp) {
		temp= Usuario->GetGrupos();
		final= NumerosToMarcas(m_saGrupos, m_uaIDS, temp);
	}
	m_LCUser.SetItemText(pos, 1, final);
	m_LCUser.SetItemData(pos, Usuario->GetID());
}

void CFormUser::FillLista(int grupo) {

	if( grupo==m_nCurrentGroup)
		return;
	m_nCurrentGroup=grupo;
	if( m_uaIDS.GetCount()<=m_nCurrentGroup)
		return;
	int id= m_uaIDS.GetAt(m_nCurrentGroup);
	CUsuario *info;
	CString temp;
	temp.Format("%d", id);
	bool primario=false;
	if( CUsuario::GetGrupos(id)==temp)
		primario= true;

	m_LCUser.DeleteAllItems();
	for( int i=0; i<m_pUsuarios.GetCount(); i++) {
		info= m_pUsuarios.GetAt(i);
		if( info->GetID() && info && (info->GetGrupo()==id || (primario && info->Pertenece(id)))) 
			AddLista(info);
	}
	m_LCUser.AutoSizeColumns();
}

void CFormUser::OnNMDblclkListgrupos(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	FillLista(pNMListView->iItem);
	*pResult = 0;
}


void CFormUser::OnNMDblclkListusuarios(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	OnEditar();
	*pResult = 0;
}

void CFormUser::OnNMRclickListusuarios(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	CNewMenu menu;

	menu.CreatePopupMenu();
	menu.InsertMenu(0, MF_BYPOSITION | MF_STRING, 22000, "&Editar");
	menu.InsertMenu(1, MF_BYPOSITION | MF_STRING, 22001, "&Cambiar Contraseña");
	menu.InsertMenu(2, MF_BYPOSITION | MF_STRING, 22002, "E&liminar");
	
	POINT point;
    GetCursorPos( &point );
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0;
}

void CFormUser::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if(::IsWindow(m_LCGrupos.GetSafeHwnd())) {
		CRect rect;
		m_LCGrupos.MoveWindow(5, 5, cx/4-5, cy-5);
		m_BNuevo.GetWindowRect(rect);
		m_BNuevo.MoveWindow(cx-rect.Width()-10, cy-rect.Height()-10,rect.Width(), rect.Height());
		m_LCUser.MoveWindow(cx/4+5, 5, 3*cx/4-5, cy-rect.Height()-20);
	}
}

void CFormUser::OnNMRclickListgrupos(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;

	CNewMenu menu;

	menu.CreatePopupMenu();
	
	menu.InsertMenu(0, MF_BYPOSITION | MF_STRING, 32000, "&Ver");
	menu.InsertMenu(1, MF_BYPOSITION | MF_STRING, 32001, "&Nuevo");
	
	POINT point;
    GetCursorPos( &point );
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0;
}

void CFormUser::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if(bActivate) {
		AfxGetMainWnd()->SendMessage(WM_USER+5000,3, (LPARAM) (const char *)"administrador\\usuarios.htm");
		AfxGetMainWnd()->SendMessage(WM_USER+5000,4, (LPARAM) (const char *)"administrador\\nuevo.htm");
	}
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
