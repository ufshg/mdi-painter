
// ChildFrm.h: CChildFrame 클래스의 인터페이스
//

#pragma once

#include "MDIPainterDoc.h"
#include "MDIPainterView.h"

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame() noexcept;

// 특성입니다.
protected:
	CSplitterWnd m_wndSplitter;
public:

// 작업입니다.
public:

// 재정의입니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

// 멤버 요소
private:
	int32_t m_i32LineThick;
	COLORREF m_sLineColor;

	HACCEL m_sAcceleratorKey;

public:
	void InitializeChildFrame();
	bool SynchronizeToolbar();

	afx_msg void OnLineThick1();
	afx_msg void OnLineThick3();
	afx_msg void OnLineThick5();
	afx_msg void OnLineColorBlack();
	afx_msg void OnLineColorLightred();
	afx_msg void OnLineColorLime();
	afx_msg void OnLineColorBlue();
	afx_msg void OnLineColorUser();
	afx_msg void OnUpdateLineThick1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineThick3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineThick5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineColorBlack(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineColorLightred(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineColorLime(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineColorBlue(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineColorUser(CCmdUI* pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void OnSelectChangeLineThickCombo();
	void OnSelectChangeLineColorCombo();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnFileClose();
	virtual void ActivateFrame(int nCmdShow = -1);
};
