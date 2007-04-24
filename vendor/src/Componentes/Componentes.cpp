// Componentes.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
# include "Componentes.h"
#include "nbMessageBox.h"
#include "resourcedll.h"
# include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE ComponentesDLL = { NULL, NULL };

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CComponentesApp

BEGIN_MESSAGE_MAP(CComponentesApp, CWinApp)
END_MESSAGE_MAP()


// CComponentesApp construction

CComponentesApp::CComponentesApp()
{
}


// The one and only CComponentesApp object

CComponentesApp theApp;


// CComponentesApp initialization

BOOL CComponentesApp::InitInstance()
{
	CWinApp::InitInstance();
	if (!AfxInitExtensionModule(ComponentesDLL, m_hInstance))
		return false;
	new CDynLinkLibrary(ComponentesDLL);
#ifndef _DEBUG
	if(!RegisterFile(CApplicacion::GetAppPath("componentes.dll"))) {

		CString texto;
		texto.Format("%s Esta corrupto.\nPor favor ReInstale %s o Informe al Administrador del sistema.", CApplicacion::GetAppName(), CApplicacion::GetAppName());
		AfxMessageBox(texto, MB_ICONERROR);
		return false;
	}
#endif
	return TRUE;
}

int MyMessageBox(CString texto, DWORD tipo, CString registro) {

	int respuesta=0;
	CnbMessageBox box;
	if( registro.IsEmpty()) {
		respuesta= AfxMessageBox(texto,tipo);
	} else {
		respuesta= AfxGetApp()->GetProfileInt("MessageBox", registro, 0);
		if( !respuesta) {
			respuesta= box.MessageBox(texto, tipo);
			if(box.GetChecked()) 
				AfxGetApp()->WriteProfileInt("MessageBox",registro, respuesta);
		}
	}
	return respuesta;
}

void ValidateScroll(CWnd *componente) {

	ASSERT(componente);

	if( ::IsWindowVisible(componente->GetSafeHwnd())) {
		CWnd *parent= componente->GetParent();
		ASSERT(parent);

		CRect rectparent, rect;
		componente->GetWindowRect(rect);

		if(parent && parent->IsKindOf(RUNTIME_CLASS(CListCtrlXP))) {
			int col=0, row=0;
			((CListCtrlXP *)parent)->GetCurrentPos(col, row);
			((CListCtrlXP *)parent)->GetSubItemRect(row, col, LVIR_LABEL, rect);
			parent->ClientToScreen(rect);
			parent= parent->GetParent();
			ASSERT(parent);
		}
		parent->GetWindowRect(rectparent);
		int delta;
		if( rect.top<rectparent.top) {
			delta= rectparent.top-rect.top+rect.Height();
			if(parent->GetScrollPos(SB_VERT)-delta<0)
				delta= parent->GetScrollPos(SB_VERT);
			parent->ScrollWindow(0,delta);
			parent->SetScrollPos(SB_VERT, parent->GetScrollPos(SB_VERT)-delta);
		}
		if( rect.bottom>rectparent.bottom) {
			delta= rectparent.bottom-rect.bottom-rect.Height();
			if(parent->GetScrollPos(SB_VERT)-delta<0) 
				delta= parent->GetScrollPos(SB_VERT);
			parent->ScrollWindow(0,delta);
			parent->SetScrollPos(SB_VERT, parent->GetScrollPos(SB_VERT)-delta);
		} 
		if( rect.left<rectparent.left) {
			delta= rectparent.left-rect.left;
			if(parent->GetScrollPos(SB_HORZ)-delta<0) 
				delta= parent->GetScrollPos(SB_HORZ);
			parent->ScrollWindow(delta, 0);
			parent->SetScrollPos(SB_HORZ, parent->GetScrollPos(SB_HORZ)-delta);
		}
		if( rect.right>rectparent.right) {
			delta= rectparent.right-rect.right;
			if(parent->GetScrollPos(SB_HORZ)-delta<0) 
				delta= parent->GetScrollPos(SB_HORZ);
			parent->ScrollWindow(delta, 0);
			parent->SetScrollPos(SB_HORZ, parent->GetScrollPos(SB_HORZ)-delta);
		}
	}
}

ULONG GetMaxLen( CString cadena) {

	FUNCTION_STACK_TRACE("FFFFE029");
	ULONG max=0, otro=0;
	CString temp;
	char a;
	
	if( cadena.IsEmpty())

		return 0;
	for( int i=0; i<cadena.GetLength(); i++) {

		a= cadena.GetAt(i);
		if( a>='0' && a<='9') // numero
			temp+= a;
		else if( a==',' || a=='-') {
			otro= atol(temp);
			if( otro>max)
				max=otro;
			temp.Empty();
		} else {
			ASSERT(false);
			return 0;
		}
	}
	if( !temp.IsEmpty()) {

		otro=atol(temp);
		if( otro>max)
			max=otro;
	}
	return max;
}

bool IsValid( CString cadena) {

	FUNCTION_STACK_TRACE("FFFFE02A");
	char a;
	char tipo=0;
	
	if( cadena.IsEmpty())

		return false;
	for( int i=0; i<cadena.GetLength(); i++) {

		a= cadena.GetAt(i);
		if( a>='0' && a<='9') { // numero
			tipo=1;
		} 
		else if( a==',' || a=='-') {
			if( tipo)
				tipo=0;
			else return false;
		} else return false;
	}
	return tipo==1;
}

