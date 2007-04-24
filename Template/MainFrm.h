// MainFrm.h: interfaz de la clase CMainFrame
//


#pragma once

#include "conector/conector.h"


class CMainFrame : public CMDIFrameWndXP
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
private:

	// varios
	bool	Messages( MySQLMessage *contenido);
	bool	DeleteHilos( MySQLMessage *hilo);
	bool	LoadDocumentos();
protected:
	CToolBarXP		m_wndToolBar;
// Atributos
public:

// Operaciones
public:

// Reemplazos
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementación
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	bool			Conectar();
	CUsuario		*m_pUsuario;
	void			ResetToolbars();
	CConector		m_pConector;

	virtual void	SaveState();
	virtual void	LoadState();


// Funciones de asignación de mensajes generadas
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateVerNavegador(CCmdUI *pCmdUI);
	afx_msg void OnConfiguracionServidores();
	afx_msg void OnVerBarradenavegacion();
	afx_msg void OnUpdateVerBarradenavegacion(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateFileNew(CCmdUI *pCmdUI);
	afx_msg LRESULT OnQueryConector(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQueryUsuario(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMySQL(WPARAM wParam, LPARAM lParam);
	int OnMySQLThreadEnd(MySQLMessage *hilo);
	int OnMySQLQueryResult(MySQLMessage *message);
	int OnQueryEmpty(int owner);
	int OnQueryTitle(MySQLMessage *message);
};


