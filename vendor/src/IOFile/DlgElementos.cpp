// DlgElementos.cpp : implementation file
//

#include "stdafx.h"
#include "IOFile.h"
#include "DlgElementos.h"


// CDlgElementos dialog

IMPLEMENT_DYNAMIC(CDlgElementos, CDialog)
CDlgElementos::CDlgElementos(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgElementos::IDD, pParent)
{
	m_szValores= NULL;
	m_szDescripciones= NULL;
}

CDlgElementos::~CDlgElementos()
{
}

void CDlgElementos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_BOK);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
	DDX_Control(pDX, IDC_EDITELEMENTOS, m_EElementos);
	DDX_Control(pDX, IDC_LISTELEMENTO, m_LCElementos);
}


BEGIN_MESSAGE_MAP(CDlgElementos, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTELEMENTO, OnNMDblclkListelemento)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LISTELEMENTO, OnNMRclickListelemento)
	ON_NOTIFY(NM_CLICK, IDC_LISTELEMENTO, OnNMLClickListElemento)
	ON_COMMAND(1001, OnAgregar)
	ON_COMMAND(1002, OnEliminar)
	ON_COMMAND(1003, OnSelAll)
END_MESSAGE_MAP()


void CDlgElementos::CheckAll(CString valor) {

	CString temp;
	while(true) {
		temp= GetLeftString(valor, ",");
		valor= GetRightString(valor, ",");
		for(int i=0; i<m_LCElementos.GetItemCount(); i++) {
			if(m_LCElementos.Items[i][0]==temp) {
				m_LCElementos.SetCheck(i);
				break;
			}
		}
		if(temp==valor) 
			break;
	}
}
// CDlgElementos message handlers
bool CDlgElementos::Validate(CString data,CString &Final, CStringArray *valores, int &inicio, int &fin) {

	int final=0;
	int pos;
	bool found= false;
	int length= data.GetLength();
	CString elementos;
	CString uno;
	CStringArray szFinal;
	while( (pos=data.Find(","))!=-1) {

		found= false;
		inicio= final;
		fin= final+pos;
		uno= data.Left(pos);
		if( !IsValidData(uno, valores)) 
			return false;
		data.Delete(0, pos+1);
		final+= pos+1;
		for( int i=0; i<szFinal.GetCount(); i++)

			if( szFinal.GetAt(i)==uno) {

				found= true;
				break;
			}
		if( !found && !uno.IsEmpty())
			szFinal.Add(uno);
	}
	inicio= final;
	fin= length;
	if( !data.IsEmpty()) {
		if( !IsValidData(data, valores)) {
			return false;
		}
	}
	found= false;
	for( int i=0; i<szFinal.GetCount(); i++)

		if( szFinal.GetAt(i)==data) {

				found= true;
				break;
		}
	Final.Empty();
	if( !found && !data.IsEmpty())
		szFinal.Add(data);
	for( int i=0; i<szFinal.GetCount(); i++) {

		if( i)
			Final+= ",";
		Final+= szFinal.GetAt(i);
	}
	return true;
}

void CDlgElementos::OnOK()
{

	int inicio=0;
	int final=0;
	CString data;
	m_EElementos.GetWindowText(data);

	if( !Validate(data, m_szFinal, m_szValores, inicio, final)) {
		m_EElementos.SetSel(inicio, final);
		m_EElementos.SetFocus();
		return;
	}
	CDialog::OnOK();
}

bool CDlgElementos::IsValidData(CString data, CStringArray *valores) {

	if( !valores)
		return false;
	if( data.IsEmpty())
		return true;
	for( int i=0; i<valores->GetCount(); i++)

		if( valores->GetAt(i)==data)

			return true;
	return false;
}

