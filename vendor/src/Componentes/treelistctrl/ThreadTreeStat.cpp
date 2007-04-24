#include "StdAfx.h"
# include "Componentes.h"
#include "treelistctrl/threadtreestat.h"
# include "EditFloat.h"
# include "treelistctrl/treelistitem.h"

CThreadTreeStat::CThreadTreeStat(CThreadTreeStat *old/*=NULL*/) : CThreadAuto(old)
{
	m_pTreeNuevo= NULL;
}

CThreadTreeStat::~CThreadTreeStat(void) {

}

bool CThreadTreeStat::IsKey(int columna, CUIntArray *keys) {

	for( int i=0; i<keys->GetCount(); i++)
		if( columna==keys->GetAt(i))
			return true;
	return false;
}


bool CThreadTreeStat::Init() {
	
	ASSERT(m_pTreeNuevo);

	int stat= AfxGetApp()->GetProfileInt("arbol", "stat", 0);

	HTREELISTITEM item= m_pTreeNuevo->GetRootItem();
	HTREELISTITEM subitem;
	int count;
	CString cadena;
	int columnas= m_pTreeNuevo->GetColumnCount();
	double *totales = new double[columnas];
	int *tipos= new int[columnas];
	CString *comun= new CString[columnas];
	double temporal=0;
	CRect rect;
	CUIntArray *keys= m_pTreeNuevo->GetKeys();
	ASSERT(keys);

	for( int i=0; i<columnas; i++) {
		tipos[i]= m_pTreeNuevo->GetColumnType(i);
		if( IsKey(i, keys))
			tipos[i]=0;
	}

	while( item && IsRunning()) {
		
		subitem= m_pTreeNuevo->GetChildItem(item);
		if( subitem) {
			count=0;
			for( int i=0; i<columnas; i++) {
				if(!IsRunning())
					break;
				totales[i]= stat==5 ? -1 : 0;
				comun[i].Empty();
			}
			if(!IsRunning())
				break;
			if( !m_pTreeNuevo || !::IsWindow(*m_pTreeNuevo))
				break;
			comun[0]= GetLeftString( m_pTreeNuevo->GetItemText(item), " (");
			while( subitem && IsRunning()) {
				if( !m_pTreeNuevo || !::IsWindow(*m_pTreeNuevo))
					break;
				count++;
				for( int i=1; i<columnas; i++) {
					if(!IsRunning())
						break;
					if( tipos[i]==1 || tipos[i]==2) {
						temporal= atof(tipos[i]==2 ? CEditFloat::GetNormal(m_pTreeNuevo->GetItemText(subitem, i)) : m_pTreeNuevo->GetItemText(subitem,i));
						switch(stat) {
							case 2: // suma
							case 3: // promedio
								totales[i]+=temporal;
								break;
							case 4: // max
								if( temporal>totales[i])
									totales[i]= temporal;
								break;
							case 5: // min
								if( temporal<totales[i] || totales[i]==-1)
									totales[i]= temporal;
								break;
							default:
								if( totales[i]==0 || totales[i]==-1) {
									if( comun[i].IsEmpty())
										comun[i]= m_pTreeNuevo->GetItemText(subitem,i);
									else if( comun[i]!=m_pTreeNuevo->GetItemText(subitem,i))
										totales[i]=1;
								}
								break;
						}
					} else if( totales[i]==0 || totales[i]==-1) {
						if( comun[i].IsEmpty())
							comun[i]= m_pTreeNuevo->GetItemText(subitem,i);
						else if( comun[i]!=m_pTreeNuevo->GetItemText(subitem,i))
							totales[i]=1;
					}
				}
				if(!IsRunning())
					break;
				subitem= m_pTreeNuevo->GetNextSiblingItem(subitem);
			}
			if( !m_pTreeNuevo || !::IsWindow(*m_pTreeNuevo))
				break;
			for( int i=0; i<columnas; i++) {
				
				if(!IsRunning())
					break;
				if( (tipos[i]==1 || tipos[i]==2) && stat && stat!=1) {
					if( count) {
						if( stat==5 && totales[i]==-1)
							continue;
						comun[i]= CEditFloat::GetFormatNumber(stat==3 ? (double) totales[i]/count : totales[i]);
					}
				} else if( totales[i]!=0 && totales[i]!=-1) {
					comun[i].Empty();
				}
			}
			if(!IsRunning())
				break;
			if( stat) {
				cadena.Format(" (%d Elemento%s)", count, count>1 ? "s" : "");
				comun[0]+= cadena;
			}
			if( m_pTreeNuevo && ::IsWindow(*m_pTreeNuevo))
				for( int i=0; i<columnas; i++) {
					if(!IsRunning())
						break;
					m_pTreeNuevo->SetItemText(item, comun[i], i);
				}
			else break;
		}
		if(!IsRunning())
			break;
		item= m_pTreeNuevo->GetNextSiblingItem(item);
	}
	delete [] totales;
	delete [] tipos;
	delete [] comun;
	CThreadAuto::Init();
	return true;
}
