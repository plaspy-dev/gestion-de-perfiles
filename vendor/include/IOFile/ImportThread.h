#pragma once
# include "import.h"

class CImportThread : public CThreadAuto
{
protected:
	virtual bool	Init();
public:
	CImportThread(void);
	~CImportThread(void);
	CWnd		*m_pParent;
	CImport		*m_pImport;
};
