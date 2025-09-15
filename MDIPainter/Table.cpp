// Table.cpp: 구현 파일
//

#include "pch.h"
#include "afxdialogex.h"
#include "Table.h"
#include "MainFrm.h"


// Table 대화 상자

IMPLEMENT_DYNAMIC(CTable, CDialog)

CTable::CTable(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TABLE, pParent)
{
	m_pSPainterDoc = nullptr;
	m_pSPainterView = nullptr;
}

CTable::~CTable()
{
	m_pSPainterView->DeleteTable();

	do 
	{
		if(!m_pSPainterView)
			break;

		m_pSPainterView->DeleteTable();

		if(!m_pSPainterDoc)
			break;

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		if(!pSMainFrame->DeleteTable(m_pSPainterDoc))
			break;
	}
	while(false);
}

void CTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TABLE, m_sListCtrl);
}


BEGIN_MESSAGE_MAP(CTable, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// Table 메시지 처리기
BOOL CTable::OnInitDialog()
{
	CDialog::OnInitDialog();

	//ModifyStyle(0, WS_CLIPCHILDREN);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_sListCtrl.SetExtendedStyle(LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_sListCtrl.GetHeaderCtrl()->EnableWindow(FALSE);
	m_sListCtrl.ModifyStyle(0, LVS_NOSCROLL);

	m_sListCtrl.InsertColumn(1, L"ITEM", LVCFMT_CENTER, 80);
	m_sListCtrl.InsertColumn(2, L"VALUE", LVCFMT_CENTER, 100);

	m_sListCtrl.InsertItem(0, L"FileName");
	m_sListCtrl.InsertItem(1, L"Offset X");
	m_sListCtrl.InsertItem(2, L"Offset Y");
	m_sListCtrl.InsertItem(3, L"Scale");
	m_sListCtrl.InsertItem(4, L"IsEdited");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTable::SetPainterView(CMDIPainterView* pSPainterView)
{
	m_pSPainterView = pSPainterView;
	m_pSPainterDoc = pSPainterView->GetDocument();
}

void CTable::SetTitle(CString strTitle)
{
	m_sListCtrl.SetItemText(0, 1, strTitle);
}

void CTable::SetOffsetX(double f64X)
{
	CString strTemp;

	strTemp.Format(GetFormat(f64X), f64X);

	m_sListCtrl.SetItemText(1, 1, strTemp); // 숫자 가공해서 잘 넣기
}

void CTable::SetOffsetY(double f64Y)
{
	CString strTemp;

	strTemp.Format(GetFormat(f64Y), f64Y);

	m_sListCtrl.SetItemText(2, 1, strTemp); // 숫자 가공해서 잘 넣기
}

void CTable::SetScale(double f64Scale)
{
	CString strTemp;

	strTemp.Format(L"%lfx", f64Scale);

	m_sListCtrl.SetItemText(3, 1, strTemp);
}

void CTable::SetEdited(bool bEdited)
{
	CString strTemp = bEdited ? L"Yes" : L"No";

	m_sListCtrl.SetItemText(4, 1, strTemp);
}

CString CTable::GetFormat(double f64Temp)
{
	CString strTemp = L"%lf";

	if(f64Temp >= 10000000.0 || f64Temp <= -10000000.0)
		strTemp = L"%e";

	return strTemp;
}

BOOL CTable::PreTranslateMessage(MSG* pMsg)
{
	BOOL bReturn = FALSE;

	switch(pMsg->message)
	{
	case WM_MOUSEWHEEL:
		{
			CWnd* pSParent = GetParent();

			if(pSParent)
			{
				::SendMessage(pSParent->GetSafeHwnd(), WM_MOUSEWHEEL, pMsg->wParam, pMsg->lParam);

				bReturn = TRUE;
			}
		}
		break;

	case WM_KEYDOWN:
		{
			CWnd* pSParentFrame = GetParentFrame();

			if(pSParentFrame)
			{
				pSParentFrame->PostMessageW(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);

				bReturn = TRUE;
			}
		}
		break;
	}

	return bReturn ? bReturn : CDialog::PreTranslateMessage(pMsg);
}

BOOL CTable::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return TRUE;
}
