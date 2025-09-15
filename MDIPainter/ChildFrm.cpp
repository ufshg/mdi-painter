
// ChildFrm.cpp: CChildFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "MDIPainter.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#include "ELineThick.h"
#include "ELineColor.h"
#include "EToolbarLineThickIndex.h"
#include "EToolbarLineColorIndex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_COMMAND(ID_LINE_THICK_1, &CChildFrame::OnLineThick1)
	ON_COMMAND(ID_LINE_THICK_3, &CChildFrame::OnLineThick3)
	ON_COMMAND(ID_LINE_THICK_5, &CChildFrame::OnLineThick5)
	ON_COMMAND(ID_LINE_COLOR_BLACK, &CChildFrame::OnLineColorBlack)
	ON_COMMAND(ID_LINE_COLOR_LIGHTRED, &CChildFrame::OnLineColorLightred)
	ON_COMMAND(ID_LINE_COLOR_LIME, &CChildFrame::OnLineColorLime)
	ON_COMMAND(ID_LINE_COLOR_BLUE, &CChildFrame::OnLineColorBlue)
	ON_COMMAND(ID_LINE_COLOR_USER, &CChildFrame::OnLineColorUser)
	ON_UPDATE_COMMAND_UI(ID_LINE_THICK_1, &CChildFrame::OnUpdateLineThick1)
	ON_UPDATE_COMMAND_UI(ID_LINE_THICK_3, &CChildFrame::OnUpdateLineThick3)
	ON_UPDATE_COMMAND_UI(ID_LINE_THICK_5, &CChildFrame::OnUpdateLineThick5)
	ON_UPDATE_COMMAND_UI(ID_LINE_COLOR_BLACK, &CChildFrame::OnUpdateLineColorBlack)
	ON_UPDATE_COMMAND_UI(ID_LINE_COLOR_LIGHTRED, &CChildFrame::OnUpdateLineColorLightred)
	ON_UPDATE_COMMAND_UI(ID_LINE_COLOR_LIME, &CChildFrame::OnUpdateLineColorLime)
	ON_UPDATE_COMMAND_UI(ID_LINE_COLOR_BLUE, &CChildFrame::OnUpdateLineColorBlue)
	ON_UPDATE_COMMAND_UI(ID_LINE_COLOR_USER, &CChildFrame::OnUpdateLineColorUser)
	ON_CBN_SELCHANGE(ID_LINE_THICK_COMBO, OnSelectChangeLineThickCombo)
	ON_CBN_SELCHANGE(ID_LINE_COLOR_COMBO, OnSelectChangeLineColorCombo)
	ON_WM_CONTEXTMENU()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CChildFrame 생성/소멸

