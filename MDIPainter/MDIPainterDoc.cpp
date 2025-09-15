
// MDIPainterDoc.cpp: CMDIPainterDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MDIPainter.h"
#endif

#include "MainFrm.h"
#include "ChildFrm.h"
#include "MDIPainterDoc.h"
#include "InformationDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma pack(push, 1)
struct SPointF64
{
	double f64X;
	double f64Y;
	int32_t i32LineThickness;
	COLORREF sLineColor;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SDataSegment
{
	uint32_t u32VertexCount;
	SPointF64 ptdStart;
	SPointF64 ptdEnd;
};
#pragma pack(pop)

// CMDIPainterDoc

IMPLEMENT_DYNCREATE(CMDIPainterDoc, CDocument)

BEGIN_MESSAGE_MAP(CMDIPainterDoc, CDocument)
	ON_COMMAND(ID_FILE_CLOSE, &CMDIPainterDoc::OnFileClose)
	ON_COMMAND(ID_FILE_SAVE, &CMDIPainterDoc::OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CMDIPainterDoc::OnUpdateFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMDIPainterDoc::OnFileSaveAs)
END_MESSAGE_MAP()


// CMDIPainterDoc 생성/소멸

CMDIPainterDoc::CMDIPainterDoc() noexcept
{
	m_strDefaultTitle = L"Untitled";
	m_strDefaultExtension = L"flp files(*.flp)|*.flp|all files(*.*)|*.*||";

	LANGID sLanguage = PRIMARYLANGID(GetUserDefaultLangID());

	if(sLanguage == LANG_KOREAN)
	{
		m_strDefaultTitle = L"제목없음";
		m_strDefaultExtension = L"flp 파일(*.flp)|*.flp|모든파일(*.*)|*.*||";
	}

	m_pVct2Lines = new std::vector<std::vector<CPointF64>>;

	m_bEdited = false;
	m_bFileOpened = false;
	m_bSaved = false;
	m_bSavedDone = false;
	m_bClosedDone = false;
}

CMDIPainterDoc::~CMDIPainterDoc()
{
	do 
	{
		// MainFrame 조회
		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		CMDIPainterApp* pSApp = dynamic_cast<CMDIPainterApp*>(AfxGetApp());

		if(!pSApp)
			break;

		POSITION sPos = pSApp->GetFirstDocTemplatePosition();

		if(!sPos)
			break;

		CDocTemplate* pSDocTemplate = pSApp->GetNextDocTemplate(sPos);

		pSDocTemplate = pSApp->GetNextDocTemplate(sPos);

		if(!pSDocTemplate)
			break;

		sPos = pSDocTemplate->GetFirstDocPosition();

		CTable* pSTable = pSMainFrame->GetTable(this);

		if(!sPos || !pSTable)
			break;

		CInformationDoc* pSInfoDoc = dynamic_cast<CInformationDoc*>(pSDocTemplate->GetNextDoc(sPos));

		if(!pSInfoDoc)
			break;

		if(!pSInfoDoc->DeleteTable(pSTable))
			break;

		if(!pSMainFrame->DeleteDoc(this))
			break;
	}
	while(false);
}

BOOL CMDIPainterDoc::OnNewDocument()
{
	BOOL bReturn = FALSE;

	do 
	{
		if(!CDocument::OnNewDocument())
			break;

		m_strDocNumber = GetTitle().Mid(10);
		m_strFileName = m_strDefaultTitle + m_strDocNumber;

		SetTitle(m_strFileName);

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		if(!pSMainFrame->AddOpenedDoc(this))
			break;

		if(!SetTable())
			break;

		bReturn = TRUE;
	}
	while(false);

	return bReturn;
}

BOOL CMDIPainterDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	bool bOpen = false;
	bool bFailed = false;
	CString strPathName;

	FILE* pSFile = nullptr;
	uint8_t* pU8Buffer = nullptr;

