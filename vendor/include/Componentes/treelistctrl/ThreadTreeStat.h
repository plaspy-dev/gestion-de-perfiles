#pragma once

# include "treelistctrl.h"
#include <afxmt.h> 
# include "hilos/ThreadAuto.h"


class __COMPONENTES_EXPORT__ CThreadTreeStat : public CThreadAuto
{
protected:
	virtual bool	Init();
	inline bool		IsKey(int columna, CUIntArray *keys);
public:
	CThreadTreeStat(CThreadTreeStat *old=NULL);
	~CThreadTreeStat(void);
	CTreeListCtrl   *m_pTreeNuevo;
};
