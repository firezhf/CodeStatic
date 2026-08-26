#include "stdafx.h"
#include "CodeParser.h"

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

CCodeBlock::CCodeBlock()
{
	dwWarning = BLOCK_WARNING_NONE;
	strFile = _T("");
	//nCommentType = 0;
	nLineStart = 0;
	nLineEnd = 0;
	nCountComment = 0;
	nCountBlank = 0;
	nCountCode = 0;
	lstCode.RemoveAll();
	eType = CODETYPE::none;
	strDate = _T("");
	strResponder = _T("");
	strCompany = _T("");
	strVersion = _T("");
	strPhase = _T("");
	strListKeyWord.RemoveAll();
}

CCodeBlock::~CCodeBlock()
{
}

void CCodeBlock::Init()
{
	dwWarning = BLOCK_WARNING_NONE;
	strFile = _T("");
	//nCommentType = 0;
	nLineStart = 0;
	nLineEnd = 0;
	nCountComment = 0;
	nCountBlank = 0;
	nCountCode = 0;
	lstCode.RemoveAll();
	eType = CODETYPE::none;
	strDate = _T("");
	strResponder = _T("");
	strCompany = _T("");
	strVersion = _T("");
	strPhase = _T("");
	strListKeyWord.RemoveAll();
}

CCodeParser::CCodeParser()
{
}


CCodeParser::~CCodeParser()
{
}

void CCodeParser::InitFileParse(CString strFile)
{
	m_nCountComment = 0;
	m_nCountBlank = 0;
	m_nCountCode = 0;
	m_strFile = strFile;
	//m_lstCodeBlock.RemoveAll();
	m_lstCodeBlockTmp.RemoveAll();
}

void CCodeParser::BrowseDir(CString strPath)
{
	CFileFind ff;
	CString szDir = strPath;
	if (szDir.Right(1) != "\\")
		szDir += "\\";
	szDir += "*.*";

	BOOL res = ff.FindFile(szDir);
	while (res)
	{
		res = ff.FindNextFile();
		if (ff.IsDirectory() && !ff.IsDots())
		{
			BrowseDir(ff.GetFilePath());
		}
		else if (!ff.IsDirectory() && !ff.IsDots())
		{
			CString str = ff.GetFilePath();
			CString strT(_T("."));
			int nIndex = str.Find(strT);
			int nCount = str.GetLength();
			CString sEx = str.Right(nCount - nIndex - 1);
			sEx.MakeLower();
			if (sEx == _T("cpp") || sEx == _T("c") || sEx == _T("h") || sEx == _T("rc") || sEx == _T("plc"))
			{
				m_strlistFile.AddTail(str);
			}

		}
	}
	ff.Close();
}

int CCodeParser::ParsePath(CString strPath)
{
	DWORD dwAttr = ::GetFileAttributes(strPath);
	if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
	{
		BrowseDir(strPath);
	}
	else // normal file
	{
		CString str = strPath;
		CString strT(_T("."));
		int nIndex = str.Find(strT);
		int nCount = str.GetLength();
		CString sEx = str.Right(nCount - nIndex - 1);
		sEx.MakeLower();
		if (sEx == _T("cpp") || sEx == _T("c") || sEx == _T("h") || sEx == _T("rc") || sEx == _T("plc"))
		{
			//CCodeParser codeparser;
			//codeparser.ParseFile(str);
			m_strlistFile.AddTail(strPath);
		}
	}

	m_lstCodeBlockInvalid.RemoveAll(); // Init invalid list 
	//Parse File
	POSITION posTotal = m_strlistFile.GetHeadPosition();
	int nCount = 1;
	while (posTotal)
	{
		CString strFile = m_strlistFile.GetNext(posTotal);
		TRACE("---Start to Parse file[%d] %S---\n", nCount++, strFile);
		ParseFile(strFile); //Parse file
	}

	//DumpBlockList(m_lstCodeBlock);// Temp code only dump

	return 0;
}

