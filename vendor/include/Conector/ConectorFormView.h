#pragma once
#include "afxext.h"
# include "realconector.h"
# include "usuario.h"
# include "usuario2.h"
# include "usuario3.h"

class __CONECTOR_EXPORT___ CConectorFormView : public CResizableFormView
{
protected:
	CConector	*m_pConector;
	CUsuario	*m_pUsuario;
	CUsuario2	*m_pUsuario2;
	CUsuario3	*m_pUsuario3;

	DECLARE_DYNAMIC(CConectorFormView)
public:
	CConectorFormView(LPCTSTR lpszTemplateName);
	CConectorFormView(UINT nIDTemplate);
	~CConectorFormView(void);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
