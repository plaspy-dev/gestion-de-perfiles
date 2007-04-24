# pragma once
///////////////////////////////////////////////////////////
// File :		DrawLine.h
// Created :	07/27/04
//

#include "DrawObject.h"

class CDrawLine : public CDrawObject
{
public:
// Construction/destruction

	CDrawLine::CDrawLine();

// Operations
	virtual void Draw( int page, CDC* dc );
	static CDrawObject* FromString( const CString & str );

// Attributes

};