int CCodeParser::ParseFile(CString strFile)
{
	CStdioFile stdFile;
	CFileException fileException;
	if (stdFile.Open(strFile, CFile::modeRead | CFile::shareDenyWrite, &fileException) == FALSE)
	{
		TRACE("Can't open file %s, error=%u\n", strFile, fileException.m_cause);
		return 0;
	}
	else
	{
		InitFileParse(strFile);
	}
	
	BOOL bInComment = FALSE;
	int nLineNumber = 0;
	CString strLineContent;
	while (stdFile.ReadString(strLineContent))
	{
		nLineNumber++;
		int nLineType = ParseLine(strLineContent, bInComment);
		switch (nLineType)
		{
		//case LINETYPE_RESERVED :
		//	break;
		case LINETYPE_BLANK :
			m_nCountBlank++;
			if (!m_lstCodeBlockTmp.IsEmpty())
			{
				m_lstCodeBlockTmp.GetHead().nCountBlank++;
			}
			break;
		case LINETYPE_CODE_PURE:
		case LINETYPE_CODE_WITHCOMMENT_11:
			m_nCountCode++;
			if (!m_lstCodeBlockTmp.IsEmpty())
			{
				m_lstCodeBlockTmp.GetHead().nCountCode++;
				m_lstCodeBlockTmp.GetHead().lstCode.AddTail(nLineNumber);
			}
			break;
		case LINETYPE_CODE_WITHCOMMENT_1X:
			m_nCountCode++;
			if (!m_lstCodeBlockTmp.IsEmpty())
			{
				m_lstCodeBlockTmp.GetHead().nCountCode++;
				m_lstCodeBlockTmp.GetHead().lstCode.AddTail(nLineNumber);
			}
			bInComment = TRUE;
			break;
		case LINETYPE_CODE_WITHCOMMENT_1XX1:
		{
			m_nCountCode++;
			BOOL bFormal = FALSE;
			CCodeBlock cbCurrent = ParseComment1XX1(strLineContent, nLineNumber, bFormal);
			if (cbCurrent.nLineStart == cbCurrent.nLineEnd && cbCurrent.nLineStart != 0) //Single line block comment
			{
				if (!m_lstCodeBlockTmp.IsEmpty()) //If has not finished block 
				{
					if (m_lstCodeBlockTmp.GetHead().strDate >= cbCurrent.strDate) //the latest date block
					{
						m_lstCodeBlockTmp.GetHead().nCountCode++;
						m_lstCodeBlockTmp.GetHead().lstCode.AddTail(nLineNumber);
					}
					else if (bFormal)
					{
						m_lstCodeBlock.GetTail().nCountCode++;
						m_lstCodeBlock.GetTail().lstCode.AddTail(nLineNumber);
					}
				}
				else if (bFormal)
				{
					m_lstCodeBlock.GetTail().nCountCode++;
					m_lstCodeBlock.GetTail().lstCode.AddTail(nLineNumber);
				}
			}
			else if (cbCurrent.nLineStart != 0 && cbCurrent.nLineEnd == 0) // Multi-line block comment at start line
			{
				m_lstCodeBlockTmp.GetHead().nCountCode++;
				m_lstCodeBlockTmp.GetHead().lstCode.AddTail(nLineNumber);
			}
			else if (cbCurrent.nLineStart == 0 && cbCurrent.nLineEnd != 0) // Multi-line block comment at end line
			{
				if (!m_lstCodeBlockTmp.IsEmpty())
				{
					if (m_lstCodeBlockTmp.GetHead().strDate >= cbCurrent.strDate) //the latest date block
					{
						m_lstCodeBlockTmp.GetHead().nCountCode++;
						m_lstCodeBlockTmp.GetHead().lstCode.AddTail(nLineNumber);
					}
					else  if (bFormal)
					{
						m_lstCodeBlock.GetTail().nCountCode++;
						m_lstCodeBlock.GetTail().lstCode.AddTail(nLineNumber);
					}
				}
				else  if (bFormal)
				{
					m_lstCodeBlock.GetTail().nCountCode++;
					m_lstCodeBlock.GetTail().lstCode.AddTail(nLineNumber);
				}
			}
			else if (!m_lstCodeBlockTmp.IsEmpty())
			{
				m_lstCodeBlockTmp.GetHead().nCountCode++;
				m_lstCodeBlockTmp.GetHead().lstCode.AddTail(nLineNumber);
			}
		}
			break;
		case LINETYPE_COMMENT_11: //pure comment
		case LINETYPE_COMMENT_1X__X1: //In comment body
			m_nCountComment++;
			if (!m_lstCodeBlockTmp.IsEmpty())
			{
				m_lstCodeBlockTmp.GetHead().nCountComment++;
			}
			break;
		case LINETYPE_COMMENT_1X:
			m_nCountComment++;
			if (!m_lstCodeBlockTmp.IsEmpty())
			{
				m_lstCodeBlockTmp.GetHead().nCountComment++;
			}
			bInComment = TRUE; //Start comment line
			break;
		case LINETYPE_COMMENT_X1:
			m_nCountComment++;
			if (!m_lstCodeBlockTmp.IsEmpty())
			{
				m_lstCodeBlockTmp.GetHead().nCountComment++;
			}
			bInComment = FALSE; //End comment line
			break;
		case LINETYPE_COMMENT_1XX1: // has block comment
		{
			m_nCountComment++;
			BOOL bFormal = FALSE;
			CCodeBlock cbTmp = ParseComment1XX1(strLineContent, nLineNumber, bFormal);
			if (cbTmp.nLineStart == cbTmp.nLineEnd && cbTmp.nLineStart != 0) //no this case : cbTmp.nCountCode = 0
			{
				if (!m_lstCodeBlockTmp.IsEmpty())
				{
					if (m_lstCodeBlockTmp.GetHead().strDate >= cbTmp.strDate) //the latest date block
						m_lstCodeBlockTmp.GetHead().nCountComment++;
					else if(bFormal)
					{
						m_lstCodeBlock.GetTail().nCountComment++; // adjust comment count
						m_lstCodeBlock.GetTail().nCountCode = 1; // adjust source code count
						m_lstCodeBlock.GetTail().lstCode.AddTail(nLineNumber);
					}
				}
				else if (bFormal)
				{
					m_lstCodeBlock.GetTail().nCountComment++; // adjust comment count
					m_lstCodeBlock.GetTail().nCountCode = 1; // adjust source code count
					m_lstCodeBlock.GetTail().lstCode.AddTail(nLineNumber);
				}
			}
			else if (cbTmp.nLineStart !=0 && cbTmp.nLineEnd == 0)
			{
				m_lstCodeBlockTmp.GetHead().nCountComment++;
			}
			else  if (cbTmp.nLineStart == 0 && cbTmp.nLineEnd != 0 && bFormal)
			{
				if (!m_lstCodeBlock.IsEmpty())
					m_lstCodeBlock.GetTail().nCountComment++; // adjust comment count
			} 
			else if (!m_lstCodeBlockTmp.IsEmpty())// not empty && not single line block && not multline-end
			{
				m_lstCodeBlockTmp.GetHead().nCountComment++;
			}
			bInComment = FALSE; //End comment line
		}
			break;
		default:
			break;
		}
	}

	//Add invalid block to list
	POSITION posT = m_lstCodeBlockTmp.GetHeadPosition();
	while (posT)
	{
		CCodeBlock cb = m_lstCodeBlockTmp.GetNext(posT);
		cb.dwWarning |= BLOCK_WARNING_START;
		m_lstCodeBlockInvalid.AddTail(cb);
	}

	stdFile.Close();
	return 0;
}

