// ChildInfoFrm.cpp: 구현 파일
//

#include "pch.h"
#include "MDIPainter.h"
#include "MainFrm.h"
#include "ChildInfoFrm.h"


// ChildInfoFrm

IMPLEMENT_DYNCREATE(CChildInfoFrm, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildInfoFrm, CMDIChildWnd)
	ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()

CChildInfoFrm::CChildInfoFrm()
{
	m_sAcceleratorKey = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
}

CChildInfoFrm::~CChildInfoFrm()
{
	// 창을 닫았음을 표시
	do
	{
		CMDIPainterApp* pSApp = dynamic_cast<CMDIPainterApp*>(AfxGetApp());

		if(!pSApp)
			break;

		pSApp->CloseInformationView();
	}
	while(false);
}

// ChildInfoFrm 메시지 처리기

void CChildInfoFrm::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	do 
	{
		CInformationDoc* pSDoc = dynamic_cast<CInformationDoc*>(GetActiveDocument());

		if(!pSDoc)
			break;

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		pSMainFrame->UpdateWindow();

		CInformationView* pSView = dynamic_cast<CInformationView*>(GetActiveView());

		if(!pSView)
			break;

		pSView->Invalidate(TRUE);
		pSView->UpdateWindow();
	}
	while(false);

	CMDIChildWnd::OnNcLButtonDown(nHitTest, point);
}

void CChildInfoFrm::ActivateFrame(int nCmdShow)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CMDIChildWnd::ActivateFrame(nCmdShow);

	do 
	{
		CInformationDoc* pSDoc = dynamic_cast<CInformationDoc*>(GetActiveDocument());

		if(!pSDoc)
			break;

		pSDoc->SetTitle(L"Information");
	}
	while(false);
}

BOOL CChildInfoFrm::PreTranslateMessage(MSG* pMsg)
{
	BOOL bReturn = FALSE;

	do 
	{
		if(pMsg->message != WM_KEYDOWN)
			break;

		if(!::TranslateAccelerator(m_hWnd, m_sAcceleratorKey, pMsg))
			break;

		bReturn = TRUE;
	}
	while(false);

	return bReturn ? bReturn : CMDIChildWnd::PreTranslateMessage(pMsg);
}