CChildFrame::CChildFrame() noexcept
{
	InitializeChildFrame();

	m_sAcceleratorKey = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_CHILDFRAME));
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if(!CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

// CChildFrame 진단

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 메시지 처리기



void CChildFrame::InitializeChildFrame()
{
	m_i32LineThick = 1;
	m_sLineColor = RGB(0, 0, 0);
}

bool CChildFrame::SynchronizeToolbar()
{
	bool bReturn = false;

	do 
	{
		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(GetParentFrame());

		if(!pSMainFrame)
			break;

		pSMainFrame->m_sLineThickCombo.SetRedraw(FALSE);
		pSMainFrame->m_sLineColorCombo.SetRedraw(FALSE);

		int32_t i32LineThickIndex = 0;
		int32_t i32LineColorIndex = 0;

		switch (m_i32LineThick)
		{
		case (int32_t)ELineThick::ELineThick_1:
			{
				i32LineThickIndex = (int32_t)EToolbarLineThickIndex::EToolbarLineThickIndex_1;
			}
			break;

		case (int32_t)ELineThick::ELineThick_3:
			{
				i32LineThickIndex = (int32_t)EToolbarLineThickIndex::EToolbarLineThickIndex_3;
			}
			break;

		case (int32_t)ELineThick::ELineThick_5:
			{
				i32LineThickIndex = (int32_t)EToolbarLineThickIndex::EToolbarLineThickIndex_5;
			}
			break;
		}

		switch(m_sLineColor)
		{
		case (COLORREF)ELineColor::ELineColor_Black:
			{
				i32LineColorIndex = (int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Black;
			}
			break;

		case (COLORREF)ELineColor::ELineColor_Lightred:
			{
				i32LineColorIndex = (int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Lightred;
			}
			break;

		case (COLORREF)ELineColor::ELineColor_Lime:
			{
				i32LineColorIndex = (int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Lime;
			}
			break;

		case (COLORREF)ELineColor::ELineColor_Blue:
			{
				i32LineColorIndex = (int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Blue;
			}
			break;

		default:
			{
				i32LineColorIndex = (int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_User;
			}
			break;
		}

		pSMainFrame->m_sLineThickCombo.SetCurSel(i32LineThickIndex);
		pSMainFrame->m_sLineColorCombo.SetCurSel(i32LineColorIndex);

		pSMainFrame->m_sLineThickCombo.SetRedraw(TRUE);
		pSMainFrame->m_sLineColorCombo.SetRedraw(TRUE);

		pSMainFrame->m_sLineThickCombo.Invalidate(TRUE);
		pSMainFrame->m_sLineColorCombo.Invalidate(TRUE);

		bReturn = true;
	}                                               
	while(false);

	return bReturn;
}

void CChildFrame::OnLineThick1()
{
	m_i32LineThick = (int32_t)ELineThick::ELineThick_1;

	do
	{
		CMDIPainterView* pSView = dynamic_cast<CMDIPainterView*>(GetActiveView());

		if(!pSView)
			break;

		pSView->SetLineThick(m_i32LineThick);

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(GetParentFrame());

		if(!pSMainFrame)
			break;

		pSMainFrame->m_sLineThickCombo.SetCurSel((int32_t)EToolbarLineThickIndex::EToolbarLineThickIndex_1);
	}
	while(false);
}

void CChildFrame::OnLineThick3()
{
	m_i32LineThick = (int32_t)ELineThick::ELineThick_3;

	do
	{
		CMDIPainterView* pSView = dynamic_cast<CMDIPainterView*>(GetActiveView());

		if(!pSView)
			break;

		pSView->SetLineThick(m_i32LineThick);

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(GetParentFrame());

		if(!pSMainFrame)
			break;

		pSMainFrame->m_sLineThickCombo.SetCurSel((int32_t)EToolbarLineThickIndex::EToolbarLineThickIndex_3);
	}
	while(false);
}

void CChildFrame::OnLineThick5()
{
	m_i32LineThick = (int32_t)ELineThick::ELineThick_5;

	do
	{
		CMDIPainterView* pSView = dynamic_cast<CMDIPainterView*>(GetActiveView());

		if(!pSView)
			break;

		pSView->SetLineThick(m_i32LineThick);

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(GetParentFrame());

		if(!pSMainFrame)
			break;

		pSMainFrame->m_sLineThickCombo.SetCurSel((int32_t)EToolbarLineThickIndex::EToolbarLineThickIndex_5);
	}
	while(false);
}

void CChildFrame::OnLineColorBlack()
{
	m_sLineColor = (COLORREF)ELineColor::ELineColor_Black;

	do
	{
		CMDIPainterView* pSView = dynamic_cast<CMDIPainterView*>(GetActiveView());

		if(!pSView)
			break;

		pSView->SetLineColor(m_sLineColor);

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(GetParentFrame());

		if(!pSMainFrame)
			break;

		pSMainFrame->m_sLineColorCombo.SetCurSel((int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Black);
	}
	while(false);
}

void CChildFrame::OnLineColorLightred()
{
	m_sLineColor = (COLORREF)ELineColor::ELineColor_Lightred;

	do
	{
		CMDIPainterView* pSView = dynamic_cast<CMDIPainterView*>(GetActiveView());

		if(!pSView)
			break;

		pSView->SetLineColor(m_sLineColor);

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(GetParentFrame());

		if(!pSMainFrame)
			break;

		pSMainFrame->m_sLineColorCombo.SetCurSel((int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Lightred);
	}
	while(false);
}

void CChildFrame::OnLineColorLime()
{
	m_sLineColor = (COLORREF)ELineColor::ELineColor_Lime;

	do
	{
		CMDIPainterView* pSView = dynamic_cast<CMDIPainterView*>(GetActiveView());

		if(!pSView)
			break;

		pSView->SetLineColor(m_sLineColor);

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(GetParentFrame());

		if(!pSMainFrame)
			break;

		pSMainFrame->m_sLineColorCombo.SetCurSel((int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Lime);
	}
	while(false);
}

void CChildFrame::OnLineColorBlue()
{
	m_sLineColor = (COLORREF)ELineColor::ELineColor_Blue;

	do
	{
		CMDIPainterView* pSView = dynamic_cast<CMDIPainterView*>(GetActiveView());

		if(!pSView)
			break;

		pSView->SetLineColor(m_sLineColor);

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(GetParentFrame());

		if(!pSMainFrame)
			break;

		pSMainFrame->m_sLineColorCombo.SetCurSel((int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Blue);
	}
	while(false);
}

void CChildFrame::OnLineColorUser()
{
	CColorDialog sColorDialog(m_sLineColor, CC_FULLOPEN);

	do
	{
		CMDIPainterView* pSView = dynamic_cast<CMDIPainterView*>(GetActiveView());

		if(!pSView)
			break;

		// 툴바 이슈, 마우스 릴리즈
		pSView->ReleaseMouse();

		if(sColorDialog.DoModal() == IDOK)
			m_sLineColor = sColorDialog.GetColor();

		m_sLineColor = sColorDialog.GetColor();

		pSView->SetLineColor(m_sLineColor);

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(GetParentFrame());

		if(!pSMainFrame)
			break;

		int32_t i32CurrentIndex = 0;

		switch(m_sLineColor)
		{
		case (COLORREF)ELineColor::ELineColor_Black:
			{
				i32CurrentIndex = (int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Black;
			}
			break;

		case (COLORREF)ELineColor::ELineColor_Lightred:
			{
				i32CurrentIndex = (int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Lightred;
			}
			break;

		case (COLORREF)ELineColor::ELineColor_Lime:
			{
				i32CurrentIndex = (int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Lime;
			}
			break;

		case (COLORREF)ELineColor::ELineColor_Blue:
			{
				i32CurrentIndex = (int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Blue;
			}
			break;

		default:
			{
				i32CurrentIndex = (int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_User;
			}
			break;
		}

		pSMainFrame->m_sLineColorCombo.SetCurSel(i32CurrentIndex);
	}
	while(false);
}

void CChildFrame::OnUpdateLineThick1(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_i32LineThick == (int32_t)ELineThick::ELineThick_1 ? 1 : 0);
}

void CChildFrame::OnUpdateLineThick3(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_i32LineThick == (int32_t)ELineThick::ELineThick_3 ? 1 : 0);
}

void CChildFrame::OnUpdateLineThick5(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_i32LineThick == (int32_t)ELineThick::ELineThick_5 ? 1 : 0);
}

void CChildFrame::OnUpdateLineColorBlack(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_sLineColor == (COLORREF)ELineColor::ELineColor_Black ? 1 : 0);
}

void CChildFrame::OnUpdateLineColorLightred(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_sLineColor == (COLORREF)ELineColor::ELineColor_Lightred ? 1 : 0);
}

void CChildFrame::OnUpdateLineColorLime(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_sLineColor == (COLORREF)ELineColor::ELineColor_Lime ? 1 : 0);
}

void CChildFrame::OnUpdateLineColorBlue(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_sLineColor == (COLORREF)ELineColor::ELineColor_Blue ? 1 : 0);
}

void CChildFrame::OnUpdateLineColorUser(CCmdUI* pCmdUI)
{
	int32_t i32Result = 0;

	do
	{
		if(m_sLineColor == (COLORREF)ELineColor::ELineColor_Black)
			break;

		if(m_sLineColor == (COLORREF)ELineColor::ELineColor_Lightred)
			break;

		if(m_sLineColor == (COLORREF)ELineColor::ELineColor_Lime)
			break;

		if(m_sLineColor == (COLORREF)ELineColor::ELineColor_Blue)
			break;

		i32Result = 1;
	}
	while(false);

	pCmdUI->SetCheck(i32Result);
}

BOOL CChildFrame::PreTranslateMessage(MSG* pMsg)
{
	BOOL bReturn = FALSE;

	do
	{
		if(pMsg->message != WM_KEYDOWN)
			break;

		if(!::TranslateAccelerator(m_hWnd, m_sAcceleratorKey, pMsg))
			break;

		if(pMsg->wParam == 'S' || pMsg->wParam == 'O' || pMsg->wParam == 'N')
		{
			CMDIPainterView* pSView = dynamic_cast<CMDIPainterView*>(GetActiveView());

			if(!pSView)
				break;

			pSView->ReleaseMouse();

			if(pMsg->wParam == 'O')
				break;
		}

		bReturn = TRUE;
	}
	while(false);

	return bReturn ? bReturn : CMDIChildWnd::PreTranslateMessage(pMsg);
}

void CChildFrame::OnSelectChangeLineThickCombo()
{
	do 
	{
		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(GetParentFrame());

		if(!pSMainFrame)
			break;

		pSMainFrame->m_sLineThickCombo.SetRedraw(FALSE);

		int32_t i32SeletedThickIndex = pSMainFrame->m_sLineThickCombo.GetCurSel();

		switch(i32SeletedThickIndex)
		{
		case (int32_t)EToolbarLineThickIndex::EToolbarLineThickIndex_1:
			{
				OnLineThick1();
			}
			break;

		case (int32_t)EToolbarLineThickIndex::EToolbarLineThickIndex_3:
			{
				OnLineThick3();
			}
			break;

		case (int32_t)EToolbarLineThickIndex::EToolbarLineThickIndex_5:
			{
				OnLineThick5();
			}
			break;
		}

		pSMainFrame->m_sLineThickCombo.SetRedraw(TRUE);
		pSMainFrame->m_sLineThickCombo.Invalidate(TRUE);
		pSMainFrame->m_sLineThickCombo.UpdateWindow();
	}
	while(false);
}

void CChildFrame::OnSelectChangeLineColorCombo()
{
	do
	{
		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(GetParentFrame());

		if(!pSMainFrame)
			break;

		pSMainFrame->m_sLineColorCombo.SetRedraw(FALSE);

		int32_t i32SeletedColorIndex = pSMainFrame->m_sLineColorCombo.GetCurSel();

		switch(i32SeletedColorIndex)
		{
		case (int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Black:
			{
				OnLineColorBlack();
			}
			break;

		case (int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Lightred:
			{
				OnLineColorLightred();
			}
			break;

		case (int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Lime:
			{
				OnLineColorLime();
			}
			break;

		case (int32_t)EToolbarLineColorIndex::EToolbarLineColorIndex_Blue:
			{
				OnLineColorBlue();
			}
			break;

		default:
			{
				OnLineColorUser();
			}
			break;
		}

		pSMainFrame->m_sLineColorCombo.SetRedraw(TRUE);
		pSMainFrame->m_sLineColorCombo.Invalidate(TRUE);
		pSMainFrame->m_sLineThickCombo.UpdateWindow();
	}
	while(false);
}

void CChildFrame::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu sPopup;
	CMenu* pSMenu = nullptr;

	do 
	{
		sPopup.LoadMenuW(IDR_CHILDPOPUP);

		pSMenu = sPopup.GetSubMenu(0);

		// nullptr
		if(!pSMenu)
			break;

		// 마우스 이벤트
		CMDIPainterView* pSView = dynamic_cast<CMDIPainterView*>(GetActiveView());

		if(!pSView)
			break;

		pSView->ReleaseMouse();

		pSMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	while(false);
}

void CChildFrame::OnNcLButtonDown(UINT nHitTest, CPoint point)
{

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	do 
	{
		CMDIPainterDoc* pSDoc = dynamic_cast<CMDIPainterDoc*>(GetActiveDocument());

		if(!pSDoc)
			break;

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		pSMainFrame->SetToolBarSaveButton(pSDoc->CheckIsEdited());
		pSMainFrame->UpdateWindow();

		CMDIPainterView* pSView = dynamic_cast<CMDIPainterView*>(GetActiveView());

		if(!pSView)
			break;

		pSView->Invalidate(TRUE);
		pSView->UpdateWindow();
	}
	while(false);

	CMDIChildWnd::OnNcLButtonDown(nHitTest, point);
}

void CChildFrame::OnClose()
{
	bool bResult = false;
	do 
	{
		CMDIPainterDoc* pSDoc = dynamic_cast<CMDIPainterDoc*>(GetActiveDocument());

		if(!pSDoc)
			break;

		pSDoc->HandleFileClose();

		bResult = pSDoc->CheckIsClosed();
	}
	while(false);

	if(bResult)
		CMDIChildWnd::OnClose();
}

void CChildFrame::ActivateFrame(int nCmdShow)
{
	CMDIChildWnd::ActivateFrame(nCmdShow);

	do 
	{
		CMDIPainterDoc* pSDoc = dynamic_cast<CMDIPainterDoc*>(GetActiveDocument());

		if(!pSDoc)
			break;

		pSDoc->InitializeTitle();
	}
	while(false);
}