bool IsValid( CString cadena, CString valor) {

	FUNCTION_STACK_TRACE("FFFFE02B");
	CUIntArray posibles;

	ULONG otro=0;
	CString temp;
	char a;
	
	if( cadena.IsEmpty())

		return false;
	for( int i=0; i<cadena.GetLength(); i++) {

		a= cadena.GetAt(i);
		if( a>='0' && a<='9') // numero
			temp+= a;
		else if( a==',') {
			if( otro) {
				ULONG nose= atol(temp);
				for( UINT h=otro; h<nose; h++)
					posibles.Add( h);
			}
			posibles.Add(atol(temp));
			temp.Empty();
			otro=0;
		} else if(a=='-'){
			otro= atol(temp);
			posibles.Add(atol(temp));
			temp.Empty();
		} else {
			ASSERT(false);
			return false;
		}
	}
	if( !temp.IsEmpty()) {
		if( otro) {
			ULONG nose= atol(temp);
			for( UINT h=otro; h<nose; h++)
				posibles.Add( h);
		}
		posibles.Add(atol(temp));
	}
	otro= valor.GetLength();
	for( int i=0; i<posibles.GetCount(); i++)

		if( otro== posibles.GetAt(i))
			return true;
	return false;
}

int SetErrorEx( CWnd *elemento, CString error, DWORD tipo, CString registro) {

	if( !registro.IsEmpty())
		MyMessageBox(error, tipo==3 ? MB_ICONERROR : MB_OK, registro);
	else if(tipo)
		AfxMessageBox(error, tipo==3 ? MB_ICONERROR : MB_OK);
	SetError(elemento, error, tipo);
	return 0;
}

void SetError( CWnd *elemento, CString error, DWORD tipo) {

	if( elemento) {
		if( elemento->IsKindOf(RUNTIME_CLASS(CEdit)))
			((CEdit *)elemento)->SetSel(0, ((CEdit *)elemento)->GetWindowTextLength());
		elemento->SetFocus();
		if(tipo) {			
			if(elemento->IsKindOf(RUNTIME_CLASS(CEditXP))) {
				if(AfxGetApp()->GetProfileInt("componentes","tooltip", 1)) 
					((CEditXP *)elemento)->SetToolTip(error, CApplicacion::GetAppName());
			} else if(elemento->IsKindOf(RUNTIME_CLASS(CComboBoxXP))) {
				if(AfxGetApp()->GetProfileInt("componentes","tooltip", 1)) 
					((CComboBoxXP *)elemento)->GetEdit()->SetToolTip(error, CApplicacion::GetAppName());
			}
		}
	}
	CWnd *handle= AfxGetMainWnd();
	if(handle)
		handle->SendMessage( WM_SETSTATUS, tipo, (LPARAM) (const char *)error);
}

void RecalculateScroll(CFormView *parent, int delta/*=10*/) {

	ASSERT(parent);
	CWnd* pChild = parent->GetWindow(GW_CHILD);
	CRect otro;
	int ancho=0;
	int alto=0;
	int minimo=50000;
	int izquierda=50000;

	while (pChild) {

		pChild->GetWindowRect(otro);
		parent->ScreenToClient(&otro);
	
		if(minimo>otro.top)
			minimo= otro.top;
		if(izquierda> otro.top)
			izquierda= otro.left;
		if(ancho<otro.right)
			ancho= otro.right;
		if(alto<otro.bottom)
			alto= otro.bottom;
		pChild = pChild->GetWindow(GW_HWNDNEXT);
	}
	parent->SetScrollSizes(MM_TEXT, CSize(abs(izquierda)+ancho+delta,abs(minimo)+alto+delta));
}

CResourceDLL::CResourceDLL()
{
	m_hOldInst = AfxGetResourceHandle ();
	ASSERT (m_hOldInst != NULL);

	AfxSetResourceHandle( ComponentesDLL.hResource );
}

CResourceDLL::~CResourceDLL()
{
	AfxSetResourceHandle( m_hOldInst );
}

bool IsWindowsNT() {

	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&osv))
	{
		switch(osv.dwPlatformId)
		{
		case VER_PLATFORM_WIN32s:
		case VER_PLATFORM_WIN32_WINDOWS:
			return false;
		case VER_PLATFORM_WIN32_NT:
		default:
			return true;
		}
	}
	return false;
}

CString GetRightString(CString cadena, CString separador) {

	int pos= cadena.Find(separador);
	if( pos==-1)
		return cadena;
	return cadena.Mid(pos+separador.GetLength(), cadena.GetLength()-pos).Trim(" \t");
}

CString GetLeftString( CString cadena, CString separador) {

	int pos= cadena.Find(separador);
	if( pos==-1)
		return cadena;
	return cadena.Left(pos);
}

void AboutBox(const CString &version, HICON icono,const CString &producto/*=""*/) {

	if(!icono)
		icono= AfxGetApp()->LoadIcon(128);
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAboutDlg dlg(version,icono, producto);
	dlg.DoModal();
}
