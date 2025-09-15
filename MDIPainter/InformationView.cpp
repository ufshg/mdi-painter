// InformationView.cpp: 구현 파일
//

#include "pch.h"
#include "MDIPainter.h"
#include "InformationView.h"
#include "InformationDoc.h"

#include <vector>
#include "Table.h"


// InformationView

IMPLEMENT_DYNCREATE(CInformationView, CFormView)

CInformationView::CInformationView()
	: CFormView(IDD_InformationView)
{
	m_i32Padding = 10;
	m_i32TableX = 0;
	m_i32TableY = 0;
	m_i32TableWidth = 182;
	m_i32TableHeight = 107;

	m_i32HorizontalCount = 0;
}

CInformationView::~CInformationView()
{
}

void CInformationView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInformationView, CFormView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FILE_SAVE, &CInformationView::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CInformationView::OnFileSaveAs)
END_MESSAGE_MAP()


// InformationView 진단

#ifdef _DEBUG
void CInformationView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CInformationView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

// InformationView 메시지 처리기
CTable* CInformationView::CreateNewTable()
{
	CTable* pSTable = nullptr;

	do 
	{
		pSTable = new CTable();

		if(!pSTable)
			break;

		pSTable->Create(IDD_TABLE, this);

		CPoint ptScrollPos = GetScrollPosition();

		int32_t i32PosX = GetNextTablePosX() - ptScrollPos.x;
		int32_t i32PosY = GetNextTablePosY() - ptScrollPos.y;

		pSTable->SetWindowPos(this, i32PosX, i32PosY, m_i32TableWidth, m_i32TableHeight, SWP_NOZORDER);

		pSTable->ShowWindow(SW_SHOW);

		CInformationDoc* pSDoc = dynamic_cast<CInformationDoc*>(GetDocument());

		if(!pSDoc)
			break;

		std::vector<CTable*>* pVctCTables = pSDoc->GetTables();
		
		if(!pVctCTables)
			break;

		pVctCTables->emplace_back(pSTable);
	}
	while(false);

	SetScroll();

	return pSTable;
}

int32_t CInformationView::GetNextTablePosX()
{
	int32_t i32Return = m_i32Padding + (m_i32Padding + m_i32TableWidth) * m_i32TableX;

	m_i32TableX += 1;

	if(m_i32TableX == m_i32HorizontalCount)
		m_i32TableX = 0;

	return i32Return;
}

int32_t CInformationView::GetNextTablePosY()
{
	int32_t i32Return = m_i32Padding + (m_i32Padding + m_i32TableHeight) * m_i32TableY;

	if(m_i32TableX == 0)
		m_i32TableY += 1;

	return i32Return;
}

bool CInformationView::SetTablePosition()
{
	bool bReturn = false;

	do
	{
		CInformationDoc* pSDoc = dynamic_cast<CInformationDoc*>(GetDocument());

		if(!pSDoc)
			break;

		std::vector<CTable*>* pVctCTables = pSDoc->GetTables();

		if(!pVctCTables)
			break;

		m_i32TableX = 0;
		m_i32TableY = 0;

		CPoint ptScrollPos = GetScrollPosition();

		int32_t i32TableCount = static_cast<int32_t>(pVctCTables->size());

		HDWP hdwp = ::BeginDeferWindowPos(i32TableCount);

		for(size_t stI = 0; stI < pVctCTables->size(); stI++)
		{
			int32_t i32PosX = GetNextTablePosX() - ptScrollPos.x;
			int32_t i32PosY = GetNextTablePosY() - ptScrollPos.y;

			::DeferWindowPos(hdwp, pVctCTables->at(stI)->GetSafeHwnd(), HWND_TOP, i32PosX, i32PosY, m_i32TableWidth, m_i32TableHeight, SWP_NOZORDER | SWP_NOREDRAW);
		}

		::EndDeferWindowPos(hdwp);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CInformationView::SetScroll()
{
	bool bReturn = false;

	do 
	{
		int32_t i32ViewWidth = m_i32Padding * 2 + m_i32TableWidth;
		int32_t i32ViewHeight = m_i32Padding + (m_i32Padding + m_i32TableHeight) * m_i32TableY;

			// 다음 줄에 원소가 있다면 한 줄 추가
		if(m_i32TableX != 0)
			i32ViewHeight += m_i32Padding + m_i32TableHeight;

		CRect rtRect;

		GetClientRect(&rtRect);

		CSize sScrollSize;

		if(m_i32TableY && rtRect.Width() < i32ViewWidth)
			sScrollSize.cx = i32ViewWidth;

		if(rtRect.Height() < i32ViewHeight)
			sScrollSize.cy = i32ViewHeight;

		SetScrollSizes(MM_TEXT, sScrollSize);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

BOOL CInformationView::OnEraseBkgnd(CDC* pDC)
{
	do 
	{
		CRect rtRect;

		GetClientRect(&rtRect);

		CInformationDoc* pSDoc = dynamic_cast<CInformationDoc*>(GetDocument());

		if(!pSDoc)
			break;

		for(CTable* pSTable : *pSDoc->GetTables())
		{
			if(!pSTable)
				continue;

			CRect rtTable;

			pSTable->GetWindowRect(&rtTable);
			ScreenToClient(&rtTable);

			pDC->ExcludeClipRect(&rtTable);
		}

		pDC->FillRect(&rtRect, &CBrush(::GetSysColor(COLOR_3DFACE)));
	}
	while(false);

	return TRUE;
}

void CInformationView::OnSize(UINT nType, int cx, int cy)
{
	do
	{
		CRect rtRect;

		GetClientRect(&rtRect);

		if(m_i32HorizontalCount == 0)
			m_i32HorizontalCount = (rtRect.Width() - m_i32Padding) / (m_i32Padding + m_i32TableWidth);

		int32_t i32AfterCount = (cx - m_i32Padding) / (m_i32Padding + m_i32TableWidth);

		if(m_i32HorizontalCount == i32AfterCount)
			break;

		m_i32HorizontalCount = i32AfterCount;

		if(m_i32HorizontalCount == 0)
			m_i32HorizontalCount = 1;

		SetTablePosition();
	}
	while(false);

	Invalidate(FALSE);
	SetScroll();
}

void CInformationView::OnFileSave()
{
	// 현재 뷰에서 저장 기능 비활성화
}

void CInformationView::OnFileSaveAs()
{
	// 현재 뷰에서 저장 기능 비활성화
}