int CCodeParser::ParseLine(CString strLine, BOOL bInComment/* = FALSE*/)
{
	int nLineType = 0;

	strLine.Trim();
	int nStrLenght = strLine.GetLength();
	
	//blank line ajudgement
	if (nStrLenght == 0)
	{
		nLineType = LINETYPE_BLANK;
		return nLineType;
	}
	
	//Analyze string
	int nIndex11 = strLine.Find(COMMENT_KEY_11);// look for a specific sub-string //
	int nIndex1X = strLine.Find(COMMENT_KEY_1X);// look for a specific sub-string /*
	int nIndexX1 = strLine.Find(COMMENT_KEY_X1);// look for a specific sub-string */
	
	if (bInComment) // In comment body
	{
		if (nIndexX1 >= 0) // To consider case [1X ..... X1.... source code........]
		{
			nLineType = LINETYPE_COMMENT_X1;
			if (nIndex1X != -1 && nIndex1X < nIndexX1)
				nLineType = LINETYPE_COMMENT_1XX1;
		}
		else
			nLineType = LINETYPE_COMMENT_1X__X1; // one line comment of comment body
		return nLineType;
	}

	//Not in comment body
	if (nIndex11 == -1) // Not find "//"
	{
		//to find "/*"
		if (nIndex1X == -1) 
			nLineType = LINETYPE_CODE_PURE;	// it is pure code line, To consider Case [...source code ....X1]
		else if (nIndex1X == 0) // a line begining with a "/*"
		{
			if (nIndexX1 == -1) // this is the first line of multi-line comment, the comment is not terminated
				nLineType = LINETYPE_COMMENT_1X;
			else
				nLineType = LINETYPE_COMMENT_1XX1; //this is one line pure comment.
		}
		else // nIndex1X > 0 means code with comment
		{
			if (nIndexX1 == -1)
				nLineType = LINETYPE_CODE_WITHCOMMENT_1X;
			else //must nIndex1X < nIndexX1
				nLineType = LINETYPE_CODE_WITHCOMMENT_1XX1;
		}
	}
	else if (nIndex11 == 0)	//a line begining with a "//", this line is pure comment
	{
		if (nIndex1X != -1 && nIndexX1 != -1 && nIndex1X < nIndexX1)
			nLineType = LINETYPE_COMMENT_1XX1;
		else 
			nLineType = LINETYPE_COMMENT_11;
	}
	else //this line is code with comment
	{
		if (nIndex1X != -1 && nIndexX1 != -1 && nIndex1X < nIndexX1)
			nLineType = LINETYPE_CODE_WITHCOMMENT_1XX1;
		else 
			nLineType = LINETYPE_CODE_WITHCOMMENT_11;
	}

	return nLineType;
}

