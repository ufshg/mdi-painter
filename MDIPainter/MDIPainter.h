
// MDIPainter.h: MDIPainter 애플리케이션의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
#include <map>

#include "MDIPainterDoc.h"

// CMDIPainterApp:
// 이 클래스의 구현에 대해서는 MDIPainter.cpp을(를) 참조하세요.
//

class CMDIPainterApp : public CWinApp
{
public:
	CMDIPainterApp() noexcept;

protected:
	DECLARE_MESSAGE_MAP()

private:
	bool m_bInfoOpened;

// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void CloseInformationView();

// 구현입니다.
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	afx_msg void OnInfoOpen();
	afx_msg void OnUpdateInfoOpen(CCmdUI* pCmdUI);
};

extern CMDIPainterApp theApp;