BOOL CDlgElementos::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_LCElementos.InsertColumn(0, "Nombre",LVCFMT_LEFT, 200);
	m_LCElementos.SetOrdenable();
	if( m_szDescripciones)
		m_LCElementos.InsertColumn(1, "Descripcion",LVCFMT_LEFT, 190);
	if( m_szValores) {
		for( int i=0; i<m_szValores->GetCount(); i++) {
			m_LCElementos.InsertItem(i, m_szValores->GetAt(i));
			if( m_szDescripciones) 
				m_LCElementos.SetItemText(i, 1, m_szDescripciones->GetAt(i));
		}
	}
	m_EElementos.SetWindowText(m_szFinal);
	SetWindowText(m_szTitulo);
	m_LCElementos.AutoSizeColumns();
	m_LCElementos.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	CheckAll(m_szFinal);
	return TRUE;
}

bool CDlgElementos::Findtext(CString text, CString find, int &inicio, int &fin) {

	bool found= false;
	if( !text.IsEmpty()) {
		// tiene mas de 1
		// buscarlo al principio
		if( find==text) { // completo
			found=true;
			inicio=0;
			fin= find.GetLength();
		} else if( find+","== text.Left(find.GetLength()+1)) {
			inicio=0;
			fin= find.GetLength();
			found=true;
		} else // buscarlo al final
		if( ","+find==text.Right(find.GetLength()+1)) {
			fin= text.GetLength();
			inicio= fin-find.GetLength();
			found=true;
		} else if( (inicio= text.Find(","+find+","))!=-1) {
			inicio++;
			fin= inicio+find.GetLength();
			found=true;
		}
	}
	return found;
}

void CDlgElementos::Agregar(CString nuevo) {

	CString current;
	m_EElementos.GetWindowText(current);
	int inicio,fin;
	if( Findtext(current, nuevo, inicio, fin)) {
		m_EElementos.SetSel(inicio, fin);
		m_EElementos.SetFocus();
		return;
	}
	// buscarlo en cualquiersitio
	if( !current.IsEmpty())
		current+= ",";
	current+= nuevo;
	m_EElementos.SetWindowText(current);
}

void CDlgElementos::OnNMDblclkListelemento(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	m_LCElementos.SetCheck(pNMListView->iItem);
	Agregar(m_LCElementos.GetItemText( pNMListView->iItem, 0));
	*pResult = 0;
}

void CDlgElementos::OnNMRclickListelemento(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	CNewMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, 1001, "&Agregar");
	menu.AppendMenu(MF_STRING, 1002, "&Eliminar");
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, 1003, "&Seleccionar Todo");
	POINT point;
    GetCursorPos( &point );
	menu.TrackPopupMenu( TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0;
}

void CDlgElementos::OnAgregar() {

	int index=-1;
	while( (index=m_LCElementos.GetNextItem(index, LVIS_SELECTED))!=-1) {
		m_LCElementos.SetCheck(index);
		Agregar(m_LCElementos.GetItemText(index, 0));
	}
}

void CDlgElementos::OnEliminar() {

	CString data;
	m_EElementos.GetWindowText(data);
	int index=-1;
	int inicio,fin;

	while( (index=m_LCElementos.GetNextItem(index, LVIS_SELECTED))!=-1) {

		m_LCElementos.SetCheck(index, 0);
		if( Findtext(data, m_LCElementos.GetItemText(index, 0), inicio, fin))
			data.Delete(inicio, fin-inicio);
	}
	inicio=0;
	for( int i=0; i<data.GetLength(); i++) {
		if( data[i]==',') {
			if( inicio) {
				data.Delete(i);
				i--;
			}
			inicio++;
		} else inicio=0;
	}
	if( data==",")
		data.Empty();
	else if( data[0]==',')
		data.Delete(0);
	m_EElementos.SetWindowText(data);
}

void CDlgElementos::OnSelAll() {

	for( int i=0; i<m_LCElementos.GetItemCount(); i++) 
		m_LCElementos.SetItemState(i, LVIS_SELECTED,LVIS_SELECTED);
}

void CDlgElementos::OnNMLClickListElemento(NMHDR *pNMHDR, LRESULT *pResult) {

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 
		return;
	int index=pNMListView->iItem;
	if(m_LCElementos.GetCheck(index)) {
		m_LCElementos.SetCheck(index);
		Agregar(m_LCElementos.Items[index][0]);
	} else {
		OnEliminar();
	}
	*pResult=0;
}

