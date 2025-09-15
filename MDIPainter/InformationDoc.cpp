// InformationDoc.cpp: 구현 파일
//

#include "pch.h"
#include "MDIPainter.h"
#include "ChildInfoFrm.h"
#include "InformationDoc.h"
#include "InformationView.h"


// InformationDoc

IMPLEMENT_DYNCREATE(CInformationDoc, CDocument)

CInformationDoc::CInformationDoc()
{
	m_pVctTables = new std::vector<CTable*>;
}

BOOL CInformationDoc::OnNewDocument()
{
	BOOL bReturn = FALSE;

	if(CDocument::OnNewDocument())
		bReturn = TRUE;

	return bReturn;
}

CInformationDoc::~CInformationDoc()
{
	if(m_pVctTables) 
	{
		for(size_t stI = 0; stI < m_pVctTables->size(); stI++)
		{
			delete m_pVctTables->at(stI);

			m_pVctTables->at(stI) = nullptr;
		}
		delete m_pVctTables;

		m_pVctTables = nullptr;
	}
}


BEGIN_MESSAGE_MAP(CInformationDoc, CDocument)
END_MESSAGE_MAP()


// InformationDoc 진단

#ifdef _DEBUG
void CInformationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CInformationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// InformationDoc serialization

void CInformationDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}
#endif

// InformationDoc 명령
CTable* CInformationDoc::CreateNewTable()
{
	CTable* pSTable = nullptr;

	do 
	{
		POSITION sPos = GetFirstViewPosition();

		if(!sPos)
			break;

		CInformationView* pSView = dynamic_cast<CInformationView*>(GetNextView(sPos));

		if(!pSView)
			break;

		pSTable = pSView->CreateNewTable();
	}
	while(false);

	return pSTable;
}

std::vector<CTable*>* CInformationDoc::GetTables()
{
	return m_pVctTables;
}

bool CInformationDoc::DeleteTable(CTable* pSTable)
{
	bool bReturn = false;

	do 
	{
		if(!pSTable)
			break;

		if(!m_pVctTables)
			break;

		// 관리중인 벡터에서 해당 값 제거
		m_pVctTables->erase(std::remove(m_pVctTables->begin(), m_pVctTables->end(), pSTable), m_pVctTables->end());


		// 동적 할당 제거
		delete pSTable;

		pSTable = nullptr;

		POSITION sPos = GetFirstViewPosition();

		CInformationView* pSView = dynamic_cast<CInformationView*>(GetNextView(sPos));

		if(!pSView)
			break;

		//	벡터 변동에 따른 최신화
		if(!pSView->SetTablePosition())
			break;

		pSView->SetScroll();
		pSView->Invalidate(TRUE);

		bReturn = true;
	}
	while(false);

	return bReturn;
}