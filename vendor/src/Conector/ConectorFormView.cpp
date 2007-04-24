#include "StdAfx.h"
#include "ConectorFormView.h"

IMPLEMENT_DYNAMIC(CConectorFormView, CResizableFormView)

CConectorFormView::CConectorFormView(LPCTSTR lpszTemplateName) : CResizableFormView(lpszTemplateName)
{
	m_pConector=NULL;
	m_pUsuario=NULL;
	m_pUsuario2= NULL;
	m_pUsuario3= NULL;
}

CConectorFormView::CConectorFormView(UINT nIDTemplate) : CResizableFormView(nIDTemplate)
{
}

CConectorFormView::~CConectorFormView(void)
{
}

void CConectorFormView::OnInitialUpdate()
{
	CResizableFormView::OnInitialUpdate();
	m_pConector=(CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	m_pUsuario=(CUsuario *)AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO);
	m_pUsuario2=(CUsuario2 *) m_pUsuario;
	m_pUsuario3=(CUsuario3 *) m_pUsuario;
}

BOOL CConectorFormView::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN || pMsg->message==WM_KEYUP) {
		if( pMsg->wParam== VK_RETURN) {

			CWnd *foco= GetFocus();
			if( foco) {
				if( !foco->IsKindOf( RUNTIME_CLASS(CButtonXP))) 
					pMsg->wParam= VK_TAB;
				else pMsg->wParam=VK_SPACE;
			}
		} else if(pMsg->wParam==VK_ESCAPE) 
			return TRUE;
	}
	return CResizableFormView::PreTranslateMessage(pMsg);
}