//One line with multi 1XX1 comment, 
/******************************* <-68->   1tab = 4space ************
[Name]
Split Comment
[Action]
For one line with multi 1xx1 comment, split each comment store in stringlist
[Input]
const CString strComment1xx1 
[InOut]
CStringList& lst1XX1 
[Output]
total count of Comment 1xx1
********************************************************************/
int CCodeParser::SplitComment1XX1(const CString strComment1XX1, CStringList& lst1XX1)
{
	CString strWork = strComment1XX1;
	lst1XX1.RemoveAll();
	//CStringList lst1XX1;
	int nIndex1X = strWork.Find(COMMENT_KEY_1X);// look for a specific sub-string /*
	int nIndexX1 = strWork.Find(COMMENT_KEY_X1);// look for a specific sub-string */
	while (nIndex1X != -1 && nIndexX1 != -1 && nIndex1X < nIndexX1)
	{
		CString strTemp = strWork.Mid(nIndex1X, nIndexX1 - nIndex1X + 2);
		lst1XX1.AddTail(strTemp);
		strWork.Delete(nIndex1X, nIndexX1 - nIndex1X + 2);
		nIndex1X = strWork.Find(COMMENT_KEY_1X);// look for a specific sub-string /*
		nIndexX1 = strWork.Find(COMMENT_KEY_X1);// look for a specific sub-string */
	}
	int nCount = lst1XX1.GetCount();
	return nCount;
}

/******************************* <-68->   1tab = 4space ************
[Name]
ParseComment1XX1
[Action]
For one line with multi 1xx1 comment, parse each comment
[Input]
const CString strComment1xx1
[InOut]
CStringList& lst1XX1
[Output]
total count of Comment 1xx1
********************************************************************/
CCodeBlock CCodeParser::ParseComment1XX1(const CString strComment1XX1, int nLineNumber, BOOL& bAddFormalBlocklist)
{
	CCBlst lstBlocksInLine; //One line has multi 1XX1, so need list to store them.
	CCodeBlock cbActive; // Key block, use it to judge how to calculate the codecount++ or nCountComment++
	CStringList lst1XX1;
	bAddFormalBlocklist = FALSE;
	int nCount = SplitComment1XX1(strComment1XX1, lst1XX1);
	if (nCount <= 0)
		return cbActive;

	POSITION pos = lst1XX1.GetHeadPosition();
	while (pos)//Parse each 1XX1 
	{
		CString str = lst1XX1.GetNext(pos);
		CCodeBlock cbWork;
		int nRet = ParseSingleComment1XX1(str, nLineNumber, cbWork);
		
		//ToDo based on different WARNING level
		if (nRet != 0) 
		{
			if (nRet & ~(BLOCK_WARNING_DATE | BLOCK_WARNING_RESPONDER | BLOCK_WARNING_COMPANY | BLOCK_WARNING_1XX1)) //Determines invalid block
			{
				m_lstCodeBlockInvalid.AddTail(cbWork);
			}
			continue;
		}

		if (cbActive.strDate < cbWork.strDate) //Keep on the latest block, in order to specified which block should be line calculated in ParseFile() function
			cbActive = cbWork;
		if (lstBlocksInLine.GetCount() == 0)
		{
			lstBlocksInLine.AddHead(cbWork);
		}
		else  // Sort by date from small to big
		{
			POSITION p = lstBlocksInLine.GetHeadPosition();
			while (p)
			{
				POSITION pt = p;
				CCodeBlock cT = lstBlocksInLine.GetNext(p);
				if (cbWork.strDate < cT.strDate)
				{
					lstBlocksInLine.InsertBefore(pt, cbWork);
					break;
				}
				else if (!p)
				{
					lstBlocksInLine.AddTail(cbWork);
				}
			}
		}
	}

	POSITION p = lstBlocksInLine.GetHeadPosition();
	WORD wReturn = 0;
	while (p)
	{
		CCodeBlock c = lstBlocksInLine.GetNext(p);
		if (c.nLineStart == c.nLineEnd && c.nLineStart != 0) //no this case : cbTmp.nCountCode = 0
		{
			wReturn |= AddSingleLineBlock(c);
		}
		else if (c.nLineStart != 0 && c.nLineEnd == 0)
		{
			wReturn |= AddMultiLineBlock(c);
		}
		else  if (c.nLineStart == 0 && c.nLineEnd != 0)
		{
			wReturn |= AddMultiLineBlock(c);
		}
	}

	if (wReturn&BLOCKlIST_FORMAL)
		bAddFormalBlocklist = TRUE;
	if (lstBlocksInLine.GetCount())
		cbActive = lstBlocksInLine.GetTail();//Get the latest block (strDate is the biggest)
	return cbActive;
}

