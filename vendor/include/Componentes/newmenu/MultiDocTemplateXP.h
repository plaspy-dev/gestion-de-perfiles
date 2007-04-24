#pragma once
# include "newmenu.h"

class   __COMPONENTES_EXPORT__ CMultiDocTemplateXP: public CMultiDocTemplate
{
	DECLARE_DYNAMIC(CMultiDocTemplateXP)
public:
  CNewMenu m_NewMenuShared;

// Constructors
public:
  CMultiDocTemplateXP(UINT nIDResource, CRuntimeClass* pDocClass,
                       CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
  ~CMultiDocTemplateXP();
  CString	GetViewClassName();
};