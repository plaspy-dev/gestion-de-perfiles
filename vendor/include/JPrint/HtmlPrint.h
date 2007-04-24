#pragma once

class __JPRINT_EXPORT__ CHtmlPrint
{
	CStringArray		m_saCampos;
	CStringArray		m_saValores;
	CStringArray		m_saCamposLista;
	CPtrArray			m_paListas;
	CHtmlEditCtrl		m_EHTMLEdit;
	bool				PreparePrint(CWnd *parent,const CString &archivo, const CString &footer);
	static				CString m_szFooterGlobal;
	static				CString m_szHeaderGlobal;
	CRect				m_rMargenes;
	CString				m_szHeader;
	CString				m_szFooter;
	void				AddLista(CListCtrlXP *lista, const CString &nombre, IDispatch* pDis);
public:
	CHtmlPrint(void);
	~CHtmlPrint(void);
	void Clean();
	CString &operator [](const CString &campo);
	void AddLista(const CString &id, CListCtrlXP *lista);
	void AddCampo(const CString &campo, const CString &valor);
	void SetMargins(int top, int bottom, int right, int left);
	void SetHeader(const CString &header);
	void SetFooter(const CString &footer);
	// footer -> normalmente el nombre del usuario.
	bool OnPrint(CWnd *parent,const CString &archivo, const CString &footer="");
	bool OnPrintPreview(CWnd *parent,const CString &archivo, const CString &footer="");
	static void OnDestroy();
};
