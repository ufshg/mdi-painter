
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once

#include <map>
#include <vector>
#include "Table.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CToolBar          m_wndMainToolBar;
	CToolBar          m_wndChildToolBar;
	CStatusBar        m_wndStatusBar;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

// 멤버 요소
private:
	std::map<int32_t, std::map<COLORREF, CPen>>* m_pMap2Pens;

public:
	CComboBox m_sLineThickCombo;
	CComboBox m_sLineColorCombo;
	std::map<CMDIPainterDoc*, CTable*>* m_pMapOpenedDocs;
	std::vector<CMDIPainterDoc*>* m_pVctOpenedDocs;

	BOOL AddCombo(CToolBar* pSToolBar, CComboBox* pSComboBox, int32_t i32Index, int32_t i32Cx, int32_t i32Cy, DWORD dwStyle, UINT nID);
	bool InitializeMainFrame();
	void SetToolBarSaveButton(bool bTemp);

	CPen* GetPen(int32_t i32LineThick, const COLORREF& sLineColor);
	CTable* GetTable(CMDIPainterDoc* pSDoc);
	bool AddOpenedDoc(CMDIPainterDoc* pSDoc);
	bool AddBeforeOpenedDocs();
	bool DeleteDoc(CMDIPainterDoc* pSDoc);
	bool DeleteTable(CMDIPainterDoc* pSDoc);
	
	HMENU GetWindowMenuPopup(HMENU hMenuBar);

	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnClose();
};


