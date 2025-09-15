#pragma once
#include "afxdialogex.h"

#include "MDIPainter.h"
#include "MDIPainterDoc.h"
#include "MDIPainterView.h"

class CMDIPainterView;

// Table 대화 상자

class CTable : public CDialog
{
	DECLARE_DYNAMIC(CTable)

public:
	CTable(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTable();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TABLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CListCtrl m_sListCtrl;
	CMDIPainterDoc* m_pSPainterDoc;
	CMDIPainterView* m_pSPainterView;

public:
	virtual BOOL OnInitDialog();

	void SetPainterView(CMDIPainterView* pSPainterView);
	void SetTitle(CString strTitle);
	void SetOffsetX(double f64X);
	void SetOffsetY(double f64Y);
	void SetScale(double f64Scale);
	void SetEdited(bool bEdited);
	CString GetFormat(double f64Temp);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};