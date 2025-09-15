#pragma once



// InformationView 폼 보기

class CInformationView : public CFormView
{
	DECLARE_DYNCREATE(CInformationView)

protected:
	CInformationView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CInformationView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_InformationView };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	int32_t m_i32Padding;
	int32_t m_i32TableX;
	int32_t m_i32TableY;
	int32_t m_i32TableWidth;
	int32_t m_i32TableHeight;

	int32_t m_i32HorizontalCount;

public:
	CTable* CreateNewTable();
	int32_t GetNextTablePosX();
	int32_t GetNextTablePosY();
	bool SetTablePosition();
	bool SetScroll();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
};