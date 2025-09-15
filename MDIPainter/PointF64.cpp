#include "pch.h"
#include "PointF64.h"

CPointF64::CPointF64()
{
	m_f64X = 0;
	m_f64Y = 0;
	m_i32LineThickness = 1;
	m_sLineColor = RGB(0, 0, 0);
}

CPointF64::CPointF64(double f64X, double f64Y)
{
	m_f64X = f64X;
	m_f64Y = f64Y;
	m_i32LineThickness = 1;
	m_sLineColor = RGB(0, 0, 0);
}

CPointF64::CPointF64(double f64X, double f64Y, int32_t i32LineThickness)
{
	m_f64X = f64X;
	m_f64Y = f64Y;
	m_i32LineThickness = i32LineThickness;
	m_sLineColor = RGB(0, 0, 0);
}

CPointF64::CPointF64(double f64X, double f64Y, int32_t i32LineThickness, COLORREF sLineColor)
{
	m_f64X = f64X;
	m_f64Y = f64Y;
	m_i32LineThickness = i32LineThickness;
	m_sLineColor = sLineColor;
}

CPointF64::CPointF64(const CPointF64& ptdPosition)
{
	m_f64X = ptdPosition.m_f64X;
	m_f64Y = ptdPosition.m_f64Y;
	m_i32LineThickness = ptdPosition.m_i32LineThickness;
	m_sLineColor = ptdPosition.m_sLineColor;
}

CPointF64::CPointF64(const CPoint& ptPosition)
{
	m_f64X = (double)ptPosition.x;
	m_f64Y = (double)ptPosition.y;
	m_i32LineThickness = 1;
	m_sLineColor = RGB(0, 0, 0);
}

CPointF64::CPointF64(const CPoint& ptPosition, int32_t i32LineThickness)
{
	m_f64X = (double)ptPosition.x;
	m_f64Y = (double)ptPosition.y;
	m_i32LineThickness = i32LineThickness;
	m_sLineColor = RGB(0, 0, 0);
}

CPointF64::CPointF64(const CPoint& ptPosition, int32_t i32LineThickness, COLORREF sLineColor)
{
	m_f64X = (double)ptPosition.x;
	m_f64Y = (double)ptPosition.y;
	m_i32LineThickness = i32LineThickness;
	m_sLineColor = sLineColor;
}

CPointF64::~CPointF64()
{

}

const CPointF64 CPointF64::operator+(const CPointF64& ptdPosition) const
{
	return CPointF64(this->m_f64X + ptdPosition.m_f64X, this->m_f64Y + ptdPosition.m_f64Y, this->m_i32LineThickness, this->m_sLineColor);
}

const CPointF64 CPointF64::operator-(const CPointF64& ptdPosition) const
{
	return CPointF64(this->m_f64X - ptdPosition.m_f64X, this->m_f64Y - ptdPosition.m_f64Y, this->m_i32LineThickness, this->m_sLineColor);
}

const CPointF64 CPointF64::operator*(double f64Scale) const
{
	return CPointF64(this->m_f64X * f64Scale, this->m_f64Y * f64Scale, this->m_i32LineThickness, this->m_sLineColor);
}

const CPointF64 CPointF64::operator/(double f64Scale) const
{
	return CPointF64(this->m_f64X / f64Scale, this->m_f64Y / f64Scale, this->m_i32LineThickness, this->m_sLineColor);
}

const CPointF64& CPointF64::operator=(const CPointF64& ptdPosition)
{
	this->m_f64X = ptdPosition.m_f64X;
	this->m_f64Y = ptdPosition.m_f64Y;
	this->m_i32LineThickness = ptdPosition.m_i32LineThickness;
	this->m_sLineColor = ptdPosition.m_sLineColor;

	return *this;
}

const CPointF64& CPointF64::operator=(const CPoint& ptPosition)
{
	this->m_f64X = ptPosition.x;
	this->m_f64Y = ptPosition.y;

	return *this;
}

bool CPointF64::operator==(const CPointF64& ptdPosition)
{
	bool bReturn = false;

	do
	{
		if(this->m_f64X != ptdPosition.m_f64X)
			break;

		if(this->m_f64Y != ptdPosition.m_f64Y)
			break;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

CPointF64::operator CPoint() const
{
	return CPoint((int)m_f64X, (int)m_f64Y);
}