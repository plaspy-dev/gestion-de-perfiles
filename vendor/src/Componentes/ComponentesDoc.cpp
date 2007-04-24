// SVEDoc.cpp: implementación de la clase CComponentesDoc
//

#include "stdafx.h"
# include "Componentes.h"
# include "ComponentesDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CComponentesDoc

IMPLEMENT_DYNCREATE(CComponentesDoc, CDocument)

BEGIN_MESSAGE_MAP(CComponentesDoc, CDocument)
END_MESSAGE_MAP()


// Construcción o destrucción de CComponentesDoc

CComponentesDoc::CComponentesDoc()
{
	// TODO: agregar aquí el código de construcción único

}

CComponentesDoc::~CComponentesDoc()
{
}

BOOL CComponentesDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: agregar aquí código de reinicio
	// (los documentos SDI volverán a utilizar este documento)

	return TRUE;
}




// Serialización de CComponentesDoc

void CComponentesDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}


// Diagnósticos de CComponentesDoc

#ifdef _DEBUG
void CComponentesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CComponentesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Comandos de CComponentesDoc

BOOL CComponentesDoc::SaveModified()
{
	CWaitCursor wait;
	AfxGetMainWnd()->SendMessage( WM_SETSTATUS, 0, (LPARAM) (const char *)"Terminando hilos en ejecucion");
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView *pView= GetNextView(pos);
		if( pView)
			if( pView->SendMessage(WM_CLOSEEX)) {
				AfxGetMainWnd()->SendMessage( WM_SETSTATUS, 0, (LPARAM) (const char *)"Cancelado");
				return false;
			}
	}
	AfxGetMainWnd()->SendMessage( WM_SETSTATUS, 0, (LPARAM) (const char *)"Listo");
	return true;
}
