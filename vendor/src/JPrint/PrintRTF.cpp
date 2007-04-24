#include "StdAfx.h"
#include "PrintRTF.h"
# include "JPrint.h"

CPrintRTF::CPrintRTF()
{
	m_pRTF=NULL;
}

CPrintRTF::~CPrintRTF()
{
}

/////////////////////////////////////////////////////////////////////////////
// CPrintRTF printing
int CPrintRTF::GetPageCount(CDC *pDC) {
	
	ASSERT(m_pRTF);
	return 1;
}

void CPrintRTF::PrintBody(CDC* pDC, CPrintInfo* pInfo)
{
	CPrintTemplate::PrintBody(pDC, pInfo);
	
	m_rectBody.left= ::MulDiv(m_rectBody.left,1440, pDC->GetDeviceCaps(LOGPIXELSX));
	m_rectBody.top= ::MulDiv(m_rectBody.top,1440, pDC->GetDeviceCaps(LOGPIXELSY));
	m_rectBody.right= ::MulDiv(m_rectBody.right,1440, pDC->GetDeviceCaps(LOGPIXELSX));
	m_rectBody.bottom= ::MulDiv(m_rectBody.bottom,1440, pDC->GetDeviceCaps(LOGPIXELSY));

	FORMATRANGE fr;
	
	fr.hdc= *pDC;
	fr.hdcTarget= *pDC;
	fr.rc= m_rectBody;
	fr.rcPage= m_rectBody;
	fr.chrg.cpMin=0;
	fr.chrg.cpMax=-1;

	m_pRTF->FormatRange(&fr, true);

	m_pRTF->DisplayBand(m_rectBody);
}

void CPrintRTF::SetRTF(COleRichEditCtrl *rtf) {

	m_pRTF= rtf;
}