	do
	{
		if(!CDocument::OnOpenDocument(lpszPathName))
			break;

		if(!m_pVct2Lines)
			break;

		// 경로 확인
		strPathName = lpszPathName;

		// 확장자명 검증
		CString strTemp = strPathName.Right(strPathName.GetLength() - strPathName.ReverseFind('.') - 1);

		if(strTemp != L"flp")
		{
			bFailed = true;
			break;
		}

		fopen_s(&pSFile, (CStringA)strPathName, "rb");

		if(!pSFile)
			break;

		fseek(pSFile, 0, SEEK_END);

		// 파일 크기
		uint32_t u32FileSize = ftell(pSFile);
		uint32_t u32FileCrc;

		rewind(pSFile);

		// 정상 파일 크기인지 확인
		if(u32FileSize < 15)
		{
			bFailed = true;
			break;
		}

		// CRC 4Bytes 제외
		u32FileSize = u32FileSize - 4;

		pU8Buffer = (uint8_t*)malloc(sizeof(uint8_t) * u32FileSize);

		if(!pU8Buffer)
			break;

		fread(pU8Buffer, sizeof(uint8_t), u32FileSize, pSFile);
		fread(&u32FileCrc, sizeof(uint32_t), 1, pSFile);

		uint32_t u32CalculatedCrc = CalculateCRC32(pU8Buffer, u32FileSize);

		if(u32CalculatedCrc != u32FileCrc)
		{
			bFailed = true;
			break;
		}

		// CRC 검증 버퍼 해제
		free(pU8Buffer);

		pU8Buffer = nullptr;

		// 로드 시작
		rewind(pSFile);

		CString strHeaderTemp;
		char i8HeaderTemp;

		// 헤더 검증
		for(int32_t i32I = 0; i32I < 11; i32I++)
		{
			fread(&i8HeaderTemp, sizeof(char), 1, pSFile);

			strHeaderTemp += i8HeaderTemp;
		}

		if(strHeaderTemp != "FourthLogic")
		{
			bFailed = true;
			break;
		}

		// Vertex 구조 크기
		uint32_t u32VertexSize;

		fread(&u32VertexSize, sizeof(uint32_t), 1, pSFile);

		// Vertex 그룹 개수
		uint32_t u32VertexGroupCount;

		fread(&u32VertexGroupCount, sizeof(uint32_t), 1, pSFile);

		for(uint32_t u32I = 0; u32I < u32VertexGroupCount; u32I++)
		{
			std::vector<CPointF64> vctTemp;
			uint32_t u32VertexCount;

			size_t stCheckData = fread(&u32VertexCount, sizeof(uint32_t), 1, pSFile);

			if(!stCheckData)
			{
				bFailed = true;
				break;
			}

			for(uint32_t u32J = 0; u32J < u32VertexCount; u32J++)
			{
				CPointF64 ptdTemp;

				stCheckData = fread(&ptdTemp.m_f64X, sizeof(double), 1, pSFile);

				if(!stCheckData)
				{
					bFailed = true;
					break;
				}

				stCheckData = fread(&ptdTemp.m_f64Y, sizeof(double), 1, pSFile);

				if(!stCheckData)
				{
					bFailed = true;
					break;
				}

				stCheckData = fread(&ptdTemp.m_i32LineThickness, sizeof(int32_t), 1, pSFile);

				if(!stCheckData)
				{
					bFailed = true;
					break;
				}

				stCheckData = fread(&ptdTemp.m_sLineColor, sizeof(uint32_t), 1, pSFile);

				if(!stCheckData)
				{
					bFailed = true;
					break;
				}

				vctTemp.emplace_back(ptdTemp);
			}

			if(bFailed)
				break;

			m_pVct2Lines->emplace_back(vctTemp);
		}

		if(bFailed)
			break;

		fclose(pSFile);

		pSFile = nullptr;
		bOpen = true;
	}
	while(false);

