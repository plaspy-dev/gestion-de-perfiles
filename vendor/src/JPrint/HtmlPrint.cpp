# include "stdafx.h"
# include "JPrint.h"
#include "HtmlPrint.h"

CString CHtmlPrint::m_szFooterGlobal;
CString CHtmlPrint::m_szHeaderGlobal;

CHtmlPrint::CHtmlPrint(void)
{
	m_rMargenes= CRect(0,0,0,0);
}

CHtmlPrint::~CHtmlPrint(void)
{
}

void CHtmlPrint::SetMargins(int top, int bottom, int right, int left) {

	m_rMargenes.top= top;
	m_rMargenes.bottom= bottom;
	m_rMargenes.right= right;
	m_rMargenes.left= left;
}

void CHtmlPrint::Clean() {

	m_saCampos.RemoveAll();
	m_saValores.RemoveAll();
}

void CHtmlPrint::AddLista(const CString &id, CListCtrlXP *lista) {

	ASSERT(lista);
	if(!lista)
		return;
	for( int i=0;i< m_saCamposLista.GetCount(); i++)
		if(m_saCamposLista[i]==id) {
			m_paListas[i]= lista;
			return;
		}
	m_saCamposLista.Add(id);
	m_paListas.Add(lista);
}

CString &CHtmlPrint::operator [](const CString &campo) {

	int index=-1;
	for( int i=0;i< m_saCampos.GetCount(); i++)
		if(m_saCampos[i]==campo) {
			index=i;
			break;
		}
	if(index==-1) {

		index= m_saCampos.Add(campo);
		m_saValores.Add("");
	} 
	ASSERT(index!=-1);
	return m_saValores[index];
}

void CHtmlPrint::AddCampo(const CString &campo, const CString &valor) {

	int index=-1;
	for( int i=0;i< m_saCampos.GetCount(); i++)
		if(m_saCampos[i]==campo) {
			index=i;
			break;
		}
	if(index==-1) {

		index= m_saCampos.Add(campo);
		m_saValores.Add(valor);
	}  else {

		m_saValores[index]= valor;
		m_saCampos[index]=campo;
	}
}

void CHtmlPrint::SetHeader(const CString &header) {

	m_szHeader= header;
}

void CHtmlPrint::SetFooter(const CString &footer) {

	m_szFooter= footer;
}

void CHtmlPrint::OnDestroy() {

	CMyRegKey key(HKEY_CURRENT_USER, "software\\microsoft\\internet explorer");
	if(!m_szFooterGlobal.IsEmpty()) 
		key.WriteString("PageSetup", "footer", m_szFooterGlobal);
	if(!m_szHeaderGlobal.IsEmpty())
		key.WriteString("PageSetup", "header", m_szHeaderGlobal);
}

