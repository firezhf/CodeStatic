//#pragma once
#include "stdafx.h"

//**************************************************************************************
//           CommonUI team source code comment format
// Single line comment : 
//		/* Date Responder@SSS [Version][Phase][Type][Req/Bug KeyWord] */
// Multi line comment : 
//		/* Date Responder@SSS [Version][Phase][Type][Req or Bug KeyWord] Start */
//		/* Date Responder@SSS [Version][Phase][Type][Req or Bug KeyWord] End */
//
// Sample    : /* 140124 ZHF@SSS [v21.20][Alpha][Modify][Drv_V21_10_a_FamiCom-0023] */
// Responder : Abbreviated name (three character e.q. ZHF)
// Date		 : YYMMDD -> 140124
// Version	 : v00.00 -> v21.20
// Phase	 : PreAlpha, Alpha, Beta, FC (Ignore AlphaN,BetaN,FCN  N = 1,2,..)
// Type		 : Add, Delete, Modify
// Req or Bug KeyWord : [JPEGCompressionPCL]
//*************************************************************************************/

#define COMMENT_KEY_11				_T("//")
#define COMMENT_KEY_1X				_T("/*")
#define COMMENT_KEY_X1				_T("*/")
#define COMMENT_KEY_AT				_T("@")
#define COMMENT_KEY_LBRACKET		_T("[")
#define COMMENT_KEY_RBRACKET		_T("]")
#define COMMENT_KEY_START			_T("start")
#define COMMENT_KEY_END				_T("end")

#define COMMENT_KEY_RESPONDER_LENGTH	3
#define COMMENT_KEY_COMPANY_LENGTH		3
#define COMMENT_KEY_DATE_LENGTH_YYMMDD	6
#define COMMENT_KEY_DATE_LENGTH_YYYYMMDD	8
//Use to determine phase key word, these data should be read from config document in the future
#define COMMENT_KEY_PHASE_LENGTH	9
#define COMMENT_KEY_ALPHA			_T("alpha")
#define COMMENT_KEY_BETA			_T("beta")
#define COMMENT_KEY_FC				_T("fc")

#define BLOCK_KEY_TYPE_LENGTH		6
#define BLOCK_KEY_ADD				_T("add")
#define BLOCK_KEY_DEL				_T("del")
#define BLOCK_KEY_MOD				_T("mod")
#define BLOCK_KEY_NONE				_T("None")


//#define LINETYPE_RESERVED				0x00000000 // reserved
#define LINETYPE_BLANK					0x00000000 // Blank line
#define LINETYPE_CODE_PURE				0x00010000 // Pure Source code line
#define LINETYPE_CODE_WITHCOMMENT_11	0x00010001 // Source code line Mix with Comment 
#define LINETYPE_CODE_WITHCOMMENT_1X	0x00010002 // Source code line Mix with Comment 
#define LINETYPE_CODE_WITHCOMMENT_X1	0x00010004 // Dummy define, cannot use in any case 
#define LINETYPE_CODE_WITHCOMMENT_1XX1	0x00010006 // Source code line Mix with Comment 
#define LINETYPE_COMMENT_11				0x00000001 // Single Comment line
#define LINETYPE_COMMENT_1X				0x00000002 // The start line of multi-Comment
#define LINETYPE_COMMENT_X1				0x00000004 // The end line of multi-Comment
#define LINETYPE_COMMENT_1XX1			0x00000006 // Single Comment line
#define LINETYPE_COMMENT_1X__X1			0x00000008 // multi Comment line(in comment body)

#define BLOCK_WARNING_NONE					0x00000000
#define BLOCK_WARNING_DATE					0x00000001
#define BLOCK_WARNING_RESPONDER				0x00000002
#define BLOCK_WARNING_COMPANY				0x00000004
#define BLOCK_WARNING_VERSION				0x00000008
#define BLOCK_WARNING_PHASE					0x00000010
#define BLOCK_WARNING_TYPE					0x00000020
#define BLOCK_WARNING_KEYWORD				0x00000040
#define BLOCK_WARNING_START					0x00000080
#define BLOCK_WARNING_END					0x00000100
#define BLOCK_WARNING_1XX1					0x10000000

#define BLOCKlIST_FORMAL					0x0001
#define BLOCKlIST_TEMP						0x0002
#define BLOCKlIST_INVALID					0x0004


class CCodeBlock
{
public:
	CCodeBlock();
	~CCodeBlock();
	CCodeBlock(const CCodeBlock& cbSrc)
	{
		dwWarning = cbSrc.dwWarning;
		strFile = cbSrc.strFile;
		//nCommentType = cbSrc.nCommentType;
		nLineStart = cbSrc.nLineStart;
		nLineEnd = cbSrc.nLineEnd;
		nCountComment = cbSrc.nCountComment;
		nCountBlank = cbSrc.nCountBlank;
		nCountCode = cbSrc.nCountCode;
		POSITION pos = cbSrc.lstCode.GetHeadPosition();
		while (pos)
		{
			int nLine = cbSrc.lstCode.GetNext(pos);
			lstCode.AddTail(nLine);
		}
		eType = cbSrc.eType;
		strDate = cbSrc.strDate;
		strResponder = cbSrc.strResponder;
		strCompany = cbSrc.strCompany;
		strVersion = cbSrc.strVersion;
		strPhase = cbSrc.strPhase;
		strListKeyWord.RemoveAll();
		pos = cbSrc.strListKeyWord.GetHeadPosition();
		while (pos)
		{
			CString str = cbSrc.strListKeyWord.GetNext(pos);
			strListKeyWord.AddTail(str);
		}
	};

