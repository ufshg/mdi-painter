#pragma once

#include "InformationDoc.h"
#include "InformationView.h"

// ChildInfoFrm 프레임

class CChildInfoFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildInfoFrm)
protected:
	CChildInfoFrm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CChildInfoFrm();

protected:
	DECLARE_MESSAGE_MAP()

private:
	HACCEL m_sAcceleratorKey;

public:
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
};


