#pragma once
# include "conector/conector.h"

class  __IOFILE_EXPORT__ CDefender
{
protected:
	CDiskID		m_pDiskID;
	CString		m_szSerial;
	unsigned int m_nID;
	CString		m_szRealSerial;
private:
	void GetSerial();
public:
	CDefender();
	CDefender(CString serial);
	~CDefender(void);
	CDefender *IsValidSerial();
	CString GetSerialNumber();
	CString GetKernel();
	void SetID(unsigned int id);
	CDiskID *GetDiskID();
};
