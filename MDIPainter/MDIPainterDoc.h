
// MDIPainterDoc.h: CMDIPainterDoc 클래스의 인터페이스
//


#pragma once

#include <vector>

#include "PointF64.h"
#include "Table.h"

class CMDIPainterDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CMDIPainterDoc() noexcept;
	DECLARE_DYNCREATE(CMDIPainterDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CMDIPainterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

// 멤버 요소
private:
	CString m_strDefaultTitle;
	CString m_strDefaultExtension;
	CString m_strFileName;

	std::vector<std::vector<CPointF64>>* m_pVct2Lines;

	bool m_bEdited;
	bool m_bFileOpened;
	bool m_bSaved;
	bool m_bSavedDone;
	bool m_bClosedDone;

	CString m_strDocNumber;

public:
	uint32_t CalculateCRC32(uint8_t* pU8Data, size_t stLength);
	std::vector<std::vector<CPointF64>>* GetPVct2Lines();
	void SetPVct2Lines(std::vector<std::vector<CPointF64>>* pVct2Temp);

	void SetEdited();
	void SaveCurrentFile();
	void HandleFileClose();
	bool CheckIsClosed();
	bool CheckIsEdited();
	void InitializeTitle();
	bool SetTable();

	afx_msg void OnFileClose();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
};
