
// MDIPainterView.h: CMDIPainterView 클래스의 인터페이스
//

#pragma once

#include <vector>
#include "PointF64.h"
#include "MDIPainterDoc.h"
#include "Table.h"

class CTable;

class CMDIPainterDoc;

class CMDIPainterView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMDIPainterView() noexcept;
	DECLARE_DYNCREATE(CMDIPainterView)

// 특성입니다.
public:
	CMDIPainterDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
protected:

// 구현입니다.
public:
	virtual ~CMDIPainterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

// 멤버 변수
private:
	std::vector <std::vector<CPointF64>>* m_pVct2Lines;
	std::vector<CPointF64>* m_pVctViewBound;
	CPointF64 m_ptdCanvasOffset;
	double m_f64CanvasScale;

	int32_t m_i32ViewX;
	int32_t m_i32ViewY;

	int32_t m_i32LineThick;
	COLORREF m_sLineColor;

	CPointF64 m_ptdMousePrev;
	bool m_bDrawingFirst;
	bool m_bLButtonDown;
	bool m_bPanningFirst;
	bool m_bMButtonDown;
	bool m_bEdited;

	CTable* m_pSTable;

public:
	bool InitializeView();

	CPointF64 ConvertCanvasToReal(const CPointF64& ptdInput);
	CPointF64 ConvertRealToCanvas(const CPointF64& ptdInput);
	int32_t ComputeClipCode(const CPointF64& ptdInput);
	void ClipAndDrawLine(const std::vector<CPointF64>& vctLine, uint32_t u32Index, CDC& sMemDC);
	bool SetViewBound(const CRect& rtRect);
	void SetLineThick(int32_t i32LineThick);
	void SetLineColor(COLORREF sLineColor);
	void ReleaseMouse();
	void InitializeEdited();
	void DeleteTable();

	std::vector<std::vector<CPointF64>>* GetPVct2Lines();
	CPen* GetPen(int32_t i32LineThick, const COLORREF& sLineColor);
	void SetPVct2Lines(std::vector<std::vector<CPointF64>>* pVct2Temp);
	void SetTable();
	void SetTableOffset();
	void SetTableScale();
	bool SetTableEdited();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // MDIPainterView.cpp의 디버그 버전
inline CMDIPainterDoc* CMDIPainterView::GetDocument() const
   { return reinterpret_cast<CMDIPainterDoc*>(m_pDocument); }
#endif

