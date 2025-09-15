#pragma once

#include <vector>
#include <algorithm>
#include "Table.h"

// InformationDoc 문서

class CInformationDoc : public CDocument
{
	DECLARE_DYNCREATE(CInformationDoc)

public:
	CInformationDoc();
	virtual ~CInformationDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // 문서 입/출력을 위해 재정의되었습니다.
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	std::vector<CTable*>* m_pVctTables;

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()

public:
	std::vector<CTable*>* GetTables();
	CTable* CreateNewTable();
	bool DeleteTable(CTable* pSTable);
};
