#pragma once
#include <conector/Conector.h>

class CArrayUsuarios : public CUsuario
{
	CArrayUsuarios *m_pSig;
public:
	CArrayUsuarios(void);
	~CArrayUsuarios(void);
	void AddUsuario( CString &nombre, DWORD id, DWORD grupo);
	CUsuario *FindUsuario( CString nombre);
	CUsuario *FindUsuario( DWORD id);
	void	DeleteAll();
	int		GetCount();
	CUsuario *GetAt(int index);
};
