
// MDIPainterView.cpp: CMDIPainterView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MDIPainter.h"
#endif

#include "MainFrm.h"
#include "ChildFrm.h"
#include "MDIPainterDoc.h"
#include "MDIPainterView.h"

#include "ChildInfoFrm.h"
#include "InformationDoc.h"

#include "ELineThick.h"
#include "ELineColor.h"
#include "EToolbarLineThickIndex.h"
#include "EToolbarLineColorIndex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum class EOutCode : int32_t
{
	EOutCode_Inside = 0b0000,
	EOutCode_Left = 0b0001,
	EOutCode_Right = 0b0010,
	EOutCode_Top = 0b0100,
	EOutCode_Bottom = 0b1000
};

// CMDIPainterView

IMPLEMENT_DYNCREATE(CMDIPainterView, CView)

BEGIN_MESSAGE_MAP(CMDIPainterView, CView)
	// 명령 자리
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CMDIPainterView 생성/소멸

CMDIPainterView::CMDIPainterView() noexcept
{
	// 포인터 변수 초기화
	m_pVct2Lines = nullptr;
	m_pVctViewBound = nullptr;

	InitializeView();
}

CMDIPainterView::~CMDIPainterView()
{
	// 동적할당 제거
	if(m_pVct2Lines)
		delete m_pVct2Lines;

	m_pVct2Lines = nullptr;

	delete m_pVctViewBound;
}

void CMDIPainterView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	do
	{
		CMDIPainterDoc* pSDoc = dynamic_cast<CMDIPainterDoc*>(GetDocument());

		if(!pSDoc)
			break;

		m_pVct2Lines = pSDoc->GetPVct2Lines();

		if(!m_pVct2Lines)
		{
			m_pVct2Lines = new std::vector<std::vector<CPointF64>>;

			if(m_pVct2Lines)
				pSDoc->SetPVct2Lines(m_pVct2Lines);
		}

		Invalidate(FALSE);
	}
	while(false);
}

// CMDIPainterView 그리기

void CMDIPainterView::OnDraw(CDC* pDC)
{
	do 
	{
		CMDIPainterDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if(!pDoc)
			break;

		CDC sMemDC;
		CBitmap bmpBitmap;
		CBitmap* pBmpOldBItmap = nullptr;
		CRect rtRect;

		GetClientRect(&rtRect);

		sMemDC.CreateCompatibleDC(pDC);
		bmpBitmap.CreateCompatibleBitmap(pDC, rtRect.Width(), rtRect.Height());

		pBmpOldBItmap = sMemDC.SelectObject(&bmpBitmap);

		sMemDC.PatBlt(0, 0, rtRect.Width(), rtRect.Height(), WHITENESS);

		SetViewBound(rtRect);

		for(std::vector<CPointF64>& vctLine : *m_pVct2Lines)
			for(uint32_t u32I = 0; u32I < vctLine.size() - 1; u32I++)
				ClipAndDrawLine(vctLine, u32I, sMemDC);

		pDC->BitBlt(0, 0, rtRect.Width(), rtRect.Height(), &sMemDC, 0, 0, SRCCOPY);

		sMemDC.SelectObject(pBmpOldBItmap);
		sMemDC.DeleteDC();
	}
	while(false);
}


// CMDIPainterView 진단

#ifdef _DEBUG
void CMDIPainterView::AssertValid() const
{
	CView::AssertValid();
}

void CMDIPainterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMDIPainterDoc* CMDIPainterView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMDIPainterDoc)));
	return (CMDIPainterDoc*)m_pDocument;
}
#endif //_DEBUG