	if(bFailed)
	{
		if(m_pVct2Lines)
		{
			delete m_pVct2Lines;

			m_pVct2Lines = nullptr;
		}

		if(pSFile)
		{
			fclose(pSFile);

			pSFile = nullptr;
		}

		if(pU8Buffer)
		{
			free(pU8Buffer);

			pU8Buffer = nullptr;
		}

		AfxMessageBox(L"Failed to load file.");
	}
	else
	{
		if(bOpen)
		{
			m_strPathName = strPathName;
			m_bEdited = false;
			m_bFileOpened = true;

			m_strFileName = strPathName.Right(strPathName.GetLength() - strPathName.ReverseFind('\\') - 1);

			if(m_strFileName.Right(4).Compare(L".flp") == 0)
				m_strFileName = m_strFileName.Left(m_strFileName.GetLength() - 4);
		}
		else
		{
			// 열기 최종 실패 -> 할당 해제
			if(m_pVct2Lines)
			{
				delete m_pVct2Lines;

				m_pVct2Lines = nullptr;
			}

			if(pSFile)
			{
				fclose(pSFile);

				pSFile = nullptr;
			}

			if(pU8Buffer)
			{
				free(pU8Buffer);

				pU8Buffer = nullptr;
			}

			AfxMessageBox(L"Failed to load file.");
		}
	}

	BOOL bReturn;

	if(bOpen && !bFailed)
	{
		bReturn = TRUE;

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(pSMainFrame)
		{
			pSMainFrame->AddOpenedDoc(this);
			SetTable();
		}

		UpdateAllViews(nullptr);
	}
	else
		bReturn = FALSE;

	return bReturn;
}

// CMDIPainterDoc serialization