int CCodeParser::ParseSingleComment1XX1(const CString strComment1XX1, int nLineNumber, CCodeBlock& cbReturn)
{
	int nRet = BLOCK_WARNING_NONE;
	cbReturn.Init();
	cbReturn.strFile = m_strFile;

	int nIndex1X = strComment1XX1.Find(COMMENT_KEY_1X);// look for a specific sub-string /*
	int nIndexX1 = strComment1XX1.Find(COMMENT_KEY_X1);// look for a specific sub-string */
	int nIndexAT = strComment1XX1.Find(COMMENT_KEY_AT);// look for a specific sub-string "@"
	int nIndexBrackL = strComment1XX1.Find(COMMENT_KEY_LBRACKET);// look for a specific sub-string "["
	int nIndexBrackR = strComment1XX1.Find(COMMENT_KEY_RBRACKET);// look for a specific sub-string "]"

	if (nIndex1X == -1 || nIndexX1 == -1)
		return nRet|BLOCK_WARNING_1XX1;
	if (nIndexAT == -1 || nIndexBrackL == -1 || nIndexBrackR == -1)
		return nRet | BLOCK_WARNING_RESPONDER | BLOCK_WARNING_COMPANY;

	//Get Date from comment method 1
	//int nDateLength = 6; //YYMMDD default format 6 bit
	//if (nIndex1X + 2 + 1 + 6 + 1 + 3 < nIndexAT) // YYYYMMDD compatible 8 bit
	//	nDateLength = 8;
	//CString strDate = strLineBlock.Mid(nIndex1X + 2, nDateLength);

	//Get Date from comment method 2
	CString strTmp = strComment1XX1.Mid(nIndex1X + 2, nIndexAT - nIndex1X - 2);
	strTmp.Trim();
	CString strDate = strTmp.SpanIncluding(_T("0123456789"));
	int nDateLength = strDate.GetLength();
	if (nDateLength != COMMENT_KEY_DATE_LENGTH_YYMMDD)
	{
		if (nDateLength == COMMENT_KEY_DATE_LENGTH_YYYYMMDD)
		{
			strDate = strDate.Right(COMMENT_KEY_DATE_LENGTH_YYMMDD); //Unify the date format to YYMMDD
		}
		else
			return nRet | BLOCK_WARNING_DATE;
	}
	//

	//Get Responder from comment ZHF@SSS
	CString strResponder = strComment1XX1.Mid(nIndexAT - COMMENT_KEY_RESPONDER_LENGTH, COMMENT_KEY_RESPONDER_LENGTH);
	//Get Company from comment ZHF@SSS
	CString strCompany = strComment1XX1.Mid(nIndexAT + 1, COMMENT_KEY_COMPANY_LENGTH);
	//strCompany.Trim();

	//Get function Key Word from comment [Key Word]
	strTmp = strComment1XX1;
	CStringList strList;
	int nLength = nIndexBrackR - nIndexBrackL;
	while (nIndexBrackL != -1 && nIndexBrackR != -1 && nLength > 2 && strTmp.IsEmpty() != TRUE)
	{
		CString str = strTmp.Mid(nIndexBrackL + 1, nLength - 1);
		strList.AddTail(str);
		strTmp.Delete(nIndexBrackL, nLength + 1);

		nIndexBrackL = strTmp.Find(COMMENT_KEY_LBRACKET);// look for a specific sub-string "["
		nIndexBrackR = strTmp.Find(COMMENT_KEY_RBRACKET);// look for a specific sub-string "]"
		nLength = nIndexBrackR - nIndexBrackL;
	}

	//Determines block type : single line block ? Multiline block -Start ? -End ?
	strTmp.MakeLower();
	if (strTmp.Find(COMMENT_KEY_START) != -1)// look for a specific sub-string "Start"
	{
		//cbRet.nCommentType = LINETYPE_CODEBLOCK_START;
		cbReturn.nLineStart = nLineNumber;
	}
	else if (strTmp.Find(COMMENT_KEY_END) != -1)// look for a specific sub-string "End"
	{
		//cbRet.nCommentType = LINETYPE_CODEBLOCK_END;
		cbReturn.nLineEnd = nLineNumber;
	}
	else
	{
		//cbRet.nCommentType = LINETYPE_CODEBLOCK_SINGLE;
		cbReturn.nLineStart = nLineNumber;
		cbReturn.nLineEnd = nLineNumber;
	}

	// TODO: remove this code when final code is complete.
	// Temp code for analyze version, phase and type, it need update later
	// Determine version : character 'V'or'v'+ two decimal digits'XX' + dot '.' + two decimal digits'XX'
	// Determine phase : compare/include with nocase - prealpha, alpha, beta, fc
	CString strVersion = strList.GetCount() ? strList.RemoveHead() : _T("");
	
	//CString strPhase = strList.GetCount() ? strList.RemoveHead() : _T("");
	CString strPhase = _T("");
	POSITION pt = strList.GetHeadPosition();
	while (pt)
	{
		POSITION p = pt;
		CString str = strList.GetNext(pt);
		CString sT = str;
		sT.MakeLower();
		if (sT.GetLength() > COMMENT_KEY_PHASE_LENGTH)
		{
			continue;
		} 
		else if (sT.Find(COMMENT_KEY_ALPHA) != -1 || sT.Find(COMMENT_KEY_BETA) != -1 || sT.Find(COMMENT_KEY_FC) != -1)
		{
			strPhase = str;
			strList.RemoveAt(p);
			break;
		}
	}

	//Determines source code type : add, modify, delete
	//CString strType = strList.GetCount() ? strList.RemoveHead() : _T("");
	CString strType = _T("");
	CCodeBlock::CODETYPE nType = CCodeBlock::none;
	pt = strList.GetHeadPosition();
	while (pt)
	{
		POSITION p = pt;
		CString strType = strList.GetNext(pt);
		strType.MakeLower();
		if (strType.GetLength() > BLOCK_KEY_TYPE_LENGTH)
		{
			continue;
		}
		else if (strType.Find(BLOCK_KEY_ADD) != -1)
		{
			nType = CCodeBlock::add;
			strList.RemoveAt(p);
			break;
		}
		else if (strType.Find(BLOCK_KEY_DEL) != -1)
		{
			nType = CCodeBlock::del;
			strList.RemoveAt(p);
			break;
		}
		else if (strType.Find(BLOCK_KEY_MOD) != -1)
		{
			nType = CCodeBlock::mod;
			strList.RemoveAt(p);
			break;
		}
	}
	if (nType == CCodeBlock::none)
		 nRet |= BLOCK_WARNING_KEYWORD; //Temp modify for G+ source code statistics 160616

	//Initial statistics
	cbReturn.nCountComment = 0;
	cbReturn.nCountBlank = 0;
	cbReturn.nCountCode = 0;

	//Sets block information from parse result
	cbReturn.eType = nType;
	cbReturn.strDate = strDate;
	cbReturn.strResponder = strResponder;
	cbReturn.strCompany = strCompany;
	cbReturn.strVersion = strVersion;
	cbReturn.strPhase = strPhase;
	cbReturn.strListKeyWord.RemoveAll();


	if (strList.GetCount() == 0) // There are not Keyword //Temp modify for G+ source code statistics 160616
		nRet |= BLOCK_WARNING_KEYWORD;
	else
	{
		POSITION pos = strList.GetHeadPosition();
		while (pos)
		{
			CString str = strList.GetNext(pos);
			cbReturn.strListKeyWord.AddTail(str);
		}
	}

	//Sets block information from parse result
	cbReturn.dwWarning = nRet;

	return nRet;
}

