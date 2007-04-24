#pragma once

class CResourceDLL  
{
public:
	CResourceDLL();
	virtual ~CResourceDLL();

protected:
	HINSTANCE m_hOldInst;
};