void CMDIPainterDoc::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CMDIPainterDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CMDIPainterDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMDIPainterDoc::SetSearchContent(const CString& value)
{
	if(value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if(pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMDIPainterDoc 진단

#ifdef _DEBUG
void CMDIPainterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMDIPainterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMDIPainterDoc 명령

void CMDIPainterDoc::SetEdited()
{
	if(!m_bEdited)
		SetTitle('*' + m_strFileName);

	m_bEdited = true;
}

void CMDIPainterDoc::OnFileClose()
{
	do 
	{
		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		CChildFrame* pSChildFrame = dynamic_cast<CChildFrame*>(pSMainFrame->GetActiveFrame());

		if(!pSChildFrame)
			break;

		pSChildFrame->PostMessageW(WM_CLOSE);
	}
	while(false);
}

void CMDIPainterDoc::OnFileSave()
{
	m_bSaved = false;

	do
	{
		CString strPathName = m_strPathName;

		if(!strPathName.IsEmpty())
		{
			m_bSaved = true;
			break;
		}

		// 지정된 이름이 없는 경우
		strPathName = m_strFileName;

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		CChildFrame* pSChildFrame = dynamic_cast<CChildFrame*>(pSMainFrame->GetActiveFrame());

		if(!pSChildFrame)
			break;

		CFileDialog sFileDialog(FALSE, L"flp", strPathName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, m_strDefaultExtension, pSChildFrame);

		if(sFileDialog.DoModal() != IDOK)
			break;

		m_strPathName = sFileDialog.GetPathName();

		CString strTemp = sFileDialog.GetPathName().Right(m_strPathName.GetLength() - m_strPathName.ReverseFind('.') - 1);

		if(strTemp != L"flp")
			m_strPathName += L".flp";

		m_bSaved = true;
	}
	while(false);

	if(m_bSaved)
		SaveCurrentFile();
}

void CMDIPainterDoc::OnFileSaveAs()
{
	m_bSaved = false;

	do
	{
		CString strPathName = m_strFileName;

		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		CChildFrame* pSChildFrame = dynamic_cast<CChildFrame*>(pSMainFrame->GetActiveFrame());

		if(!pSChildFrame)
			break;

		CFileDialog sFileDialog(FALSE, L"flp", strPathName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, m_strDefaultExtension, pSChildFrame);

		if(sFileDialog.DoModal() != IDOK)
			break;

		m_strPathName = sFileDialog.GetPathName();

		CString strTemp = sFileDialog.GetPathName().Right(m_strPathName.GetLength() - m_strPathName.ReverseFind('.') - 1);

		if(strTemp != L"flp")
			m_strPathName += L".flp";

		m_bSaved = true;
	}
	while(false);

	if(m_bSaved)
		SaveCurrentFile();
}

uint32_t CMDIPainterDoc::CalculateCRC32(uint8_t* pU8Data, size_t stLength)
{
	uint32_t u32Crc = 0xFFFFFFFF;
	const uint8_t* pU8Temp = nullptr;

	for(size_t stI = 0; stI < stLength; stI++)
	{
		pU8Temp = pU8Data + stI;
		u32Crc ^= *pU8Temp;

		for(int32_t i32J = 0; i32J < 8; i32J++)
		{
			if(u32Crc & 1)
				u32Crc = (u32Crc >> 1) ^ 0xEDB88320;
			else
				u32Crc = u32Crc >> 1;
		}
	}

	return u32Crc ^ 0xFFFFFFFF;
}

void CMDIPainterDoc::SaveCurrentFile()
{
	m_bSavedDone = false;

	CMainFrame* pSMainFrame = nullptr;
	CChildFrame* pSChildFrame = nullptr;
	CMDIPainterView* pSView = nullptr;

	bool bDone = false;
	FILE* pSFile = nullptr;

	do
	{
		pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		pSChildFrame = dynamic_cast<CChildFrame*>(pSMainFrame->GetActiveFrame());

		if(!pSChildFrame)
			break;

		pSView = dynamic_cast<CMDIPainterView*>(pSChildFrame->GetActiveView());

		if(!pSView)
			break;

		std::vector<std::vector<CPointF64>>* pVct2Lines = pSView->GetPVct2Lines();

		if(!pVct2Lines)
			break;

		fopen_s(&pSFile, (CStringA)m_strPathName, "wb");

		if(!pSFile)
			break;

		// 헤더
		const char* pI8Header = "FourthLogic";

		for(int32_t i = 0; i < 11; i++)
			fwrite(&pI8Header[i], 1, 1, pSFile);

		// Vertex Size - uint32_t / u32
		uint32_t u32VertexSize = 24;

		fwrite(&u32VertexSize, 4, 1, pSFile);

		// Vertex Group Count - uint32_t / u32
		uint32_t u32VertexGroupCount = (uint32_t)pVct2Lines->size();

		fwrite(&u32VertexGroupCount, 4, 1, pSFile);

		std::vector<SDataSegment> vctBuffer;

		// Nth Vertex Data - uint32_t / u32
		for(uint32_t u32I = 0; u32I < u32VertexGroupCount; u32I++)
		{
			CPointF64 ptdStart = pVct2Lines->at(u32I)[0];
			CPointF64 ptdEnd = pVct2Lines->at(u32I)[1];

			SPointF64 STempStart = { ptdStart.m_f64X, ptdStart.m_f64Y, ptdStart.m_i32LineThickness, ptdStart.m_sLineColor };
			SPointF64 STempEnd = { ptdEnd.m_f64X, ptdEnd.m_f64Y, ptdEnd.m_i32LineThickness, ptdEnd.m_sLineColor };

			SDataSegment SVertexData;

			SVertexData = { 2, STempStart, STempEnd };

			vctBuffer.emplace_back(SVertexData);
		}

		fwrite(vctBuffer.data(), sizeof(SDataSegment), vctBuffer.size(), pSFile);
		fclose(pSFile);

		pSFile = nullptr;

		// CRC32 계산하기
		fopen_s(&pSFile, (CStringA)m_strPathName, "rb");

		if(!pSFile)
			break;

		fseek(pSFile, 0, SEEK_END);

		uint32_t u32FileSize = ftell(pSFile);

		rewind(pSFile);

		// 버퍼 할당
		uint8_t* pU8Buffer = (uint8_t*)malloc(sizeof(uint8_t) * u32FileSize);

		if(!pU8Buffer)
			break;

		fread(pU8Buffer, 1, u32FileSize, pSFile);
		fclose(pSFile);

		uint32_t u32Crc = CalculateCRC32(pU8Buffer, u32FileSize);

		free(pU8Buffer);

		pU8Buffer = nullptr;

		fopen_s(&pSFile, (CStringA)m_strPathName, "ab");

		if(!pSFile)
			break;

		fwrite(&u32Crc, 4, 1, pSFile);
		fclose(pSFile);

		bDone = true;
	}
	while(false);

	if(bDone)
	{
		m_bEdited = false;
		m_bFileOpened = true;

		// View에 존재하는 bIsEdited 변수도 초기화 해준다.
		if(pSView)
			pSView->InitializeEdited();

		m_strFileName = m_strPathName.Right(m_strPathName.GetLength() - m_strPathName.ReverseFind('\\') - 1);

		if(m_strFileName.Right(4).Compare(L".flp") == 0)
			m_strFileName = m_strFileName.Left(m_strFileName.GetLength() - 4);

		SetTitle(m_strFileName);

		// 저장에 성공
		m_bSavedDone = true;

		// 테이블 수정
		CTable* pSTable = pSMainFrame->GetTable(this);

		if(pSTable)
		{
			pSTable->SetTitle(m_strFileName);
			pSTable->SetEdited(false);
		}
	}
	else
	{
		m_bSavedDone = false;
		AfxMessageBox(L"Failed to save the file.");
	}
}

std::vector<std::vector<CPointF64>>* CMDIPainterDoc::GetPVct2Lines()
{
	return m_pVct2Lines;
}

void CMDIPainterDoc::SetPVct2Lines(std::vector<std::vector<CPointF64>>* pVct2Temp)
{
	m_pVct2Lines = pVct2Temp;
}

void CMDIPainterDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_bEdited);
}

void CMDIPainterDoc::HandleFileClose()
{
	CMainFrame* pSMainFrame = nullptr;
	CChildFrame* pSChildFrame = nullptr;

	m_bClosedDone = false;

	do
	{
		pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		pSChildFrame = dynamic_cast<CChildFrame*>(pSMainFrame->GetActiveFrame());

		if(!pSChildFrame)
			break;

		if(!m_bEdited)
		{
			m_bClosedDone = true;
			break;
		}

		int32_t i32StatusCode = MessageBox(pSMainFrame->m_hWnd, L"There is an unsaved document " + m_strFileName + L". Do you want to save it?", L"MDIPainter", MB_YESNOCANCEL | MB_ICONWARNING | MB_TASKMODAL);

		switch(i32StatusCode)
		{
		case IDCANCEL:
			break;

		case IDYES:
			{
				OnFileSave();

				if(m_bSaved && m_bSavedDone)
					m_bClosedDone = true;
			}
			break;

		case IDNO:
			{
				m_bClosedDone = true;
			}
		}
	}
	while(false);
}

bool CMDIPainterDoc::CheckIsClosed()
{
	return m_bClosedDone;
}

bool CMDIPainterDoc::CheckIsEdited()
{
	return m_bEdited;
}

void CMDIPainterDoc::InitializeTitle()
{
	SetTitle(m_strFileName);
}

bool CMDIPainterDoc::SetTable()
{
	bool bReturn = false;
	CTable* pSTable = nullptr;

	do 
	{
		CMainFrame* pSMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

		if(!pSMainFrame)
			break;

		pSTable = pSMainFrame->GetTable(this);

		if(pSTable)
		{
			pSTable->SetTitle(m_strFileName);
			pSTable->SetEdited(m_bEdited);

			POSITION sPos = GetFirstViewPosition();

			if(sPos)
			{
				CMDIPainterView* pSView = dynamic_cast<CMDIPainterView*>(GetNextView(sPos));

				if(pSView)
					pSView->SetTable();
				else
				{
					pSTable->SetOffsetX(0.0);
					pSTable->SetOffsetY(0.0);
					pSTable->SetScale(1.0);
				}
			}
		}

		bReturn = true;
	}
	while(false);

	return bReturn;
}