//Adds the specified single line Block into list
WORD CCodeParser::AddSingleLineBlock(CCodeBlock& cbSingleLine)
{
	m_lstCodeBlock.AddTail(cbSingleLine);
	return BLOCKlIST_FORMAL;
}

//Adds the specified multi-line Block into list
WORD CCodeParser::AddMultiLineBlock(CCodeBlock& cbMultiLine)
{
	if (cbMultiLine.nLineStart != 0 && cbMultiLine.nLineEnd == 0) // Source code block - Start
	{
		//Calculated  only for the topmost level of nested/recursion layers
		if (m_lstCodeBlockTmp.GetCount() == 0)
		{
			m_lstCodeBlockTmp.AddHead(cbMultiLine);
		}
		else  // Sort by date from big/latest to small/old; sample: 140410->140409->140309->130309->120309...
		{
			POSITION p = m_lstCodeBlockTmp.GetHeadPosition();
			while (p)
			{
				POSITION pt = p;
				CCodeBlock c = m_lstCodeBlockTmp.GetNext(p);
				if (c.strDate <= cbMultiLine.strDate)
				{
					m_lstCodeBlockTmp.InsertBefore(pt, cbMultiLine); // Keep the sort order
					break;
				}
				else if (!p) //If step at end of the list, that means the data of cbMultiLine is smallest in the list.
				{
					m_lstCodeBlockTmp.AddTail(cbMultiLine);
				}
			}
		}
		return BLOCKlIST_TEMP;
	}
	else if (cbMultiLine.nLineStart == 0 && cbMultiLine.nLineEnd != 0) // Source code block - End
	{
		//Find cbMultiLine in temp list, add it in formal and remove it from temp list
		POSITION pos = m_lstCodeBlockTmp.GetHeadPosition();
		while (pos)
		{
			POSITION posOld = pos;
			CCodeBlock cbTmp = m_lstCodeBlockTmp.GetNext(pos);

			//find related block-start
			if (cbTmp == cbMultiLine)
			{
				cbTmp.nLineEnd = cbMultiLine.nLineEnd;
				m_lstCodeBlockTmp.RemoveAt(posOld); // remove item from nested/recursion layers set
				m_lstCodeBlock.AddTail(cbTmp);
				//break;
				return BLOCKlIST_FORMAL;
			}
			else if (!pos) //If step at end of the list, that means cbMultiLine is not found and add it to invalid list.
			{
				cbMultiLine.dwWarning |= BLOCK_WARNING_START;
				m_lstCodeBlockInvalid.AddTail(cbMultiLine);
				return BLOCKlIST_INVALID;
			}
		}
	}
	return BLOCKlIST_INVALID;
}