// CMDIPainterView 메시지 처리기
bool CMDIPainterView::InitializeView()
{
	bool bReturn = false;

	do 
	{
		if(m_pVctViewBound)
		{
			CRect rtRect;

			GetClientRect(&rtRect);

			m_i32ViewX = rtRect.Width();
			m_i32ViewY = rtRect.Height();
		}
		else
		{
			m_i32ViewX = 0;
			m_i32ViewY = 0;
		}

		if(m_pVct2Lines)
			m_pVct2Lines->clear();

		m_ptdCanvasOffset = CPointF64();
		m_f64CanvasScale = 1.0;

		if(m_pVctViewBound)
		{
			m_pVctViewBound->at(0) = CPointF64();
			m_pVctViewBound->at(1) = CPointF64();
		}
		else
		{
			m_pVctViewBound = new std::vector<CPointF64>(2);
			
			if(!m_pVctViewBound)
				break;
		}

		m_i32LineThick = 1;
		m_sLineColor = RGB(0, 0, 0);

		m_ptdMousePrev = CPointF64();
		m_bDrawingFirst = true;
		m_bLButtonDown = false;
		m_bPanningFirst = true;
		m_bMButtonDown = false;
		m_bEdited = false;

		m_pSTable = nullptr;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

CPointF64 CMDIPainterView::ConvertCanvasToReal(const CPointF64& ptdInput)
{
	// 캔버스 상에 존재하는 좌표를 실제 저장되는 좌표로 변환하는 함수 구현하기.
	CPointF64 ptdReturn = (ptdInput / m_f64CanvasScale) - m_ptdCanvasOffset;

	return ptdReturn;
}

CPointF64 CMDIPainterView::ConvertRealToCanvas(const CPointF64& ptdInput)
{
	// 실제 저장된 좌표가 캔버스에 그려질 수 있도록 변환하는 함수 구현하기.
	CPointF64 ptdReturn = (ptdInput + m_ptdCanvasOffset) * m_f64CanvasScale;

	return ptdReturn;
}

int32_t CMDIPainterView::ComputeClipCode(const CPointF64& ptdInput)
{
	int32_t i32Return = (int32_t)EOutCode::EOutCode_Inside;

	if(ptdInput.m_f64X < m_pVctViewBound->at(0).m_f64X)
		i32Return |= (int32_t)EOutCode::EOutCode_Left;

	if(ptdInput.m_f64X > m_pVctViewBound->at(1).m_f64X)
		i32Return |= (int32_t)EOutCode::EOutCode_Right;

	if(ptdInput.m_f64Y < m_pVctViewBound->at(0).m_f64Y)
		i32Return |= (int32_t)EOutCode::EOutCode_Top;

	if(ptdInput.m_f64Y > m_pVctViewBound->at(1).m_f64Y)
		i32Return |= (int32_t)EOutCode::EOutCode_Bottom;

	return i32Return;
}

void CMDIPainterView::ClipAndDrawLine(const std::vector<CPointF64>& vctLine, uint32_t u32Index, CDC& sMemDC)
{
	double f64TempX1 = vctLine[u32Index].m_f64X;
	double f64TempY1 = vctLine[u32Index].m_f64Y;
	double f64TempX2 = vctLine[u32Index + 1].m_f64X;
	double f64TempY2 = vctLine[u32Index + 1].m_f64Y;

	int32_t i32Code1 = ComputeClipCode(vctLine[u32Index]);
	int32_t i32Code2 = ComputeClipCode(vctLine[u32Index + 1]);
	bool bAccept = false;

	while(true)
	{
		if(!(i32Code1 | i32Code2))
		{
			bAccept = true;
			break;
		}
		else if(i32Code1 & i32Code2)
			break;
		else
		{
			double f64TempX = 0., f64TempY = 0.;
			int32_t i32Code = (int32_t)EOutCode::EOutCode_Inside;

			if(i32Code1 > i32Code2)
				i32Code = i32Code1;
			else
				i32Code = i32Code2;

			if(i32Code & (int32_t)EOutCode::EOutCode_Left)
			{
				f64TempY = f64TempY1 + (f64TempY2 - f64TempY1) * (m_pVctViewBound->at(0).m_f64X - f64TempX1) / (f64TempX2 - f64TempX1);
				f64TempX = m_pVctViewBound->at(0).m_f64X;
			}
			else if(i32Code & (int32_t)EOutCode::EOutCode_Right)
			{
				f64TempY = f64TempY1 + (f64TempY2 - f64TempY1) * (m_pVctViewBound->at(1).m_f64X - f64TempX1) / (f64TempX2 - f64TempX1);
				f64TempX = m_pVctViewBound->at(1).m_f64X;
			}
			else if(i32Code & (int32_t)EOutCode::EOutCode_Top)
			{
				f64TempX = f64TempX1 + (f64TempX2 - f64TempX1) * (m_pVctViewBound->at(0).m_f64Y - f64TempY1) / (f64TempY2 - f64TempY1);
				f64TempY = m_pVctViewBound->at(0).m_f64Y;
			}
			else if(i32Code & (int32_t)EOutCode::EOutCode_Bottom)
			{
				f64TempX = f64TempX1 + (f64TempX2 - f64TempX1) * (m_pVctViewBound->at(1).m_f64Y - f64TempY1) / (f64TempY2 - f64TempY1);
				f64TempY = m_pVctViewBound->at(1).m_f64Y;
			}

			if(i32Code == i32Code1)
			{
				f64TempX1 = f64TempX;
				f64TempY1 = f64TempY;

				i32Code1 = ComputeClipCode(CPointF64(f64TempX1, f64TempY1));
			}
			else
			{
				f64TempX2 = f64TempX;
				f64TempY2 = f64TempY;

				i32Code2 = ComputeClipCode(CPointF64(f64TempX2, f64TempY2));
			}
		}
	}

	do
	{
		if(!bAccept)
			break;

		CPointF64 ptdTemp1 = CPointF64(f64TempX1, f64TempY1);
		CPointF64 ptdTemp2 = CPointF64(f64TempX2, f64TempY2);

		CPen* pSPen = GetPen(vctLine[u32Index].m_i32LineThickness, vctLine[u32Index].m_sLineColor);
		CPen* pSOldpen = sMemDC.SelectObject(pSPen);

		// nullptr 확인
		if(!pSPen)
			break;

		if(!pSOldpen)
			break;

		if(ptdTemp1 == ptdTemp2)
		{
			CPointF64 ptdTemp = ConvertRealToCanvas(ptdTemp1);

			sMemDC.MoveTo(ptdTemp);

			// 펜의 굵기가 1인 점을 화면에 출력하기 위한 보정 (원본 훼손 X)
			if(vctLine[u32Index].m_i32LineThickness == 1)
				ptdTemp = ptdTemp + CPointF64(1.0, 0.0);

			sMemDC.LineTo(ptdTemp);
		}
		else
		{
			sMemDC.MoveTo(ConvertRealToCanvas(ptdTemp1));
			sMemDC.LineTo(ConvertRealToCanvas(ptdTemp2));
		}

		sMemDC.SelectObject(pSOldpen);
	}
	while(false);

	return;
}

bool CMDIPainterView::SetViewBound(const CRect& rtRect)
{
	bool bReturn = false;
	double f64MarginValue = 2.0;

	do 
	{
		if(!m_pVctViewBound)
			break;

		m_pVctViewBound->at(0) = ConvertCanvasToReal(CPointF64(-f64MarginValue, -f64MarginValue));
		m_pVctViewBound->at(1) = ConvertCanvasToReal(CPointF64((double)rtRect.Width() + f64MarginValue, (double)rtRect.Height() + f64MarginValue));

		bReturn = true;
	}
	while(false);


	return bReturn;
}

void CMDIPainterView::SetLineThick(int32_t i32LineThick)
{
	m_i32LineThick = i32LineThick;

	// 드로잉 중 바로 반영
	do
	{
		if(!m_bLButtonDown)
			break;

		std::vector<CPointF64> vctTemp;
		CPointF64 ptdTemp = CPointF64(m_pVct2Lines->back()[1]);

		ptdTemp.m_i32LineThickness = m_i32LineThick;

		vctTemp.emplace_back(ptdTemp);
		vctTemp.emplace_back(ptdTemp);

		m_pVct2Lines->emplace_back(vctTemp);

		Invalidate(FALSE);
	}
	while(false);
}

void CMDIPainterView::SetLineColor(COLORREF sLineColor)
{
	m_sLineColor = sLineColor;

	do
	{
		if(!m_bLButtonDown)
			break;

		CPointF64 ptdTemp = CPointF64(m_pVct2Lines->back()[1]);
		std::vector<CPointF64> vctTemp;

		ptdTemp.m_sLineColor = m_sLineColor;

		vctTemp.emplace_back(ptdTemp);
		vctTemp.emplace_back(ptdTemp);

		m_pVct2Lines->emplace_back(vctTemp);

		Invalidate(FALSE);
	}
	while(false);
}

void CMDIPainterView::ReleaseMouse()
{
	if(m_bLButtonDown || m_bMButtonDown)
	{
		m_bLButtonDown = false;
		m_bDrawingFirst = true;
		m_bMButtonDown = false;
		m_bPanningFirst = true;

		ReleaseCapture();
	}
}

CPen* CMDIPainterView::GetPen(int32_t i32LineThick, const COLORREF& sLineColor)
{
	CPen* pSReturn = nullptr;

	do
	{
		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		pSReturn = pSMainFrame->GetPen(i32LineThick, sLineColor);
	}
	while(false);

	return pSReturn;
}

void CMDIPainterView::OnLButtonDown(UINT nFlags, CPoint point)
{
	do
	{
		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		pSMainFrame->UpdateWindow();

		if(!m_bDrawingFirst)
			break;

		m_ptdMousePrev = ConvertCanvasToReal(CPointF64(point, m_i32LineThick, m_sLineColor));
		m_bLButtonDown = false;
		m_bDrawingFirst = true;

		std::vector<CPointF64> vctTemp;

		vctTemp.emplace_back(m_ptdMousePrev);
		vctTemp.emplace_back(m_ptdMousePrev);

		m_pVct2Lines->emplace_back(vctTemp);

		Invalidate(FALSE);

		// 도큐먼트에 수정 여부 업데이트
		if(m_bEdited)
			break;

		m_bEdited = true;

		CMDIPainterDoc* pSDoc = dynamic_cast<CMDIPainterDoc*>(GetDocument());

		if(!pSDoc)
			break;

		pSDoc->SetEdited();

		// 테이블에 반영
		SetTableEdited();

		// 저장 버튼 상태 강제 반영
		pSMainFrame->SetToolBarSaveButton(true);
		pSMainFrame->UpdateWindow();
	}
	while(false);

	m_ptdMousePrev = ConvertCanvasToReal(CPointF64(point, m_i32LineThick, m_sLineColor));
	m_bLButtonDown = true;
	m_bDrawingFirst = false;

	RECT rtClient;

	SetCapture();
	GetClientRect(&rtClient);
	ClientToScreen(&rtClient);

	CView::OnLButtonDown(nFlags, point);
}

void CMDIPainterView::OnLButtonUp(UINT nFlags, CPoint point)
{
	do
	{
		if(m_bMButtonDown)
		{
			m_bLButtonDown = false;
			m_bDrawingFirst = true;
			break;
		}

		if(!m_bLButtonDown)
			break;

		m_bLButtonDown = false;
		m_bDrawingFirst = true;

		std::vector<CPointF64> vctTemp;

		vctTemp.emplace_back(m_ptdMousePrev);
		vctTemp.emplace_back(ConvertCanvasToReal(CPointF64(point, m_i32LineThick, m_sLineColor)));

		if(!(vctTemp[0] == vctTemp[1]))
			m_pVct2Lines->emplace_back(vctTemp);

		if(!m_bMButtonDown)
			ReleaseCapture();

		Invalidate(FALSE);
	}
	while(false);

	CView::OnLButtonUp(nFlags, point);
}

void CMDIPainterView::OnMButtonDown(UINT nFlags, CPoint point)
{
	do
	{
		if(!m_bPanningFirst)
			break;

		m_ptdMousePrev = ConvertCanvasToReal(CPointF64(point));
		m_bMButtonDown = false;
		m_bPanningFirst = true;

		Invalidate(FALSE);
	}
	while(false);

	m_ptdMousePrev = ConvertCanvasToReal(CPointF64(point));
	m_bMButtonDown = true;
	m_bPanningFirst = false;

	RECT rtClient;

	SetCapture();
	GetClientRect(&rtClient);
	ClientToScreen(&rtClient);

	CView::OnMButtonDown(nFlags, point);
}

void CMDIPainterView::OnMButtonUp(UINT nFlags, CPoint point)
{
	do
	{
		if(!m_bMButtonDown)
			break;

		m_bMButtonDown = false;
		m_bPanningFirst = true;

		CPointF64 ptdDiff = ConvertCanvasToReal(CPointF64(point)) - m_ptdMousePrev;
		m_ptdCanvasOffset = m_ptdCanvasOffset + ptdDiff;

		if(!m_bLButtonDown)
			ReleaseCapture();

		Invalidate(FALSE);
		SetTableOffset();
	}
	while(false);

	CView::OnMButtonUp(nFlags, point);
}

void CMDIPainterView::OnMouseMove(UINT nFlags, CPoint point)
{
	switch(m_bMButtonDown)
	{
	case true:
		{
			CPointF64 ptdDiff = ConvertCanvasToReal(CPointF64(point, m_i32LineThick, m_sLineColor)) - m_ptdMousePrev;

			m_ptdCanvasOffset = m_ptdCanvasOffset + ptdDiff;

			m_ptdMousePrev = ConvertCanvasToReal(CPointF64(point, m_i32LineThick, m_sLineColor));

			Invalidate(FALSE);
			SetTableOffset();
		}
		break;

	case false:
		{
			if(!m_bLButtonDown)
				break;

			std::vector<CPointF64> vctTemp;

			CPointF64 ptdTemp = ConvertCanvasToReal(CPointF64(point, m_i32LineThick, m_sLineColor));

			vctTemp.emplace_back(m_ptdMousePrev);
			vctTemp.emplace_back(ptdTemp);

			m_pVct2Lines->emplace_back(vctTemp);

			m_ptdMousePrev = ptdTemp;

			Invalidate(FALSE);
		}
		break;
	}

	CView::OnMouseMove(nFlags, point);
}

BOOL CMDIPainterView::OnEraseBkgnd(CDC* pDC)
{
	// 화면 깜빡임 방지
	return TRUE;
}

void CMDIPainterView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect rtRect;

	GetClientRect(&rtRect);

	if(m_i32ViewX == 0)
		m_i32ViewX = rtRect.Width();

	if(m_i32ViewY == 0)
		m_i32ViewY = rtRect.Height();

	// 화면 조정 이전 중심 좌표 (real)
	CPointF64 ptdBeforeCenter = ConvertCanvasToReal(CPointF64((double)m_i32ViewX / 2, (double)m_i32ViewY / 2));

	// 조정 이벤트 발생 후 화면 중심 좌표
	CPointF64 ptdAfterCenter = CPointF64((double)cx / 2, (double)cy / 2) / m_f64CanvasScale;

	// offset
	m_ptdCanvasOffset = ptdAfterCenter - ptdBeforeCenter;

	// resize 정보 업데이트
	m_i32ViewX = cx;
	m_i32ViewY = cy;

	Invalidate(FALSE);
	SetTable();
}

BOOL CMDIPainterView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CPoint ptCursorPosition = pt;
	double f64BeforeScaleTemp = m_f64CanvasScale;

	do
	{
		CWinApp* pSApp = AfxGetApp();

		if(!pSApp)
			break;

		RECT rtClient;

		pSApp->m_pMainWnd->GetWindowRect(&rtClient);
		ScreenToClient(&ptCursorPosition);

		// 마우스 휠 업
		if(zDelta > 0)
		{
			m_f64CanvasScale *= 1.5;

			if(m_f64CanvasScale > 100000.0)
				m_f64CanvasScale = 100000.0;
		}
		else
		{
			m_f64CanvasScale *= 0.7;

			if(m_f64CanvasScale < 0.000001)
				m_f64CanvasScale = 0.000001;
		}
	}
	while(false);

	CPointF64 ptdBeforePosition = CPointF64(ptCursorPosition) / f64BeforeScaleTemp - m_ptdCanvasOffset;

	m_ptdCanvasOffset = CPointF64(ptCursorPosition) / m_f64CanvasScale - ptdBeforePosition;

	Invalidate(FALSE);
	SetTable();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

std::vector<std::vector<CPointF64>>* CMDIPainterView::GetPVct2Lines()
{
	return m_pVct2Lines;
}

void CMDIPainterView::SetPVct2Lines(std::vector<std::vector<CPointF64>>* pVct2Temp)
{
	if(m_pVct2Lines)
		delete m_pVct2Lines;

	m_pVct2Lines = pVct2Temp;
}

void CMDIPainterView::SetTable()
{
	SetTableOffset();
	SetTableScale();
}

void CMDIPainterView::SetTableOffset()
{
	do 
	{
		if(m_pSTable)
			break;

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		CMDIPainterDoc* pSDoc = dynamic_cast<CMDIPainterDoc*>(GetDocument());

		if(!pSDoc)
			break;

		m_pSTable = pSMainFrame->GetTable(pSDoc);

		if(m_pSTable)
			m_pSTable->SetPainterView(this);
	}
	while(false);

	if(m_pSTable)
	{
		m_pSTable->SetOffsetX(m_ptdCanvasOffset.m_f64X);
		m_pSTable->SetOffsetY(m_ptdCanvasOffset.m_f64Y);
		m_pSTable->UpdateWindow();
	}
}

void CMDIPainterView::SetTableScale()
{
	do
	{
		if(m_pSTable)
			break;

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		CMDIPainterDoc* pSDoc = dynamic_cast<CMDIPainterDoc*>(GetDocument());

		if(!pSDoc)
			break;

		m_pSTable = pSMainFrame->GetTable(pSDoc);

		if(m_pSTable)
			m_pSTable->SetPainterView(this);
	}
	while(false);

	if(m_pSTable)
	{
		m_pSTable->SetScale(m_f64CanvasScale);
		m_pSTable->UpdateWindow();
	}
}

bool CMDIPainterView::SetTableEdited()
{
	bool bReturn = false;

	do 
	{
		if(m_pSTable)
			break;

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		CMDIPainterDoc* pSDoc = dynamic_cast<CMDIPainterDoc*>(GetDocument());

		if(!pSDoc)
			break;

		m_pSTable = pSMainFrame->GetTable(pSDoc);

		if(m_pSTable)
			m_pSTable->SetPainterView(this);
	}
	while(false);

	if(m_pSTable)
	{
		m_pSTable->SetEdited(m_bEdited);
		m_pSTable->UpdateWindow();
		
		bReturn = true;
	}

	return bReturn;
}

void CMDIPainterView::InitializeEdited()
{
	m_bEdited = false;
}

void CMDIPainterView::DeleteTable()
{
	m_pSTable = nullptr;
}