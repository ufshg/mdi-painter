
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MDIPainter.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#include "Table.h"
#include "InformationDoc.h"

#include "ELineThick.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	m_pMap2Pens = nullptr;
	m_pMapOpenedDocs = nullptr;
	m_pVctOpenedDocs = nullptr;

	InitializeMainFrame();
}

CMainFrame::~CMainFrame()
{
	delete m_pMap2Pens;
	delete m_pMapOpenedDocs;
	delete m_pVctOpenedDocs;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int i32Return = -1;

	do 
	{
		if(CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
			break;

		if(!m_wndMainToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		   !m_wndMainToolBar.LoadToolBar(IDR_MAINFRAME))
		{
			TRACE0("도구 모음을 만들지 못했습니다.\n");
			break;      // 만들지 못했습니다.
		}

		// ChildFrame에 보여줄 차일드 툴바
		if(!m_wndChildToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		   !m_wndChildToolBar.LoadToolBar(IDR_CHILDFRAME))
		{
			TRACE0("도구 모음을 만들지 못했습니다.\n");
			break;      // 만들지 못했습니다.
		}

		if(!m_wndStatusBar.Create(this))
		{
			TRACE0("상태 표시줄을 만들지 못했습니다.\n");
			break;      // 만들지 못했습니다.
		}

		m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

			// 선 두께 콤보 박스
		if(!AddCombo(&m_wndChildToolBar, &m_sLineThickCombo, 6, 60, 200, WS_CHILD | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_VSCROLL | CBS_HASSTRINGS, ID_LINE_THICK_COMBO))
			break;

		m_sLineThickCombo.AddString(L"1 px");
		m_sLineThickCombo.AddString(L"3 px");
		m_sLineThickCombo.AddString(L"5 px");

		m_sLineThickCombo.SetCurSel(0);

		// 선 색상 콤보 박스
		if(!AddCombo(&m_wndChildToolBar, &m_sLineColorCombo, 7, 120, 200, WS_CHILD | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_VSCROLL | CBS_HASSTRINGS, ID_LINE_COLOR_COMBO))
			break;

		m_sLineColorCombo.AddString(L"BLACK");
		m_sLineColorCombo.AddString(L"LIGHTRED");
		m_sLineColorCombo.AddString(L"LIME");
		m_sLineColorCombo.AddString(L"BLUE");
		m_sLineColorCombo.AddString(L"User defined");

		m_sLineColorCombo.SetCurSel(0);

		m_wndMainToolBar.ShowWindow(SW_SHOW);
		m_wndChildToolBar.ShowWindow(SW_HIDE);

		RecalcLayout();

		i32Return = 0;
	}
	while(false);

	return i32Return;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

BOOL CMainFrame::AddCombo(CToolBar* pSToolBar, CComboBox* pSComboBox, int32_t i32Index, int32_t i32Cx, int32_t i32Cy, DWORD dwStyle, UINT nID)
{
	CRect rtRect;
	BOOL bReturn = FALSE;

	do
	{
		pSToolBar->GetItemRect(i32Index, rtRect);

		if(i32Index == 6)
			rtRect.left = rtRect.left + 5;
		else
			rtRect.left = rtRect.left + 45;

		rtRect.right = rtRect.left + i32Cx;
		rtRect.bottom = rtRect.top + i32Cy;

		if(!pSComboBox->Create(dwStyle, rtRect, pSToolBar, nID))
			break;

		pSComboBox->ShowWindow(SW_SHOW);
		pSToolBar->SetButtonInfo(i32Index, nID, TBBS_DROPDOWN, i32Cx);

		bReturn = TRUE;
	}
	while(false);

	return bReturn;
}

bool CMainFrame::InitializeMainFrame()
{
	bool bReturn = false;

	do 
	{
		// m_pMap2Pens
		if(!m_pMap2Pens)
		{
			m_pMap2Pens = new std::map<int32_t, std::map<COLORREF, CPen>>;

			if(!m_pMap2Pens)
				break;

			m_pMap2Pens->insert({ (int32_t)ELineThick::ELineThick_1, std::map<COLORREF, CPen>() });
			m_pMap2Pens->insert({ (int32_t)ELineThick::ELineThick_3, std::map<COLORREF, CPen>() });
			m_pMap2Pens->insert({ (int32_t)ELineThick::ELineThick_5, std::map<COLORREF, CPen>() });
		}
		else
		{
			m_pMap2Pens->at((int32_t)ELineThick::ELineThick_1).clear();
			m_pMap2Pens->at((int32_t)ELineThick::ELineThick_3).clear();
			m_pMap2Pens->at((int32_t)ELineThick::ELineThick_5).clear();
		}

		// m_pMapOpenedDocs
		if(!m_pMapOpenedDocs)
		{
			m_pMapOpenedDocs = new std::map<CMDIPainterDoc*, CTable*>;

			if(!m_pMapOpenedDocs)
				break;
		}
		else
			m_pMapOpenedDocs->clear();

		// m_pVctOpenedDocs
		if(!m_pVctOpenedDocs)
		{
			m_pVctOpenedDocs = new std::vector<CMDIPainterDoc*>;

			if(!m_pVctOpenedDocs)
				break;
		}
		else
			m_pVctOpenedDocs->clear();

		bReturn = true;
	}
	while(false);

	return bReturn;
}

void CMainFrame::SetToolBarSaveButton(bool bTemp)
{
	if(bTemp)
		m_wndChildToolBar.GetToolBarCtrl().EnableButton(ID_FILE_SAVE, TRUE);
	else
		m_wndChildToolBar.GetToolBarCtrl().EnableButton(ID_FILE_SAVE, FALSE);

	m_wndChildToolBar.Invalidate(TRUE);
	m_wndChildToolBar.UpdateWindow();
}

CPen* CMainFrame::GetPen(int32_t i32LineThick, const COLORREF& sLineColor)
{
	CPen* pSReturn = nullptr;

	do 
	{
		if(!m_pMap2Pens)
			break;

		if(m_pMap2Pens->at(i32LineThick).find(sLineColor) != m_pMap2Pens->at(i32LineThick).end())
		{
			pSReturn = &m_pMap2Pens->at(i32LineThick)[sLineColor];
			break;
		}

		m_pMap2Pens->at(i32LineThick)[sLineColor].CreatePen(PS_INSIDEFRAME, i32LineThick, sLineColor);

		pSReturn = &m_pMap2Pens->at(i32LineThick)[sLineColor];
	}
	while(false);

	return pSReturn;
}

CTable* CMainFrame::GetTable(CMDIPainterDoc* pSDoc)
{
	CTable* pSTable = nullptr;

	do
	{
		if(m_pMapOpenedDocs->find(pSDoc) == m_pMapOpenedDocs->end())
			break;

		CTable* pSTemp = m_pMapOpenedDocs->at(pSDoc);

		if(!pSTemp)
			break;

		pSTable = pSTemp;
	}
	while(false);

	return pSTable;
}

bool CMainFrame::AddOpenedDoc(CMDIPainterDoc* pSDoc)
{
	bool bReturn = false;
	CTable* pSNewTable = nullptr;

	do 
	{ 
		if(m_pMapOpenedDocs->find(pSDoc) != m_pMapOpenedDocs->end())
			break;
		
		CMDIPainterApp* pSApp = dynamic_cast<CMDIPainterApp*>(AfxGetApp());

		if(!pSApp)
			break;

		POSITION sPos = pSApp->GetFirstDocTemplatePosition();

		if(!sPos)
			break;

		CDocTemplate* pSDocTemplate = pSApp->GetNextDocTemplate(sPos);
		pSDocTemplate = pSApp->GetNextDocTemplate(sPos);

		if(!pSDocTemplate)
			break;

		sPos = pSDocTemplate->GetFirstDocPosition();

		if(sPos)
		{
			CInformationDoc* pSInfoDoc = dynamic_cast<CInformationDoc*>(pSDocTemplate->GetNextDoc(sPos));

			if(!pSInfoDoc)
				break;

			pSNewTable = pSInfoDoc->CreateNewTable();
		}

		if(pSNewTable)
			m_pMapOpenedDocs->insert({ pSDoc, pSNewTable });
		else
			m_pMapOpenedDocs->insert({ pSDoc, nullptr });

		m_pVctOpenedDocs->emplace_back(pSDoc);

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CMainFrame::AddBeforeOpenedDocs()
{
	bool bReturn = false;

	do 
	{
		// Information Doc 가져오기 시작
		CMDIPainterApp* pSApp = dynamic_cast<CMDIPainterApp*>(AfxGetApp());

		if(!pSApp)
			break;

		POSITION sPos = pSApp->GetFirstDocTemplatePosition();

		if(!sPos)
			break;

		CDocTemplate* pSDocTemplate = pSApp->GetNextDocTemplate(sPos);
		pSDocTemplate = pSApp->GetNextDocTemplate(sPos);

		if(!pSDocTemplate)
			break;

		sPos = pSDocTemplate->GetFirstDocPosition();

		if(!sPos)
			break;

		CInformationDoc* pSInfoDoc = dynamic_cast<CInformationDoc*>(pSDocTemplate->GetNextDoc(sPos));

		if(!pSInfoDoc)
			break;
		// Information Doc 가져오기 끝

		// vector 순회
		if(m_pVctOpenedDocs->empty())
			break;

		for(std::vector<CMDIPainterDoc*>::iterator sTemp = m_pVctOpenedDocs->begin(); sTemp != m_pVctOpenedDocs->end(); ++sTemp)
		{
			if(*sTemp)
			{
				CTable* pSTable = pSInfoDoc->CreateNewTable();

				if(pSTable)
				{
					m_pMapOpenedDocs->at(*sTemp) = pSTable;

					(*sTemp)->SetTable();
				}
			}
		}

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CMainFrame::DeleteDoc(CMDIPainterDoc* pSDoc)
{
	bool bReturn = false;

	do 
	{
		if(!pSDoc)
			break;

		if(m_pMapOpenedDocs->find(pSDoc) == m_pMapOpenedDocs->end())
			break;

		m_pMapOpenedDocs->erase(pSDoc);

		for(std::vector<CMDIPainterDoc*>::iterator sTemp = m_pVctOpenedDocs->begin(); sTemp != m_pVctOpenedDocs->end(); ++sTemp)
		{
			if(*sTemp == pSDoc)
			{
				m_pVctOpenedDocs->erase(sTemp);
				break;
			}
		}

		bReturn = true;
	}
	while(false);

	return bReturn;
}

bool CMainFrame::DeleteTable(CMDIPainterDoc* pSDoc)
{
	bool bReturn = false;

	do 
	{
		if(m_pMapOpenedDocs->find(pSDoc) == m_pMapOpenedDocs->end())
			break;

		m_pMapOpenedDocs->at(pSDoc) = nullptr;

		bReturn = true;
	}
	while(false);

	return bReturn;
}

HMENU CMainFrame::GetWindowMenuPopup(HMENU hMenuBar)
{
	return NULL;
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	do
	{
		CChildFrame* pSChildFrame = dynamic_cast<CChildFrame*>(GetActiveFrame());

		if(!pSChildFrame)
		{
			m_wndMainToolBar.ShowWindow(SW_SHOW);
			m_wndChildToolBar.ShowWindow(SW_HIDE);
		}
		else
		{
			m_wndMainToolBar.ShowWindow(SW_HIDE);

			if(!pSChildFrame->SynchronizeToolbar())
				break;

			m_wndChildToolBar.ShowWindow(SW_SHOW);
		}
	}
	while(false);

	RecalcLayout();
	CMDIFrameWnd::OnUpdateFrameTitle(bAddToTitle);
}

void CMainFrame::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu sPopup;
	CMenu* pSMenu = nullptr;

	do
	{
		sPopup.LoadMenuW(IDR_MAINPOPUP);

		pSMenu = sPopup.GetSubMenu(0);

		// nullptr
		if(!pSMenu)
			break;

		pSMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	while(false);
}

void CMainFrame::OnClose()
{
	int32_t i32StatusCode = MessageBoxW(L"Do you really want to quit the program?", L"MDIPainter", MB_OKCANCEL | MB_ICONWARNING | MB_TASKMODAL);

	if(i32StatusCode == IDOK)
	{
		CMDIChildWnd* pSChild = dynamic_cast<CMDIChildWnd*>(MDIGetActive());
		bool bClosed = true;

		while(pSChild)
		{
			HWND sChildWnd = pSChild->GetSafeHwnd();

			::SendMessage(sChildWnd, WM_CLOSE, 0, 0);

			if(::IsWindow(sChildWnd))
			{
				bClosed = false;
				break;
			}

			pSChild = dynamic_cast<CMDIChildWnd*>(MDIGetActive());
		}

		if(bClosed)
			CMDIFrameWnd::OnClose();
	}
}