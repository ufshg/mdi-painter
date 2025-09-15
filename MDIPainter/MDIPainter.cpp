
// MDIPainter.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MDIPainter.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "MDIPainterDoc.h"
#include "MDIPainterView.h"

#include "ChildInfoFrm.h"
#include "InformationDoc.h"
#include "InformationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMDIPainterApp

BEGIN_MESSAGE_MAP(CMDIPainterApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMDIPainterApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	//ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CMDIPainterApp::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN, &CMDIPainterApp::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, &CMDIPainterApp::OnFileNew)
	ON_COMMAND(ID_INFO_OPEN, &CMDIPainterApp::OnInfoOpen)
	ON_UPDATE_COMMAND_UI(ID_INFO_OPEN, &CMDIPainterApp::OnUpdateInfoOpen)
END_MESSAGE_MAP()


// CMDIPainterApp 생성

CMDIPainterApp::CMDIPainterApp() noexcept
{

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
	#ifdef _MANAGED
		// 애플리케이션을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
		//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
		//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
	#endif

		// TODO: 아래 애플리케이션 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
		// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MDIPainter.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.

	m_bInfoOpened = false;
}

// 유일한 CMDIPainterApp 개체입니다.

CMDIPainterApp theApp;


// CMDIPainterApp 초기화

BOOL CMDIPainterApp::InitInstance()
{
	BOOL bReturn = FALSE;

	do 
	{
		INITCOMMONCONTROLSEX InitCtrls;

		InitCtrls.dwSize = sizeof(InitCtrls);
		InitCtrls.dwICC = ICC_WIN95_CLASSES;

		InitCommonControlsEx(&InitCtrls);

		CWinApp::InitInstance();

		if(!AfxOleInit())
		{
			AfxMessageBox(IDP_OLE_INIT_FAILED);
			break;
		}

		AfxEnableControlContainer();
		EnableTaskbarInteraction(FALSE);

		SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));
		LoadStdProfileSettings(4);

		CMultiDocTemplate* pDocTemplate;
		pDocTemplate = new CMultiDocTemplate(IDR_CHILDMENU, RUNTIME_CLASS(CMDIPainterDoc), RUNTIME_CLASS(CChildFrame), RUNTIME_CLASS(CMDIPainterView));

		if(!pDocTemplate)
			break;

		AddDocTemplate(pDocTemplate);

		pDocTemplate = nullptr;

		pDocTemplate = new CMultiDocTemplate(IDR_MAINMENU, RUNTIME_CLASS(CInformationDoc), RUNTIME_CLASS(CChildInfoFrm), RUNTIME_CLASS(CInformationView));

		if(!pDocTemplate)
			break;

		AddDocTemplate(pDocTemplate);

		// 주 MDI 프레임 창을 만듭니다.
		CMainFrame* pMainFrame = new CMainFrame;

		if(!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINMENU))
		{
			delete pMainFrame;
			break;
		}

		m_pMainWnd = pMainFrame;

		pMainFrame->ShowWindow(m_nCmdShow);
		pMainFrame->UpdateWindow();

		bReturn = TRUE;
	}
	while(false);

	return bReturn;
}


int CMDIPainterApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CMDIPainterApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 대화 상자 데이터입니다.
	#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_ABOUTBOX
	};
	#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CMDIPainterApp 메시지 처리기

void CMDIPainterApp::CloseInformationView()
{
	m_bInfoOpened = false;
}

void CMDIPainterApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CMDIPainterApp::OnFileOpen()
{
	do 
	{
		CString strFilter = L"flp files(*.flp)|*.flp|all files(*.*)|*.*||";

		LANGID sLanguage = PRIMARYLANGID(GetUserDefaultLangID());

		if(sLanguage == LANG_KOREAN)
			strFilter = L"flp 파일(*.flp)|*.flp|모든파일(*.*)|*.*||";

		CFileDialog sFileDialog(TRUE, L"flp", NULL, OFN_HIDEREADONLY, strFilter);

		if(sFileDialog.DoModal() != IDOK)
			break;
		
		OpenDocumentFile(sFileDialog.GetPathName());
	}
	while(false);
}

void CMDIPainterApp::OnFileNew()
{
	do 
	{
		CDocTemplate* pSDocTemplate = nullptr;

		POSITION sPos = GetFirstDocTemplatePosition();

		if(!sPos)
			break;

		pSDocTemplate = GetNextDocTemplate(sPos);

		if(!pSDocTemplate)
			break;
		
		pSDocTemplate->OpenDocumentFile(NULL);
	}
	while(false);
}

void CMDIPainterApp::OnInfoOpen()
{
	do 
	{
		CDocTemplate* pSDocTemplate = nullptr;

		POSITION sPos = GetFirstDocTemplatePosition();

		if(!sPos)
			break;

		pSDocTemplate = GetNextDocTemplate(sPos);
		pSDocTemplate = GetNextDocTemplate(sPos);

		if(!pSDocTemplate)
			break;

		if(!pSDocTemplate->OpenDocumentFile(NULL))
			break;
		
		m_bInfoOpened = true;
		
		// 기존에 열려있던 Paint View 정보 모두 반영
		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		if(!pSMainFrame->AddBeforeOpenedDocs())
			break;
	}
	while(false);
}

void CMDIPainterApp::OnUpdateInfoOpen(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_bInfoOpened);
}