void CHtmlPrint::AddLista(CListCtrlXP *lista, const CString &nombre, IDispatch* pDis) {

	IHTMLTable *tabla;
	HRESULT hr;
	ASSERT(lista);

	hr = pDis->QueryInterface( IID_IHTMLTable, (void **)&tabla );
	tabla->deleteRow(0); // eliminamos el nombre de la tabla;

	// agregamos los encabezados
	IDispatch *pDisRow;
	IHTMLTableRow *row;

	// agregamos los encabezados.
	hr= tabla->insertRow(-1, &pDisRow); // agregamos la lista
	hr = pDisRow->QueryInterface( IID_IHTMLTableRow, (void **)&row);
	CHeaderCtrl *header= lista->GetHeaderCtrl();
	ASSERT(header);
	CString clase;
	CString cadena;
	
	clase= "header_" + nombre;
	BSTR bstrClase= clase.AllocSysString();	

	for(int j=0; j<lista->GetColumnCount(); j++) {

		IHTMLElement *celda;
		IHTMLTableCell *celdareal;

		row->insertCell(-1, &pDisRow);
		hr= pDisRow->QueryInterface(IID_IHTMLElement, (void **)&celda);
		hr= pDisRow->QueryInterface(IID_IHTMLTableCell, (void **)&celdareal);

		HDITEM item;
		char szCadena[MAX_PATH];
		item.cchTextMax= MAX_PATH;
		item.pszText= szCadena;
		item.mask=HDI_TEXT | HDI_FORMAT;
		header->GetItem(j, &item);
		cadena= szCadena;
		cadena= "<strong>" + cadena + "</strong>";
		BSTR texto=  cadena.AllocSysString();
		celda->put_innerHTML(texto);
		::SysFreeString(texto);
		celda->put_className(bstrClase);

		// establecemos la alineacion
		if(item.fmt&HDF_CENTER)
			celdareal->put_align(L"center");
		else if(item.fmt&HDF_RIGHT)
			celdareal->put_align(L"right");
		else celdareal->put_align(L"left");
	}

	::SysFreeString(bstrClase);
	clase= "body_" + nombre;
	bstrClase= clase.AllocSysString();

	// agregamos los elementos de la lista
	for(int i=0; i<lista->GetItemCount(); i++) {
	
		hr= tabla->insertRow(-1, &pDisRow); // agregamos la lista
		hr = pDisRow->QueryInterface( IID_IHTMLTableRow, (void **)&row);

		for(int j=0; j<lista->GetColumnCount(); j++) {
			
			IHTMLElement *celda;
			IHTMLTableCell *celdareal;

			row->insertCell(-1, &pDisRow);
			hr= pDisRow->QueryInterface(IID_IHTMLElement, (void **)&celda);
			hr= pDisRow->QueryInterface(IID_IHTMLTableCell, (void **)&celdareal);

			cadena= lista->GetItemText(i, j);
			if(cadena.IsEmpty())
				cadena="&nbsp;";
			BSTR texto= cadena.AllocSysString();
			celda->put_innerHTML(texto);
			celda->put_className(bstrClase);
			::SysFreeString(texto);

			// establecemos la alineacion
			HDITEM item;
			item.mask=HDI_FORMAT;
			header->GetItem(j, &item);
			if(item.fmt&HDF_CENTER)
				celdareal->put_align(L"center");
			else if(item.fmt&HDF_RIGHT)
				celdareal->put_align(L"right");
			else celdareal->put_align(L"left");
		}
	}
	::SysFreeString(bstrClase);
}