void CCodeParser::DumpBlock(CCodeBlock& cbSrc)
{
	TRACE("<<Start<<<<<<< Block Information <<<<<<<<<Start<<\n");
	TRACE("strFile = %X\n",			cbSrc.dwWarning);
	TRACE("strFile = %S\n",			cbSrc.strFile);
	TRACE("strDate = %S \n",		cbSrc.strDate);
	TRACE("strResponder = %S \n",	cbSrc.strResponder);
	TRACE("strCompany = %S \n",		cbSrc.strCompany);
	TRACE("strVersion = %S \n",		cbSrc.strVersion);
	TRACE("strPhase = %S \n",		cbSrc.strPhase);
	TRACE("eType = %S \n", cbSrc.eType == CCodeBlock::add ? BLOCK_KEY_ADD : cbSrc.eType == CCodeBlock::mod ? BLOCK_KEY_MOD : cbSrc.eType == CCodeBlock::del ? BLOCK_KEY_DEL : BLOCK_KEY_NONE);
	TRACE("nLineStart = %d \n",		cbSrc.nLineStart);
	TRACE("nLineEnd = %d \n",		cbSrc.nLineEnd);
	TRACE("nCountComment = %d \n",	cbSrc.nCountComment);
	TRACE("nCountBlank = %d \n",	cbSrc.nCountBlank);
	TRACE("nCountCode = %d \n",		cbSrc.nCountCode);
	POSITION pos = cbSrc.lstCode.GetHeadPosition();
	while (pos)
	{
		int nCode = cbSrc.lstCode.GetNext(pos);
		TRACE(" -lstCode = (%d) \n", nCode);
	}
	TRACE("strListKeyWord Count = %d\n", cbSrc.strListKeyWord.GetCount());
	pos = cbSrc.strListKeyWord.GetHeadPosition();
	while (pos)
	{
		CString str = cbSrc.strListKeyWord.GetNext(pos);
		TRACE(" -strListKeyWord = %S \n", str);
	}
	TRACE(">>End>>>>>>> Block Information >>>>>>>>>End>>\n");
}

void CCodeParser::DumpBlockList(CCBlst& lstCodeBlock)
{
	TRACE("<<Start<<<<<<< Block List Information <<<<<<<<<Start<<\n");
	int nCount = lstCodeBlock.GetCount();
	TRACE("Total Block Count = %d \n", nCount);
	POSITION posTotal = lstCodeBlock.GetHeadPosition();
	while (posTotal)
	{
		CCodeBlock cbHead = lstCodeBlock.GetNext(posTotal);
		DumpBlock(cbHead);
	}
	TRACE("Total Block Count = %d \n", nCount);
	TRACE(">>End>>>>>>>Block List Information >>>>>>>>>End>>\n");
}

void CCodeParser::OutputToCSV(CString strFile)
{
	CStdioFile stdFile;
	CFileException fileException;
	if (stdFile.Open(strFile, CFile::modeWrite | CFile::shareDenyWrite | CFile::modeCreate | CFile::typeText, &fileException) == FALSE)
	{
		TRACE("Can't open file %s, error=%u\n", strFile, fileException.m_cause);
		return ;
	}
	else
	{
		//Set table header
		stdFile.WriteString(_T("WarningCode,File,Date,Responder,Company,Version,Phase,Type,StartLine,EndnLine,Comment,Blank,Code,CodeDetail,KeyWord Count,KeyWord\n"));

		//Archive parsed blocks
		int nCount = m_lstCodeBlock.GetCount();
		POSITION posTotal = m_lstCodeBlock.GetHeadPosition();
		while (posTotal)		// Loop through each block in list
		{
			CCodeBlock cbSrc = m_lstCodeBlock.GetNext(posTotal);
			OutputBlock(stdFile, cbSrc);
		}

		//Any block in temp list when finished parse a file is invalid block
		nCount = m_lstCodeBlockInvalid.GetCount();
		posTotal = m_lstCodeBlockInvalid.GetHeadPosition();
		while (posTotal)		// Loop through each block in list
		{
			CCodeBlock cbSrc = m_lstCodeBlockInvalid.GetNext(posTotal);
			OutputBlock(stdFile, cbSrc);
		}

	}
	stdFile.Close();
}

