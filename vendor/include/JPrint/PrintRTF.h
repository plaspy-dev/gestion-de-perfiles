#pragma once
# include <iofile/IOFile.h>
# include "PrintTemplate.h"

class __JPRINT_EXPORT__ CPrintRTF : public CPrintTemplate
{
	// Construction
public:
	CPrintRTF();
	virtual ~CPrintRTF();

	// Operations
public:
	void SetRTF(COleRichEditCtrl *rtf);
	// Implementation
protected:
	virtual void PrintBody(CDC* pDC, CPrintInfo* pInfo);
	virtual int GetPageCount(CDC *pDC);
protected:
	COleRichEditCtrl *m_pRTF;
};