bool CHtmlPrint::PreparePrint(CWnd *parent,const CString &archivo, const CString &footer) {

	ASSERT(parent);
	HRESULT hr;

	if(!::IsWindow(m_EHTMLEdit)) {
		if(!m_EHTMLEdit.Create("Impresión", WS_CHILD|WS_BORDER|WS_VSCROLL, CRect(0,0,0,0), parent, 0x1245 )) {
			AfxMessageBox("No se puede inicializar el Internet Explorer");
			return false;
		}
		CMyRegKey key1(HKEY_CURRENT_USER, "software\\microsoft\\internet explorer");
		if(m_szFooterGlobal.IsEmpty())
			m_szFooterGlobal= key1.GetString("PageSetup", "footer");
		if(m_szHeaderGlobal.IsEmpty())
			m_szHeaderGlobal= key1.GetString("PageSetup", "header");
	} else {
		hr= m_EHTMLEdit.NewDocument();
		ASSERT(SUCCEEDED(hr));
	}

	::ATL::CComPtr<IHTMLDocument2> spHTMLDocument;

	m_EHTMLEdit.GetDHtmlDocument(&spHTMLDocument);
	CString estado;
	if (spHTMLDocument)
	{
		while(estado!="complete") {
			BSTR bEstado;
			spHTMLDocument->get_readyState(&bEstado);
			estado= bEstado;
			MSG msg;
			while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	::ATL::CComPtr<IWebBrowser2> m_pBrowserApp;
	LPUNKNOWN lpUnk = m_EHTMLEdit.GetControlUnknown();
	hr = lpUnk->QueryInterface(IID_IWebBrowser2, (void**) &m_pBrowserApp);
	if (!SUCCEEDED(hr))
	{
		ASSERT(false);
		return FALSE;
	}

	{
		COleVariant vEmpty;
		COleVariant vURL(archivo, VT_BSTR);
		hr= m_pBrowserApp->Navigate2(vURL, vEmpty, vEmpty, vEmpty, vEmpty);
		if(!SUCCEEDED(hr)) {
			ASSERT(false);
			return false;
		}
	}

	spHTMLDocument=NULL;
	m_EHTMLEdit.GetDHtmlDocument(&spHTMLDocument);
	if (spHTMLDocument)
	{
		estado="";
		while(estado!="complete") {
			BSTR bEstado;
			spHTMLDocument->get_readyState(&bEstado);
			estado= bEstado;
			MSG msg;
			while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	IHTMLDocument2 *ppDocument;
	VERIFY(m_EHTMLEdit.GetDHtmlDocument(&ppDocument));

	estado="";
	while(estado!="complete") {
		BSTR bEstado;
		ppDocument->get_readyState(&bEstado);
		estado= bEstado;
		MSG msg;
		while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	IHTMLElementCollection *ppColection;
	hr= ppDocument->get_all(&ppColection);
	ASSERT(SUCCEEDED(hr));

	long tamano=0;
	ppColection->get_length(&tamano);

	for(long i=0; i<tamano; i++) {

		COleVariant index(i);
		IDispatch* pDis;
		hr = ppColection->item( index, index, (IDispatch**) &pDis );
		ASSERT(SUCCEEDED(hr));

		IHTMLElement* pElem;
		hr = pDis->QueryInterface( IID_IHTMLElement, (void **)&pElem );

		BSTR tag,text;
		pElem->get_tagName(&tag);
		CString temp(tag);
		if(temp.CompareNoCase("table")==0) {
			
			COleVariant valor;
			pElem->getAttribute(L"id", 0, &valor);
			CString id;
			id= valor;
			for(int j=0; j<m_saCamposLista.GetCount(); j++) {
				if(!id.CompareNoCase(m_saCamposLista[j])) {
					AddLista((CListCtrlXP *)m_paListas[j], m_saCamposLista[j], pDis);
					// actualizamos el tamaño de los elementos del archivo
					ppColection->get_length(&tamano);
				}
			}
		} else if(temp.CompareNoCase("label")==0) {

			pElem->get_innerText(&text);
			CString valor(text);
			for(int j=0; j<m_saCampos.GetCount(); j++) {
				valor.Replace(m_saCampos[j], m_saValores[j]);
				if(valor.Find("$")==-1)
					break;
			}
			if(valor.IsEmpty())
				valor=" ";
			BSTR bFinal= valor.AllocSysString();
			pElem->put_innerText(bFinal);
			::SysFreeString(bFinal);
		}
	}

	CString temp;

	CMyRegKey key(HKEY_CURRENT_USER, "software\\microsoft\\internet explorer");
	key.WriteString("PageSetup", "footer", m_szFooter.IsEmpty() ? footer + "&b&d" : m_szFooter);
	key.WriteString("PageSetup", "header", m_szHeader.IsEmpty() ? m_szHeaderGlobal : m_szFooter);
	temp.Format("%g", m_rMargenes.bottom ? m_rMargenes.bottom/25.4 : AfxGetApp()->GetProfileInt("printer", "cy", 10)/25.4);
	key.WriteString("PageSetup", "margin_bottom", temp);
	temp.Format("%g", m_rMargenes.left ? m_rMargenes.left/25.4 : AfxGetApp()->GetProfileInt("printer", "x", 10)/25.4);
	key.WriteString("PageSetup", "margin_left", temp);
	temp.Format("%g", m_rMargenes.right ? m_rMargenes.right/25.4 : AfxGetApp()->GetProfileInt("printer", "cx", 10)/25.4);
	key.WriteString("PageSetup", "margin_right", temp);
	temp.Format("%g", m_rMargenes.top ? m_rMargenes.top/25.4 : AfxGetApp()->GetProfileInt("printer", "y", 10)/25.4);
	key.WriteString("PageSetup", "margin_top", temp);

	key.WriteString("Main", "Print_Background", "yes");

	spHTMLDocument=NULL;
	m_EHTMLEdit.GetDHtmlDocument(&spHTMLDocument);
	if (spHTMLDocument)
	{
		estado="";
		while(estado!="complete") {
			BSTR bEstado;
			spHTMLDocument->get_readyState(&bEstado);
			estado= bEstado;
			MSG msg;
			while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	return true;
}

bool CHtmlPrint::OnPrint(CWnd *parent,const CString &archivo, const CString &footer) {

	return CHtmlPrint::PreparePrint(parent, archivo, footer) && !FAILED(m_EHTMLEdit.PrintDocument());
}

bool CHtmlPrint::OnPrintPreview(CWnd *parent,const CString &archivo, const CString &footer) {

	return CHtmlPrint::PreparePrint(parent, archivo, footer) && !FAILED(m_EHTMLEdit.PrintPreview());
}