//Private function only for archive a block, can not use it independently
//Not includes the header name of csv table
void CCodeParser::OutputBlock(CStdioFile& stdFile, CCodeBlock& cbSrc)
{
	CString strWarning;
	strWarning.Format(_T("%X"), cbSrc.dwWarning);
	stdFile.WriteString(strWarning);
	stdFile.WriteString(_T(","));

	stdFile.WriteString(cbSrc.strFile);
	stdFile.WriteString(_T(","));

	if (cbSrc.strDate.Left(1) == _T("0"))
		stdFile.WriteString(_T("'") + cbSrc.strDate);
	else
		stdFile.WriteString(cbSrc.strDate);
	stdFile.WriteString(_T(","));

	stdFile.WriteString(cbSrc.strResponder);
	stdFile.WriteString(_T(","));

	stdFile.WriteString(cbSrc.strCompany);
	stdFile.WriteString(_T(","));

	stdFile.WriteString(cbSrc.strVersion);
	stdFile.WriteString(_T(","));

	stdFile.WriteString(cbSrc.strPhase);
	stdFile.WriteString(_T(","));

	CString eType = (cbSrc.eType == CCodeBlock::add ? BLOCK_KEY_ADD : cbSrc.eType == CCodeBlock::mod ? BLOCK_KEY_MOD : cbSrc.eType == CCodeBlock::del ? BLOCK_KEY_DEL : BLOCK_KEY_NONE);
	stdFile.WriteString(eType);
	stdFile.WriteString(_T(","));

	CString nLineStart;
	nLineStart.Format(_T("%d"), cbSrc.nLineStart);
	stdFile.WriteString(nLineStart);
	stdFile.WriteString(_T(","));

	CString nLineEnd;
	nLineEnd.Format(_T("%d"), cbSrc.nLineEnd);
	stdFile.WriteString(nLineEnd);
	stdFile.WriteString(_T(","));

	CString nCountComment;
	if (cbSrc.dwWarning == BLOCK_WARNING_NONE)
	{
		nCountComment.Format(_T("%d"), cbSrc.nCountComment);
	}
	else
	{
		nCountComment.Format(_T("%d"), 0);
	}
	stdFile.WriteString(nCountComment);
	stdFile.WriteString(_T(","));

	CString nCountBlank;
	if (cbSrc.dwWarning == BLOCK_WARNING_NONE)
	{
		nCountBlank.Format(_T("%d"), cbSrc.nCountBlank);
	}
	else
	{
		nCountBlank.Format(_T("%d"), 0);
	}
	stdFile.WriteString(nCountBlank);
	stdFile.WriteString(_T(","));

	CString nCountCode;
	if (cbSrc.dwWarning == BLOCK_WARNING_NONE)
	{
		nCountCode.Format(_T("%d"), cbSrc.nCountCode);
	}
	else
	{
		nCountCode.Format(_T("%d"), cbSrc.nCountCode >= 1 ? 0 : cbSrc.nCountCode);
	}
	stdFile.WriteString(nCountCode);
	stdFile.WriteString(_T(","));

	//Set detailed source code line number
	CString strCode;
	POSITION pos = cbSrc.lstCode.GetHeadPosition();
	while (pos)
	{
		if (cbSrc.dwWarning != BLOCK_WARNING_NONE)
			break;
		int nCode = cbSrc.lstCode.GetNext(pos);
		CString strTemp;
		strTemp.Format(_T("{%d}"), nCode);
		strCode += strTemp;
	}
	stdFile.WriteString(strCode);
	stdFile.WriteString(_T(","));

	//Set KeyWord count and detailed KeyWord
	CString GetCount;
	GetCount.Format(_T("%d"), cbSrc.strListKeyWord.GetCount());
	stdFile.WriteString(GetCount);
	stdFile.WriteString(_T(","));
	pos = cbSrc.strListKeyWord.GetHeadPosition();
	while (pos)
	{
		CString str = cbSrc.strListKeyWord.GetNext(pos);
		stdFile.WriteString(str);
		stdFile.WriteString(_T(","));
	}

	stdFile.WriteString(_T("\n")); // Finished one record then return
}
