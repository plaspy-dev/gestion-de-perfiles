#include "StdAfx.h"
#include "newmenu/multidoctemplatexp.h"

IMPLEMENT_DYNAMIC(CMultiDocTemplateXP,CMultiDocTemplate)

CMultiDocTemplateXP::CMultiDocTemplateXP(UINT nIDResource, 
                                           CRuntimeClass* pDocClass,
                                           CRuntimeClass* pFrameClass,
                                           CRuntimeClass* pViewClass)
: CMultiDocTemplate(nIDResource,pDocClass,pFrameClass,pViewClass)
{
  if (m_nIDResource != 0 && m_hMenuShared)
  {
    // Attach the menu.
    m_NewMenuShared.LoadMenu(m_hMenuShared);
    // Try to load icons to the toolbar.
    m_NewMenuShared.LoadToolBar(m_nIDResource);
  }
}

CString CMultiDocTemplateXP::GetViewClassName() {
    
	ASSERT(m_pViewClass);
	return m_pViewClass->m_lpszClassName;
}

CMultiDocTemplateXP::~CMultiDocTemplateXP()
{
  // Let only the CNewMenu to destroy the menu.
  if(m_hMenuShared==m_NewMenuShared.m_hMenu)
  {
    m_hMenuShared = NULL;
  }
}