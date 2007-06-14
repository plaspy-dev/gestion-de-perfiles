#pragma once


// CImportarDlg dialog

class CImportarDlg : public CDialog
{
	DECLARE_DYNAMIC(CImportarDlg)
	CConector	*m_pConector;
	int			m_nLineas;
	int			m_nActual;
	enum {
		query_tablas=1,
	};
	CMiscelaneosEx m_pTablas;
	CMiscelaneosEx m_pTablasAlias;
	CMiscelaneosEx m_pCampos;
	int				m_nPKZ;
public:
	CImportarDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportarDlg();

// Dialog Data
	enum { IDD = IDD_INDEXARDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTextProgressCtrl m_PCProgreso;
	LRESULT OnMySQLMessage(WPARAM wParam, LPARAM lParam);
	bool LoadXML();
	CString m_szArchivo;
	bool ParseFile();
	bool ParseCabeceras(CStdioFile *file);
	bool FindColumna(const CString &columna);
	bool ParseLinea(CStdioFile *file, const CString &old="", bool bPrimero=false);
	bool SetQuery(const CString &tabla, const CString &columna, const CString &valor);
	bool ParseQuerys(CString pkz);
	CString ParseValor(const CString &tabla, const CString &campo, const CString &valor);
	CString ParseTablaUsuario(const CString &campo, CString valor);
	CString ParseTablaEmpresa(const CString &campo, CString valor);
	CString ParseTablaEstudios(const CString &campo, CString valor);
	CString ParseTablaEstudiosFuera(const CString &campo, CString valor);
	CString GetArea(CString area);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
