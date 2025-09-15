#pragma once
class CPointF64
{
public:
	CPointF64();
	CPointF64(double f64X, double f64Y);
	CPointF64(double f64X, double f64Y, int32_t i32LineThickness);
	CPointF64(double f64x, double f64Y, int32_t i32LineThickness, COLORREF sLineColor);
	CPointF64(const CPointF64& ptdPosition);
	CPointF64(const CPoint& ptPosition);
	CPointF64(const CPoint& ptPosition, int32_t i32LineThickness);
	CPointF64(const CPoint& ptPosition, int32_t i32LineThickness, COLORREF sLineColor);
	virtual ~CPointF64();

	const CPointF64 operator+(const CPointF64& ptdPosition) const;
	const CPointF64 operator-(const CPointF64& ptdPosition) const;
	const CPointF64 operator*(double f64Scale) const;
	const CPointF64 operator/(double f64Scale) const;
	const CPointF64& operator=(const CPointF64& ptdPosition);
	const CPointF64& operator=(const CPoint& ptPosition);
	bool operator==(const CPointF64& ptdPosition);
	operator CPoint() const;

public:
	double m_f64X;
	double m_f64Y;
	int32_t m_i32LineThickness;
	COLORREF m_sLineColor;
};