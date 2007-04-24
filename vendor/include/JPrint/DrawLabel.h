# pragma once
///////////////////////////////////////////////////////////
// File :		DrawLabel.h
// Created :	07/27/04
//

#include "DrawObject.h"

class CDrawLabel : public CDrawObject
{
public:
// Construction/destruction
	CDrawLabel::CDrawLabel();

// Operations
	virtual void Draw( int page, CDC* dc );
	static CDrawObject* FromString( const CString & str );
	void SetText( const CString& text );
	const CString& GetText() const;

private:
	void ReplaceFields( int page, CString & str ) const;
};
