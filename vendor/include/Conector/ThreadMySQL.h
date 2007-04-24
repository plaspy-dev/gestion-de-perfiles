#pragma once

# include "ArrayDB.h"

class CConector;

class CThreadMySQL : public CThreadAuto
{
private:

	bool			Init();
	CArrayDB		*m_pArrayDB;
	virtual bool	Kill(bool wait=false);
public:
	CThreadMySQL(CThreadMySQL *threadOld=NULL);
	~CThreadMySQL();
	CArrayDB		*GetArrayDB() const;
	void			SetArrayDB( CArrayDB *ArrayDB);
	int				m_nReintentos;
	int				m_nType;
	CConector		*m_pConector;
	int				m_nID;
	CString			m_szQuery;
	CWnd*			m_pParent;
	CWnd*			m_pMainWnd;
	int				GetState();
};