	DWORD dwWarning;
	//Location information
	CString strFile;
	//int nCommentType;
	int nLineStart;
	int nLineEnd;
		
	//Statistics
	int nCountComment;
	int nCountBlank;
	int nCountCode;
	CList<int, int&> lstCode; //Detailed source code line number

	//Block information 
	enum CODETYPE{	add = 1,
					del = 2,
					mod = 3,
					none= 4} eType;
	CString strDate;
	CString strResponder;
	CString strCompany;
	CString strVersion;
	CString strPhase;
	CStringList strListKeyWord;

	void Init();

public:
	inline CCodeBlock& operator=(const CCodeBlock& cbSrc)
	{
		dwWarning = cbSrc.dwWarning;
		strFile = cbSrc.strFile;
		//nCommentType = cbSrc.nCommentType;
		nLineStart = cbSrc.nLineStart;
		nLineEnd = cbSrc.nLineEnd;
		nCountComment = cbSrc.nCountComment;
		nCountBlank = cbSrc.nCountBlank;
		nCountCode = cbSrc.nCountCode;
		POSITION pos = cbSrc.lstCode.GetHeadPosition();
		while (pos)
		{
			int nLine = cbSrc.lstCode.GetNext(pos);
			lstCode.AddTail(nLine);
		}
		eType = cbSrc.eType;
		strDate = cbSrc.strDate;
		strResponder = cbSrc.strResponder;
		strCompany = cbSrc.strCompany;
		strVersion = cbSrc.strVersion;
		strPhase = cbSrc.strPhase;
		strListKeyWord.RemoveAll();
		pos = cbSrc.strListKeyWord.GetHeadPosition();
		while (pos)
		{
			CString str = cbSrc.strListKeyWord.GetNext(pos);
			strListKeyWord.AddTail(str);
		}
		return (*this);
	};
	inline BOOL operator==(const CCodeBlock& cbSrc)
	{
		if (dwWarning != cbSrc.dwWarning)
			return FALSE;
		if (strFile != cbSrc.strFile)
			return FALSE;
		if (eType != cbSrc.eType)
			return FALSE;
		if (strDate != cbSrc.strDate)
			return FALSE;
		if (strResponder != cbSrc.strResponder)
			return FALSE;
		if (strCompany != cbSrc.strCompany)
			return FALSE;
		if (strVersion != cbSrc.strVersion)
			return FALSE;
		if (strPhase != cbSrc.strPhase)
			return FALSE;
		int nCount = strListKeyWord.GetCount();
		if (nCount != cbSrc.strListKeyWord.GetCount())
			return FALSE;
		while (nCount)
		{
			CString str = strListKeyWord.GetAt(strListKeyWord.FindIndex(nCount-1));
			if (str != cbSrc.strListKeyWord.GetAt(strListKeyWord.FindIndex(nCount-1)))
				return FALSE;
			nCount--;
		}
		return TRUE;
	};
};

typedef CList<CCodeBlock, CCodeBlock&> CCBlst;

class CCodeParser
{
public:
	CCodeParser();
	~CCodeParser();

private:
	//Count total lines for comment, blank and source code
	int m_nCountComment;
	int m_nCountBlank;
	int m_nCountCode;
	CString m_strFile;
	CStringList m_strlistFile;

	CCBlst m_lstCodeBlock;
	CCBlst m_lstCodeBlockInvalid; //Store invalid block 
	CCBlst m_lstCodeBlockTmp;

public:
	void InitFileParse(CString strFile);
	void BrowseDir(CString strPath);
	int ParseFile(CString strFile);
	int ParsePath(CString strPath);
	int ParseLine(CString strLine, BOOL bInComment = FALSE);
	CCodeBlock ParseComment1XX1(const CString strComment1XX1, int nLineNumber,BOOL& bAddFormalBlocklist);
	int SplitComment1XX1(const CString strComment1XX1, CStringList& lst1XX1);
	int ParseSingleComment1XX1(const CString strComment1XX1, int nLineNumber, CCodeBlock& cbReturn);

	WORD AddSingleLineBlock(CCodeBlock& cbSingleLine);
	WORD AddMultiLineBlock(CCodeBlock& cbMultiLine);

	void OutputToCSV(CString strFile);
	void DumpBlock(CCodeBlock& cbSrc);
	void DumpBlockList(CCBlst& lstCodeBlock);
private:
	void OutputBlock(CStdioFile& stdFile, CCodeBlock& cbSrc);
};