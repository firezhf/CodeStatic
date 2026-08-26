/**[f***************************************************************
;
; l4bsprc.cpp
;
; Copyright (C) 2001-2009 Canon Inc.
; All rights reserved.  Company confidential.
;
 **f]**************************************************************/

/*------------ INCLUDE FILES ------------*/
#include "uimaster.h"
#include "cdevid.h"
#include "sheet.h"
#include "attenum.h"
#include "cmutls.h"
#include "regkey.h"

#include "clibutil.h"
#include "cmctlid.h"
#include "cmsv.h"
#include "cmsvid.h"
#include "cmusr.h"
#include "cmsvutls.h"

#include "pdlctlid.h"
#include "pdlui.h"

/* 020829 EBI@CSL Show OverlayMessage */
#include "pdldo.h"			/* for LPPDL_DEVICEOPTION */
#include "pdldopt.h"		/* for GetPdlDoPointer */
#include "pdldmpt.h"		/* for GetPdlDmPointer */

#include "pdloptid.h"
#include "pdlcdmid.h"
#include "devoptid.h"
#include "pdlcapmc.h"
#include "pdlcapfn.h"
#include "pdlcap.h"
#include "pdlcapui.h"		/* for bMessageCheckProfile */
#include "clappinf.h"
#include "devinfid.h"


#include "l4uidef.h"
#include "l4opndlg.h"
#include "l4bsprc.h"
#include "l4help.h"
#include "l4usr.h"
#include "l4prv.h"
#include "l4prf.h"
#include "l4obj.h"
#include "l4spcver.h"

#include "l4_proc.h"
#include "l4svcm.h"
#include "l4pcheck.h"
#include "pcsprtman.h"		/* 090909 KIS@CIS Secure print spec change. mod sprtman.h ->pcsprtman.h */
#include "l4nsvsdk.h"

#include "pdlutil.h"
#include "l4utl.h"
#include "pdlcdvid.h"
#include "pdlprtid.h"
#include "L4pconf.h"

#include "pdlcomid.h"	/* 20071015 MFG@CSSSI [v1220][Store&NumberPrint] */

#include "dbgPdlUI.h"	/*_ CMNDBGMSG */
#include "FncQuery.h"	/* 081120 LIPS-BUGUpdateNo2 */
#include "PCCOMN.H"		/* 090108 ISI@IST *//* MOD LIPSDEV_v20.00_0008 [4] */
#include "pcsvngui.h"	/* 090610 ISI@IST */
#include "cmsvpg.h"		/* 090610 ISI@IST */
#include "PCutils.h"	/* 090610 ISI@IST */
#include "pcdevid.h"	/* 090610 ISI@IST */
#include "pcsvid.h"/* 140818 ZMJ@SSS [v21.35][Alpha][Add][REQ000140][ECOStaple] */

/*------------ DEFINITIONS ------------*/
/*_ IR PAPER TYPE */
static PAPERTYPELIST PSType_ItmTbl[] = {
	{ ID_MEDIA_PLANE,				ID_MEDIAFLG_PLANE			},
	{ ID_MEDIA_RECYCLED,			ID_MEDIAFLG_RECYCLED		},
	{ ID_MEDIA_COLOR,				ID_MEDIAFLG_COLOR			},
	{ ID_MEDIA_THICKNESS,			ID_MEDIAFLG_THICKNESS		},
//	{ ID_MEDIA_THICKNESSH,			ID_MEDIAFLG_THICKNESSH		},
	{ ID_MEDIA_TRANSPARENCY,		ID_MEDIAFLG_TRANSPARENCY	},
	{ ID_MEDIA_LABELS,				ID_MEDIAFLG_LABELS			},
	{ ID_MEDIA_TRACING,				ID_MEDIAFLG_TRACING			},
	{ ID_MEDIA_POSTCARD,			ID_MEDIAFLG_POSTCARD		},
	{ ID_MEDIA_ENVELOPE,			ID_MEDIAFLG_ENVELOPE		},
	{ ID_MEDIA_INDEXTAB,			ID_MEDIAFLG_INDEXTAB		},
	{ ID_MEDIA_COATED,				ID_MEDIAFLG_COATED			},
	{ ID_MEDIA_JAPANESEPAPER,		ID_MEDIAFLG_JAPANESEPAPER	},
	{ ID_MEDIA_PREPANCHED,			ID_MEDIAFLG_PREPANCHED		},
	{ ID_MEDIA_PLANEL,				ID_MEDIAFLG_PLANEL			},
	{ ID_MEDIA_FLIMSY,				ID_MEDIAFLG_FLIMSY			},
	{ ID_MEDIA_LETTERHEAD,			ID_MEDIAFLG_LETTERHEAD		},
	{ ID_MEDIA_ROUGH,				ID_MEDIAFLG_ROUGH			},
	{ ID_MEDIA_BOND,				ID_MEDIAFLG_BOND			},
	{ ID_MEDIA_GLOSSYPAPER,			ID_MEDIAFLG_GLOSSYPAPER		},
	{ ID_MEDIA_PASTEBOARD,			ID_MEDIAFLG_PASTEBOARD		},	/*_ 030620 SKG@CSL 厚紙1 */
//	{ ID_MEDIA_PASTEBOARD_2,		ID_MEDIAFLG_PASTEBOARD2		},	/*_ 030620 SKG@CSL 厚紙2 */
//	{ ID_MEDIA_PASTEBOARD_3,		ID_MEDIAFLG_PASTEBOARD3		},	/*_ 030620 SKG@CSL 厚紙3 */
//	{ ID_MEDIA_PASTEBOARD_4,		ID_MEDIAFLG_PASTEBOARD4		},	/*_ 厚紙4(厚紙L) */
//	{ ID_MEDIA_INDEXTAB_2,			ID_MEDIAFLG_INDEXTAB2		},	/*_ 030620 SKG@CSL インデックス紙2 */
	{ ID_MEDIA_COATED_S1,			ID_MEDIAFLG_COATED_S1		},	/* 20060414 WZC@CSSSI [E313CD_PAPERTYPE] */
	{ ID_MEDIA_PRINTERDEFAULT,		ID_MEDIAFLG_PRINTERDEFAULT	},
	{0,								0							},
};

/* 20060329 ZYS@CSSSI [v1102][E315_PAPERTYPE] Start */
static SHORT MediaType1_ItmTbl[] = {
		ID_MEDIA_PLANE,
		ID_MEDIA_RECYCLED,
		ID_MEDIA_RECYCLED_2,
		ID_MEDIA_RECYCLED_5,
		ID_MEDIA_RECYCLED_7,
        ID_MEDIA_COLOR,
		ID_MEDIA_PREPANCHED,
		ID_MEDIA_PREPANCHED_2,
		ID_MEDIA_BOND,	/* 20060413 ZYS@CSSSI [v1102][E315_PAPERTYPE][MODIFY001] */
		ID_MEDIA_PLANE2,
		ID_MEDIA_PLAIN3,		/* 091207 YKA@CIS For Plane3 */
		ID_MEDIA_PASTEBOARD,
		ID_MEDIA_PASTEBOARD_2,
		ID_MEDIA_PASTEBOARD_3,
		ID_MEDIA_PASTEBOARD_4,
		ID_MEDIA_TRANSPARENCY,
		ID_MEDIA_FLIMSY,
		ID_MEDIA_LABELS,
		ID_MEDIA_POSTCARD,
		ID_MEDIA_INDEXTAB,
		ID_MEDIA_INDEXTAB_2,	
		ID_MEDIA_COATED_S2,
		ID_MEDIA_COATED_S3,
		ID_MEDIA_COATED_S4,
		ID_MEDIA_COATED_S5,
		ID_MEDIA_COATED_S6,
		ID_MEDIA_COATED_D2,
		ID_MEDIA_COATED_D3,
		ID_MEDIA_COATED_D4,
		ID_MEDIA_COATED_D5,
		ID_MEDIA_COATED_D6,
		ID_MEDIA_EMBOSSED_1,
		ID_MEDIA_EMBOSSED_2,
		ID_MEDIA_EMBOSSED_3,
		ID_MEDIA_EMBOSSED_4,
		ID_MEDIA_EMBOSSED_5,
		ID_MEDIA_EMBOSSED_6,
		ID_MEDIA_VELLUM_1,
		ID_MEDIA_VELLUM_2,
		/* 20060821 CHF@CSSSI [E314][E314_PAPERTYPE] Start*/
		ID_MEDIA_EMBOSSED_7,
		ID_MEDIA_PASTEBOARD_5,
		ID_MEDIA_COATED_S1,
		ID_MEDIA_COATED_S7,
		ID_MEDIA_COATED_D1,
		ID_MEDIA_COATED_D7,
		/* 20060821 CHF@CSSSI [E314][E314_PAPERTYPE] End*/
		ID_MEDIA_FLIMSY_2, /* 20080610 HXM@CSSSI [v1300][Paper]*/
		/* 141108 JJJ@SSS [v21.45][PreAlpha][Add][AddNewDevice_Ewloe] Start */
		ID_MEDIA_PASTEBOARD_6,
		ID_MEDIA_TRACING,
		ID_MEDIA_TRANSPARENT_FILM,
		ID_MEDIA_LETTERHEAD,
		/* 141108 JJJ@SSS [v21.45][PreAlpha][Add][AddNewDevice_Ewloe] End */
		0	,
	};
/* 20060329 ZYS@CSSSI [v1102][E315_PAPERTYPE] End */

/*_ BIJ/LBP PAPER TYPE */
static PAPERTYPELIST LBPSType_ItmTbl[] = {
	{ ID_MEDIA_PLANE,					ID_MEDIAFLG_LP_PLANE				},
	{ ID_MEDIA_PLANE2,					ID_MEDIAFLG_LP_PLANE2				},
	{ ID_MEDIA_PLANEL,					ID_MEDIAFLG_LP_PLANEL				},
	{ ID_MEDIA_THICKNESS,				ID_MEDIAFLG_LP_THICKNESS			},
	{ ID_MEDIA_THICKNESSH,				ID_MEDIAFLG_LP_THICKNESSH			},
	{ ID_MEDIA_PASTEBOARD,				ID_MEDIAFLG_LP_PASTEBOARD			},
	{ ID_MEDIA_PASTEBOARD_2,			ID_MEDIAFLG_LP_PASTEBOARD2			},
	{ ID_MEDIA_PASTEBOARD_4,			ID_MEDIAFLG_LP_PASTEBOARD4			},
	{ ID_MEDIA_TRANSPARENCY,			ID_MEDIAFLG_LP_TRANSPARENCY			},
	{ ID_MEDIA_TRANSPARENCY_QUICKDRY,	ID_MEDIAFLG_LP_TRANSPARENCY_QUICKDRY},
	{ ID_MEDIA_COATED,					ID_MEDIAFLG_LP_COATED				},
	{ ID_MEDIA_COATED_MATTECT,			ID_MEDIAFLG_LP_COATED_MATTECT		},
	{ ID_MEDIA_POSTCARD,				ID_MEDIAFLG_LP_POSTCARD				},
	{ ID_MEDIA_POSTCARD_IJ,				ID_MEDIAFLG_LP_POSTCARD_IJ			},
	{ ID_MEDIA_PRINTERDEFAULT,			ID_MEDIAFLG_LP_PRINTERDEFAULT		},
	{0,									0									},
};


#define DOCNAMEBUFSIZE		34

#define OFFSET_MIN			1
#define OFFSET_MAX			9999

#define EMBEDDOCUMENTNUMBER_MIN		1	/* 091116 SAI@IST add for LIPS4_i_QA V20.20-00044 */

#define LOGICPOSMAX		32639	/*_ 論理座標の最大値(32639 dot) *//* 20120514 XIA@SSS [v20.85][PreAlpha][NF_2085_1-BasicSheet][Add] */


/*------------ PROTOTYPES ------------*/
static BOOL CALLBACK bIsEnableItemOnOpenDocProp(LPUIINTERFACETABLE lpuift, WORD wCtrlID, LPSVLISTITEM lpList);

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/02/07:0.00:ITO:CCD3
********************************************************************/
L4SvBsProc::L4SvBsProc()
{
	m_lpTooltip = NULL;
	m_lpPreview = NULL;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/02/07:0.00:ITO:CCD3
********************************************************************/
L4SvBsProc::L4SvBsProc(UINT uSheetID, UINT uHelpKeyID):L4SvProc(uSheetID, uHelpKeyID)
{
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/02/05:0.00:ITO:CCD3
********************************************************************/
BOOL L4SvBsProc::bMessageFilter(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL	bRet = FALSE;

	PUIDBGSvIN(("L4SvBsProc::bMessageFilter()"));

	switch (uMsg) {

	case WM_COMMAND:

		switch (m_wCtrlID) {

		case IDC_CB_JOBOPERATIONS: 
		case IDC_CB_FINISHING3:	/* 140818 ZMJ@SSS [v21.35][Alpha][Add][REQ000140][ECOStaple] */
			switch (m_wNotifyCode) {
			case CBN_DROPDOWN:
			case CBN_CLOSEUP:
			case CBN_KILLFOCUS:
				bRet = TRUE;
				break;
			}
			break;
		}
	}

	if (bRet == FALSE)
		bRet = BsProc.bMessageFilter(uMsg, wParam, lParam);

	if (bRet == FALSE)
		bRet = L4SvProc::bMessageFilter(uMsg, wParam, lParam);

	PUIDBGSvOUT(("L4SvBsProc::bMessageFilter() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/02/07:0.00:ITO:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	L4Profile	*lpL4Profile = NULL;
	BOOL		bRet;


	PUIDBGSvIN(("L4SvBsProc::bOnInitDialog()"));

	bRet = L4SvProc::bOnInitDialog(hWnd, wParam, lParam);

	lpL4Profile = L4USR_lpGetProfile(m_lpuift);
	if(lpL4Profile){
		TCHAR	szDocPreSet[32] = {0};
		ULIB_ADM_GetDoc_Preset(m_lpuift, m_lpdm, szDocPreSet);
		lpL4Profile->PRF_bSetProfileQulSelFromCurProfile(m_lpuift, (LPTSTR)szDocPreSet);
	}

	/* Common Basic Procedure */
	BsProc.bOnInitDialog(hWnd, m_lpdm, m_lpuift);

	m_lpTooltip	= BsProc.m_lpTooltip;

	if (m_lpTooltip) {

		CMTOOLTIP		tt;
		short			docDst = 0;
		CmSvConflict	*lpCmSvConflict = NULL;

		lpCmSvConflict = CMSVUSR_lpGetConflict(m_lpuift);
		if (lpCmSvConflict == NULL) {
			PUIDBGSv(("Get Address error L.%d", __LINE__));
			goto EXIT;
		}

		ULIB_ADM_GetDoc_Destination(m_lpuift, m_lpdm, &docDst);

		/* 080603 WAT@IST 2LINE */
		/* [Japanese]お気に入りの２行化前の場合はツールチップを表示する */
		if ( CMUSR_iGetSupportUI2lineFlag(m_lpuift) == CM_UIORIGINAL) {
		/* 080603 WAT@IST 2LINE */
			/* Storing Destination & Secured Button */
			/* Storing Destination & Secured Button */
			/* if (lpCmSvConflict->CNF_bIsSvVisible(m_lpuift, IDSV_COMMON_JOBOPERATIONSDETAILSETTING)) { */
				tt.wKind			= TT_KIND_BUTTON;
				tt.wCtrlID			= IDC_PB_JOBOPERATIONS_DETAILSSETTING;
			/* 20061027 LYN@CSSSI [CSSSI_LIPS_v12.00-00004] Start */
				if(docDst == ID_DST_SECUREDPRINT){
					tt.uStrID		=	IDR_STR_TTP_SECUREDBUTTON;
				}else if(docDst == ID_DST_HOLDQUEUE){
					tt.uStrID		=	IDR_STR_TOOLTIP_HOLDBUTTON;
				}else{
					tt.uStrID		=	IDR_STR_TTP_MAILBOXSETUP;
				}
			/*	tt.uStrID			= (docDst == ID_DST_SECUREDPRINT) ? IDR_STR_TTP_SECUREDBUTTON : IDR_STR_TTP_MAILBOXSETUP;*/
			/* 20061027 LYN@CSSSI [CSSSI_LIPS_v12.00-00004] End */
				tt.lpfnGetStrFnc	= NULL;

				m_lpTooltip->TTM_vAddTooltip(&tt);
			/* } */
		/* 080603 WAT@IST 2LINE */
		/* [Japanese]お気に入りの２行化前の場合はツールチップを表示する */
		}
		/* 080603 WAT@IST 2LINE */

		/* Lock Button */
		/* if (lpCmSvConflict->CNF_bIsSvVisible(m_lpuift, IDSV_COMMON_PAGECOMPOSERLOCK)) { */
			tt.wKind			= TT_KIND_BUTTON;
			tt.wCtrlID			= IDC_PB_CHANGE_PAGECOMPOSER;
			tt.uStrID			= IDR_STR_TTP_LOCKBUTTON;
			tt.lpfnGetStrFnc	= NULL;	
			
			m_lpTooltip->TTM_vAddTooltip(&tt);
		/* } */

		/* Status Monitor Button */
		/* if (lpCmSvConflict->CNF_bIsSvVisible(m_lpuift, IDSV_COMMON_STATUSMONITOR)) { */
			tt.wKind			= TT_KIND_BUTTON;
			tt.wCtrlID			= IDC_PB_SM;
			tt.uStrID			= IDR_STR_TTP_JOBMONITOR;
			tt.lpfnGetStrFnc	= NULL;

			m_lpTooltip->TTM_vAddTooltip(&tt);
		/* } */

		/* Job Operation Commbo Box */
		if (lpCmSvConflict->CNF_bIsSvVisible(m_lpuift, IDSV_COMMON_JOBOPERATION)) {
			tt.wKind			= TT_KIND_COMMBOBOX;
			tt.wCtrlID			= IDC_CB_JOBOPERATIONS;
			tt.uStrID			= 0;
			tt.lpfnGetStrFnc	= PCSV_lpGetToolTipString;

			m_lpTooltip->TTM_vAddTooltip(&tt);
		}

		/* 140819 ZMJ@SSS [v21.35][Alpha][Add][REQ000140][ECOStaple] Start */
		/* Eco Staple */
		if (PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_ECOSTAPLE_SUPPORT) == TRUE){
			if (lpCmSvConflict->CNF_bIsSvVisible(m_lpuift, IDSV_FINISHING_METHOD)) {
				tt.wKind			= TT_KIND_COMMBOBOX;
				tt.wCtrlID			= IDC_CB_FINISHING3;
				tt.uStrID			= 0;
				tt.lpfnGetStrFnc	= PCSV_lpGetToolTipString;

				m_lpTooltip->TTM_vAddTooltip(&tt);
			}
		}
		/* 140819 ZMJ@SSS [v21.35][Alpha][Add][REQ000140][ECOStaple] End */
	}

	/* 020712 ITO */
	vUpdateTooltip();

	/* Create Preview Object */
	m_lpPreview = bCreatePreviewClass(m_hPrevCtrl, m_lpuift, m_lpdm, m_uSheetID);

	BsProc.vInitPreview(m_lpPreview);
	
	m_bForciblyDisable	= FALSE;
	m_bForciblyHide	= FALSE;

	if (m_lpuift->ULIB_GetOpenDocPropData(m_lpuift)) {
		m_bForciblyDisable	= TRUE;
	}

	if (m_lpuift->ULIB_GetOpenDocPropDataEx(m_lpuift, ODPDATA_HIDELIST)) {
		m_bForciblyHide	= TRUE;
	}

EXIT:
	PUIDBGSvOUT(("L4SvBsProc::bOnInitDialog() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/02/07:0.00:ITO:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnDestroy(WPARAM wParam, LPARAM lParam)
{
	PUIDBGSvIN(("L4SvBsProc::bOnDestroy()"));

	/* Basic Class */
	L4SvProc::bOnDestroy(wParam, lParam);

	/* Common Basic Procedure */
	BsProc.bOnDestroy();

	/* Delete Preview Object */
	if (m_lpPreview) {
		delete m_lpPreview;
		m_lpPreview = NULL;
	}

	PUIDBGSvOUT(("L4SvBsProc::bOnDestroy() return FALSE"));

	return FALSE;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/02/07:0.00:ITO:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnCommand(WPARAM wParam, LPARAM lParam)
{
	BOOL	bRet 				= FALSE;
	BOOL	bSendSetActiveFlg	= FALSE;
	BOOL	bCustomMark			= FALSE;
	LPPDL_DEVICEOPTION	lpDvOpt = NULL;		/* 110608 CHL@SSS [V20.60][Alpha][Add][CR0010][共通設定からのユーザー情報入力対応] */

	PUIDBGSvIN(("L4SvBsProc::bOnCommand()"));

	lpDvOpt = GetPdlDoPointer(CMUSR_lpGetCurDevoption(m_lpuift));	/* 110608 CHL@SSS [V20.60][Alpha][Add][CR0010][共通設定からのユーザー情報入力対応] */

	if (m_bForciblyDisable) {
		if (bForciblyOnCommand()){
			PUIDBGSvOUT(("L4SvBsProc::bOnCommand() return TRUE L.%d", __LINE__));
			return TRUE;
		}
	}

	switch (m_wCtrlID) {

	case IDC_CB_JOBOPERATIONS: 
	case IDC_CB_FINISHING3:	/* 140818 ZMJ@SSS [v21.35][Alpha][Add][REQ000140][ECOStaple] */ 

		PUIDBGSv(("L4SvBsProc::bOnCommand() <IDC_CB_JOBOPERATIONS> "));

		if (m_wNotifyCode == CBN_DROPDOWN) {
			if(m_lpTooltip){
				m_lpTooltip->TTM_vEnableTooltip(m_hCtrl, TRUE);
			}
			PUIDBGSvOUT(("L4SvBsProc::bOnCommand()[CBN_DROPDOWN] return FALSE L.%d", __LINE__));
			return FALSE;
		}
		else if (m_wNotifyCode == CBN_CLOSEUP) {
			if(m_lpTooltip){
				m_lpTooltip->TTM_vEnableTooltip(m_hCtrl, FALSE);
			}
			PUIDBGSvOUT(("L4SvBsProc::bOnCommand()[CBN_CLOSEUP] return FALSE L.%d", __LINE__));
			return FALSE;
		}
		else if (m_wNotifyCode == CBN_KILLFOCUS) {
			if(m_lpTooltip){
				m_lpTooltip->TTM_vEnableTooltip(m_hCtrl, FALSE);
			}
			PUIDBGSvOUT(("L4SvBsProc::bOnCommand()[CBN_KILLFOCUS] return FALSE L.%d", __LINE__));
			return FALSE;
		}

		/*bRet = bOnCmd_JobOperation();*/ /* 20080707 ZDL@CSSSI [v12.40][UI Extension]*/
		break;

	case IDC_PB_ADDPROFILE:
		bRet = bOnCmd_AddProfile();
		if (bRet) {
			CmWaterMark		*lpWtm = NULL;
			DWORD 			dwFlag = 0L;

			bSendSetActiveFlg = TRUE;

			/* Undefined stamp	*/
			ULIB_ADM_GetStm_Flags(m_lpuift, m_lpdm, &dwFlag);
			if (dwFlag & ID_STAMPPRINT_ON){
				ULIB_ADM_SetStm_Title(m_lpuift, m_lpdm, TEXT(""));
			}

			/* Undefined Stamp	*/
			lpWtm = CMUSR_lpGetWaterMark(m_lpuift);
			if(lpWtm){
				lpWtm->WTM_bUndefinedStamp(TRUE);
			}
		}
		break;

	case IDC_PB_EDITPROFILE:
		bRet = bOnCmd_EditProfile();
		if (bRet)
			bSendSetActiveFlg = TRUE;
		break;

	case IDC_PB_VIEWSETTING:
		bRet = bOnCmd_ViewSettings();
		break;

	case IDC_PB_JOBOPERATIONS_DETAILSSETTING:
		bRet = bOnCmd_JobOperationDetail();
		break;

	case IDC_PB_SM:
		PCConfiguration		*lpCfg = GetlpPCConfiguration(m_lpuift);
		if (lpCfg)
			bRet = lpCfg->CFG_bExecStatusMonitor();
		break;
	
	}

	/* 140701 ZYS@SSS [v21.35][PreAlpha][Add][Drv_V21_35_a_FamiCom-0041][StapleMergeSpecChange] Start */
	{
		CmSvConflict* lpCmSvConflict = lpGetConflictObject(m_lpuift);

		UINT SVIDTbl[] = {
			IDSV_LAYOUT_BINDDIRECT,
			0
		};

		lpCmSvConflict->vReLoadSv(m_lpuift, m_lpdm, SVIDTbl);
	}
	/* 140701 ZYS@SSS [v21.35][PreAlpha][Add][Drv_V21_35_a_FamiCom-0041][StapleMergeSpecChange] End */

	/* Basic Class */
	if (L4SvProc::bOnCommand(wParam, lParam)) {
		bRet 		= TRUE;

		/*_ 030812 NON@CSL LIPS4 LegacyDriver障害票 v10.20_00052 */
		m_wCtrlID = GET_WM_COMMAND_ID (wParam, lParam);
		
		/* P840CPT-0002 CHI@CORE-UI	*/
		if (BsProc.bShowCustomMark(m_wCtrlID))
			bCustomMark	= TRUE;
	}

	/* Common Basic Procedure */
	if (bRet == FALSE) {

		m_wCtrlID = GET_WM_COMMAND_ID (wParam, lParam);/* 130424 DWY@SSS [v21.05][Alpha][Add][Drv_v21_05_a_FamiCom-0044] */

		bRet = BsProc.bOnCommand(m_wCtrlID, m_wNotifyCode);
		if (bRet) 
			bSendSetActiveFlg = TRUE;
	}

	/* Data is changed */
	if (bRet) {
		short		sQuaSelection = 0;

		/* 090415 TAM@IST for V20.00 仕様変更 Store時には詳細DLGを開く */
		if (GET_WM_COMMAND_ID(wParam, lParam) == IDC_CB_JOBOPERATIONS) {
			short			sDest = 0;
			
			ULIB_ADM_GetDoc_Destination(m_lpuift, m_lpdm, &sDest);

			if (sDest == ID_DST_STORE) {
				if (IDYES == CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_034)) {
					L4_bOpenDialog(m_lpuift, m_hDlg, SID_MAILBOX, m_lpdm);
				}
			}
			/* 110608 CHL@SSS [V20.60][Alpha][Add][CR0010][共通設定からのユーザー情報入力対応] Start */
			else if (sDest == ID_DST_SECUREDPRINT) {
				if ( !(lpDvOpt->dwUnitFlags & ID_UNIT_SET_USER_NAME) || (m_lpuift->ULIB_bIsCustomizing(m_lpuift, m_lpuift->ULIB_GetPrinterHandle(m_lpuift))) ){
					CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_037);
					UTL_vEnableControl(m_hDlg, IDC_PB_JOBOPERATIONS_DETAILSSETTING, FALSE);
				}
				else{
					if (IDYES == CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_255)) {
						L4_bOpenDialog(m_lpuift, m_hDlg, SID_SECURED, m_lpdm);
					}
				}
			}
			/* 110608 CHL@SSS [V20.60][Alpha][Add][CR0010][共通設定からのユーザー情報入力対応] End */
		}
		/* 090415 TAM@IST for V20.00 仕様変更 Store時には詳細DLGを開く */
	
		switch(m_wCtrlID){
//#if 0	/* 100615 WAT@IST ObjUpdate */
//		case IDC_PB_DETAILSETTING:
//			ULIB_ADM_GetQal_Selection(m_lpuift, m_lpdm, &sQuaSelection);
//			if (!(sQuaSelection & ID_QUASELECTION_ON_EDIT)) {
//				sQuaSelection |= ID_QUASELECTION_ON_EDIT;
//				ULIB_ADM_SetQal_Selection(m_lpuift, m_lpdm, &sQuaSelection);
//			}
//			break;
//#endif	/* 100615 WAT@IST ObjUpdate */
			
		case IDC_CB_PROFILE:
		case IDC_PB_ADDPROFILE:

			CmSvConflict		*lpCmSvConflict = NULL;
			L4Objective			*lpL4Obj = NULL;
			LPPDL_CDEVMODE		lpPdlDevMode = NULL;

			/* 140701 ZYS@SSS [v21.35][PreAlpha][Add][CommonUI_v21.35-0025][StapleMergeSpecChange] Start */
			CmSv* lpCmSv = NULL;
			CmSvItem *lpCmSvItem = NULL;
			short	sFshStaplePosition = ID_SLOCATION_TL;
			/* 140701 ZYS@SSS [v21.35][PreAlpha][Add][CommonUI_v21.35-0025][StapleMergeSpecChange] End */

			/* お気に入り選択時、[その他の設定]を包含するお気に入り時UserDataにメンバ設定 */
			bOnCmd_ProfileComboBox();

			/* 140701 ZYS@SSS [v21.35][PreAlpha][Add][CommonUI_v21.35-0025][StapleMergeSpecChange] */
			ULIB_ADM_GetFin_StaplePosition(m_lpuift, m_lpdm, &sFshStaplePosition);
			
			/* Reset SV */
			lpCmSvConflict = CMSVUSR_lpGetConflict(m_lpuift);
			if (lpCmSvConflict){
				lpCmSvConflict->CNF_vResetAllSv(m_lpuift, m_hDlg, m_uSheetID);
				/* 140701 ZYS@SSS [v21.35][PreAlpha][Add][CommonUI_v21.35-0025][StapleMergeSpecChange] Start */
				lpCmSv = lpCmSvConflict->lpGetCmSv(IDSV_FINISHING_POSITION);

				if(lpCmSv)
					lpCmSvItem = dynamic_cast<CmSvItem *>(lpCmSv);
				
				if(lpCmSvItem) {
					ULIB_ADM_SetFin_StaplePosition(m_lpuift, m_lpdm, &sFshStaplePosition);

					lpCmSvItem->bLoad(m_lpuift, m_lpdm);

					lpCmSvConflict->vCheckConflict(m_lpuift, lpCmSv, 1);
				}
				/* 140701 ZYS@SSS [v21.35][PreAlpha][Add][CommonUI_v21.35-0025][StapleMergeSpecChange] End */
			}


			lpL4Obj	= L4USR_lpGetObjective(m_lpuift);
			lpPdlDevMode = GetPdlDmPointer(((LPSHAREDEV)m_lpdm));

			if(lpL4Obj) {
				
				if( lpL4Obj->OBJ_bIsSupportQuaDetail() ){
				/* ユーザ設定サポート機種 */

					if (lpPdlDevMode) { 
						/* 選択したお気に入りが[印刷品質]:ユーザ設定を包含していなかった場合 */
						if (lpPdlDevMode->pcCdevmode.pdlQuality.quaPrintQualityFlags != ID_QUALITY_USER) {
							short	sQualSelection = 0;
							/* L4Objective::m_quaSelectionへ現在選択されている(お気に入りが包含している)印刷目的をCopy */
							/* m_lpdmから印刷目的DEVMODEを取得 */
							ULIB_ADM_GetQal_Selection (m_lpuift, m_lpdm, &sQualSelection);

//							lpL4Obj->OBJ_bSet_quaSelection(sQualSelection);					/* @@@ 080831 TAM@IST for CCD3.5 */
							
						} else {
						/* 選択したお気に入りが[印刷品質]:ユーザ設定を包含していた場合 */
						
							/* ユーザデータQUADETAILにDEVMODEの値を反映 */
//							L4USR_lpInitQuaDetailUser(m_lpuift, ID_INITQUADETAIL_CURDEV);	/* @@@ 080831 TAM@IST for CCD3.5 */
						}
					}
				}
			}
			break;
		}

		/* Set profile custom Mark ON, send WM_NOTIFY:PSN_SETACTIVE, & update preview */
		BsProc.vDataChanged(bCustomMark, bSendSetActiveFlg);

		/* 020712 ITO */
		vUpdateTooltip();
	}


	PUIDBGSvOUT(("L4SvBsProc::bOnCommand() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/02/07:0.00:ITO:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnDrawItem(WPARAM wParam, LPARAM lParam)
{
	BOOL				bRet = FALSE;

	PUIDBGDrawIN(("L4SvBsProc::bOnDrawItem()"));

	if (m_bForciblyDisable) {
		if (bForciblyOnDraw()){
			PUIDBGDraw(("L4SvBsProc::bOnCommand() return TRUE L.%d", __LINE__));
			return TRUE;
		}
	}

	/* Basic Class */
	bRet = L4SvProc::bOnDrawItem(wParam, lParam);

	/* Common Basic Procedure */
	if (BsProc.bOnDrawItem(m_wCtrlID, m_lpdis))
		bRet = TRUE;

	PUIDBGDrawOUT(("L4SvBsProc::bOnDrawItem() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/02/07:0.00:ITO:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnSetActive(WPARAM wParam, LPARAM lParam)
{
	BOOL	bRet = FALSE;

	PUIDBGSvIN(("L4SvBsProc::bOnSetActive()"));

	/* Basic Class */
	bRet = L4SvProc::bOnSetActive(wParam, lParam);

	/* Common Basic Procedure */
	if (BsProc.bOnSetActive(wParam, lParam))
		bRet = TRUE;

	/* 020712 ITO */
	vUpdateTooltip();

	PUIDBGSvOUT(("L4SvBsProc::bOnSetActive() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
********************************************************************/
BOOL L4SvBsProc::bOnKillActive(WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = FALSE;
	
	PUIDBGSvIN(("L4SvBsProc::bOnKillActive()"));

	if (m_lpPreview){
		m_lpPreview->PRV_vDisposeOfPreview();
	}

	/* 140701 ZYS@SSS [v21.35][PreAlpha][Add][StapleMergeSpecChange] Start */
	CmSvConflict* lpCmSvConflict = lpGetConflictObject(m_lpuift);

	CmSv* lpCmSv = lpCmSvConflict->lpGetCmSv(IDSV_LAYOUT_BINDDIRECT);

	if (lpCmSv) {
		lpCmSvConflict->vCheckConflict(m_lpuift, lpCmSv, 1);
	}
	/* 140701 ZYS@SSS [v21.35][PreAlpha][Add][StapleMergeSpecChange] End */

	/* Basic Class */
	bRet = L4SvProc::bOnKillActive(wParam, lParam);

	/* Common Basic Procedure */
	BsProc.vOnKillActive();

	PUIDBGSvOUT(("L4SvBsProc::bOnKillActive() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/09/17:0.00:KTO:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnQueryNewPalette(WPARAM wParam, LPARAM lParam)
{
	BOOL	bRet = FALSE;
	
	PUIDBGSvIN(("L4SvBsProc::bOnQueryNewPalette()"));

	if (m_lpPreview){
		bRet = m_lpPreview->PRV_bQueryNewPalette();
	}

	PUIDBGSvOUT(("L4SvBsProc::bOnQueryNewPalette() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/09/17:0.00:KTO:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnPaletteChanged(WPARAM wParam, LPARAM lParam)
{
	PUIDBGSvIN(("L4SvBsProc::bOnPaletteChanged()"));

	if (m_lpPreview){
		m_lpPreview->PRV_bOnPaletteChanged((HWND)wParam);
	}

	PUIDBGSvOUT(("L4SvBsProc::bOnPaletteChanged() return FALSE"));

	return FALSE;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/02/07:0.00:ITO:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnPreviewCommand(WPARAM wParam, LPARAM lParam)
{
	BOOL	bRet = FALSE;

	PUIDBGSvIN(("L4SvBsProc::bOnPreviewCommand()"));

	/* Basic Class */
	bRet = L4SvProc::bOnPreviewCommand(wParam, lParam);

	/* Common Basic Procedure */
	if (BsProc.bOnPreviewCommand(bRet))
		bRet = TRUE;

	if (bRet) {
		/* Set profile custom Mark ON, send WM_NOTIFY:PSN_SETACTIVE, & update preview */
		BsProc.vDataChanged(TRUE, FALSE);
	}

	PUIDBGSvOUT(("L4SvBsProc::bOnPreviewCommand() bRet=%d", bRet));

	return bRet;
}
/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/08/27:0.00:KTO:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnActivate(WPARAM wParam, LPARAM lParam)
{
	PUIDBGSvIN(("L4SvBsProc::bOnActivate()"));

	switch (wParam) {
	case WA_ACTIVE:
	case WA_CLICKACTIVE:
		break;
		
	case WA_INACTIVE:
		BsProc.vInactiveWindow();
		break;
	}

	PUIDBGSvOUT(("L4SvBsProc::bOnActivate() return FALSE"));

	return FALSE;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/05/22:0.00:ITO:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnPreMsgBoxOpen(WPARAM wParam, LPARAM lParam)
{
	BOOL	bRet = FALSE;

	PUIDBGSvIN(("L4SvBsProc::bOnPreMsgBoxOpen()"));

	/* Basic Class */
	bRet = L4SvProc::bOnPreMsgBoxOpen(wParam, lParam);

	/* Common Basic Procedure */
	BsProc.vOnPreMsgBoxOpen();

	PUIDBGSvOUT(("L4SvBsProc::bOnPreMsgBoxOpen() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/04/03:0.00:ITO:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnMouseMove(WPARAM wParam, LPARAM lParam)
{
	BOOL	bRet = FALSE;
	POINT	mpos;

	PUIDBGMouseIN(("L4SvBsProc::bOnMouseMove()"));

	mpos.x = (int)LOWORD(lParam);
	mpos.y = (int)HIWORD(lParam);

	/* Common Basic Procedure */
	bRet = BsProc.bOnMouseMove(mpos);

	PUIDBGMouseOUT(("L4SvBsProc::bOnMouseMove() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/11/22:1.00:NON@CSL:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnApply(WPARAM wParam, LPARAM lParam)
{
	BOOL 			bRet = TRUE;
	L4Objective		*lpL4Obj;
	LPDEVMODE		lpdm = NULL;
	LPPDL_CDEVMODE	lpPdlCDM = NULL;


	PUIDBGSvIN(("L4SvBsProc::bOnApply()"));

	lpPdlCDM = GetPdlDmPointer(((LPSHAREDEV)m_lpdm));

	/* Basic Class */
	bRet = CmSvProc::bOnApply(wParam, lParam);

	/* Store Secured Print data in the registry */
	if( ID_STATE3_DISABLE != PDL_GetJobMngSecure(m_lpuift->ULIB_GetPrinterCap (m_lpuift)) ){
		/* TCHAR		szDeviceName[MAX_PRINTERNAME]; */
		LPTSTR		szDeviceName = NULL;
		
		szDeviceName = new TCHAR[MAX_PRINTERNAME];
		if(FAILED(LSTRCPY_SAFE(szDeviceName, MAX_PRINTERNAME, m_lpuift->ULIB_GetDeviceName(m_lpuift)))){
			if(szDeviceName){
				delete [] szDeviceName;
			}
			PUIDBGSv(("LSTRCPY_SAFE error L.%d", __LINE__));
			PUIDBGSvOUT(("L4SvBsProc::bOnApply() return FALSE L.%d", __LINE__));
			return FALSE;
		}

		//bSaveSecuredPrintData( m_lpuift->ULIB_GetPrinterHandle(m_lpuift), szDeviceName, &(lpPdlCDM->pcCdevmode.pdlSecuredPrint) );	/* 090909 KIS@CIS Secure print spec change. add 1st param. */ /* 100902 KIS@CIS LIPS4_i_QA V20.40-00013 - del */
		
		if(szDeviceName)
			delete [] szDeviceName;
		
	}

	/* Store QUADETAIL data in the registry */
	lpL4Obj	= L4USR_lpGetObjective(m_lpuift);
	if(lpL4Obj) {
		if( lpL4Obj->OBJ_bIsSupportQuaDetail() ){
			DWORD	dwBasicValue = 0L;
			/* 印刷品質サポート機種:ユーザ設定だった場合はユーザデータQUADETAILにDEVMODEの値を反映 */
			if(lpPdlCDM->pcCdevmode.pdlQuality.quaPrintQualityFlags == ID_QUALITY_USER){
//				L4USR_lpInitQuaDetailUser(m_lpuift, ID_INITQUADETAIL_CURDEV);		/* @@@ 080831 TAM@IST for CCD3.5 */
			}
		}
	}

	PUIDBGSvOUT(("L4SvBsProc::bOnApply() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/02/07:0.00:ITO:CCD3
********************************************************************/
BOOL L4SvBsProc::bGetDlgInfo(UINT idSheetID, LPCM_DIALOGINFO lpDlgInfo)
{
	PUIDBGSvIN(("L4SvBsProc::bGetDlgInfo()"));
	PUIDBGSvOUT(("L4SvBsProc::bGetDlgInfo() bRet=%d", L4_bGetDlgInfo(m_lpuift, idSheetID, lpDlgInfo, m_lpdm)));

	return L4_bGetDlgInfo(m_lpuift, idSheetID, lpDlgInfo, m_lpdm);
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/02/07:0.00:ITO:CCD3
********************************************************************/
BOOL L4SvBsProc::bGetSheetsInfo(UINT idPropertySheetID, LPCM_SHEETSINFO lpSheetsInfo)
{
	PUIDBGSvIN(("L4SvBsProc::bGetSheetsInfo()"));
	PUIDBGSvOUT(("L4SvBsProc::bGetSheetsInfo() bRet=%d", L4_bGetSheetsInfo(m_lpuift, idPropertySheetID, lpSheetsInfo, m_lpdm)));

	return L4_bGetSheetsInfo(m_lpuift, idPropertySheetID, lpSheetsInfo, m_lpdm);
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/02/07:0.00:ITO:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnCmd_AddProfile()
{
	BOOL	bRet = FALSE;
	INT		iRet;

	PUIDBGSvIN(("L4SvBsProc::bOnCmd_AddProfile()"));

	iRet = L4_iOpenDialog(m_lpuift, m_hDlg, SID_ADDPROFILE, m_lpdm, &bRet);

	if (iRet == IDOK) {
		L4Profile	*lpL4Profile;
		TCHAR	szDocPreSet[32] = {0};
		short	shProQuaSel = 0;

		lpL4Profile = L4USR_lpGetProfile(m_lpuift);
		ULIB_ADM_GetDoc_Preset(m_lpuift, m_lpdm, szDocPreSet);

		if(lpL4Profile){
			/* 現在選択されているお気に入りが所有している外部DEVMODEの印刷目的の値--->L4Profile::m_shProQuaSelectionへ設定 */
			lpL4Profile->PRF_bSetProfileQulSelFromCurProfile(m_lpuift, (LPTSTR)szDocPreSet);
			shProQuaSel = lpL4Profile->PRF_shGetquaSelectionFromCurProfile(m_lpuift);
			if(shProQuaSel == ID_QUASELECTION_OTHER){
				ULIB_ADM_SetQal_Selection(m_lpuift, m_lpdm, &shProQuaSel);
			}
		}
		bRet = TRUE;
	}

	PUIDBGSvOUT(("L4SvBsProc::bOnCmd_AddProfile() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/02/07:0.00:ITO:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnCmd_EditProfile()
{
	BOOL	bRet = FALSE;
	INT		iRet;

	PUIDBGSvIN(("L4SvBsProc::bOnCmd_EditProfile()"));

	iRet = L4_iOpenDialog(m_lpuift, m_hDlg, SID_EDITPROFILE, m_lpdm, &bRet);

	if (iRet == IDOK) bRet = TRUE;

	PUIDBGSvOUT(("L4SvBsProc::bOnCmd_EditProfile() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/02/07:0.00:ITO:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnCmd_ViewSettings()
{
	BOOL		bRet = FALSE;
	UINT		nStartPage;

	PUIDBGSvIN(("L4SvBsProc::bOnCmd_ViewSettings()"));

	switch (m_uSheetID) {
	case SID_PAGESETUP:		nStartPage = 0; break;
	case SID_FINISHING:		nStartPage = 1; break;
	case SID_PAPERSOURCE:	nStartPage = 2; break;
	case SID_QUALITY:		nStartPage = 3; break;
	default:				nStartPage = 0; break;
	}

	/*bRet = L4_bOpenSheets(m_lpuift, m_hDlg, PSID_VIEWSETTINGS, nStartPage, m_lpdm);*/
	/* 120229 MFG@SSS [v20.80][PreAlpha][Modify][ViewSettingsExtension] */
	L4_iOpenDialog(m_lpuift, m_hDlg, SID_VIEWSETTINGS_BOLD, m_lpdm, &bRet);	/* 120405 DWY@SSS [v20.80][Alpha][Modify][Drv12_BR2_V20_80_a_FamiCom-0001][Delete bRet =] *//* 追加 */

	PUIDBGSvOUT(("L4SvBsProc::bOnCmd_ViewSettings() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/02/07:0.00:ITO:CCD3
********************************************************************/
BOOL L4SvBsProc::bOnCmd_JobOperationDetail()
{
	BOOL		bRet = FALSE;
	short		sdocDestination = 0;

	PUIDBGSvIN(("L4SvBsProc::bOnCmd_JobOperationDetail()"));

	ULIB_ADM_GetDoc_Destination(m_lpuift, m_lpdm, &sdocDestination);

	switch (sdocDestination) {
	case ID_DST_SECUREDPRINT:	/* Secured Print */
		bRet = L4_bOpenDialog(m_lpuift, m_hDlg, SID_SECURED, m_lpdm);
		break;

	case ID_DST_STORE:			/* Box  */
	case ID_DST_STOREPRINT:		/* プリンタ + 保存 */
		bRet = L4_bOpenDialog(m_lpuift, m_hDlg, SID_MAILBOX, m_lpdm);
		
		break;
		/* 20060824 LYN@CSSSI [E314][HoldQueue] Start */
	case ID_DST_HOLDQUEUE:
		bRet = L4_bOpenDialog(m_lpuift, m_hDlg, SID_HOLDQUEUE, m_lpdm);
		break;
		/* 20060824 LYN@CSSSI [E314][HoldQueue] End */

	case ID_DST_PRINTER:		/* プリンタ */
	case ID_DST_INTRPT:			/* 割り込み印刷 */
	case ID_DST_PAGECOMPOSER:	/* PageComposer */
	default:
		bRet = L4_bOpenDialog(m_lpuift, m_hDlg, SID_MAILBOX, m_lpdm); //  for text
		break;
	}
	
	PUIDBGSvOUT(("L4SvBsProc::bOnCmd_ViewSettings() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:03/01/17:0.00:NON@CSL
********************************************************************/
BOOL L4SvBsProc::bOnCmd_ProfileComboBox()
{	
	L4Profile	*lpL4Profile;
	TCHAR	szDocPreSet[32] = {0};
	short	shProfileQuaSel = 0;

	PUIDBGSvIN(("L4SvBsProc::bOnCmd_ProfileComboBox()"));

	lpL4Profile = L4USR_lpGetProfile(m_lpuift);
	ULIB_ADM_GetDoc_Preset(m_lpuift, m_lpdm, szDocPreSet);

	if(lpL4Profile){
		/* 現在選択されているお気に入りが所有している外部DEVMODEの印刷目的の値--->L4Profile::m_shProQuaSelectionへ設定 */
		lpL4Profile->PRF_bSetProfileQulSelFromCurProfile(m_lpuift, (LPTSTR)szDocPreSet);
	}

	PUIDBGSvOUT(("L4SvBsProc::bOnCmd_ProfileComboBox() return TRUE"));
	
	return TRUE;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/06/25:0.00:ITO:CCD3
********************************************************************/
BOOL L4CmBsProc::bCheckProfile(LPDEVMODE lpdmProfile, LPDEVMODE lpdmCurrent)
{
	BOOL			bRet = FALSE;
	TCHAR/*char*/	tszDocName[DOCNAMEBUFSIZE];	/* 141028 GPG@SSS [v21.45][PreAlpha][Modify][UnicodeStoreDetail] */
	short			sPrnID = 0;
	LPPDL_CDEVMODE	lpPdlCurDev = NULL;
	LPPDL_CDEVMODE	lpPdlProDev = NULL;
	BYTE			byMBBoxNum = 0;
	DWORD			dwCurDocFlgs = 0, dwProDocFlgs = 0;
	/* 20061123 WQN@CSSSI [v1200][Add New Device][VERUP] Start */
	LPPDL_DEVICEOPTION	lpDevOpt 	= NULL;
	DWORD				dwSpecFlag 	= 0L;
	/* 20061123 WQN@CSSSI [v1200][Add New Device][VERUP] End */
	LPPDL_QUALITY		lpPdlProQuality = NULL;
	LPPDL_QUALITY		lpPdlCurQuality = NULL;
	LPPDL_PAPERSOURCE	lpPdlProPaperSource = NULL;		/* 091125 ISI@IST *//* MOD LIPS4_i_QA V20.20-00100 AND CIS Bug Report [BDL] */

	PUIDBGProcIN(("L4CmBsProc::bCheckProfile()"));

	/* The check at the time of profile selection is performed here. */

	sPrnID = (short)m_lpuift->ULIB_GetPrinterID(m_lpuift);
	/* 20061123 WQN@CSSSI [v1200][Add New Device][VERUP] Start */
	lpDevOpt = GetPdlDoPointer(m_lpuift->ULIB_GetDeviceOption(m_lpuift));
	/* 20061123 WQN@CSSSI [v1200][Add New Device][VERUP] End */
	lpPdlCurDev = GetPdlDmPointer(((LPSHAREDEV)lpdmCurrent));
	lpPdlProDev = GetPdlDmPointer(((LPSHAREDEV)lpdmProfile));
	lpPdlCurQuality = (LPPDL_QUALITY)&(lpPdlCurDev->pcCdevmode.pdlQuality);	/* @@@ */
	lpPdlProQuality = (LPPDL_QUALITY)&(lpPdlProDev->pcCdevmode.pdlQuality);	/* @@@ */
	LMEMSET(tszDocName, 0, sizeof(TCHAR/*char*/)*DOCNAMEBUFSIZE);	/* 141028 GPG@SSS [v21.45][PreAlpha][Modify][UnicodeStoreDetail] */
//#if 1	/* 091125 ISI@IST *//* MOD LIPS4_i_QA V20.20-00100 AND CIS Bug Report [BDL] */
	lpPdlProPaperSource = (LPPDL_PAPERSOURCE)&(lpPdlProDev->pcCdevmode.pdlPaperSource);
//#endif

	/* 020828 EBI@CSL show Conflict Message */
	bMessageCheckProfile(lpdmProfile, lpdmCurrent);
	
	if (lpPdlCurDev && lpPdlProDev) {
		/* E207UGLIPS-00060 */
		if( ID_STATE3_DISABLE != PDL_GetJobMngSecure(m_lpuift->ULIB_GetPrinterCap (m_lpuift)) ){
			LMEMCPY ((LPPDL_SECUREDPRINT)&(lpPdlProDev->pcCdevmode.pdlSecuredPrint), (LPPDL_SECUREDPRINT)&(lpPdlCurDev->pcCdevmode.pdlSecuredPrint), sizeof (PDL_SECUREDPRINT));
		}
	
		/*_ 041111 YMS@CSL お気に入りが変更された場合にBox番号がデフォルト値になる障害の対応(Legacy障害票V10.60_00028) */
		/*_ Mailbox setting */
		//if( PDLCMN_FunctionQuery(sPrnID, QUERY_BASE_MULTIBOXSUPPORT) ){	/* 150128 ZMJ@SSS [v21.50][PreAlpha][Delete][REQ000557] */
		LMEMCPY(lpPdlProDev->pcCdevmode.pdlBoxNumber.BoxNumber, lpPdlCurDev->pcCdevmode.pdlBoxNumber.BoxNumber, sizeof(BYTE)*13);
		/* 150128 ZMJ@SSS [v21.50][PreAlpha][Delete][REQ000557] Start */
		//}
		//else {
		//	ULIB_ADM_GetMal_BoxNumber2(m_lpuift, lpdmCurrent, &byMBBoxNum);	
		//	ULIB_ADM_SetMal_BoxNumber2(m_lpuift, lpdmProfile, &byMBBoxNum);
		//}
		/* 150128 ZMJ@SSS [v21.50][PreAlpha][Delete][REQ000557] End */

		/* Document Name setting */
		ULIB_ADM_GetMal_DocumentNameT(m_lpuift, lpdmCurrent, tszDocName);	/* 141028 GPG@SSS [v21.45][PreAlpha][Modify][UnicodeStoreDetail][Change from ULIB_ADM_GetMal_DocumentName] */
		ULIB_ADM_SetMal_DocumentNameT(m_lpuift, lpdmProfile, tszDocName);	/* 141028 GPG@SSS [v21.45][PreAlpha][Modify][UnicodeStoreDetail][Change from ULIB_ADM_SetMal_DocumentName] */
	
		/*_ Data Name setting */
		/*_ ID_USEDOCNAME_ON の引継ぎ */
		ULIB_ADM_GetDoc_Flags(m_lpuift, lpdmCurrent, &dwCurDocFlgs);
		ULIB_ADM_GetDoc_Flags(m_lpuift, lpdmProfile, &dwProDocFlgs);
		if (dwCurDocFlgs & ID_USEDOCNAME_ON) {
			dwProDocFlgs |= ID_USEDOCNAME_ON;
		}
		else {
			dwProDocFlgs &= ~ID_USEDOCNAME_ON;
		}
		ULIB_ADM_SetDoc_Flags(m_lpuift, lpdmProfile, &dwProDocFlgs);

		/* 20061030 LYN@CSSSI [v1200][CSSSI_LIPS_v12.00-00035] Start */
		/*if(JOBMNG_HOLDQUEUE & (CAP_GETJOBMNGFLAGS (sPrnID))){*/
		if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_BASE_HOLDSUPPORT) == TRUE){
			LMEMCPY((LPPDL_SECUREDPRINT)&(lpPdlProDev->pcCdevmode.pdlBoxNumber), (LPPDL_SECUREDPRINT)&(lpPdlCurDev->pcCdevmode.pdlBoxNumber), sizeof (PDL_BOXNUMBER));
			if(lpPdlCurDev->pcCdevmode.pdlBoxNumber.hqDestOption == HQ_DEST_USEMANUAL){
				lpPdlProDev->pcCdevmode.pdlBoxNumber.hqDestOption = HQ_DEST_USEMANUAL;		
			}
			else{
				lpPdlProDev->pcCdevmode.pdlBoxNumber.hqDestOption = HQ_DEST_USEFILENAME;
			}
		}
		/* 20061030 LYN@CSSSI [v1200][CSSSI_LIPS_v12.00-00035] End */
	
		/* 090821 YKA@CIS For V20.00 START */
		/* [お気に入り]を切り替えても現在の"ユーザ設定"の値は保持したままにする為の対応を入れました。 */
		/* ※但し、"ユーザ設定"が選択されている[お気に入り]を選択した場合はその値に変わります。       */
		if (lpPdlProQuality->quaPrintQualityFlags != ID_QUALITY_USER){
			lpPdlProQuality->quaUserGraphicMode			= lpPdlCurQuality->quaUserGraphicMode;
			lpPdlProQuality->quaUserBitsPixel			= lpPdlCurQuality->quaUserBitsPixel;
			lpPdlProQuality->quaUserSurfaceBitsPixel	= lpPdlCurQuality->quaUserSurfaceBitsPixel;
			lpPdlProQuality->quaUserPrivQuality			= lpPdlCurQuality->quaUserPrivQuality;
			lpPdlProQuality->quaUserPixelDepth			= lpPdlCurQuality->quaUserPixelDepth;
			lpPdlProQuality->quaUserGrayTonePattern		= lpPdlCurQuality->quaUserGrayTonePattern;
			lpPdlProQuality->quaUserHalfToning			= lpPdlCurQuality->quaUserHalfToning;
			lpPdlProQuality->quaUserHostRendering		= lpPdlCurQuality->quaUserHostRendering;
			lpPdlProQuality->quaUserTonerSave			= lpPdlCurQuality->quaUserTonerSave;
			lpPdlProQuality->quaUserFlags				= lpPdlCurQuality->quaUserFlags;
			lpPdlProQuality->quaUserSelection			= lpPdlCurQuality->quaUserSelection;
			lpPdlProQuality->quaUserRenderingFlags		= lpPdlCurQuality->quaUserRenderingFlags;
			lpPdlProQuality->quaUserImgAccel			= lpPdlCurQuality->quaUserImgAccel;
			lpPdlProQuality->quaUserGrayout				= lpPdlCurQuality->quaUserGrayout;
			lpPdlProQuality->quaUserAdvancedSmoothing	= lpPdlCurQuality->quaUserAdvancedSmoothing;
			lpPdlProQuality->quaUserLineControl			= lpPdlCurQuality->quaUserLineControl;
			lpPdlProQuality->quaUserJpegThrough			= lpPdlCurQuality->quaUserJpegThrough;
			lpPdlProQuality->quaUserGradationSmoothing	= lpPdlCurQuality->quaUserGradationSmoothing;
			lpPdlProQuality->quaUserImageCompression	= lpPdlCurQuality->quaUserImageCompression;
		}
		/* 090821 YKA@CIS For V20.00 END */

//#if 1	/* 091125 ISI@IST *//* MOD LIPS4_i_QA V20.20-00100 AND CIS Bug Report [BDL] */
		lpPdlProPaperSource->sFrontSourceID = PCUTILS_shGetFrontBackCoverSourceSupported(m_lpuift, lpdmProfile, lpdmCurrent, TRUE);
		lpPdlProPaperSource->sBackSourceID = PCUTILS_shGetFrontBackCoverSourceSupported(m_lpuift, lpdmProfile, lpdmCurrent, FALSE);
//#endif

		bRet = TRUE;
	}

	PUIDBGProcOUT(("L4CmBsProc::bCheckProfile() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]	bMessageCheckProfile
;[Action] Profile Conflict Message(PMSID_022) Indication Check 
	選択したお気に入りに不適切な設定がある場合メッセージを表示する
;[Format]
;[Input]
;[Output]
;cording:02/07/10:2.00:KNK@CSI:CCD3
********************************************************************/
BOOL L4CmBsProc::bMessageCheckProfile(LPDEVMODE lpdmProfile, LPDEVMODE lpdmCurrent)
{
	BOOL					bRet = TRUE;
	LPDEVMODE				lpdm;
	LPDEVMODE				lpDefdm;
	LPPDL_CDEVMODE			lpCDM;
	LPCAP_PRINTERCAP		lpCap;
	LPPDL_DEVICEOPTION		lpPDLDevOption;
	LPPDL_FINISHING			lpPDLFinishing;
	LPPDL_PAPERSOURCE		lpPDLPaperSource;
	LPPDL_QUALITY			lpPDLQuality;
	LPPDL_PAGESETUP			lpPDLPageSetup = NULL;
	L4Objective				*lpL4Obj;
	DWORD					dwloFlags = 0L, dwfshFlags = 0L, dwBkltFlags = 0L, dwPaperFlag = 0L;
	DWORD					dwReason = 0x0L, dwRsnFixBin = 0x0L, dwSupport = 0x0L;
	DWORD					dwCapQuaFlags = 0x0L, dwQualFlag = 0x0L;
	DWORD					dwStampFlag = 0x0, dwDocFlags = 0x0;
//	WORD					wDevVer   = 0L;		/* 090610 IKM@IST del for CapabilityData */
/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] Start */
//@@@ [SUPPORT] Add Start : KTO@CSL 060220 CCDコアGR障害票 E307GCL-0011
	DWORD					dwOptFlags = 0L;
//@@@ [SUPPORT] Add End
/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] End */
	DWORD					dwAvailMediaType1Array[MEDIATYPE1_ARRAY_SIZE];	/* 20060329 ZYS@CSSSI [v1102][E315_PAPERTYPE] */
	short					sCategory = 0, sPaperSelection = 0, sNUpPages = 0;
	short					sFirstPage = 0, s2ndPage = 0, sLastPage = 0, sInterLeaf = 0, sOHPSupply = 0, sDefaultSource = 0;	/* 20060425 LHW@CSSSI [v1110][LIPS_v11.00-0076] sOHPSupply */
	short					sPapFormSID = 0, sfshFixBin = 0, sloDevCategory = 0, sPaperType = 0, sInFormSID = 0;
	short					sQualPrivQuality = 0, sQualSelection = 0, sQualJpegThrough = 0, squaGraphicMode = 0, squaTonerSave = 0;
	short					sPrnID = 0, sOptPriority = 0, sloStretch = 0, squaPixelDepth = 0, squaColorMode = 0, sdocDestination = 0, sDocDestDefault = 0;
	short					squaGrayOut = 0;
	short					sDate = 0, sNumber = 0, sEdge = 0, sName = 0;
	short					sovlMode = 0;
	short					sletPages = 0;
	int						i;
	BYTE					byCursor = 0;
	short 					sMargin = 0, sBookletMargin = 0, sMaxBindMarginMm = 0;
	BOOL					bPaperTypeAvail;	/* 20060329 ZYS@CSSSI [v1102][E315_PAPERTYPE] */
	LPCAP_MEDIA				lpMedia;			/* 20060706 YUW@CSSSI [v1115][LIPS_v11.15-0007] */
	short					sPaperTypeNum = 0;	/* 20060706 YUW@CSSSI [v1115][LIPS_v11.15-0007] */
	SIZE					MaxSize = {0,0}, MinSize = {0,0};/* 20070906 LYN@CSSSI [v1220][SDK_kurumi_UDP] */
	size_t					nLen = -1;			/* string length *//* 20080701 WQN@CSSSI [v1240][LIPS_v12.35_Beta-0028] */
	/* 090423 WAT@IST LIPS4_i_QA V20.00-00137 */
	BOOL	bEqualTrimmer   = FALSE;
	BOOL	bEqualTrimmerTB   = FALSE;
	SHORT				sTrimMinmm = 0, sTrimMaxmm = 0, sTrimDefmm = 0, sTrimMinin = 0, sTrimMaxin = 0, sTrimDefin = 0;	/* 050930 KTA@IST add for Thu, 29 Sep 2005 14:56:45 */
	/* 090423 WAT@IST LIPS4_i_QA V20.00-00137 */

	BOOL	bMechanicalBin, bOutPaper, bDuplex, bBooklet, bGroup, bStaple, bStaplePos, bJobOffset, bRotate, bOffSet;
	BOOL	bFaceUp, bFold, bFixBin, bMechaSort, bStack, bUserSep, bInserter, bCopy;
	BOOL	bUserPaper, bPunch, bZFold, bCFold, bTonerDensity, bPaperType, bManual2sided, bOutputDest;/* 20060713 ZHF@CSSSI [v1125][CFold] */
	BOOL	bPaperSourceMedia, bTrayName, bNup, bStamp, bMixPaper, bForbid, bPanelDisplay;
	BOOL	bMetaFile, bBookletNup,bLayoutOpt;
	BOOL	bDefaultSource, bFirst, b2nd, bInterLeaf, bOHPSupply, bDefaultSourcePt, bFirstPt, b2ndPt, bLastPt, bInterLeafPt;	/* 20060425 LHW@CSSSI [v1110][LIPS_v11.00-0076] bOHPSupply */
	BOOL	bStretch, bSpHalfTone, bLast, bInformSize, bOutformSize, bDestination, bProof;
	BOOL	bBox, bPrintFromLastPage, bPcCollate, bPcNup, bLoOpt, bletFlags, bPaperSelection;
	BOOL	bColor/*, bCatalog*/;	/* 20071203 ZYS@CSSSI [v1225][REMOVE_SAMPLEPRINT] */													/*_ PrintQuality _*/
	BOOL	bGraphicMode, bModeDetail, bReso, bColorDepth, bColorHalfToneObj;	/*_ PrintQualityDetail _*/
	BOOL	bMonoHalfToneObj, bHalfToning, bTTSub, bVarit, bTonerSave;			/*_ PrintQualityDetail _*/
	BOOL	bGrayOut, bJPGThrough, bLineClear;									/*_ PrintQualityAdvanced _*/
	BOOL	bICM, bMatchingMethod, bMonitorProfile, bCanoFine, bDevCMS;			/*_ ColorSetting _*/
	BOOL	b50mm, bBooklet50mm, bStapleGroup, bFinishingMethodOff;/* 20080701 SYC@CSSSI [v1235][LIPS_v12.35_Beta-0021] */
/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] Start */
//@@@ [SUPPORT] Mod Start : KTO@CSL 060220 CCDコアGR障害票 E307GCL-0011
//	BOOL	bOvlay, bPerfectBind, bQualSelection, bAddNumPrint, bletPages, bOffsetCopies;
	BOOL	bOvlay, bPerfectBind, bQualSelection, bAddNumPrint, bletPages, bOffsetCopies, bDeviceBooklet;
	BOOL	bEmbedDocumentNumber;	/* 091116 SAI@IST add for LIPS4_i_QA V20.20-00044 */
//@@@ [SUPPORT] Mod End
/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] End */
	BOOL	bFixMonoColor;	/* 20060303 JZH@CSSSI [v1100][MONOCOLORFIX] */
	BOOL	bDestinationMsg;	/* 20060511 YUW@CSSSI [v1110][v11.02_00010] */
	BOOL	bPaperSelectionOHP;	/* 20060515 ZYS@CSSSI [v1110][CSSSI_LIPS_v11.10-00032] */
	BOOL	bCreepCorrection;	/* 20060717 ZYS@CSSSI [v1125][IB535] */
	BOOL	bJobJimon;	/* 20061010 CYH@CSSSI [v1200][JobJimon] */
	BOOL	bFrontBack;	/* 20061016 ZYS@CSSSI [v1200][E314_OPTION] */
	BOOL	bTranscription;/* 20070206 LYN@CSSSI [v1205][TRANSCRIPTION_MODE3] */
	BOOL	bOutputFace, bImgRotate;	/* 20070413 ZYS@CSSSI [v1210][FINISHINGMODES_CFOLD] */
	BOOL    bTrimWidth;	/* 20070703 LYN@CSSSI [v1215][LIPS_v12.10-0032] */
	BOOL	bAppBooklet;	/* 20070627 ZYS@CSSSI [v1215][APPLICATION_BOOKLET] */
	BOOL	bBodyPaper;		/* 20070907 LRH@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] */
	BOOL	bFinishingPaper;		/* 20070907 LRH@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] */
	BOOL	bCoverPaper;		/* 20070907 ZYS@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] */
	BOOL	bTabNoShift;		/* 20070912 WQN@CSSSI [v1215][CR0278][BOXHOLDQUEUE_TABNOSHIFT] */
	BOOL	bSetTrimWidth;		/* 20080401 CCR@CSSSI [v1235][E314_ThreeSideTrimmer] */
	BOOL	bCltnAll;		/* 20080408 MFG@CSSSI [v1235][ClearToner] */
	BOOL	bCltnPartly;	/* 20080408 MFG@CSSSI [v1235][ClearToner] */
	BOOL	bVFold;			/* 20080614 WQN@CSSSI [v1240][VFOLD] */
	BOOL	bSaddleFold;	/* 20080706 SYC@CSSSI [v1240][MultiFunctionFold] */
	BOOL	bSecureWatermark; 
	BOOL	bQueryRet = FALSE;
	BOOL	bFnMethodOff;	/* 20090327 SAI@IST add for LIPS4_i_QA V20.00-00090 */
	BOOL	bEmbedDocNum = TRUE;	/* 100318 YKA@CIS [Iona] Spec Change */
	BOOL	bShiftUnit = TRUE;		/* 100318 YKA@CIS [Iona] Spec Change */
	DWORD	dwAbility = 0, dwDrvDefault = 0;	/* 090108 ISI@IST *//* MOD LIPSDEV_v20.00_0008 [4] */
	LPCAP_PAPER			pPaper = NULL;			/* 090108 ISI@IST *//* MOD LIPSDEV_v20.00_0008 [4] */
	short	sBindDirect = 0;					/* 090108 ISI@IST *//* MOD LIPSDEV_v20.00_0008 [4] */
	BOOL	bTABSupport = TRUE;					/* 100317 ISI@IST *//* for V20.35 Dufftown */
	
	/* 121210 SQW@SSS [v20.90][Alpha][Add][Drv_v20_90_a_FamiCom-0014] add 7 and 8 for YG3 */
	BOOL		bIsEqual = FALSE, bIsEqual2 = FALSE, bIsEqual3 = FALSE, bIsEqual4 = FALSE, bIsEqual5 = FALSE, bIsEqual6 = FALSE, bIsEqual7 = FALSE, bIsEqual8 = FALSE;
	BOOL		bImPaperType = FALSE;
	LPSHAREDO	lpShareDO = NULL;
	HANDLE		hPrinter = NULL;
	EXCEPTIONSPEC_V100		stExceptionSpecV101 = {0};	/* 100317 ISI@IST *//* for V20.35 Dufftown */

	BOOL		bAutoColor = TRUE;	/* 100527 WAT@IST ColorUpdate */

	bMechanicalBin = bOutPaper = bDuplex = bBooklet = bGroup = bStaple = bStaplePos = bJobOffset = bRotate = bOffSet = 
	bFaceUp = bFold = bFixBin = bMechaSort = bStack = bUserSep = bInserter = bReso = bCopy =
	bUserPaper = bPunch = bZFold =  bCFold = bColorDepth = bColorHalfToneObj = bMonoHalfToneObj = bGrayOut = /* 20060713 ZHF@CSSSI [v1125][CFold] */
	bColor = bLineClear = bTTSub = bTonerDensity = bPaperType = bManual2sided = bOutputDest = bPaperSourceMedia = 
	bTrayName = bNup = bStamp = bCanoFine = bVarit = bMixPaper = bForbid = bPanelDisplay = bJPGThrough = bGraphicMode = 
	bMetaFile = bBookletNup = bLayoutOpt = bDefaultSource = bFirst = b2nd = bInterLeaf = bOHPSupply = bDefaultSourcePt = bFirstPt =	/* 20060425 LHW@CSSSI [v1110][LIPS_v11.00-0076] sOHPSupply */
	b2ndPt = bLastPt = bInterLeafPt = bStretch = bHalfToning = bModeDetail = bSpHalfTone = bPanelDisplay = bLast = 
	bInformSize = bOutformSize = bDestination = bProof = bBox = bICM = bPrintFromLastPage = bPcCollate = bPcNup = bLoOpt =
	bletFlags = bPaperSelection /* = bCatalog */ = bMatchingMethod = bMonitorProfile = bTonerSave = b50mm = bBooklet50mm = bOvlay = /* 20071203 ZYS@CSSSI [v1225][REMOVE_SAMPLEPRINT] */
/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] Start */
//@@@ [SUPPORT] Mod Start : KTO@CSL 060220 CCDコアGR障害票 E307GCL-0011
//	bPerfectBind = bDevCMS = bQualSelection = bAddNumPrint = bletPages = bOffsetCopies = bStapleGroup = TRUE;
	bPerfectBind = bDevCMS = bQualSelection = bAddNumPrint = bletPages = bOffsetCopies = bStapleGroup = bFinishingMethodOff = bDeviceBooklet = /* 20080701 SYC@CSSSI [v1235][LIPS_v12.35_Beta-0021] */
//@@@ [SUPPORT] Mod End	
/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] End */
	bDestinationMsg =		/* 20060511 YUW@CSSSI [v1110][v11.02_00010] */
	bPaperSelectionOHP =	/* 20060515 ZYS@CSSSI [v1110][CSSSI_LIPS_v11.10-00032] */
	bCreepCorrection =		/* 20060717 ZYS@CSSSI [v1125][IB535] */
	bTranscription =		/* 20070206 LYN@CSSSI [v1205][TRANSCRIPTION_MODE3] */
	bFrontBack =			/* 20061016 ZYS@CSSSI [v1200][E314_OPTION] */
	bOutputFace = bImgRotate =	/* 20070413 ZYS@CSSSI [v1210][FINISHINGMODES_CFOLD] */
	bTrimWidth =			/* 20070703 LYN@CSSSI [v1215][LIPS_v12.10-0032] */
	bAppBooklet =			/* 20070627 ZYS@CSSSI [v1215][APPLICATION_BOOKLET] */
	bBodyPaper =			/* 20070907 LRH@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] */
	bFinishingPaper =		/* 20070907 LRH@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] */
	bCoverPaper =			/* 20070907 ZYS@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] */
	bTabNoShift =			/* 20070912 WQN@CSSSI [v1215][CR0278][BOXHOLDQUEUE_TABNOSHIFT] */
	bSetTrimWidth=			/* 20080401 CCR@CSSSI [v1235][E314_ThreeSideTrimmer] */
	bCltnAll =			/* 20080408 MFG@CSSSI [v1235][ClearToner] */
	bCltnPartly =		/* 20080408 MFG@CSSSI [v1235][ClearToner] */
	bVFold =			/* 20080614 WQN@CSSSI [v1240][VFOLD] */
	bFixMonoColor =		/* 20060303 JZH@CSSSI [v1100][MONOCOLORFIX] */
	bSecureWatermark = /* 20090419 SAI@IST add for LIPS4_i_QA V20.00-00097 */
	bFnMethodOff =		/* 20090327 SAI@IST add for LIPS4_i_QA V20.00-00090 */
	bEmbedDocumentNumber =	/* 091116 SAI@IST add for LIPS4_i_QA V20.20-00044 */
	bSaddleFold = TRUE;	/* 20080706 SYC@CSSSI [v1240][MultiFunctionFold] */
	CMSDATA				CMSData = {0};

#if (WINVER < 0x0500) && defined(WINNT)
	/* Win2000 のお気に入りをNT40で読み込んだ際のチェック	*/
	BOOL				b2K = TRUE;
	short				sloNUpPages = 0;
	short				sEdging = 0;
	short				sPrintDate = 0x0;
	short				sUserName = 0x0;
	short				sPageNumber = 0x0;
	short				squaColor = 0;
	CMSDATA				CMSData = {0};	/* 090210 ISI@IST *//* LIPS_v20.00_Alpha-0047 */
#endif

	BOOL	bSkipWhitePage = TRUE;
	BOOL	bGutter = TRUE;
	short	sLoBindMargin = 0;
	BOOL	bCheckCoverSupport = FALSE;
	/* 140612 ZMJ@SSS [v21.35][PreAlpha][Add][ECOStaple] Start */
	//CDM_SIZE	cdmSiz;	/* 140903 SQW@SSS [v21.35][Beta][Delete][CommonUI_v21.35-0050] */
	BOOL	bECOStaple = TRUE;
	BOOL	bOficioSetting = FALSE;
	/* 140612 ZMJ@SSS [v21.35][PreAlpha][Add][ECOStaple] End */
	BOOL	bIgnorePaperSelection = FALSE;	/* 140808 CHL@SSS [v21.40][PreAlpha][Add][UsabilityUp4SelectPaperType] */

	PUIDBGProcIN(("L4CmBsProc::bMessageCheckProfile()"));
	PUIDBGProfileIN(("L4CmBsProc::bMessageCheckProfile()"));

	lpdm 				= lpdmCurrent;
	lpDefdm				= m_lpuift->ULIB_GetDevMode(m_lpuift, CUIID_DRIVER_DEFAULT);
	lpCDM				= GetPdlDmPointer(((LPSHAREDEV)lpdmProfile));
	lpCap			 	= (LPCAP_PRINTERCAP)(m_lpuift->ULIB_GetPrinterCap(m_lpuift));
	lpPDLDevOption		= GetPdlDoPointer(m_lpuift->ULIB_GetDeviceOption(m_lpuift));
	lpPDLFinishing		= &(lpCDM->pcCdevmode.pdlFinishing);
	lpPDLQuality		= &(lpCDM->pcCdevmode.pdlQuality);
	lpPDLPaperSource	= &(lpCDM->pcCdevmode.pdlPaperSource);
	lpPDLPageSetup		= &(lpCDM->pcCdevmode.pdlPageSetup);
	sPrnID				= (short)m_lpuift->ULIB_GetPrinterID(m_lpuift);
	lpL4Obj				= L4USR_lpGetObjective(m_lpuift);
	
	sMargin=sBookletMargin=sMaxBindMarginMm=0;
	sMaxBindMarginMm 	= ULIB_CAP_GetMaxBindMarginMm(m_lpuift);
	lpMedia				= ULIB_CAP_GetPaperTypeInfo(m_lpuift);	/* 20060706 YUW@CSSSI [v1115][LIPS_v11.15-0007] */
	sPaperTypeNum		= PDL_GetNumPaperType(lpCap);			/* 20060706 YUW@CSSSI [v1115][LIPS_v11.15-0007] */
	
	ULIB_ADM_GetLay_Flags		(m_lpuift, lpdmProfile, &dwloFlags);
	ULIB_ADM_GetLay_Category	(m_lpuift, lpdmProfile, &sCategory);
	ULIB_ADM_GetLay_DevCategory	(m_lpuift, lpdmProfile, &sloDevCategory);
	ULIB_ADM_GetLay_OutFormSID	(m_lpuift, lpdmProfile, &sPapFormSID);
	ULIB_ADM_GetLay_InFormSID	(m_lpuift, lpdmProfile, &sInFormSID);
	ULIB_ADM_GetLay_PrivScale   (m_lpuift, lpdmProfile, &sloStretch);
	ULIB_ADM_GetLay_NupPages	(m_lpuift, lpdmProfile, &sNUpPages);
	ULIB_ADM_GetLay_BindMargin	(m_lpuift, lpdmProfile, &sMargin);
	/* 140903 SQW@SSS [v21.35][Beta][Delete][CommonUI_v21.35-0050] Start */
	//LMEMSET(&cdmSiz, 0, sizeof(CDM_SIZE));	/* 140612 ZMJ@SSS [v21.35][PreAlpha][Add][ECOStaple] */
	//ULIB_ADM_GetLay_OutSizeFormSize(m_lpuift, m_lpdm, &cdmSiz);	/* 140612 ZMJ@SSS [v21.35][PreAlpha][Add][ECOStaple] */
	/* 140903 SQW@SSS [v21.35][Beta][Delete][CommonUI_v21.35-0050] End */

	ULIB_ADM_GetLyo_Edging		(m_lpuift, lpdmProfile, &sEdge);
	ULIB_ADM_GetLyo_Date		(m_lpuift, lpdmProfile, &sDate);
	ULIB_ADM_GetLyo_Name		(m_lpuift, lpdmProfile, &sName);
	ULIB_ADM_GetLyo_Number		(m_lpuift, lpdmProfile, &sNumber);

	ULIB_ADM_GetFin_Flags		(m_lpuift, lpdmProfile, &dwfshFlags);
	ULIB_ADM_GetFin_FixbinNo	(m_lpuift, lpdmProfile, &sfshFixBin);
	ULIB_ADM_GetBok_letFlags	(m_lpuift, lpdmProfile, &dwBkltFlags);
	ULIB_ADM_GetBok_letBindMargin(m_lpuift, lpdmProfile, &sBookletMargin);
	ULIB_ADM_GetBok_letPages	(m_lpuift, lpdmProfile, &sletPages);

	ULIB_ADM_GetPps_Flags		(m_lpuift, lpdmProfile, &dwPaperFlag);
	ULIB_ADM_GetPps_PaperType	(m_lpuift, lpdmProfile, &sPaperType);
	ULIB_ADM_GetPps_FirstPage	(m_lpuift, lpdmProfile, &sFirstPage);
	ULIB_ADM_GetPps_2ndPage		(m_lpuift, lpdmProfile, &s2ndPage);
	ULIB_ADM_GetPps_LastPage	(m_lpuift, lpdmProfile, &sLastPage);
	ULIB_ADM_GetPps_Interleaf	(m_lpuift, lpdmProfile, &sInterLeaf);
	ULIB_ADM_GetPps_OHP			(m_lpuift, lpdmProfile, &sOHPSupply);		/* 20060425 LHW@CSSSI [v1110][LIPS_v11.00-0076] Get OHP Supply */
	ULIB_ADM_GetPps_PaperSelection(m_lpuift, lpdmProfile, &sPaperSelection);
	sDefaultSource = lpdmProfile->dmDefaultSource;
	
	ULIB_ADM_GetQal_Flags		(m_lpuift, lpdmProfile, &dwQualFlag);
	ULIB_ADM_GetQal_Selection	(m_lpuift, lpdmProfile, &sQualSelection);
	ULIB_ADM_GetQal_JPEGThrough	(m_lpuift, lpdmProfile, &sQualJpegThrough);
	ULIB_ADM_GetQal_PrivQuality	(m_lpuift, lpdmProfile, &sQualPrivQuality);
	ULIB_ADM_GetQal_GraphicsMode(m_lpuift, lpdmProfile, &squaGraphicMode);
	ULIB_ADM_GetQal_PixelDepth	(m_lpuift, lpdmProfile, &squaPixelDepth);
	ULIB_ADM_GetQal_ColorMode	(m_lpuift, lpdmProfile, &squaColorMode);
	ULIB_ADM_GetQal_TonerSave	(m_lpuift, lpdmProfile, &squaTonerSave);
	
	ULIB_ADM_GetStm_Flags(m_lpuift, lpdmProfile, &dwStampFlag );
	ULIB_ADM_GetDoc_Destination(m_lpuift, lpdmProfile, &sdocDestination);
	ULIB_ADM_GetDoc_Flags		(m_lpuift, lpdmProfile, &dwDocFlags);
	ULIB_ACDO_GetOpt_Priority	(m_lpuift, &sOptPriority);
	ULIB_ADM_GetOvl_Mode		(m_lpuift, lpdmProfile, &sovlMode);
	ULIB_ADM_GetDoc_Destination (m_lpuift, lpdm, &sDocDestDefault);
/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] Start */
//@@@ [SUPPORT] Add Start : KTO@CSL 060220 CCDコアGR障害票 E307GCL-0011
	ULIB_ACDO_GetOpt_Flags (m_lpuift, &dwOptFlags);
//@@@ [SUPPORT] Add End
/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] End */
	/*_ 031021 UCH@CSL FS技術評価障害票 No-00054 */
	/*_ 原稿用紙、出力用紙サイズがサポートされているか */
	bInformSize  = L4_UTL_bIsPaperSupported( sInFormSID, lpCap);
	bOutformSize = L4_UTL_bIsOutPaperSupported( sPapFormSID, lpCap);

	/* 140808 CHL@SSS [v21.40][PreAlpha][Add][UsabilityUp4SelectPaperType] Start */
	if(PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_PAPE_MFP_PSPT_SETSAMETIME)){
		if(sPaperSelection == ID_PS_ALLPAGES){
			bIgnorePaperSelection = TRUE;
		}
	}
	/* 140808 CHL@SSS [v21.40][PreAlpha][Add][UsabilityUp4SelectPaperType] End */
//	wDevVer = DEVVERSTR_0200;		/* 090610 IKM@IST del for CapabilityData */
	
	/* 印刷目的が現在のバージョンでサポートされているか */
	if (!(L4_UTL_bIsItemOnList ((WORD)sQualSelection, lpCap))){
		bQualSelection = FALSE;
		/* CCD2--->CCD3：お気に入りに包含されている印刷目的をマップ
			[文書/表]--->[一般]
			[写真画像]--->[写真]:DEVMODEのIDは同じなので、L4_UTL_bIsItemOnList()で継承される */
		if (sQualSelection == ID_QUASELECTION_DOCUMENT_TABLE) {
			sQualSelection = ID_QUASELECTION_GENERAL;
		} else {
			lpCDM->pcCdevmode.pdlQuality.quaPrintQualityFlags = ID_QUALITY_USER;
		}
		ULIB_ADM_SetQal_Selection (m_lpuift, lpdmProfile, &sQualSelection);
		
	} else {
		short	shQuaSelTmp = sQualSelection;
		shQuaSelTmp &= ~ID_QUASELECTION_ON_EDIT;
		if (shQuaSelTmp == ID_QUASELECTION_OTHER) {
			if(lpL4Obj) {
				/* ユーザ設定サポート機種([その他の設定]印刷目的非サポート機種)： */
				/* [その他の設定]印刷目的のお気に入り読み込み時--->印刷目的:一般 印刷品質:ユーザ設定 へ丸め処理 */
				if( lpL4Obj->OBJ_bIsSupportQuaDetail() ){
					bQualSelection = FALSE;
					sQualSelection = ID_QUASELECTION_GENERAL;
					ULIB_ADM_SetQal_Selection (m_lpuift, lpdmProfile, &sQualSelection);
					
					lpCDM->pcCdevmode.pdlQuality.quaPrintQualityFlags = ID_QUALITY_USER;
				}
			}
		}
	}
	/* 050601 KTO@CSL NUPと[最終ページから印刷]とオーバレイとの禁則 */
#if !defined(WINNT) || (WINVER >= 0x0500)
	if (ULIB_CAP_GetEngineType(m_lpuift) == ID_ENGINETYPE_BJ) {
		if ((sovlMode != ID_OVERLAYMODE_NONE) && (dwfshFlags & ID_REVERSE_ON) && (sCategory == ID_CATEGORY_NUP)) {
				bOvlay = FALSE;
		}
	}
#endif
	/* 051026 KTO@CSL [保存][ホールド]と[オーバレイ]-[デバイス側のフォームを使用]の禁則 および[保存][セキュアプリント][ホールド]と[オーバレイ]-[デバイス側に保存]の禁則 */
	if ((sovlMode == ID_OVERLAYMODE_PRINT) && (lpPDLPageSetup->pageOverlayPrintType == ID_PDLOVERLAY_PRINTTYPE_IMAGEDATADEVICE)) {
		if (sDocDestDefault == ID_DST_STORE || sDocDestDefault == ID_DST_HOLDQUEUE) {	/* 20070331 WQN@CSSSI [v1205][E314_D_LIPS-0015] add hold queue check */
			bOvlay = FALSE;
		}
	} else if ((sovlMode == ID_OVERLAYMODE_CREATE) && (lpPDLPageSetup->pageOverlayCreateType == ID_PDLOVERLAY_CREATETYPE_DEVICE)) {
		if ((sDocDestDefault == ID_DST_STORE) || (sDocDestDefault == ID_DST_SECUREDPRINT) || (sDocDestDefault == ID_DST_HOLDQUEUE)) { /* 20070331 WQN@CSSSI [v1205][E314_D_LIPS-0015] add hold queue check */
			bOvlay = FALSE;
		}
	}

	/* 20070807 ZYS@CSSSI [v1215][E230FGH_D_LIPS-0009] Start */
	if ((sovlMode == ID_OVERLAYMODE_PRINT) && (lpPDLPageSetup->pageOverlayPrintType == ID_PDLOVERLAY_PRINTTYPE_IMAGEDATADEVICE)) {
		if (lpCDM->pcCdevmode.pdlLayout.loFlags & ID_LAYOUT_PERFECTBIND) {
			bOvlay = FALSE;
		}
	}
	/* 20070807 ZYS@CSSSI [v1215][E230FGH_D_LIPS-0009] End */

/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] Start */
//@@@ [SUPPORT] Add Start : KTO@CSL 060220 CCDコアGR障害票 E307GCL-0011
	/* 060220 KTO@CSL [編集＋プレビュー]時はオーバレイOFF */
	if (((sovlMode == ID_OVERLAYMODE_PRINT) || (sovlMode == ID_OVERLAYMODE_CREATE)) && (sdocDestination == ID_DST_PAGECOMPOSER))
		bOvlay = FALSE;
//@@@ [SUPPORT] Add End
/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] End */

	/* 20080408 MFG@CSSSI [v1235][ClearToner] Start */
	if( (PDLCMN_FunctionQuery(sPrnID, QUERY_PAGE_CLEAR_TONER_SUPPORT) == FALSE) && lpPDLPageSetup->pageClearTonerMode != ID_PDLCLEARTONER_NONE) {
		lpPDLPageSetup->pageClearTonerMode = ID_PDLCLEARTONER_NONE;
	}

	if(lpPDLPageSetup->pageClearTonerMode == ID_PDLCLEARTONER_ALLCLEAR) {
		if((sDocDestDefault == ID_DST_STORE) || (sDocDestDefault == ID_DST_PAGECOMPOSER) || (sDocDestDefault == ID_DST_HOLDQUEUE)) {	/* 20080605 MFG@CSSSI [v1235][CR0386] */
			bCltnAll = FALSE;
		}
	}

	/* 20080605 MFG@CSSSI [v1235][CR0386] Start */
	if(lpPDLPageSetup->pageClearTonerMode == ID_PDLCLEARTONER_PARTLYCLEAR){
		if ((sDocDestDefault == ID_DST_STORE) || 
			(sDocDestDefault == ID_DST_HOLDQUEUE) ||
			(sDocDestDefault == ID_DST_PAGECOMPOSER)) {
				bCltnPartly = FALSE;
		}
	}

	if(lpPDLPageSetup->pageClearTonerMode == ID_PDLCLEARTONER_PARTLYCLEAR){
		if ((sDocDestDefault == ID_DST_STORE) || 
			(sDocDestDefault == ID_DST_HOLDQUEUE) ||
			(sDocDestDefault == ID_DST_PAGECOMPOSER) ||
			(sDocDestDefault == ID_DST_SECUREDPRINT)) {
				bCltnPartly = FALSE;
		}
	}
	/* 20080605 MFG@CSSSI [v1235][CR0386] End */

	/* 20080408 MFG@CSSSI [v1235][ClearToner] End */

/*	if (dwfshFlags & ID_MECHANICALBIN_ON){
		bMechanicalBin = (lpPDLDevOption->optFinishing & ID_FSH_MULTIOUTPUTTRAY12_B2) ? TRUE : FALSE;
		if (!bMechanicalBin){
			dwfshFlags &= ~ID_MECHANICALBIN_ON;
			if (dwOptFin & ID_FSH_MULTIOUTPUTTRAY12_B2){
				if (dwfshFlags & (ID_MECHANICAL_ON | ID_STAPLE_ON)){
					dwfshFlags &= ~(ID_MECHANICAL_ON | ID_STAPLE_ON);
					dwfshFlags |= ID_SORT_ON;
				}else if (dwfshFlags & ID_GROUP_ON){
					dwfshFlags &= ~ID_GROUP_ON;	
				}
			}
			ULIB_ADM_SetFin_Flags(m_lpuift, lpdm, &dwfshFlags);
		}
	}
*/
	
	/* 020711 MYS@CSL LIPS4 LegacyDriver障害票 V890_490_00027 対策 */
	if( dwloFlags & ID_OUTPAPERFIXED_ON ) {
		bOutPaper = L4_UTL_bIsOutPutSizePossible(m_lpuift);
	}
	
	if( (dwloFlags & ID_2SIDEDPRINT_ON) && (sCategory != ID_CATEGORY_BOOKLET) &&
		(!(lpCDM->pcCdevmode.pdlLayout.loFlags & ID_LAYOUT_PERFECTBIND)) )
		bDuplex = L4_UTL_bIsDuplexPossible(m_lpuift, (LPDWORD)NULL, lpdmProfile); /* 20051202 JZH@CSSSI [v1100][IB252][IB263] */

	/*_ 050905 UCH@CSL 片面機のお気に入りを両面可能機に読み込んだときの丸め処理 */
	if ( !(lpdmProfile->dmFields & DM_DUPLEX) ) {
		if ( (BOOL)L4_UTL_bIsDuplexPossible(m_lpuift, (LPDWORD)NULL, lpdmProfile) ) {	/* 20051202 JZH@CSSSI [v1100][IB252][IB263] */
			lpdmProfile->dmFields |= DM_DUPLEX;
		}
	}
	
	if( sCategory == ID_CATEGORY_BOOKLET ) {
		/* 20070627 ZYS@CSSSI [v1215][CSSSI_LIPS_v12.15-00041] Start */
		DWORD	dwBookletReason = 0x0L;
		bBooklet = L4_UTL_bIsBookletPossible( m_lpuift, &dwBookletReason, lpdmProfile);	/* 20051202 JZH@CSSSI [v1100][IB252][IB263] */
		if (dwBookletReason & CMRE_APPLICATIONBOOKLET) {
			bAppBooklet = FALSE;
		}
		/* 20070627 ZYS@CSSSI [v1215][CSSSI_LIPS_v12.15-00041] End */
	}
	
	if (!PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_TRIMMERSUPPORT)) {
		if(lpCDM->pcCdevmode.pdlFinishing.fshFlags & ID_FSH_USE_TRIMMER){
			bTrimWidth = FALSE;
		}
	}
	else{
		bEqualTrimmer = PDLCMN_bIsEqualFinisherID(ID_FSH_TRIMMER_FORE, lpPDLDevOption->dwFinisher);

		if( bEqualTrimmer == FALSE){
			bEqualTrimmer = PDLCMN_bIsEqualFinisherID(ID_FSH_TRIMMER_INBLT, lpPDLDevOption->dwFinisher);
		}

		if((lpCDM->pcCdevmode.pdlFinishing.fshFlags &  ID_FSH_PERFECTBIND_ON)){
			bEqualTrimmer = TRUE;
		}

		if( bEqualTrimmer == FALSE){
			if(lpCDM->pcCdevmode.pdlFinishing.fshFlags & ID_FSH_USE_TRIMMER){
				bTrimWidth = FALSE;
			}
		}

		if( sCategory == ID_CATEGORY_BOOKLET ){
		PDLCMN_vGetTrimWidthAbility(ID_TRIMMER_BOOK, sPrnID, &sTrimMinmm, &sTrimMaxmm, &sTrimDefmm, &sTrimMinin, &sTrimMaxin, &sTrimDefin);
			if(lpCDM->pcCdevmode.pdlFinishing.fshTrimmerSize < sTrimMinmm){
				bTrimWidth = FALSE;
			}
			else if(lpCDM->pcCdevmode.pdlFinishing.fshTrimmerSize > sTrimMaxmm){
				bTrimWidth = FALSE;
			}
		}
	}

	/* 20080401 CCR@CSSSI [v1235][E314_ThreeSideTrimmer] Start */
	if (PDLCMN_FunctionQuery(sPrnID, QUERY_FINI_TOPBTM_TRIMMERSUPPORT)){
		{
//#if 1	/* 100427 KIS@CIS LIPS4_i_QA V20.35-00009 */
			if ( (lpPDLDevOption->dwFinisher & ID_FSH_TRIMMER_FORE) &&
				 (lpPDLDevOption->dwFinisher & ID_FSH_TRIMMER_TOPBTM))
//#else	/* 100427 KIS@CIS LIPS4_i_QA V20.35-00009 */
//			if (lpPDLDevOption->dwFinisher & (ID_FSH_TRIMMER_FORE | ID_FSH_TRIMMER_TOPBTM))
//#endif	/* 100427 KIS@CIS LIPS4_i_QA V20.35-00009 */
			{
				if (lpCDM->pcCdevmode.pdlFinishing.fshFlags & ID_FSH_USE_TRIMMER){
					if (!(lpCDM->pcCdevmode.pdlFinishing.fshFlags & ID_FSH_ADJUST_TRIMMER)){
						bSetTrimWidth = FALSE;
					}
				}
			}
		}
	}
	/* 20080401 CCR@CSSSI [v1235][E314_ThreeSideTrimmer] End */
	if ( lpCDM->pcCdevmode.pdlFinishing.fshFlags & ID_FSH_PERFECTBIND_ON ) {
/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] Start */
//@@@ [SUPPORT] Mod Start : KTO@CSL 060220 CCDコアGR障害票 E307GCL-0011
//		bPerfectBind = FALSE;

		if(	(lpPDLDevOption->dwDevFuncFlags & ID_DEVFUNC_PERFECTBINDER) != ID_DEVFUNC_PERFECTBINDER ){
			bPerfectBind = FALSE;
		} 

		else if (!(lpPDLDevOption->dwFinisher & ID_FSH_PERFECTBINDER)) {
			bPerfectBind = FALSE;
		}
		else if /*(*/(sdocDestination == ID_DST_PAGECOMPOSER) /*&& (dwOptFlags & ID_PAGECOMPOSER_LOCK))*/ {  /*110408 JJJ@SSS [v20.60][PreAlpha][Modify][OutputMethodFirst][ホールド＆デバイスのセキュリティ印刷機能] */
			bPerfectBind = FALSE;
		}
/*110408 JJJ@SSS [v20.60][PreAlpha][Delete][OutputMethodFirst][ホールド＆デバイスのセキュリティ印刷機能] Start*/
//#if 0
//		else if ((bPerfectBind) && (sdocDestination == ID_DST_PAGECOMPOSER)) {
//			/* [編集＋プレビュー]ロックOFF時は[くるみ製本]優先 */
//			/* 20060511 YUW@CSSSI [v1110][v11.02_00010] Start */
//			bDestinationMsg = FALSE;
//			/* sdocDestination = ID_DST_PRINTER;
//			ULIB_ADM_SetDoc_Destination(m_lpuift, lpdmProfile, &sdocDestination); */
//			/* 20060511 YUW@CSSSI [v1110][v11.02_00010] End */
//		}
//#endif
/*110408 JJJ@SSS [v20.60][PreAlpha][Delete][OutputMethodFirst][ホールド＆デバイスのセキュリティ印刷機能] End*/
//@@@ [SUPPORT] Mod End
/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] End */

		/* 20070907 ZYS@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] Start */
		/* くるみ表紙の設定 */
		if (bPerfectBind) {
			/* 用紙サイズ */
			if (!L4_UTL_bIsPBPaperPossible(m_lpuift, lpdmProfile)) {
				bCoverPaper = FALSE;
				lpCDM->pcCdevmode.pdlFinishing.fshPBPaper = DMPAPER_A3;
			}

			/* 給紙部 */
			/* 20080627 HXM@CSSSI [v1300][Multi-Inserter] Start */
			if(sPrnID == IDM_iPR1110){
				if(!((lpCDM->pcCdevmode.pdlFinishing.fshPBSource == DMBIN_MULTIINSERTER1)||(lpCDM->pcCdevmode.pdlFinishing.fshPBSource == DMBIN_MULTIINSERTER2) || (lpCDM->pcCdevmode.pdlFinishing.fshPBSource == DMBIN_MULTIINSERTER3))){
					short sCoverPaperSize;

					if ((DMPAPER_CCDUSER_TOP <= lpCDM->pcCdevmode.pdlFinishing.fshPBPaper) && (lpCDM->pcCdevmode.pdlFinishing.fshPBPaper <= DMPAPER_CCDUSER_LAST)) {
						sCoverPaperSize = DMPAPER_USER;					/* Check the User Defined Page Size: 256 */
					} else {
						sCoverPaperSize = lpCDM->pcCdevmode.pdlFinishing.fshPBPaper;	/* Check the fixed Page Size */
					}
					
					if (!L4_UTL_bCheckPaperSourceSet(m_lpuift, lpdmProfile, MAKELONG(lpCDM->pcCdevmode.pdlFinishing.fshPBSource, sCoverPaperSize))) {
						bCoverPaper = FALSE;
						lpCDM->pcCdevmode.pdlFinishing.fshPBSource		= DMBIN_CASSETTE1;
						lpCDM->pcCdevmode.pdlFinishing.fshPBPrintMethod	= ID_PBPRINTMETHOD_NOTPRINT;/* くるみ印刷方法 - 印刷しない */
						lpCDM->pcCdevmode.pdlFinishing.fshPBFrontSide	= ID_PRINTSIDE_NONE;		/* 表紙の印刷面   -なし */
						lpCDM->pcCdevmode.pdlFinishing.fshPBBackSide	= ID_PRINTSIDE_NONE;		/* 裏表紙の印刷面 -なし */
					}

				}
			}
			/* 20080627 HXM@CSSSI [v1300][Multi-Inserter] End */
			else if (lpCDM->pcCdevmode.pdlFinishing.fshPBSource	!= ID_DMBIN_MULTIINSERTER2) {	/* 100420 KIS@CIS BDL_Dev V20.35-00007 - DMBIN_MULTIINSERTER2->ID_DMBIN_MULTIINSERTER2 */
				short sCoverPaperSize;

				if ((DMPAPER_CCDUSER_TOP <= lpCDM->pcCdevmode.pdlFinishing.fshPBPaper) && (lpCDM->pcCdevmode.pdlFinishing.fshPBPaper <= DMPAPER_CCDUSER_LAST)) {
					sCoverPaperSize = DMPAPER_USER;					/* Check the User Defined Page Size: 256 */
				} else {
					sCoverPaperSize = lpCDM->pcCdevmode.pdlFinishing.fshPBPaper;	/* Check the fixed Page Size */
				}

				if (!L4_UTL_bCheckPaperSourceSet(m_lpuift, lpdmProfile, MAKELONG(lpCDM->pcCdevmode.pdlFinishing.fshPBSource, sCoverPaperSize))) {
					bCoverPaper = FALSE;
					lpCDM->pcCdevmode.pdlFinishing.fshPBSource		= DMBIN_CASSETTE1;
					lpCDM->pcCdevmode.pdlFinishing.fshPBPrintMethod	= ID_PBPRINTMETHOD_NOTPRINT;/* くるみ印刷方法 - 印刷しない */
					lpCDM->pcCdevmode.pdlFinishing.fshPBFrontSide	= ID_PRINTSIDE_NONE;		/* 表紙の印刷面   -なし */
					lpCDM->pcCdevmode.pdlFinishing.fshPBBackSide	= ID_PRINTSIDE_NONE;		/* 裏表紙の印刷面 -なし */
				}
			}
		}
		/* 20070907 ZYS@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] End */

		/* 20070907 LRH@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] Start */
		if (!L4_UTL_bIsBodyPagePossible(m_lpuift, lpdmProfile)) {
			bBodyPaper = FALSE;
		}

		if (!L4_UTL_bIsFinishingPaperPossible(m_lpuift, lpdmProfile)) {
			bFinishingPaper = FALSE;
		}
		/* 20070907 LRH@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] End */
	}
	
	if (dwfshFlags & ID_ROTATE_ON){
		bRotate = L4_UTL_bIsRotatePossible(m_lpuift, lpdmProfile);/* 20060925 ZYS@CSSSI [v1125][LIPS_v11.25-0023] */
		if(bRotate) {
			/* OPENCPCA機の場合 */
			if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_NOT_OPENCPCA_DEVICE) == FALSE ){
				if ((!(dwfshFlags & ID_SORT_ON)) && (!(dwfshFlags & ID_GROUP_ON))) { /* 指定しない=SORTもGROUPも立っていない状態 */
					bRotate = FALSE;
				}
			}

//#if 1	/* 100802 YKA@CIS [Iona] Spec Change */
			if (PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_HDD_OFF_DRIVER_COLLATE)) {
				if (!(lpPDLDevOption->dwUnitFlags & ID_UNIT_EQUIP_HDD)) {
					if (dwfshFlags & ID_SORT_ON) {
						if(dwfshFlags & ID_ROTATE_ON) {
							bRotate = FALSE;
						}
					}
				}
			}
//#endif	/* 100802 YKA@CIS [Iona] Spec Change */
		}
	}
	/*_ Staple Group */
	if ( (dwfshFlags & ID_STAPLE_ON) && (dwfshFlags & ID_GROUP_ON) ) {
		if(PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_STAPLEGROUP_SUPPORT)){
			bStapleGroup = L4_UTL_bIsStaplePossible(m_lpuift,NULL,lpdmProfile);	/* 20060713 WQN@CSSSI [v1125][LIPS_v11.15-0019] */

			/* 20080701 SYC@CSSSI [v1235][LIPS_v12.35_Beta-0021] Start */
			/* For Conflict of Staple&Group vs. HoldQueue, Staple&Group vs. Store */
			if ((sdocDestination == ID_DST_HOLDQUEUE) || (sdocDestination == ID_DST_STORE)) {
				bStapleGroup = FALSE;
			}
			/* 20080701 SYC@CSSSI [v1235][LIPS_v12.35_Beta-0021] End */
		}
		else {
			bStapleGroup = FALSE;
		}
	}
	
	/* 排紙方法は指定しないの場合 */
	if ((dwfshFlags & (ID_SORT_ON | ID_STAPLE_ON | ID_GROUP_ON | ID_FACEUP_ON | ID_JOBOFFSET_ON)) == 0) {
		if (sdocDestination == ID_DST_STORE) {
			bFnMethodOff = FALSE;
		}
	}

	PDLCMN_bGetFinisherAbility(lpCap, lpPDLDevOption, &dwAbility);
	ULIB_ADM_GetDoc_Flags(m_lpuift, lpdmProfile, &dwDrvDefault);
	pPaper	= PDL_GetPaper(lpCap);
	
	if(dwAbility & ENABLE_JOBOFFSET){
		/*_ 「指定しない」と「シフト」のコンフリクト対応 */
		DWORD dwWorkFshFlags;
		dwWorkFshFlags = (ID_SORT_ON | ID_GROUP_ON | ID_STAPLE_ON);
		if(!(dwWorkFshFlags & dwfshFlags)){
			if(lpCDM->pcCdevmode.pdlFinishing.fshFlags & ID_PDL_OFFSET_ON){	/* 050224 AYA@IST WinV1.30JP00008 */
				bOffSet = FALSE;
			}
		}
		/* 050920 KTA@IST add start for WinV1.45JP00028 */
		/* ドライバデフォルト以外のお気に入り */
		if( ((dwDrvDefault & ID_DOCPROP_PRESETDD) == 0 ) &&
			((dwDrvDefault & ID_DOCPROP_PRESETCUSTOM) == 0 ) ){		/* 061124 AYA@IST V2.00_C_00004 */
			if((lpCDM->pcCdevmode.pdlFinishing.fshFlags & ID_PDL_OFFSET_ON) != 0){
				pPaper = NULL;
				pPaper	= PDL_GetPaper(lpCap);
				if( pPaper != NULL ) {
					for( ; pPaper->szPaperName[0]; pPaper++ ){
						if( pPaper->usID == (USHORT)sPapFormSID ){
							switch (pPaper->FinishingFlag.tJobOffset){
							case ID_STATE3_DISABLE:
								/* 今後Oficioの判断を入れる必要がある */
								bOffSet = FALSE;
								break;
							default:
								break;
							}
							break;
						}
					}
				}
				
//#if 1	/* 091020 KIS@CIS [Greater] Offset can not be set with A5/Stmt when Finisher is set to ShiftTray */
				bQueryRet = PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_ONLYSHIFTTRAY_SHIFT_SUPPORT);
				if(bQueryRet == TRUE){	/* シフトトレイのみでシフトを許可する用紙を持つデバイス */
					bIsEqual  = PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher);
					bIsEqual2 = PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_1, lpPDLDevOption->dwFinisher);
					
					if ((bIsEqual == TRUE) || (bIsEqual2 == TRUE)) {
						switch (sPapFormSID ){
						case DMPAPER_A5:
						case DMPAPER_STATEMENT:
							bOffSet = FALSE;
							break;
						default:
							break;
						}
					}
				}
//#endif	/* 091020 KIS@CIS [Greater] Offset can not be set with A5/Stmt when Finisher is set to ShiftTray */
			}
			
		}
		/* 050920 KTA@IST add end for WinV1.45JP00028 */
	}else{
		/* ドライバデフォルト以外のお気に入り */
		if( ((dwDrvDefault & ID_DOCPROP_PRESETDD) == 0 ) &&
			((dwDrvDefault & ID_DOCPROP_PRESETCUSTOM) == 0 ) ){		/* 061124 AYA@IST V2.00_C_00004 */
			if((lpCDM->pcCdevmode.pdlFinishing.fshFlags & ID_PDL_OFFSET_ON) != 0){
				bOffSet = FALSE;
			}
		}
	}
	

	if (dwfshFlags & ID_GROUP_ON){
		bGroup = L4_UTL_bIsGroupPossible(m_lpuift, lpdmProfile);	/* 20060217 JZH@CSSSI [v1100][LIPS_v11.00-0052] */
	}
	

	if(PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_SMALLSIZE_ZFOLD_SUPPORT) == TRUE){
		/* ステイプルまたはパンチ設定で、かつZ折りが設定されている場合、用紙混載指定以外は、コンフリクトメッセージ表示。 */
		if ((sPrnID == IDM_iPR1110) ||
			(sPrnID == IDM_iPRSJ100)){
			if ((PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher)) ||
				(PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_1, lpPDLDevOption->dwFinisher))){
				if ((((dwfshFlags & ID_STAPLE_ON) != 0) || ((dwfshFlags & ID_PUNCH_ON) != 0)) && ((dwfshFlags & ID_ZFOLD_ON) != 0)){
					if ((lpCDM->pcCdevmode.pdlFinishing.fshMixedPaperFlags & ID_MIXEDPAPER_ON) == 0){
						switch(sPapFormSID){
						case DMPAPER_A4:
						case DMPAPER_LETTER:
						case DMPAPER_LEGAL:
							if((dwfshFlags & ID_STAPLE_ON) != 0){
								bStaple = FALSE;
							}
							if((dwfshFlags & ID_PUNCH_ON) != 0){
								bPunch = FALSE;
							}
							bZFold = FALSE;
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
	
	/* Staple Merge */
	switch (PDL_GetStaple(lpCap)) {
	case ID_STATE3_DISABLE:
		if (dwfshFlags & ID_STAPLE_ON){
			bStaple = FALSE;
		}
		break;
	
	default:
		/*	Staple Possible Device */
		if(!(dwAbility & ENABLE_STAPLE)){
			if (dwfshFlags & ID_STAPLE_ON){
				bStaple = FALSE;
			}
		}
		else{
			pPaper	= PDL_GetPaper(lpCap);
			if( pPaper != NULL ) {
				for( ; pPaper->szPaperName[0]; pPaper++ ){
					if( pPaper->usID == (USHORT)sPapFormSID ){
						switch (pPaper->FinishingFlag.tStaple){
						case ID_STATE3_DISABLE:
							if (dwfshFlags & ID_STAPLE_ON){
								/* 120220 CHL@SSS [v20.80][PreAlpha][Modify][AddNewDevice_Bedford] Start */
								/* Bedford YG3: Custom paper supports staple */
								/* 121008 MFG@SSS [v20.90][PreAlpha][Modify][Pushcha_YG3] Start */
								// 								if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_FINYG3_SUPPORT) ){
								// 									if( (sPapFormSID == DMPAPER_USER) &&
								// 										((PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_2, lpPDLDevOption->dwFinisher) || PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_2, lpPDLDevOption->dwFinisher))) ){
								// 									}
								// 									else{
								// 										bStaple = FALSE;
								// 									}
								// 								}
								if((sPapFormSID == DMPAPER_USER) && PDLCMN_bFinisherYNSeleted((short)m_lpuift->ULIB_GetPrinterID(m_lpuift), lpPDLDevOption)){}	/* 150318 CHL@SSS [v21.50][Alpha][Add][Drv_V21_50_a_FamiCom-0008] */
								else if((lpPDLDevOption->dwDevFuncFlags2 & ID_DEVFUNC2_FINYG3_SUPPORT) == ID_DEVFUNC2_FINYG3_SUPPORT){
									if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_FINYG3_SUPPORT) ){
										if( (sPapFormSID == DMPAPER_USER) &&
											((PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_2, lpPDLDevOption->dwFinisher) || PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_2, lpPDLDevOption->dwFinisher))) ){
										}
										else{
											bStaple = FALSE;
										}
									}else if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_FINYG3_3RDFIN_SUPPORT) ){
										if( (sPapFormSID == DMPAPER_USER) &&
											((PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_3, lpPDLDevOption->dwFinisher) || PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_3, lpPDLDevOption->dwFinisher))) ){
										}
										else{
											bStaple = FALSE;
										}
									}
									else{
										/* 140702 ZMJ@SSS [v21.35][PreAlpha][Delete][CommonUI_v21.35-0026] Start */
										/* 140702 ZMJ@SSS [v21.35][PreAlpha][Add][CommonUI_v21.35-0017] Start */
										//bOficioSetting = PCUTLS_bCheckOficioSetting(m_lpuift, lpdmProfile, cdmSiz);
										//if(bOficioSetting == FALSE){
										/* 140702 ZMJ@SSS [v21.35][PreAlpha][Add][CommonUI_v21.35-0017] End */
										/* 140702 ZMJ@SSS [v21.35][PreAlpha][Delete][CommonUI_v21.35-0026] End */
										bStaple = FALSE;
										//}/* 140702 ZMJ@SSS [v21.35][PreAlpha][Delete][CommonUI_v21.35-0026] */
									}
								}
								/* 121008 MFG@SSS [v20.90][PreAlpha][Modify][Pushcha_YG3] End */
								/* 120220 CHL@SSS [v20.80][PreAlpha][Modify][AddNewDevice_Bedford] End */
							}
						/* 130124 GPG@SSS [v21.00][PreAlpha][Add][Bath2] Start */
							break;
						default:
							if (dwfshFlags & ID_STAPLE_ON){
								if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_FINYN_SUPPORT) ){
									if( (sPapFormSID == DMPAPER_USER) &&
										(!(PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher) || PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_1, lpPDLDevOption->dwFinisher))) ){   /* 130402 JJJ@SSS [v21.05][Alpha][Modify][Drv_V21_05_a_FamiCom-0004] */
/* 130402 JJJ@SSS [v21.05][Alpha][Delete][Drv_V21_05_a_FamiCom-0004] */
//									}	
// 									else{
/* 130402 JJJ@SSS [v21.05][Alpha][Delete][Drv_V21_05_a_FamiCom-0004] */
										bStaple = FALSE;
									}
								}
							}
							break;
						/* 130124 GPG@SSS [v21.00][PreAlpha][Add][Bath2] End */
						}
						break;
					}
				}
			}
		}
		
		break;
	}
	
	
	if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_STAPLEGROUP_SUPPORT) == FALSE){
		if (dwfshFlags & ID_STAPLE_ON){
			if (dwfshFlags & ID_GROUP_ON){
				bStaple	=	FALSE;		/* 050411 AYA@IST TRUE->FALSE changed */
			}
		}
	}else {
	/* 071108 WAT@IST ADD Hold Staple Group Conflict */
	/* Store or Hold選択時Staple GroupはSortに丸める */
		if((sDocDestDefault & ID_DST_HOLDQUEUE) || (sDocDestDefault & ID_DST_STORE)){
			if ((dwfshFlags & ID_STAPLE_ON) && (dwfshFlags & ID_GROUP_ON)){
				bStaple	=	FALSE;
			}
		}
	}
	/* 140612 ZMJ@SSS [v21.35][PreAlpha][Add][ECOStaple] Start */
	if((PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_ECOSTAPLE_SUPPORT)) == FALSE){
		if( lpPDLFinishing->fshFlags2 & ID_FSH_ECOSTAPLE_ON){
			bECOStaple = FALSE;
		}
	}
	else{
		/* 150117 ZMJ@SSS [v21.50][PreAlpha][Add][CommonUI_v21.50-0014][AddNewDevice_Ewloe] Start */
		if( lpPDLFinishing->fshFlags2 & ID_FSH_ECOSTAPLE_ON){
			switch (m_lpuift->ULIB_GetPrinterID(m_lpuift))
			{
			case IDM_iRADVC3320:
			case IDM_iRADVC3330:
			case IDM_iRADVC3320L:
		/* 150117 ZMJ@SSS [v21.50][PreAlpha][Add][CommonUI_v21.50-0014][AddNewDevice_Ewloe] End */
				bIsEqual = PDLCMN_bIsEqualFinisherID(ID_FSH_INNERFINISHER, lpPDLDevOption->dwFinisher);
				if ((bIsEqual == FALSE) ||
					((dwfshFlags & ID_STAPLE_ON) != ID_STAPLE_ON)	||
					(lpPDLFinishing->fshMixedPaperFlags & ID_MIXEDPAPER_ON)){
						if( lpPDLFinishing->fshFlags2 & ID_FSH_ECOSTAPLE_ON){
							bECOStaple = FALSE;
						}
				}

				if( lpPDLFinishing->fshFlags2 & ID_FSH_ECOSTAPLE_ON){	/* 140702 ZMJ@SSS [v21.35][PreAlpha][Modify][CommonUI_v21.35-0017][ECOStaple] */
					switch(sPapFormSID){
					case DMPAPER_A3:
					case DMPAPER_A4:
					case DMPAPER_B4:
					case DMPAPER_B5:
					case DMPAPER_LETTER:
					case DMPAPER_11X17:
					case DMPAPER_TABLOID:
						break;
					/* 140903 SQW@SSS [v21.35][Beta][Delete][CommonUI_v21.35-0050] Start */
					//case DMPAPER_USER:
					//	bOficioSetting = PCUTLS_bCheckOficioSetting(m_lpuift, lpdmProfile, cdmSiz);
					//	if(bOficioSetting == FALSE){
					//		bECOStaple = FALSE;
					//	}
					//	break;
					/* 140903 SQW@SSS [v21.35][Beta][Delete][CommonUI_v21.35-0050] End */

					default:
						bECOStaple = FALSE;
						break;
					}
					break;

			/* 150117 ZMJ@SSS [v21.50][PreAlpha][Add][CommonUI_v21.50-0014][AddNewDevice_Ewloe] Start */
			case IDM_iRADV8500S:
			case IDM_iRADV8595:
				bIsEqual = PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_3, lpPDLDevOption->dwFinisher);
				bIsEqual2 = PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_3, lpPDLDevOption->dwFinisher);
				if (((bIsEqual == FALSE) && (bIsEqual2 == FALSE)) ||
					((dwfshFlags & ID_STAPLE_ON) != ID_STAPLE_ON)	||
					(lpPDLFinishing->fshMixedPaperFlags & ID_MIXEDPAPER_ON)){
						if( lpPDLFinishing->fshFlags2 & ID_FSH_ECOSTAPLE_ON){
							bECOStaple = FALSE;
						}
				}

				switch(sPapFormSID){
				case DMPAPER_A3:
				case DMPAPER_A4:
				case DMPAPER_LETTER:
				case DMPAPER_11X17:
				case DMPAPER_TABLOID:
					break;

				default:
					bECOStaple = FALSE;
					break;
				}
				break;

			case IDM_iRADV6565:  
			case IDM_iRADV6575:
			case IDM_iRADV6500S:
				bIsEqual = PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher);
				bIsEqual2 = PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_1, lpPDLDevOption->dwFinisher);
				if (((bIsEqual == FALSE) && (bIsEqual2 == FALSE)) ||
					((dwfshFlags & ID_STAPLE_ON) != ID_STAPLE_ON)	||
					(lpPDLFinishing->fshMixedPaperFlags & ID_MIXEDPAPER_ON)){
						if( lpPDLFinishing->fshFlags2 & ID_FSH_ECOSTAPLE_ON){
							bECOStaple = FALSE;
						}
				}

				switch(sPapFormSID){
				case DMPAPER_A3:
				case DMPAPER_A4:
				case DMPAPER_LETTER:
				case DMPAPER_11X17:
				case DMPAPER_TABLOID:
					break;

				default:
					bECOStaple = FALSE;
					break;
				}
				break;

			default:
				break;
				}
			}
		/* 150117 ZMJ@SSS [v21.50][PreAlpha][Add][CommonUI_v21.50-0014][AddNewDevice_Ewloe] End */
		}
	}
	/* 140612 ZMJ@SSS [v21.35][PreAlpha][Add][ECOStaple] End */
	/* 041102 AYA@NCT BDL120-00063 */
	ULIB_ADM_GetLay_BindDirect(m_lpuift, lpdmProfile, &sBindDirect);
	if(sPapFormSID == DMPAPER_B5){
		if (dwfshFlags & ID_PUNCH_ON){
			if( (sBindDirect == ID_BINDDIRECT_SHORTTR) || (sBindDirect == ID_BINDDIRECT_SHORTBL)){
				if (dwfshFlags & ID_STAPLE_ON){
					/* 120220 CHL@SSS [v20.80][PreAlpha][Modify][AddNewDevice_Bedford] Start */
					/* Bedford YG3: B5R could support staple and punch at the same time */
					/* 121008 MFG@SSS [v20.90][PreAlpha][Modify][Pushcha_YG3] Start */
					if((lpPDLDevOption->dwDevFuncFlags2 & ID_DEVFUNC2_FINYG3_SUPPORT) == ID_DEVFUNC2_FINYG3_SUPPORT){
						if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_FINYG3_SUPPORT) ){
							if( !PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_2, lpPDLDevOption->dwFinisher) && !PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_2, lpPDLDevOption->dwFinisher) ){
								bStaple = FALSE;
							}
						}else if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_FINYG3_3RDFIN_SUPPORT) ){
							if( !PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_3, lpPDLDevOption->dwFinisher) && !PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_3, lpPDLDevOption->dwFinisher) ){
								bStaple = FALSE;
							}
						}
					}
					/* 121008 MFG@SSS [v20.90][PreAlpha][Modify][Pushcha_YG3] End */
					/* 130124 GPG@SSS [v21.00][PreAlpha][Add][Bath2] Start */
					else if ( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_FINYN_SUPPORT) ){
						/* 150117 JJJ@SSS [v21.50][Alpha][Modify][CommonUI_v21.50-0009] Start */
						if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_3RDFINXV2_SUPPORT) && 
							PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_2RDFINYN_SUPPORT)){
							if( !PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_3, lpPDLDevOption->dwFinisher) && !PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_3, lpPDLDevOption->dwFinisher)&&
								!PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_2, lpPDLDevOption->dwFinisher) && !PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_2, lpPDLDevOption->dwFinisher)){
								bStaple = FALSE;
							}
						}else{
							if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_FINXV2_SUPPORT) ){
								if( !PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher) && !PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_1, lpPDLDevOption->dwFinisher) ){
									bStaple = FALSE;
								}
							}else{ 
								if( !PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher) && !PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_1, lpPDLDevOption->dwFinisher) )
									bStaple = FALSE;
							}
						}
						/* 150117 JJJ@SSS [v21.50][Alpha][Modify][CommonUI_v21.50-0009] End */
					}
					/* 130124 GPG@SSS [v21.00][PreAlpha][Add][Bath2] End */
					else{
						bStaple = FALSE;
					}
					/* 120220 CHL@SSS [v20.80][PreAlpha][Modify][AddNewDevice_Bedford] End */
				}
			}
		}
	}
	
	/* 100707 WAT@IST V20.35_00024 */
	if(PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_EXECUTIVE_PUNCHSTAPLE_UNSUPPORT)){
		if(sPapFormSID == DMPAPER_EXECUTIVE){
			if (dwfshFlags & ID_PUNCH_ON){
				if( (sBindDirect == ID_BINDDIRECT_SHORTTR) || (sBindDirect == ID_BINDDIRECT_SHORTBL)){
					if (dwfshFlags & ID_STAPLE_ON){
						bStaple = FALSE;
					}
				}
			}
		}
	}
	/* 100707 WAT@IST V20.35_00024 */

	if (dwfshFlags & ID_JOBOFFSET_ON){
		bJobOffset = L4_UTL_bIsJobOffsetPossible(m_lpuift);
	}
	
	if (dwfshFlags & ID_FACEUP_ON){
		bFaceUp = L4_UTL_bIsFaceUpPossible (m_lpuift);
	}
	
	if (dwBkltFlags & ID_CENTERFOLD_ON){
		if(!(dwAbility & ENABLE_CENTERFOLD)) {
			bFold = FALSE;
		}
		else {	
			bFold = L4_UTL_bIsCenterFoldPossible(m_lpuift, lpdmProfile);
		}
	}
	
	/* 20080614 WQN@CSSSI [v1240][VFOLD] Start */
	
	if ( (sCategory == ID_CATEGORY_BOOKLET) && ( (dwBkltFlags & ID_BOOKLET_DIVIDESET) ||
		 (dwBkltFlags & ID_BOOKLET_MARGIN_ON) ) ) {
		if (sOptPriority == ID_PRIORITY_PRINTER) bletFlags = FALSE;
	}
	
    if(sCategory == ID_CATEGORY_BOOKLET){		/* 20060605 WQN@CSSSI [v1110][v1110][LIPS_v11.10-0008] */
		if (lpPDLFinishing) {
			if (lpPDLFinishing->fshFlags & ID_FSH_BOOKLET_IN_DEVICE ) {
				/* 20060713 YUW@CSSSI [v1125][DEVBOOKLETOPT] */
				if (sdocDestination == ID_DST_PAGECOMPOSER) {
		  /*110408 JJJ@SSS [v20.60][PreAlpha][Modify][OutputMethodFirst][ホールド＆デバイスのセキュリティ印刷機能] Start*/
					//if (dwOptFlags & ID_PAGECOMPOSER_LOCK) {
						/* [編集＋プレビュー]ロックON時は[ホスト側で処理]に丸める */
						bDeviceBooklet = FALSE;
						lpPDLFinishing->fshFlags &= ~ID_FSH_BOOKLET_IN_DEVICE;
					//} else {
						/* 編集＋プレビュー]ロックOFF時は[デバイス側で処理]優先 */
						/* 20060511 YUW@CSSSI [v1110][v11.02_00010] Start */
						//bDestinationMsg = FALSE;
						/* sdocDestination = ID_DST_PRINTER;
						ULIB_ADM_SetDoc_Destination(m_lpuift, lpdmProfile, &sdocDestination); */
						/* 20060511 YUW@CSSSI [v1110][v11.02_00010] End */
					//}
		 /*110408 JJJ@SSS [v20.60][PreAlpha][Modify][OutputMethodFirst][ホールド＆デバイスのセキュリティ印刷機能] End*/
				}
			}
	//@@@ [SUPPORT] Add End
	/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] End */
				/* 20070627 ZYS@CSSSI [v1215][APPLICATION_BOOKLET] Start */
			else if (lpPDLFinishing->fshFlags2 & ID_FSH_BOOKLET_IN_APPLICATION){
				/*	bAppBooklet = FALSE; */ /* No need to display any message */
				if (sdocDestination == ID_DST_PAGECOMPOSER) {
		/*110408 JJJ@SSS [v20.60][PreAlpha][Modify][OutputMethodFirst][ホールド＆デバイスのセキュリティ印刷機能] Start*/
					//if (dwOptFlags & ID_PAGECOMPOSER_LOCK) {
						/* [編集＋プレビュー]ロックON時は[ホスト側で処理]に丸める */
						bAppBooklet = FALSE;
						lpPDLFinishing->fshFlags2 &= ~ID_FSH_BOOKLET_IN_APPLICATION;
					//} else {
						/* 編集＋プレビュー]ロックOFF時は[アプリケーション側で処理]優先 */
						//bDestinationMsg = FALSE;
					//}
		/*110408 JJJ@SSS [v20.60][PreAlpha][Modify][OutputMethodFirst][ホールド＆デバイスのセキュリティ印刷機能] End*/
				}
			}
				/* 20070627 ZYS@CSSSI [v1215][APPLICATION_BOOKLET] End */
			else {
				/* 20060713 YUW@CSSSI [v1125][DEVBOOKLETOPT] */
					if (sOptPriority == ID_PRIORITY_PRINTER) {
	/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] Start */
	//@@@ [SUPPORT] Add Start : KTO@CSL 060220 CCDコアGR障害票 E307GCL-0011
						bDeviceBooklet = FALSE;
	//@@@ [SUPPORT] Add End
	/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] End */
						lpPDLFinishing->fshFlags |= ID_FSH_BOOKLET_IN_DEVICE;
					}
			}

			/* 20060717 ZYS@CSSSI [v1125][IB535] Start */
			if (lpPDLFinishing->fshFlags & ID_FSH_BOOKLET_USE_CREEP) {	
			}
			/* 20060717 ZYS@CSSSI [v1125][IB535] End */
		}
	}

	/* 20070205 LYN@CSSSI [v1205][SmallSizeLowCurl] Start */
	if (lpPDLFinishing->fshSmallSizeCurlAdjustment != ID_SMALLSIZELOWCURL_PRNDEF){
			lpPDLFinishing->fshSmallSizeCurlAdjustment = ID_SMALLSIZELOWCURL_PRNDEF;
	}
	/* 20070205 LYN@CSSSI [v1205][SmallSizeLowCurl] End */
	
/* 20080706 SYC@CSSSI [v1240][MultiFunctionFold] Start */
	if(	(lpPDLDevOption->dwDevFuncFlags & ID_DEVFUNC_FOLD_DETAIL) == ID_DEVFUNC_FOLD_DETAIL ){
		/* [排紙面指定] */
		/* 080827 SAI@IST DEVMODE共通化対応*/

		if (lpPDLFinishing->fshFaceOfOutputCopies != ID_OUTPUTFACE_PRINTERDEFAULT) {
			if ( (lpPDLFinishing->fshFlags & ID_FSH_CFOLD_ON) ||
				 (lpPDLFinishing->fshFlags2 & (ID_FSH_ACCORDION_ON|ID_FSH_FOUR_ON|ID_FSH_MIDDLE_ON)) ) {	/* 090330 WAT@IST LIPS4_i_QA V20.00-00042 */
				lpPDLFinishing->fshFaceOfOutputCopies = ID_OUTPUTFACE_PRINTERDEFAULT;
				bOutputFace = FALSE;
			}
		}
		
		/* [印刷の向きを180度回転する] */
		if ((lpPDLFinishing->fshFlags & ID_FSH_ROTATEPRINT180_ON) != 0){
			if ( (lpPDLFinishing->fshFlags & ID_FSH_CFOLD_ON) ||
				 (lpPDLFinishing->fshFlags2 & (ID_FSH_ACCORDION_ON|ID_FSH_FOUR_ON|ID_FSH_MIDDLE_ON)) ) {	/* 090330 WAT@IST LIPS4_i_QA V20.00-00042 */
				lpCDM->pdlLipsOther.LIPSfinishing.devImgRotate = ID_IMGROTATE_NONE;
				bImgRotate = FALSE;
			}
		}
	} else {
		/* 20070413 ZYS@CSSSI [v1210][FINISHINGMODES_CFOLD] Start */
		/* [排紙面指定] */
		/* 080827 SAI@IST DEVMODE共通化対応*/
		if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_MISCFINISHINGMODES_SUPPORT) == TRUE){
			if (lpPDLFinishing->fshFaceOfOutputCopies != ID_OUTPUTFACE_PRINTERDEFAULT) {
				if (lpPDLFinishing->fshFlags & ID_FSH_CFOLD_ON) {
					lpPDLFinishing->fshFaceOfOutputCopies = ID_OUTPUTFACE_PRINTERDEFAULT;
					bOutputFace = FALSE;
				}
			}
		}

		/* [印刷の向きを180度回転する] */
		if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_MISCFINISHINGMODES_SUPPORT) == TRUE){
			if (lpCDM->pdlLipsOther.LIPSfinishing.devImgRotate != ID_IMGROTATE_NONE) {
				if (lpPDLFinishing->fshFlags & ID_FSH_CFOLD_ON) {
					lpCDM->pdlLipsOther.LIPSfinishing.devImgRotate = ID_IMGROTATE_NONE;
					bImgRotate = FALSE;
				}
			}
		}
		/* 20070413 ZYS@CSSSI [v1210][FINISHINGMODES_CFOLD] End */
	}
/* 20080706 SYC@CSSSI [v1240][MultiFunctionFold] End */

	if((lpPDLFinishing->fshFlags & ID_FSH_SECUREWATERMARK_ON) != 0){
		bQueryRet = PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_SECUREWATERMARK_SUPPORT);
		if(bQueryRet == TRUE){
			/* 061127 AYA@IST for E307GCL/E313C/D Version UP */
			/* 地紋拡張キット・A1 未装着 */
			if ((lpPDLDevOption->dwUnitFlags & ID_UNIT_SECURE_WATERMARK) == 0) {
				bSecureWatermark	= FALSE;
			}
			/* 製本印刷の処理方法がデバイス側で処理 */
			if (sCategory == ID_CATEGORY_BOOKLET){		/* 070806 AYA@IST BDL215-00039 */
				if ((lpPDLFinishing->fshFlags & ID_FSH_BOOKLET_IN_DEVICE) != 0){
					bSecureWatermark = FALSE;
				}
			}											/* 070806 AYA@IST BDL215-00039 */
			/* くるみ製本 */	/* 070725 AYA@IST E230FGH_D_BDL-0009 */
			if(((lpPDLFinishing->fshFlags & ID_FSH_PERFECTBIND_ON) != 0)){
				bSecureWatermark = FALSE;
			}
/*110406 JJJ@SSS [v20.60][PreAlpha][Add][OutputMethodFirst][ホールド＆デバイスのセキュリティ印刷機能] Start*/
			if (sdocDestination & ID_DST_HOLDQUEUE){
				if(PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_DEVICEPCAM_SUPPORT)){
					if (PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_HOLDPCAM_CONFLICT)){  /*110406 JJJ@SSS [v20.60][PreAlpha][Add][HoldJimonConflict][ホールド＆デバイスのセキュリティ印刷機能] */
						bSecureWatermark = FALSE;
					}else if(bSecureWatermark){/* 110509 LYN@SSS [Modify][CommonUI_v20.60-0020] 地紋使用可能の場合のみ */
						CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_253); 
					}
				}           
			}
/*110406 JJJ@SSS [v20.60][PreAlpha][Add][OutputMethodFirst][ホールド＆デバイスのセキュリティ印刷機能] End*/
		}
		else{
			bSecureWatermark	= FALSE;
		}
	}

	/* 20070206 LYN@CSSSI [v1205][TRANSCRIPTION_MODE3] Start */
	if (lpPDLFinishing->fshSpecialService != ID_TRANSCRIPTION_PRNDEF){
		if( (PDLCMN_FunctionQuery(sPrnID, QUERY_FINI_SPECIALSERVICE_MODE_B_SUPPORT) == FALSE) &&
			(PDLCMN_FunctionQuery(sPrnID, QUERY_FINI_SPECIALSERVICE_MODE3_4_SUPPORT) == FALSE) ){
			lpPDLFinishing->fshSpecialService = ID_TRANSCRIPTION_PRNDEF;
		}else if (lpPDLFinishing->fshSpecialService == ID_TRANSCRIPTION_MODE4){
			if( PDLCMN_FunctionQuery(sPrnID, QUERY_FINI_SPECIALSERVICE_MODE3_4_SUPPORT) == FALSE ){
				lpPDLFinishing->fshSpecialService = ID_TRANSCRIPTION_PRNDEF;
				bTranscription = FALSE;
			}
		}else if (lpPDLFinishing->fshSpecialService == ID_TRANSCRIPTION_MODE3){
			if( (PDLCMN_FunctionQuery(sPrnID, QUERY_FINI_SPECIALSERVICE_MODE_B_SUPPORT) == FALSE) &&
				(PDLCMN_FunctionQuery(sPrnID, QUERY_FINI_SPECIALSERVICE_MODE3_4_SUPPORT) == FALSE) ){
				lpPDLFinishing->fshSpecialService = ID_TRANSCRIPTION_PRNDEF;
				bTranscription = FALSE;
			}
		}
	}
	/* 20070206 LYN@CSSSI [v1205][TRANSCRIPTION_MODE3] End */

	
	/*_ 部数印字を行う */
	if (lpPDLFinishing->fshFlags & ID_ADDNUMBERPRINT_ON) {
		lpPDLFinishing->fshFlags &= ~ID_ADDNUMBERPRINT_ON;
	}
	
	if ( (sletPages < 1) || (sletPages > (short)PDL_GetCapInfo (lpCap, CAPINFOID_GETFOLDMAXNUM, NULL, ID_FOLD_SADDLE)) ) {
		ULIB_ADM_GetBok_letPages (m_lpuift, lpDefdm, &sletPages);
		ULIB_ADM_SetBok_letPages (m_lpuift, lpdmProfile, &sletPages);
		if (dwBkltFlags & ID_BOOKLET_DIVIDESET) bletPages = FALSE;
	}
	
	if (sOptPriority == ID_PRIORITY_PRINTER) {
		switch (sPaperSelection) {
		case ID_PS_ALLPAGES:
		case ID_PS_TRANSPARENCY:
			break;
		default:
			bPaperSelection = FALSE;
		}
	}
		

	if (dwfshFlags & ID_FIXEDBIN_ON){
		dwRsnFixBin = 0x0L;
		bFixBin = L4_UTL_bIsFixedBinPossible (m_lpuift, (LPDWORD)&dwRsnFixBin);
	}
	
		bIsEqual = PDLCMN_bIsEqualFinisherID(ID_FSH_NONE, lpPDLDevOption->dwFinisher);
		if(bIsEqual == TRUE){
			/* not Finisher for E307GCL */
			bQueryRet = PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_PAPEROUTPUT_USE_NOTOPTION);
			if(bQueryRet == TRUE){
				/* 060202 KTA@IST modify start for v1.60 E307 paperoutput conflict */
				if((lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
				(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_INNERTRAY_A)){
					;
				}
				else if(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_B){
					/* 用紙タイプコンフリクト */
					if(lpPDLPaperSource->psFlags & PAPERSRC_MEDIA){
						bImPaperType = PDLCMN_bImpossible_PaperType(m_lpuift->ULIB_GetPrinterID(m_lpuift), sPaperSelection, lpPDLPaperSource, ID_IMPOSSIBLE_PAPEROUTPUT);
						if(bImPaperType == TRUE){
							bFixBin = FALSE;
							PUIDBGPrint(("bFixBin PaperType Conflict 1\n"));
						}
					}
					/* 給紙方法コンフリクト */
					else if (sPaperSelection == ID_PS_TRANSPARENCY){
						bFixBin = FALSE;
					}
				}
				else if(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_C){
					/* 用紙タイプコンフリクト */
					if(lpPDLPaperSource->psFlags & PAPERSRC_MEDIA){
						bImPaperType = PDLCMN_bImpossible_PaperType(m_lpuift->ULIB_GetPrinterID(m_lpuift), sPaperSelection, lpPDLPaperSource, ID_IMPOSSIBLE_PAPEROUTPUT1);
						if(bImPaperType == TRUE){
							bFixBin = FALSE;
							PUIDBGPrint(("bFixBin PaperType Conflict 2\n"));
						}
					}
					/* 給紙方法コンフリクト */
					else if (sPaperSelection == ID_PS_TRANSPARENCY){
						bFixBin = FALSE;
					}
				}
				
				/* 060202 KTA@IST modify end for v1.60 E307 paperoutput conflict */
				else{
					bFixBin = FALSE;
				}
			}
			else{
				if(lpPDLFinishing->fshPaperOutput != ID_PDL_PAPEROUTPUT_AUTO){
					bFixBin = FALSE;
				}
			}
		}
		
		lpShareDO		= CMUSR_lpGetCurDevoption(m_lpuift);
		hPrinter = m_lpuift->ULIB_GetPrinterHandle(m_lpuift);
		if ( hPrinter == NULL ) {		/* 100715 JUN@CIS for prefast */
			return FALSE;
		}
		switch(sPrnID){
		case IDM_iR2320:
		case IDM_iR2420:
			/* Inner2WayTray */
			bIsEqual = PDLCMN_bIsEqualFinisherID(ID_FSH_INNER2WAYTRAY, lpPDLDevOption->dwFinisher);
			if(bIsEqual == TRUE){
				if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
					(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_A) ||
					(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_B) ){
					if( lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_B){
						/* 給紙方法コンフリクト */
						if (sPaperSelection == ID_PS_TRANSPARENCY){
							bFixBin = FALSE;
						}
						/* 用紙タイプコンフリクト */
						else if(lpPDLPaperSource->psFlags & PAPERSRC_MEDIA){
							bImPaperType = PDLCMN_bImpossible_PaperType2(sPrnID, sPaperSelection, ID_IMPOSSIBLE_PAPEROUTPUT, lpShareDO, (LPSHAREDEV)lpdmProfile, hPrinter, PDL_GetPDLID(lpCap), UTL_GetPrinterName(m_lpuift), ((LPSHAREDEV)CMUSR_lpGetCurDevmode(m_lpuift))->cdmModelName);/* 141112 ZXP@SSS [v21.45][CommonUI_v21.40-0023][Modify] Add DriverName *//* 20110818 ZXP@SSS [v20.65][GetPrinterSpeedUp] */
							if(bImPaperType == TRUE){
								bFixBin = FALSE;
							}
						}
					}
				}
			}
			
			/* Finisher-U1 & OptionTray */
			bIsEqual = PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher);
			if(bIsEqual == TRUE){
				bIsEqual = PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHERTRAY, lpPDLDevOption->dwFinisher);
				if(bIsEqual == TRUE){	/* OptionTray */
					if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
						(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_A) ||
						(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_B)){
						;
					}
					else{
						bFixBin = FALSE;
					}
				}
				/* Finisher-U1のみ設定している時はAuto固定であるため、Auto以外だったらアラート表示を行う。 */
				else if( lpPDLFinishing->fshPaperOutput != ID_PDL_PAPEROUTPUT_AUTO ){
					bFixBin = FALSE;
				}
			}
			break;

		case IDM_iRADVC9065:
		case IDM_iRADVC9060:
		case IDM_iRADVC7055:
		case IDM_iRADVC7000:
		case IDM_iRADVC9000:
		case IDM_iRADV8105:
		case IDM_iRADV8095:
		case IDM_iRADV8000:
		case IDM_iRADV6075:
		case IDM_iRADV6065:
		case IDM_iRADV6000:
		case IDM_iRADV600A:	/* 090911 ISA@IST for add PCL EFI */
		/* 120220 CHL@SSS [v20.80][PreAlpha][Add][AddNewDevice_Bedford] Start */
		case IDM_iRADV8205:
		case IDM_iRADV8295:
		/* 120220 CHL@SSS [v20.80][PreAlpha][Add][AddNewDevice_Bedford] End */
		/* 130903 JJJ@SSS [v21.45][PreAlpha][Add][AddNewDevice_Ewloe] Start */
		case IDM_iRADV8505:
		case IDM_iRADV8595:
		/* 130903 JJJ@SSS [v21.45][PreAlpha][Add][AddNewDevice_Ewloe] End */
		case IDM_iRADVC9280:	/* 120208 GPG@SSS [v20.80][PreAlpha][Add][AddNewDevice_Chesterfield] */
		case IDM_iRADVC7270:	/* 120208 GPG@SSS [v20.80][PreAlpha][Add][AddNewDevice_Chesterfield] */
			bIsEqual	= PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher);
			if(bIsEqual == FALSE){
				bIsEqual	= PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_1, lpPDLDevOption->dwFinisher);
			}
			if(bIsEqual == TRUE){
				if((lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
				   (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_A) ||
				   (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_C)){
					;
				} 
				else if (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_MIDDLETRAY_B){
					if (lpPDLFinishing->fshFlags & ID_PDL_OFFSET_ON){
						bFixBin = FALSE;
					}
				}
				else{
					bFixBin = FALSE;
				}
			}
			break;

			/* 130122 GPG@SSS [v21.00][PreAlpha][Add][Bath2] Start */
		case IDM_iPRC800:
			bIsEqual	= PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher);
			bIsEqual2	= PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_2, lpPDLDevOption->dwFinisher);	/* 140319 ZMJ@SSS [v21.30][PreAlpha][Add][AddNewDevice_Honiara2] */
			bIsEqual3	= PDLCMN_bIsEqualFinisherID(ID_FSH_STACKER_1, lpPDLDevOption->dwFinisher);/* 140321 ZMJ@SSS [v21.30][PreAlpha][Add][HCS3] */
			if(bIsEqual == FALSE){
				bIsEqual	= PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_1, lpPDLDevOption->dwFinisher);
			}
			if(bIsEqual == TRUE){
				/* 140321 ZMJ@SSS [v21.30][PreAlpha][Add][HCS3] Start */
				if(bIsEqual3 == TRUE){
					switch (lpPDLFinishing->fshPaperOutput){
					case ID_PDL_PAPEROUTPUT_AUTO:
					case ID_PDL_PAPEROUTPUT_LOWERLEFT_TRAY:	/* 140603 DWY@SSS [v21.30][Alpha][Modify][Drv_V21_30_a_FamiCom-0042][HCS3] */
						break;
					case ID_PDL_PAPEROUTPUT_UPPERLEFT_TRAY:	/* 140603 DWY@SSS [v21.30][Alpha][Modify][Drv_V21_30_a_FamiCom-0042][HCS3] */
						if(dwfshFlags & ID_STAPLE_ON){
							bFixBin = FALSE;
						}
						break;
					case ID_PDL_PAPEROUTPUT_STACKER_OUTTRAY:
					case ID_PDL_PAPEROUTPUT_STACKER:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
						}

						if ( lpPDLFinishing->fshSpeciesOfHolePunch != ID_FINI_SPECIES_HOLEPUNCH_MULTIHOLES ){
							if ( dwfshFlags & ID_PUNCH_ON ) {
								bFixBin = FALSE;
							}
						}
						break;
					default:
						bFixBin = FALSE;
						break;
					}
				} else{
				/* 140321 ZMJ@SSS [v21.30][PreAlpha][Add][HCS3] End */
					if((lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
						(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERLEFT_TRAY)){
							;
					} else if (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERLEFT_TRAY) {
						if ( dwfshFlags & ID_STAPLE_ON ){
							bFixBin = FALSE;
						}
					} else {
						bFixBin = FALSE;
					}
				}
			}
			/* 140319 ZMJ@SSS [v21.30][PreAlpha][Add][AddNewDevice_Honiara2] Start */	
			if(bIsEqual2 == FALSE){
				bIsEqual2	= PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_2, lpPDLDevOption->dwFinisher);
			}
			if(bIsEqual2 == TRUE){
				if(bIsEqual3 == TRUE){
					switch (lpPDLFinishing->fshPaperOutput){
					case ID_PDL_PAPEROUTPUT_AUTO:
					case ID_PDL_PAPEROUTPUT_MIDDLETRAY_B:
					case ID_PDL_PAPEROUTPUT_LOWERTRAY_C:
						break;
						/* Fin_X4/X5: Upper Tray does not support Staple */
					case ID_PDL_PAPEROUTPUT_UPPERTRAY_A:
						if (lpPDLFinishing->fshFlags & ID_PDL_OFFSET_ON){
							bFixBin = FALSE;
						}
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
						}
						break;
					case ID_PDL_PAPEROUTPUT_STACKER_OUTTRAY:
						if (lpPDLFinishing->fshFlags & ID_PDL_OFFSET_ON){
							bFixBin = FALSE;
						}

						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
						}

						if ( lpPDLFinishing->fshSpeciesOfHolePunch != ID_FINI_SPECIES_HOLEPUNCH_MULTIHOLES ){
							if ( dwfshFlags & ID_PUNCH_ON ) {
								bFixBin = FALSE;
							}
						}
					case ID_PDL_PAPEROUTPUT_STACKER:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
						}

						if ( lpPDLFinishing->fshSpeciesOfHolePunch != ID_FINI_SPECIES_HOLEPUNCH_MULTIHOLES ){
							if ( dwfshFlags & ID_PUNCH_ON ) {
								bFixBin = FALSE;
							}
						}
						break;
					default:
						bFixBin = FALSE;
						break;
					}
				} else{
					if((lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
						(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_MIDDLETRAY_B) ||
						(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_C)){
							;
					} 
					else if (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_A){
						if (lpPDLFinishing->fshFlags & ID_PDL_OFFSET_ON){
							bFixBin = FALSE;
						}
					}
				}
			}

			if ((!bIsEqual) && (!bIsEqual2)) {
				if(bIsEqual3 == TRUE){
					switch (lpPDLFinishing->fshPaperOutput){
					case ID_PDL_PAPEROUTPUT_AUTO:
					case ID_PDL_PAPEROUTPUT_STACKER_OUTTRAY:
					case ID_PDL_PAPEROUTPUT_STACKER:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
						}

						if ( lpPDLFinishing->fshSpeciesOfHolePunch != ID_FINI_SPECIES_HOLEPUNCH_MULTIHOLES ){
							if ( dwfshFlags & ID_PUNCH_ON ) {
								bFixBin = FALSE;
							}
						}
						break;
					default:
						bFixBin = FALSE;
						break;
					}
				}
			}
			/* 140319 ZMJ@SSS [v21.30][PreAlpha][Add][AddNewDevice_Honiara2] End */	
			break;
			/* 130122 GPG@SSS [v21.00][PreAlpha][Add][Bath2] End */

		/* 120220 CHL@SSS [v20.80][PreAlpha][Add][AddNewDevice_Bedford] Start */
		/* Miskin's Sample Tray is UPPERTRAY */
		case IDM_iRADV6275:
		case IDM_iRADV6265:
/* 150228 JJJ@SSS [v21.50][Alpha][Delete][Drv_V21_50_a_FamiCom-0014] Start */
/* Fin-XV2's Tray [Upper] support shift for Rotherwick. */
// 		case IDM_iRADV6575: /* 141118 JJJ@SSS [v21.45][PreAlpha][Add][AddNewDevice_Ewloe] */
// 		case IDM_iRADV6565: /* 141118 JJJ@SSS [v21.45][PreAlpha][Add][AddNewDevice_Ewloe] */
/* 150228 JJJ@SSS [v21.50][Alpha][Delete][Drv_V21_50_a_FamiCom-0014] End */
			bIsEqual	= PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher);
			if(bIsEqual == FALSE){
				bIsEqual	= PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_1, lpPDLDevOption->dwFinisher);
			}
			if(bIsEqual == TRUE){
				if((lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
				   (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_MIDDLETRAY_B) ||
				   (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_C)){
					;
				} 
				else if (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_A){
					if (lpPDLFinishing->fshFlags & ID_PDL_OFFSET_ON){
						bFixBin = FALSE;
					}
				}
				else{
					bFixBin = FALSE;
				}
			}
			break;
		/* 120220 CHL@SSS [v20.80][PreAlpha][Add][AddNewDevice_Bedford] End */

		case IDM_iRADVC5045:
		case IDM_iRADVC5030:
		case IDM_iRADVC5000:
		/* 100806 MFG@SSS [v20.50][PreAlpha][Add][AddNewDevice_Glasgow&Guildford] Start */
		case IDM_iRADV4045:
		case IDM_iRADV4025:
		/* 100806 MFG@SSS [v20.50][PreAlpha][Add][AddNewDevice_Glasgow&Guildford] End */
		/* 120921 PAH@SSS [v20.90][PreAlpha][Add][AddNewDevice_TunbridgeWells] Start */
		case IDM_iRADV4225:
		case IDM_iRADV4245:
		/* 120921 PAH@SSS [v20.90][PreAlpha][Add][AddNewDevice_TunbridgeWells] End */
		case IDM_LBP8900:/* 131228 DWY@SSS [v21.20][Alpha][Add][AddNewDevice_Chipping] */
		/* 110625 GPG@SSS [v20.65][PreAlpha][Add][AddNewDevice_Grasmere] Start */
		case IDM_iRADVC5235:
		case IDM_iRADVC5250:
		/* 110625 GPG@SSS [v20.65][PreAlpha][Add][AddNewDevice_Grasmere] End */
		case IDM_iRADVC2230:	/* 111122 CHL@SSS [v20.75][PreAlpha][Add][AddNewDevice_Donnington] */	
		case IDM_IRADVC2218:	/* 120706 PAH@SSS [v20.85][Alpha][Add][AddNewDevice_Karachi] */
		case IDM_iRADVC3330:	/* 140529 ZWW@SSS [v21.35][PreAlpha][Add][AddNewDevice_Wien] */
		case IDM_iRADVC350:		/* 130117 SQW@SSS [v21.00][PreAlpha][Add][AddNewDevice_Reykjavik] */
			bIsEqual	= PDLCMN_bIsEqualFinisherID(ID_FSH_INNER2WAYTRAY, lpPDLDevOption->dwFinisher);
			/* 140612 ZMJ@SSS [v21.35][PreAlpha][Modify][AddNewDevice_Wien_2] Start */
			bIsEqual2 = PDLCMN_bIsEqualFinisherID(ID_FSH_COPYTRAY,	lpPDLDevOption->dwFinisher);
			bIsEqual3 = PDLCMN_bIsEqualFinisherID(ID_FSH_INNERFINISHER,	lpPDLDevOption->dwFinisher);
			/* 140818 DWY@SSS [v21.35][Alpha][Delete][Common_REQ000141][Drv_V21_35_a_FamiCom-0008][AddNewDevice_Wien]Start */
			//if( (PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_FINISHERTRAY_UNSUPPORT)) &&
			//	(bIsEqual3) ){
			//		bIsEqual4 = TRUE;
			//}else{
			/* 140818 DWY@SSS [v21.35][Alpha][Delete][Common_REQ000141][Drv_V21_35_a_FamiCom-0008][AddNewDevice_Wien]End */
				bIsEqual4 = PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHERTRAY,	lpPDLDevOption->dwFinisher);
		//		bIsEqual4 = lpPDLDevOption->dwFinisher & ID_FSH_FINISHERTRAY;
			/* 140612 ZMJ@SSS [v21.35][PreAlpha][Modify][AddNewDevice_Wien_2] End */
			//}/* 140818 DWY@SSS [v21.35][Alpha][Delete][Common_REQ000141][Drv_V21_35_a_FamiCom-0008][AddNewDevice_Wien] */
			
			if( bIsEqual ){
				/* Inner 2way Tray + Copy Tray */
				if( bIsEqual2 ){
					if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
						(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_A) ||
						(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_B) ){
						;
						
					} else if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_RIGHTTRAY_C) ){
						if( dwfshFlags & ID_STAPLE_ON || lpPDLFinishing->fshFlags & ID_PDL_OFFSET_ON ){
							bFixBin = FALSE;
						}
						
					} else {
						bFixBin = FALSE;
					}
				}
				/* Inner 2way Tray のみ */
				else {
					if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
						(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_A) ||
						(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_B) ){
						;
					} else {
						bFixBin = FALSE;
					}
				}
			}
			
			if( bIsEqual3 ){
				if( bIsEqual2 ){
					/* Inner Finisher + Copy Tray + Additional Tray */
					if( bIsEqual4 ){
						/* 140923 ZMJ@SSS [v21.35][Alpha][Add][REQ000220][AddNewDevice_Wien_2] Start */
						if(PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_EXTENTEDOUTPUTUNIT_SUPPORT)){
							/* Inner Finisher +  Extended Output Unit */
							if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
								(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_A) ){
									;

							} else if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_RIGHTTRAY_C) ||
								(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_B) ){
								if( dwfshFlags & ID_STAPLE_ON || lpPDLFinishing->fshFlags & ID_PDL_OFFSET_ON ){
									bFixBin = FALSE;
								}

							} else {
								bFixBin = FALSE;
							}
						}
						else{
						/* 140923 ZMJ@SSS [v21.35][Alpha][Add][REQ000220][AddNewDevice_Wien_2] End */
							if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
								(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_A) ||
								(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_B) ){
								;
							
							} else if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_RIGHTTRAY_C) ){
								if( dwfshFlags & ID_STAPLE_ON || lpPDLFinishing->fshFlags & ID_PDL_OFFSET_ON ){
									bFixBin = FALSE;
								}
							
							} else {
								bFixBin = FALSE;
							}
						}
					}
					/* Inner Finisher + Copy Tray */
					else {
						if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
							(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_INNERTRAY_A) ){
							;
							
						} else if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_RIGHTTRAY_B) ){
							if( dwfshFlags & ID_STAPLE_ON || lpPDLFinishing->fshFlags & ID_PDL_OFFSET_ON ){
								bFixBin = FALSE;
							}
							
						} else {
							bFixBin = FALSE;
						}
					}
				}
				/* Inner Finisher + Additional Tray */
				else if( bIsEqual4 ){
					/* 140923 ZMJ@SSS [v21.35][Alpha][Add][REQ000220][AddNewDevice_Wien_2] Start */
					if(PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_EXTENTEDOUTPUTUNIT_SUPPORT)){
						/* Inner Finisher +  Extended Output Unit */
						if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
							(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_A) ){
								;

						} else if( lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_B ){
							if( dwfshFlags & ID_STAPLE_ON || lpPDLFinishing->fshFlags & ID_PDL_OFFSET_ON ){
								bFixBin = FALSE;
							}

						} else {
							bFixBin = FALSE;
						}
					}
					else{
						/* 140923 ZMJ@SSS [v21.35][Alpha][Add][REQ000220][AddNewDevice_Wien_2] End */
						if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
							(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_A) ||
							(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_B) ){
							;
						} else {
							bFixBin = FALSE;
						}
					}
				}
				/* Inner Finisher のみ */
				else {
					if( lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO ){
						;
					} else {
						bFixBin = FALSE;
					}
				}
			}
			
			bIsEqual2	= PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher);
			if(bIsEqual2 == FALSE){
				bIsEqual2	= PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_1, lpPDLDevOption->dwFinisher);
			}
			if(bIsEqual2 == TRUE){
				if((lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
				   (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_A) ||
				   (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_B)){
					;
				}
				else if (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_INNERTRAY_C){
					if( dwfshFlags & ID_STAPLE_ON ) {
						bFixBin = FALSE;
					}
				}
				else{
					bFixBin = FALSE;
				}
			}
			
			/* Copy Tray のみ。フラグを == で見る事で他のものが付いていないことを確認できる。 */
			if( lpPDLDevOption->dwFinisher == ID_FSH_COPYTRAY ){
				if((lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
				   (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_INNERTRAY_A) ||
				   (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_RIGHTTRAY_B)){
					;
				} else {
					bFixBin = FALSE;
				}
			}
			
			break;
			
		case IDM_iPR1110:
		case IDM_iPRSJ100:
			bIsEqual	= PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher);
			bIsEqual2	= PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_1, lpPDLDevOption->dwFinisher);
			bIsEqual3	= PDLCMN_bIsEqualFinisherID(ID_FSH_STACKER_1, lpPDLDevOption->dwFinisher);
			bIsEqual4	= PDLCMN_bIsEqualFinisherID(ID_FSH_STACKER_2, lpPDLDevOption->dwFinisher);
			bIsEqual5	= PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_2, lpPDLDevOption->dwFinisher);
			bIsEqual6	= PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_2, lpPDLDevOption->dwFinisher);
			/* 121210 SQW@SSS [v20.90][Alpha][Add][Drv_v20_90_a_FamiCom-0014] Start */
			bIsEqual7	= PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_3, lpPDLDevOption->dwFinisher);
			bIsEqual8	= PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_3, lpPDLDevOption->dwFinisher);
			/* 121210 SQW@SSS [v20.90][Alpha][Add][Drv_v20_90_a_FamiCom-0014] End */
			if((bIsEqual == TRUE) || ( bIsEqual2 == TRUE )){
				if(bIsEqual3 == TRUE){
					/* Finisher-AB1 or Saddle Finisher-AB2 + High Capacity Stacker-C1 */
					switch (lpPDLFinishing->fshPaperOutput){
					case ID_PDL_PAPEROUTPUT_AUTO:
					case ID_PDL_PAPEROUTPUT_UPPERLEFT_TRAY:
					case ID_PDL_PAPEROUTPUT_LOWERLEFT_TRAY:
						break;
					case ID_PDL_PAPEROUTPUT_STACKER_OUTTRAY:
					case ID_PDL_PAPEROUTPUT_STACKER:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
							break;
						}
						if ( dwfshFlags & ID_PUNCH_ON ) {
							bFixBin = FALSE;
							break;
						}
						break;
					default:
						bFixBin = FALSE;
						break;
					}
				}
				else if(bIsEqual4 == TRUE){
					/* Finisher-AB1 or Saddle Finisher-AB2 + High Capacity Stacker-C1 x 2 */
					switch (lpPDLFinishing->fshPaperOutput){
					case ID_PDL_PAPEROUTPUT_AUTO:
					case ID_PDL_PAPEROUTPUT_UPPERLEFT_TRAY:
					case ID_PDL_PAPEROUTPUT_LOWERLEFT_TRAY:
						break;
					case ID_PDL_PAPEROUTPUT_STACKER_A_OUTTRAY:
					case ID_PDL_PAPEROUTPUT_STACKER_B_OUTTRAY:
					case ID_PDL_PAPEROUTPUT_STACKER_A_STACK:
					case ID_PDL_PAPEROUTPUT_STACKER_B_STACK:
					case ID_PDL_PAPEROUTPUT_AUTOCHANGE_STACKER_A:
					case ID_PDL_PAPEROUTPUT_AUTOCHANGE_STACKER_B:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
							break;
						}
						if ( dwfshFlags & ID_PUNCH_ON ) {
							bFixBin = FALSE;
							break;
						}
						break;
					default:
						bFixBin = FALSE;
						break;
					}
				}
				else{
					/* Finisher-AB1 or Saddle Finisher-AB2 */
					switch (lpPDLFinishing->fshPaperOutput){
					case ID_PDL_PAPEROUTPUT_AUTO:
					case ID_PDL_PAPEROUTPUT_UPPERLEFT_TRAY:
					case ID_PDL_PAPEROUTPUT_LOWERLEFT_TRAY:
						break;
					default:
						bFixBin = FALSE;
						break;
					}
				}
			}
			else if((bIsEqual5 == TRUE) || (bIsEqual6 == TRUE)){
				if(bIsEqual3 == TRUE){
					switch (lpPDLFinishing->fshPaperOutput){
					case ID_PDL_PAPEROUTPUT_AUTO:
					case ID_PDL_PAPEROUTPUT_UPPERTRAY_A:
					case ID_PDL_PAPEROUTPUT_MIDDLETRAY_B:
					case ID_PDL_PAPEROUTPUT_LOWERTRAY_C:
						break;
					case ID_PDL_PAPEROUTPUT_STACKER_OUTTRAY:
					case ID_PDL_PAPEROUTPUT_STACKER:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
							break;
						}
						if ( dwfshFlags & ID_PUNCH_ON ) {
							bFixBin = FALSE;
							break;
						}
						break;
					default:
						bFixBin = FALSE;
						break;
					}
				}
				else if(bIsEqual4 == TRUE){
					/* Finisher-AB1 or Saddle Finisher-AB2 + High Capacity Stacker-C1 x 2 */
					switch (lpPDLFinishing->fshPaperOutput){
					case ID_PDL_PAPEROUTPUT_AUTO:
					case ID_PDL_PAPEROUTPUT_UPPERTRAY_A:
					case ID_PDL_PAPEROUTPUT_MIDDLETRAY_B:
					case ID_PDL_PAPEROUTPUT_LOWERTRAY_C:
						break;
					case ID_PDL_PAPEROUTPUT_STACKER_A_OUTTRAY:
					case ID_PDL_PAPEROUTPUT_STACKER_B_OUTTRAY:
					case ID_PDL_PAPEROUTPUT_STACKER_A_STACK:
					case ID_PDL_PAPEROUTPUT_STACKER_B_STACK:
					case ID_PDL_PAPEROUTPUT_AUTOCHANGE_STACKER_A:
					case ID_PDL_PAPEROUTPUT_AUTOCHANGE_STACKER_B:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
							break;
						}
						if ( dwfshFlags & ID_PUNCH_ON ) {
							bFixBin = FALSE;
							break;
						}
						break;
					default:
						bFixBin = FALSE;
						break;
					}
				}
				else{
					/* Finisher-AB1 or Saddle Finisher-AB2 */
					switch (lpPDLFinishing->fshPaperOutput){
					case ID_PDL_PAPEROUTPUT_AUTO:
					case ID_PDL_PAPEROUTPUT_UPPERTRAY_A:
					case ID_PDL_PAPEROUTPUT_MIDDLETRAY_B:
					case ID_PDL_PAPEROUTPUT_LOWERTRAY_C:
						break;
					default:
						bFixBin = FALSE;
						break;
					}
				}
				/* 121210 SQW@SSS [v20.90][Alpha][Add][Drv_v20_90_a_FamiCom-0014] Start */
			}else if((bIsEqual7 == TRUE) || ( bIsEqual8 == TRUE )){
				if(bIsEqual3 == TRUE){
					/* Finisher-AK1 or Saddle Finisher-AK2 + High Capacity Stacker-C1 */
					switch (lpPDLFinishing->fshPaperOutput){
					case ID_PDL_PAPEROUTPUT_AUTO:
					case ID_PDL_PAPEROUTPUT_LOWERLEFT_TRAY:
						break;
					case ID_PDL_PAPEROUTPUT_UPPERLEFT_TRAY:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
						}
						break;
					case ID_PDL_PAPEROUTPUT_STACKER_OUTTRAY:
					case ID_PDL_PAPEROUTPUT_STACKER:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
						}

						if ( lpPDLFinishing->fshSpeciesOfHolePunch != ID_FINI_SPECIES_HOLEPUNCH_MULTIHOLES ){
							if ( dwfshFlags & ID_PUNCH_ON ) {
								bFixBin = FALSE;
							}
						}
						break;
					default:
						bFixBin = FALSE;
						break;
					}
				}
				else if(bIsEqual4 == TRUE){
					/* Finisher-AK1 or Saddle Finisher-AK2 + High Capacity Stacker-C1 x 2 */
					switch (lpPDLFinishing->fshPaperOutput){
					case ID_PDL_PAPEROUTPUT_AUTO:
					case ID_PDL_PAPEROUTPUT_LOWERLEFT_TRAY:
						break;
					case ID_PDL_PAPEROUTPUT_UPPERLEFT_TRAY:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
						}
						break;
					case ID_PDL_PAPEROUTPUT_STACKER_A_OUTTRAY:
					case ID_PDL_PAPEROUTPUT_STACKER_B_OUTTRAY:
					case ID_PDL_PAPEROUTPUT_STACKER_A_STACK:
					case ID_PDL_PAPEROUTPUT_STACKER_B_STACK:
					case ID_PDL_PAPEROUTPUT_AUTOCHANGE_STACKER_A:
					case ID_PDL_PAPEROUTPUT_AUTOCHANGE_STACKER_B:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
						}

						if ( lpPDLFinishing->fshSpeciesOfHolePunch != ID_FINI_SPECIES_HOLEPUNCH_MULTIHOLES ){
							if ( dwfshFlags & ID_PUNCH_ON ) {
								bFixBin = FALSE;
							}
						}
						break;
					default:
						bFixBin = FALSE;
						break;
					}
				}
				else{
					/* Finisher-AK1 or Saddle Finisher-AK2 */
					switch (lpPDLFinishing->fshPaperOutput){
					case ID_PDL_PAPEROUTPUT_AUTO:
					case ID_PDL_PAPEROUTPUT_LOWERLEFT_TRAY:
						break;
					case ID_PDL_PAPEROUTPUT_UPPERLEFT_TRAY:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
						}
						break;
					default:
						bFixBin = FALSE;
						break;
					}
				}
			}
			/* 121210 SQW@SSS [v20.90][Alpha][Add][Drv_v20_90_a_FamiCom-0014] End */
			else if(bIsEqual3 == TRUE){
				/* Only High Capacity Stacker-C1 */
				switch (lpPDLFinishing->fshPaperOutput){
				case ID_PDL_PAPEROUTPUT_AUTO:
					break;
				case ID_PDL_PAPEROUTPUT_STACKER_OUTTRAY:
				case ID_PDL_PAPEROUTPUT_STACKER:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
							break;
						}
						if ( dwfshFlags & ID_PUNCH_ON ) {
							bFixBin = FALSE;
							break;
						}
						break;
					break;
				default:
					bFixBin = FALSE;
					break;
				}
			}
			else if(bIsEqual4 == TRUE){
				/* Only High Capacity Stacker-C1 x 2 */
				switch (lpPDLFinishing->fshPaperOutput){
				case ID_PDL_PAPEROUTPUT_AUTO:
					break;
				case ID_PDL_PAPEROUTPUT_STACKER_A_OUTTRAY:
				case ID_PDL_PAPEROUTPUT_STACKER_B_OUTTRAY:
				case ID_PDL_PAPEROUTPUT_STACKER_A_STACK:
				case ID_PDL_PAPEROUTPUT_STACKER_B_STACK:
				case ID_PDL_PAPEROUTPUT_AUTOCHANGE_STACKER_A:
				case ID_PDL_PAPEROUTPUT_AUTOCHANGE_STACKER_B:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
							break;
						}
						if ( dwfshFlags & ID_PUNCH_ON ) {
							bFixBin = FALSE;
							break;
						}
						break;
					break;
				default:
					bFixBin = FALSE;
					break;
				}
			}
			break;

		case IDM_iRADVC2030:
			bIsEqual	= PDLCMN_bIsEqualFinisherID(ID_FSH_INNER2WAYTRAY, lpPDLDevOption->dwFinisher);
			bIsEqual2 = lpPDLDevOption->dwFinisher & ID_FSH_COPYTRAY;
			bIsEqual3 = lpPDLDevOption->dwFinisher & ID_FSH_INNERFINISHER;
			bIsEqual4 = lpPDLDevOption->dwFinisher & ID_FSH_FINISHERTRAY;
			
			if( bIsEqual ){
				/* Inner 2way Tray + Copy Tray */
				if( bIsEqual2 ){
					if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
						(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_A) ||
						(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_B) ){
						;
						
					} else if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_RIGHTTRAY_C) ){
						if( dwfshFlags & ID_STAPLE_ON || lpPDLFinishing->fshFlags & ID_PDL_OFFSET_ON ){
							bFixBin = FALSE;
						}
						
					} else {
						bFixBin = FALSE;
					}
				}
				/* Inner 2way Tray のみ */
				else {
					if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
						(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_A) ||
						(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_B) ){
						;
					} else {
						bFixBin = FALSE;
					}
				}
			}
			
			if( bIsEqual3 ){
				if( bIsEqual2 ){
					/* Inner Finisher + Copy Tray + Additional Tray */
					if( bIsEqual4 ){
						if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
							(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_A) ||
							(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_B) ){
							;
							
						} else if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_RIGHTTRAY_C) ){
							if( dwfshFlags & ID_STAPLE_ON || lpPDLFinishing->fshFlags & ID_PDL_OFFSET_ON ){
								bFixBin = FALSE;
							}
							
						} else {
							bFixBin = FALSE;
						}
					}
					/* Inner Finisher + Copy Tray */
					else {
						if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
							(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_INNERTRAY_A) ){
							;
							
						} else if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_RIGHTTRAY_B) ){
							if( dwfshFlags & ID_STAPLE_ON || lpPDLFinishing->fshFlags & ID_PDL_OFFSET_ON ){
								bFixBin = FALSE;
							}
							
						} else {
							bFixBin = FALSE;
						}
					}
				}
				/* Inner Finisher + Additional Tray */
				else if( bIsEqual4 ){
					if( (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
						(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_LOWERTRAY_A) ||
						(lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_UPPERTRAY_B) ){
						;
					} else {
						bFixBin = FALSE;
					}
				}
				/* Inner Finisher のみ */
				else {
					if( lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO ){
						;
					} else {
						bFixBin = FALSE;
					}
				}
			}

			/* Copy Tray のみ。フラグを == で見る事で他のものが付いていないことを確認できる。 */
			if( lpPDLDevOption->dwFinisher == ID_FSH_COPYTRAY ){
				if((lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_AUTO) ||
				   (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_INNERTRAY_A) ||
				   (lpPDLFinishing->fshPaperOutput == ID_PDL_PAPEROUTPUT_RIGHTTRAY_B)){
					;
				} else {
					bFixBin = FALSE;
				}
			}
			break;
			
//#if 1	/* 100316 KIS@CIS [V20.35] for Nottingham */
		case IDM_iPRC7010VP:
		case IDM_iPRC6010VP:
		case IDM_iPRC6010:
			bIsEqual	= PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher);
			bIsEqual2	= PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_1, lpPDLDevOption->dwFinisher);
			bIsEqual3	= PDLCMN_bIsEqualFinisherID(ID_FSH_STACKER_1, lpPDLDevOption->dwFinisher);
			bIsEqual4	= PDLCMN_bIsEqualFinisherID(ID_FSH_STACKER_2, lpPDLDevOption->dwFinisher);
			
			if((bIsEqual == TRUE) || (bIsEqual2 == TRUE)){
				if(bIsEqual3 == TRUE){
					/* Finisher-AB1 or Saddle Finisher-AB2 + High Capacity Stacker-C1 */
					switch (lpPDLFinishing->fshPaperOutput){
					case ID_PDL_PAPEROUTPUT_AUTO:
					case ID_PDL_PAPEROUTPUT_UPPERLEFT_TRAY:
					case ID_PDL_PAPEROUTPUT_LOWERLEFT_TRAY:
						break;
					case ID_PDL_PAPEROUTPUT_STACKER_OUTTRAY:
					case ID_PDL_PAPEROUTPUT_STACKER:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
						}

						if ( lpPDLFinishing->fshSpeciesOfHolePunch != ID_FINI_SPECIES_HOLEPUNCH_MULTIHOLES ){
							if ( dwfshFlags & ID_PUNCH_ON ) {
								bFixBin = FALSE;
							}
						}
						break;
					default:
						bFixBin = FALSE;
						break;
					}
				}
				else if(bIsEqual4 == TRUE){
					/* Finisher-AB1 or Saddle Finisher-AB2 + High Capacity Stacker-C1 x 2 */
					switch (lpPDLFinishing->fshPaperOutput){
					case ID_PDL_PAPEROUTPUT_AUTO:
					case ID_PDL_PAPEROUTPUT_UPPERLEFT_TRAY:
					case ID_PDL_PAPEROUTPUT_LOWERLEFT_TRAY:
						break;
					case ID_PDL_PAPEROUTPUT_STACKER_A_OUTTRAY:
					case ID_PDL_PAPEROUTPUT_STACKER_B_OUTTRAY:
					case ID_PDL_PAPEROUTPUT_STACKER_A_STACK:
					case ID_PDL_PAPEROUTPUT_STACKER_B_STACK:
					case ID_PDL_PAPEROUTPUT_AUTOCHANGE_STACKER_A:
					case ID_PDL_PAPEROUTPUT_AUTOCHANGE_STACKER_B:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
						}

						if ( lpPDLFinishing->fshSpeciesOfHolePunch != ID_FINI_SPECIES_HOLEPUNCH_MULTIHOLES ){
							if ( dwfshFlags & ID_PUNCH_ON ) {
								bFixBin = FALSE;
							}
						}
						break;
					default:
						bFixBin = FALSE;
						break;
					}
				}
				else{
					/* Finisher-AB1 or Saddle Finisher-AB2 */
					switch (lpPDLFinishing->fshPaperOutput){
					case ID_PDL_PAPEROUTPUT_AUTO:
					case ID_PDL_PAPEROUTPUT_UPPERLEFT_TRAY:
					case ID_PDL_PAPEROUTPUT_LOWERLEFT_TRAY:
						break;
					default:
						bFixBin = FALSE;
						break;
					}
				}
			}
			else if(bIsEqual3 == TRUE){
				/* Only High Capacity Stacker-C1 */
				switch (lpPDLFinishing->fshPaperOutput){
				case ID_PDL_PAPEROUTPUT_AUTO:
					break;
				case ID_PDL_PAPEROUTPUT_STACKER_OUTTRAY:
					/* do not break */
				case ID_PDL_PAPEROUTPUT_STACKER:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
						}

						if ( lpPDLFinishing->fshSpeciesOfHolePunch != ID_FINI_SPECIES_HOLEPUNCH_MULTIHOLES ){
							if ( dwfshFlags & ID_PUNCH_ON ) {
								bFixBin = FALSE;
							}
						}
					break;
				default:
					bFixBin = FALSE;
					break;
				}
			}
			else if(bIsEqual4 == TRUE){
				/* Only High Capacity Stacker-C1 x 2 */
				switch (lpPDLFinishing->fshPaperOutput){
				case ID_PDL_PAPEROUTPUT_AUTO:
					break;
				case ID_PDL_PAPEROUTPUT_STACKER_A_OUTTRAY:
				case ID_PDL_PAPEROUTPUT_STACKER_B_OUTTRAY:
				case ID_PDL_PAPEROUTPUT_STACKER_A_STACK:
				case ID_PDL_PAPEROUTPUT_STACKER_B_STACK:
				case ID_PDL_PAPEROUTPUT_AUTOCHANGE_STACKER_A:
				case ID_PDL_PAPEROUTPUT_AUTOCHANGE_STACKER_B:
						if ( dwfshFlags & ID_STAPLE_ON ) {
							bFixBin = FALSE;
						}

						if ( lpPDLFinishing->fshPaperOutput != ID_FINI_SPECIES_HOLEPUNCH_MULTIHOLES ){
							if ( dwfshFlags & ID_PUNCH_ON ) {
								bFixBin = FALSE;
							}
						}
					break;
				default:
					bFixBin = FALSE;
					break;
				}
			}
			break;
//#endif	/* 100316 KIS@CIS [V20.35] for Nottingham */

		default:
			break;
		}
	
	/*_ 040611 UCH@CSL デバイスソート不可 */
	/* v20.35_Alpha [DriverCollate] [Modify] ! 100318 ZYS@SSS */
	if (PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_DRIVERSORT_SUPPORT) == FALSE) {
		if (PDL_GetCollate(lpCap) == ID_STATE3_DISABLE) {
			if (dwloFlags & ID_PCCOLLATE_ON){
				if (sOptPriority == ID_PRIORITY_PRINTER) bPcCollate = FALSE;
			}
		}
	}
	
	if (sOptPriority == ID_PRIORITY_PRINTER) {
		if ( sEdge != ID_EDGING_NONE || sDate & ID_LAYOUTPOS_PRINT ||
			 sName & ID_LAYOUTPOS_PRINT || sNumber & ID_LAYOUTPOS_PRINT ) {
			bLoOpt = FALSE;
		}
	}
	
	if (dwloFlags & ID_PCNUP_ON) {
		if ( (sNUpPages != ID_NUP_2PAGES) && (sNUpPages != ID_NUP_4PAGES) ) {
			if (sOptPriority == ID_PRIORITY_PRINTER) bPcNup = FALSE;
		}
	}
	
/* v20.35_Alpha [DriverAutoColor] [Modify] Start. */
/* ! 100302 SYC@SSS */
//#if 1
	dwloFlags &= ~ID_PCAUTOCOLOR_ON;
	ULIB_ADM_SetLay_Flags(m_lpuift, lpdmProfile, &dwloFlags);
//#else
//	if (dwloFlags & ID_PCAUTOCOLOR_ON) {
//		if (sOptPriority == ID_PRIORITY_PRINTER) {
//			dwloFlags &= ~ID_PCAUTOCOLOR_ON;
//			ULIB_ADM_SetLay_Flags(m_lpuift, lpdmProfile, &dwloFlags);
//		}
//	}
//#endif
/* v20.35_Alpha [DriverAutoColor] [Modify] End. */

	if (dwfshFlags & ID_MECHANICAL_ON){
		dwReason = 0x0L;
		bMechaSort = L4_UTL_bIsMechanicalSortPossible (m_lpuift, (LPDWORD)&dwReason);
	}
	
	if (dwfshFlags & ID_BINSTACK_ON){
		dwReason = 0x0L;
		bStack = L4_UTL_bIsStacktPossible (m_lpuift, (LPDWORD)&dwReason);
	}
	
	if (dwfshFlags & ID_BINUSERSEPARATE_ON){
		dwReason = 0x0L;
		bUserSep = L4_UTL_bIsUserSepModePossible (m_lpuift, (LPDWORD)&dwReason);
	}
	
	if( dwPaperFlag & ID_COVERINSERTER_ON ){
		bInserter = L4_UTL_bFixInserter(m_lpuift);
	}
	
	/*_ 990924 YAO@CSL */
	/*_ 051005 KBT@CSL V10.90:E244 LIPSモード時、300dpi非サポート */
	if( (sQualPrivQuality == ID_RESO_1200) || (PDL_GetEngineType(lpCap) == ID_ENGINETYPE_BJ) ){
		dwReason = 0x0L;
		bReso = L4_UTL_bIsResolutionPossible (m_lpuift, lpdmProfile, (LPDWORD)&dwReason);
	}
	
	/*_ 040610 UCH@CSL 最終ページから印刷 */
	/*_ Lプリの「最終ページから印刷」はID_REVERSE_ONがたつ。LBP1910は必ずフェースアップとセットで同フラグがたつので */
	/*_ ID_REVERSE_ONのみの場合をLプリの「最終ページから印刷」と判定して丸め処理を通す */
	if ( (dwfshFlags & ID_REVERSE_ON) && (!(dwfshFlags & ID_FACEUP_ON)) ) {
		if (PDL_GetEngineType(lpCap) != ID_ENGINETYPE_BJ) {
			bPrintFromLastPage = FALSE;
		}
		else if (sOptPriority == ID_PRIORITY_PRINTER) {
			bPrintFromLastPage = FALSE;
		}
	}
	
	/*_ 051110 KBT@CSL モノクロ時にICMモードを丸める */
#if defined(WINNT) && (WINVER >= 0x0500)
	if( ULIB_CAP_GetColor(m_lpuift) == ID_STATE3_DISABLE){
		if ((lpdm->dmICMMethod == DMICMMETHOD_DRIVER) || (lpdm->dmICMMethod == DMICMMETHOD_SYSTEM)){
			bICM = FALSE;
		}
	}
#endif
#if !defined (WINNT)
	/*_ 040607 NON@CSL Color Gear Lite対応(Lプリンタ)---ICMモード非サポート */
	if (PDL_GetEngineType(lpCap) == ID_ENGINETYPE_BJ) {
		if (dwQualFlag & ID_ICM_ON) {
			bICM = FALSE;
		}
		if ((lpdm->dmICMMethod == DMICMMETHOD_DRIVER) || (lpdm->dmICMMethod == DMICMMETHOD_SYSTEM)
		    || (lpdm->dmFields & DM_ICMMETHOD)) {
			bICM = FALSE;
		}
	}
	/*_ 051110 KBT@CSL モノクロ時にICMモードを丸める */
	if( ULIB_CAP_GetColor(m_lpuift) == ID_STATE3_DISABLE){
		if (dwQualFlag & ID_ICM_ON) {
			bICM = FALSE;
		}
	}
#endif	/*_ !defined (WINNT) */


	switch (sPaperSelection){
	case ID_PS_ALLPAGES:
		bDefaultSource		= L4_UTL_bCheckPaperSourceSet(m_lpuift, lpdmProfile, MAKELONG(sDefaultSource, sPapFormSID));
		break;
	case ID_PS_FIRSTLAST:
		bFirst				= L4_UTL_bCheckPaperSourceSet(m_lpuift, lpdmProfile, MAKELONG(sFirstPage, sPapFormSID));
		bDefaultSource		= L4_UTL_bCheckPaperSourceSet(m_lpuift, lpdmProfile, MAKELONG(sDefaultSource, sPapFormSID));
		bLast				= L4_UTL_bCheckPaperSourceSet(m_lpuift, lpdmProfile, MAKELONG(sLastPage, sPapFormSID));
		break;
	case ID_PS_FIRST2NDLAST:
		bFirst				= L4_UTL_bCheckPaperSourceSet(m_lpuift, lpdmProfile, MAKELONG(sFirstPage, sPapFormSID));
		b2nd				= L4_UTL_bCheckPaperSourceSet(m_lpuift, lpdmProfile, MAKELONG(s2ndPage, sPapFormSID));
		bDefaultSource		= L4_UTL_bCheckPaperSourceSet(m_lpuift, lpdmProfile, MAKELONG(sDefaultSource, sPapFormSID));
		bLast				= L4_UTL_bCheckPaperSourceSet(m_lpuift, lpdmProfile, MAKELONG(sLastPage, sPapFormSID));
		break;
	case ID_PS_TRANSPARENCY:
		bInterLeaf			= L4_UTL_bCheckPaperSourceSet(m_lpuift, lpdmProfile, MAKELONG(sInterLeaf, sPapFormSID));
		break;
	case ID_PS_ADDCOVER:
		bFirst				= L4_UTL_bCheckPaperSourceSet(m_lpuift, lpdmProfile, MAKELONG(sFirstPage, sPapFormSID));
		bDefaultSource		= L4_UTL_bCheckPaperSourceSet(m_lpuift, lpdmProfile, MAKELONG(sDefaultSource, sPapFormSID));
		break;
	}

		/* 用紙サイズと用紙タイプ 000203 YAO@CSL */
	if(lpPDLPaperSource->psFlags & PAPERSRC_MEDIA){
		switch (sPaperSelection){ 
		case ID_PS_ALLPAGES:
			bDefaultSourcePt	= L4_UTL_bCheckPaperTypeSet(m_lpuift, lpdmProfile, MAKELONG(lpPDLPaperSource->sPaperTypeID, sPapFormSID));
			break;
		case ID_PS_FIRSTLAST:
			bFirstPt			= L4_UTL_bCheckPaperTypeSet(m_lpuift, lpdmProfile, MAKELONG(lpPDLPaperSource->s1stMediaID, sPapFormSID));
			bDefaultSourcePt	= L4_UTL_bCheckPaperTypeSet(m_lpuift, lpdmProfile, MAKELONG(lpPDLPaperSource->sPaperTypeID, sPapFormSID));
			bLastPt				= L4_UTL_bCheckPaperTypeSet(m_lpuift, lpdmProfile, MAKELONG(lpPDLPaperSource->sLastMediaID, sPapFormSID));
			break;
		case ID_PS_FIRST2NDLAST:
			bFirstPt			= L4_UTL_bCheckPaperTypeSet(m_lpuift, lpdmProfile, MAKELONG(lpPDLPaperSource->s1stMediaID, sPapFormSID));
			b2ndPt				= L4_UTL_bCheckPaperTypeSet(m_lpuift, lpdmProfile, MAKELONG(lpPDLPaperSource->s2ndMediaID, sPapFormSID));
			bLastPt				= L4_UTL_bCheckPaperTypeSet(m_lpuift, lpdmProfile, MAKELONG(lpPDLPaperSource->sLastMediaID, sPapFormSID));
			bDefaultSourcePt	= L4_UTL_bCheckPaperTypeSet(m_lpuift, lpdmProfile, MAKELONG(lpPDLPaperSource->sPaperTypeID, sPapFormSID));
			break;
		case ID_PS_TRANSPARENCY:
			bInterLeafPt		= L4_UTL_bCheckPaperTypeSet(m_lpuift, lpdmProfile, MAKELONG(lpPDLPaperSource->sInterLeafMediaID, sPapFormSID));
			break;
		case ID_PS_ADDCOVER:
			bFirstPt			= L4_UTL_bCheckPaperTypeSet(m_lpuift, lpdmProfile, MAKELONG(lpPDLPaperSource->s1stMediaID, sPapFormSID));
			bDefaultSourcePt	= L4_UTL_bCheckPaperTypeSet(m_lpuift, lpdmProfile, MAKELONG(lpPDLPaperSource->sPaperTypeID, sPapFormSID));
			break;
		}
	}
	/* 140808 CHL@SSS [v21.40][PreAlpha][Add][UsabilityUp4SelectPaperType] Start */
	if(bIgnorePaperSelection){
		bDefaultSourcePt	= L4_UTL_bCheckPaperTypeSet(m_lpuift, lpdmProfile, MAKELONG(lpPDLPaperSource->sPaperTypeID, sPapFormSID));
	}
	/* 140808 CHL@SSS [v21.40][PreAlpha][Add][UsabilityUp4SelectPaperType] End */

	/* Stretch	*/
	if( (sloStretch < 25) || (sloStretch > 200) ){
		bStretch = FALSE;
	}

	/* Copy	*/
	
	{
		short sCopy = 0;
		sCopy = lpdmProfile->dmCopies;
		if( (sCopy < 1) || (sCopy > (short)PDL_GetMaxCopies(lpCap)) ){
			bCopy = FALSE;
		}
	}

#ifdef WINNT
	bUserPaper = L4_UTL_bCheckUserPaper( m_lpuift , lpdmProfile );
#endif
	
	/* Punch	*/
	if (dwfshFlags & ID_PUNCH_ON){
//#if 0	/* 100721 WAT@IST v20.20_Alpha_036 */
//		bPunch = L4_UTL_bIsHolePunchPossible( m_lpuift, lpdmProfile);	/* 20061227 WZC@CSSSI [v1210][LIPS_v12.00-0049] */
//#else
		bPunch = UTL_bIsHolePunchPossible(m_lpuift, lpdmProfile);
//#endif	/* 100721 WAT@IST v20.20_Alpha_036 */
	}
	
	if(lpPDLDevOption->dwPuncherType == ID_PUNCHERTYPE_NONE){
		if( lpCDM->pcCdevmode.pdlFinishing.fshSpeciesOfHolePunch != ID_FINI_SPECIES_HOLEPUNCH_NONE){
			bPunch = FALSE;
		}
	}
	
/* 20080706 SYC@CSSSI [v1240][MultiFunctionFold] Start */
	if(	(lpPDLDevOption->dwDevFuncFlags & ID_DEVFUNC_FOLD_DETAIL) == ID_DEVFUNC_FOLD_DETAIL ){
		/* ZFold */
		if (dwfshFlags & ID_ZFOLD_ON) {
			bZFold = L4_UTL_bIsZFoldPossible_E220( m_lpuift, lpdmProfile );
		}

		/* CFold 3Fold 4Fold*/
		if ( (lpPDLFinishing->fshFlags & ID_FSH_CFOLD_ON) ||
				 (lpPDLFinishing->fshFlags2 & (ID_FSH_ACCORDION_ON|ID_FSH_FOUR_ON|ID_FSH_MIDDLE_ON)) ) {	/* 090330 WAT@IST LIPS4_i_QA V20.00-00042 */
			bCFold = L4_UTL_bIsCFoldPossible_E220( m_lpuift, lpdmProfile );
		}

		/* SaddleFold*/
		if (lpPDLFinishing->fshFlags & ID_SADDLEFOLD_ON) {
			bSaddleFold = L4_UTL_bIsSaddleFoldPossible_E220( m_lpuift, lpdmProfile );
		}
	} else {
		/* ZFold	*/
		if( dwfshFlags & ID_ZFOLD_ON ){
			bZFold = L4_UTL_bIsZFoldPossible( m_lpuift, lpdmProfile );/* 20060731 ZHF@CSSSI [v1125][CFold] Add the parameter lpdmProfile */
		}
		
		/* CFold	*/ /* 20060713 ZHF@CSSSI [v1125][CFold] Start */
		if( lpPDLFinishing->fshFlags & ID_FSH_CFOLD_ON ){
			bCFold = L4_UTL_bIsCFoldPossible( m_lpuift, lpdmProfile );
		}/* 20060713 ZHF@CSSSI [v1125][CFold] End */
	}
/* 20080706 SYC@CSSSI [v1240][MultiFunctionFold] End */

	/*_ とじしろ */
	if (sMargin > sMaxBindMarginMm) b50mm = FALSE;
	
	/*_ 製本とじしろ */
	if ((sBookletMargin/10) > sMaxBindMarginMm) bBooklet50mm = FALSE;
	
	/* 100527 WAT@IST ColorUpdate */
	if (squaColorMode == ID_COLOR_AUTO && sOptPriority == ID_PRIORITY_PRINTER && sCategory == ID_CATEGORY_NUP && ((sNUpPages == ID_NUP_2PAGES) || (sNUpPages == ID_NUP_4PAGES))) {
		bAutoColor = FALSE;
	}
	/* 100527 WAT@IST ColorUpdate */

	/* 110412 CCR@SSS [v20.65][PreAlpha][Add][NupWithDeviceBooklet] Start */
//#if 1	/* 111008 CCR@SSS [v20.65][Beta][Delete][CR0027] NupWithDeviceBooklet Rollback */
	if (squaColorMode == ID_COLOR_AUTO && sOptPriority == ID_PRIORITY_PRINTER && sCategory == ID_CATEGORY_BOOKLET && ((sNUpPages == ID_NUP_2PAGES) || (sNUpPages == ID_NUP_4PAGES))) {
		LPPDL_LAYOUT	lpLo = NULL;

		if (lpCDM) {
			lpLo	= (LPPDL_LAYOUT)&(lpCDM->pcCdevmode.pdlLayout);
		}

		if (lpLo == NULL) {
			return FALSE;
		}

		bQueryRet = PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_PAGE_DEVICEBOOKLETANDNUP_SUPPORT);
		if (bQueryRet == TRUE) {
			if (lpLo) {
				if(lpLo->loFlags & ID_BOOKLETNUP_ON) {
					if ((sNUpPages == ID_NUP_2PAGES) || (sNUpPages == ID_NUP_4PAGES)) {
						bAutoColor = FALSE;
					}
				}
			}

		}
	}
//#endif
	/* 110412 CCR@SSS [v20.65][PreAlpha][Add][NupWithDeviceBooklet] End */

	/* Win9x v7.31 SQA 142	*//* グラフィックスシート丸め	*/
	/*_ 020619 KBT@CSL COLOR IR 階調固定4BIT */
	if (L4_UTL_bFixColorDepth(m_lpuift, lpdmProfile)){
		bColorDepth = FALSE;
	}

	/* グラフィックモード対応機種で、イメージモード + 中間調：「パネル優先」の場合は丸める */
	if (!(PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift),  QUERY_FINI_RASTERIZEATHOST_SUPPORT))) {
		short	sQuaGraphicMode = 0;
		ULIB_ADM_GetQal_GraphicsMode(m_lpuift, lpdmProfile, &sQuaGraphicMode);
		if ((sQuaGraphicMode == ID_GRAPHICMODE_RASTER)) {
			if (PDL_GetColor(lpCap) == ID_STATE3_DISABLE) {	/* モノクロ機 */
				if (lpPDLQuality->quaGrayTonePattern == ID_HALFTONE_PRINTERDEFAULT) {
					bMonoHalfToneObj = FALSE;
				}
			} else {	/* カラー機 */
				if (lpPDLQuality->quaHalfToning == ID_HALFTONE_PRINTERDEFAULT){
					bColorHalfToneObj = FALSE;
				}
			}
		}
	}
	bGrayOut = L4_UTL_bFixGrayOut(m_lpuift, lpdmProfile);
	
	/*_ モノクロ中間調 */
	if (L4_UTL_bFixHalfToning(m_lpuift, lpdmProfile))
		bHalfToning = FALSE;
	
#ifdef WINNT
	if (L4_UTL_bFixModeDetail(m_lpuift))
		bModeDetail = FALSE;
#endif

	/* 特殊中間調 */
	if (lpPDLQuality->quaRenderingFlags & ID_SPECIALHT_ON){
		bSpHalfTone = FALSE;
		
		if (!bSpHalfTone)
			lpPDLQuality->quaRenderingFlags &= ~ID_SPECIALHT_ON;
	}

	bColor = L4_UTL_bFixColor(m_lpuift, lpdmProfile);
	
	if (!(PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_QLTY_LINECLEAR_SUPPORT)))    /* 110819 JJJ@SSS [v20.65][Alpha][Add][Drv11_BR3_V20_65_a_FamiCom-0031] */
	{
		short	sColor = 0;

		ULIB_ADM_GetQal_ColorMode(m_lpuift, lpdmProfile, &sColor);
		
		if (lpPDLQuality->quaRenderingFlags & ID_LINECLEAR_ON){
			bLineClear = FALSE;
		}
	}
	
	/*_ 001219 SZK@CSL for No DeviceFont */
	if (lpdm->dmTTOption == DMTT_SUBDEV){
		if (!L4_UTL_bIsTTSubstitutePossible( m_lpuift )){
			bTTSub = FALSE;
			lpdm->dmTTOption = DMTT_BITMAP;
		}	
	}
	
	/*_ 010116 SZK@CSL トナー濃度調整 */
	/* 110622 CHL@SSS [v20.65][PreAlpha][Add][Toner Density Printer Default] */
	if(!( PDLCMN_FunctionQuery(sPrnID, QUERY_QLTY_TONERDENSITYADJUST_SUPPORT))){
		if(!( PDLCMN_FunctionQuery(sPrnID, QUERY_QLTY_TONERDENSITY4PAIR1_SUPPORT))){
			if (lpPDLQuality->quaRenderingFlags & ID_TONERDENSITY_ON){
				bTonerDensity = FALSE;
				lpPDLQuality->quaRenderingFlags &= ~ID_TONERDENSITY_ON;
				lpPDLQuality->quaTonerDensityBlack = ID_TONERDENSITY_4;
			} else {
				lpPDLQuality->quaTonerDensityBlack = ID_TONERDENSITY_4;
			}
		}
	}

	/* 050811 KTO@CSL E230-0002仕様変更 */
	if (lpPDLQuality->quaRenderingFlags & ID_DEVICECMS_ON) {
		if( PDLCMN_FunctionQuery(sPrnID, QUERY_QLTY_DEVICE_CMS) == FALSE ){
			bDevCMS = FALSE;
		}
	}
	
//#if 1	/* 100524 ISI@IST *//* MOD LIPS4_i_QA V20.35-00043 */
	if( ((lpPDLPaperSource->psFlags & PAPERSRC_MEDIA) == PAPERSRC_MEDIA) ||
		bIgnorePaperSelection ){ /* 140808 CHL@SSS [v21.40][PreAlpha][Add][UsabilityUp4SelectPaperType] *//* Need or not??? sPaperType is used by LBP */
//#endif
		bPaperType = L4_UTL_bCheckPaperType( m_lpuift, lpdmProfile);
		
		/* 000515 SZK@CSL PaperType Conflict */
		if(PDL_IsPaperTypeSupported(lpCap)){
			/*_ 040628 UCH@CSL 「用紙タイプで指定」の判定を付加 */
			if (bPaperType){
				if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_BASE_LBP) == FALSE ){
					L4_UTL_vPaperMediaType1List(m_lpuift, lpdmProfile, dwAvailMediaType1Array);

					for (i = 0; MediaType1_ItmTbl[i] != 0; i++){
						if (MediaType1_ItmTbl[i] == sPaperType){
							bPaperTypeAvail = L4_LIB_bCheckMediaType1(dwAvailMediaType1Array, (SHORT)MediaType1_ItmTbl[i]);
							if (bPaperTypeAvail == FALSE) {
								bPaperType = FALSE;
								break;
							}
						}
					}
					if (sPaperSelection == ID_PS_TRANSPARENCY){
						bPaperTypeAvail = L4_LIB_bCheckMediaType1(dwAvailMediaType1Array, (SHORT)ID_MEDIA_TRANSPARENCY);
						if (bPaperTypeAvail == FALSE){
							if(lpPDLPaperSource->psFlags & PAPERSRC_MEDIA) {
								bPaperType = FALSE;
							}
						}
					}
				} else {
					dwSupport = L4_UTL_dwPaperTypeList_LBP(m_lpuift, lpdmProfile);
					if (sPaperSelection == ID_PS_TRANSPARENCY){
					}
					else {
						for (i = 0 ;LBPSType_ItmTbl[i].MediaID ; i++){
							if (LBPSType_ItmTbl[i].MediaID == sPaperType){
								if (!(LBPSType_ItmTbl[i].MediaFlag & dwSupport)){
									bPaperType = FALSE;
									break;
								}
							}
						}
					}
				}
			}
		}

		/* 100112 ISI@IST *//* MOD LIPS4_i_QA V20.25-00017 */
		if(bPaperType == TRUE){		/* 100119 ISI@IST *//* MOD LIPS4_i_QA V20.25-00051, PS_i_QA V20.25-00021, PCL_i_QA V20.25-00016 */
			bPaperType = bMsgChkPaperTypeFTTA(lpdmProfile, lpdmCurrent);
		}
//#if 1	/* 100524 ISI@IST *//* MOD LIPS4_i_QA V20.35-00043 */
	}
//#endif

//#if 1	/* 100317 ISI@IST *//* for V20.35 Dufftown */
	stExceptionSpecV101.dwStrcutVer = 0x00001001;
	stExceptionSpecV101.lpCap = lpCap;
	stExceptionSpecV101.lpPDLDevOption = lpPDLDevOption;
	
	if(TRUE == PDLCMN_bIndexPaperSelected(m_lpuift->ULIB_GetPrinterID(m_lpuift), lpPDLPaperSource->sPaperTypeID, lpPDLPaperSource->sLocalPaperType) ){
		if( FALSE == PDLCMN_bReChkExceptionSpec((LPVOID)&stExceptionSpecV101) ){
			bTABSupport = FALSE;
		}
	}
//#endif

	bManual2sided = L4_UTL_bFixManual2Sided( m_lpuift );

	if (dwfshFlags & ID_FIXEDBIN_ON){
		if (L4_UTL_bFix7BinSorterFinishing(m_lpuift))
			bOutputDest = FALSE;
	}

	if (ID_STATE3_ON == PDL_GetColor(lpCap)) {
		lpdmProfile->dmFields |= DM_COLOR;
	}
	
	/* 000417 YAO@CSL W2KLS4V4.10#00042 */
	bPaperSourceMedia = L4_UTL_bFixPaperSourceMediaPossible(m_lpuift, lpdmProfile);/* 20060425 ZYS@CSSSI [v1110][IB407] */
	
	/* 20060706 YUW@CSSSI [v1115][LIPS_v11.15-0007] Start */
	if (sPaperSelection == ID_PS_TRANSPARENCY) {
		if( PDLCMN_FunctionQuery(sPrnID, QUERY_BASE_LBP) ){
			bPaperSelectionOHP = FALSE;
			if (lpMedia) {
				for( i = 0; i < sPaperTypeNum; i++, lpMedia++ ) {
					if (lpMedia->usID == ID_MEDIA_TRANSPARENCY) {
						bPaperSelectionOHP = TRUE;
						break;
					}
				}
			}
		}
	}
	/* 20060706 YUW@CSSSI [v1115][LIPS_v11.15-0007] End */
	
	/* 20060525 ZYS@CSSSI [v1110][LIPS_v11.02-0004] Start */
	/* Set the initial values of the items on [表紙/裏表紙の設定] dialog */
	if( PDLCMN_FunctionQuery(sPrnID, QUERY_PAPE_COVER_SUPPOURT) == FALSE ){
		/* 表紙の設定 */
		if (lpPDLPaperSource->psFlags & ID_INSERTSHEET_FRONTCOVER) {
			lpPDLPaperSource->psFlags  &= ~ ID_INSERTSHEET_FRONTCOVER;	/* 表紙の設定を行う */
			lpPDLPaperSource->sFrontPrintSide	= ID_PRINTSIDE_NONE;	/* 印刷面 */
			lpPDLPaperSource->sFrontSourceID	= DMBIN_CASSETTE1;		/* 給紙部 */
		}

		/* 裏表紙の設定 */
		if (lpPDLPaperSource->psFlags & ID_INSERTSHEET_BACKCOVER) {
			lpPDLPaperSource->psFlags  &= ~ ID_INSERTSHEET_BACKCOVER;	/* 裏表紙の設定を行う */
			lpPDLPaperSource->sBackPrintSide	= ID_PRINTSIDE_NONE;	/* 印刷面 */
			lpPDLPaperSource->sBackSourceID		= DMBIN_CASSETTE1;		/* 給紙部 */
		}
	}
	/* 20060525 ZYS@CSSSI [v1110][LIPS_v11.02-0004] End */

	/* Use Skip Blank Pages Mode */
	/* 091127 WAT@IST V20.25 */
	/* 050617 KTA@IST add for v1.50 */
	if (lpPDLFinishing->fshPaperSave != ID_FSH_PAPERSAVE_DEFAULT){
		bSkipWhitePage = PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_SKIPWHITEPAGE_SUPPORT);
	}
	/* 091127 WAT@IST V20.25 */

	/* MOD BDL_Dev V20.05-00046 [1] */
	if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_SKIPWHITEPAGE_SUPPORT) == TRUE ){

//#if 0	/* 100108 WAT@IST LIPS4_i_QA V20.25-00006 */
//		/* 070309 AYA@IST BDL205-00009 */
//		if( ((dwDrvDefault & ID_DOCPROP_PRESETDD) == 0 ) && ((dwDrvDefault & ID_DOCPROP_PRESETCUSTOM) == 0 ) ){
//			if(bSkipWhitePage == TRUE){
//				if(lpPDLFinishing->fshPaperSave != ID_FSH_PAPERSAVE_OFF){
//					if ((sdocDestination & ID_DST_PAGECOMPOSER) != 0){
//						bSkipWhitePage = FALSE;
//					}
//				}
//			}
//		}
//#endif	/* 100108 WAT@IST LIPS4_i_QA V20.25-00006 */
		/* 091127 WAT@IST V20.25 */
		if (lpPDLFinishing->fshPaperSave == ID_FSH_PAPERSAVE_ON){
			bSkipWhitePage = FALSE;
		}
		/* 091127 WAT@IST V20.25 */

	}
	
	/* MOD BDL_Dev V20.05-00046 [2] */
	/* デバイスとじしろ非対応機種のみメッセージを表示 */
	if (PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_DEVICE_BINDMARGIN_UNSUPPORT) == TRUE) {	/* 090916 WAT@IST LIPS4_i_QA V20.20-00028 */
		if(sOptPriority == ID_PRIORITY_PRINTER){
			ULIB_ADM_GetLay_BindMargin(m_lpuift, lpdmProfile, &sLoBindMargin);
			if( sLoBindMargin != 0 ){
				bGutter = FALSE;
			}
		}
	}	/* 090916 WAT@IST LIPS4_i_QA V20.20-00028 */

	if(PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_PAPE_COVER_SUPPOURT)){
		if(PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_PAPE_COVER_JP_SUPPOURT)){
			bCheckCoverSupport = TRUE;
		}
		else{
			if (PCUTILS_bSupportFunction( m_lpuift, IDR_FUNC_LOCALE_USA)){
				bCheckCoverSupport = TRUE;
			}
		}
		/* 140624 CHL@SSS [v21.35][PreAlpha][Add][SimpleBoxSupport] Start */
		/* SimpleBoxサポート機種で保存モードならCoverは非サポート */
		if( (sDocDestDefault == ID_DST_STORE) && PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_BASE_SIMPLEBOXSUPPORT) ){
			if ( (lpPDLPaperSource->wInsertKind & ID_INSERTSHEET_FRONTCOVER ) || (lpPDLPaperSource->wInsertKind & ID_INSERTSHEET_BACKCOVER) ){
				bCheckCoverSupport = FALSE;
			}
		}
		/* 140624 CHL@SSS [v21.35][PreAlpha][Add][SimpleBoxSupport] End */
	}

	if(!bCheckCoverSupport){
		if (( lpPDLPaperSource->wInsertKind & ID_INSERTSHEET_FRONTCOVER ) || ( lpPDLPaperSource->wInsertKind & ID_INSERTSHEET_BACKCOVER )){
			bFrontBack = FALSE;
		}
	}
	/* 090610 ISI@IST *//* MOD BDL_Dev V20.02-00005 */
	else{
		if (( lpPDLPaperSource->wInsertKind & ID_INSERTSHEET_FRONTCOVER ) || ( lpPDLPaperSource->wInsertKind & ID_INSERTSHEET_BACKCOVER )){
			if( bMsgChkFrontBackCoverSource(m_lpuift, lpdmProfile, lpdmCurrent) == FALSE ){
				bFrontBack = FALSE;
			}
		}
	}

#if (WINVER < 0x0500) && defined(WINNT)
	/* Win2000 のお気に入りをNT40で読み込んだ際のチェック	*//* 2000/03/14 CHI	 */
	/* 丸め処理はマージでやってもらう	*/
	if (dwloFlags & (ID_PCBOOKLET_ON | ID_PCCOLLATE_ON |
								ID_PCBIND_ON | ID_PCREVERSE_ON )) {
		b2K = FALSE;
	}
	if (sCategory == ID_CATEGORY_CATALOG)
		b2K = FALSE;
	if (sCategory == ID_CATEGORY_POSTER)
		b2K = FALSE;

	ULIB_ADM_GetLay_NupPages(m_lpuift, lpdmProfile, &sloNUpPages);

	if ((sCategory == ID_CATEGORY_NUP) &&
		((sloNUpPages == ID_NUP_6PAGES) ||
		(sloNUpPages == ID_NUP_8PAGES) ||
		(sloNUpPages == ID_NUP_9PAGES) ||
		(sloNUpPages == ID_NUP_16PAGES)))
		b2K = FALSE;
	
	if (dwBkltFlags & ID_BOOKLET_DIVIDESET)
		b2K = FALSE;
	if (dwBkltFlags & ID_BOOKLET_MARGIN_ON)
		b2K = FALSE;
		
	ULIB_ADM_GetLyo_Edging(m_lpuift, lpdmProfile, &sEdging);

	if (sEdging != ID_EDGING_NONE)
		b2K = FALSE;

	ULIB_ADM_GetLyo_Date(m_lpuift, lpdmProfile, &sPrintDate);
	
	if (sPrintDate & ID_LAYOUTPOS_PRINT)
		b2K = FALSE;

	 ULIB_ADM_GetLyo_Name(m_lpuift, lpdmProfile, &sUserName);

	if (sUserName & ID_LAYOUTPOS_PRINT)
		b2K = FALSE;

	ULIB_ADM_GetLyo_Number(m_lpuift, lpdmProfile, &sPageNumber);

	if (sPageNumber & ID_LAYOUTPOS_PRINT)
		b2K = FALSE;
	if (sPaperSelection == ID_PS_ADDCOVER)
		b2K = FALSE;

	ULIB_ADM_GetQal_ColorMode(m_lpuift, lpdmProfile, &squaColor);

	if (squaColor == ID_COLOR_AUTO)
		b2K = FALSE;

	ULIB_ADM_GetQal_Cmsdata(m_lpuift, lpdmProfile,  &CMSData);
	
	if (CMSData.OBJECT[CMM_MODE_OBJ_IMAGE].tnl.MatchingOption == CMM_MATCHING_OPTION_3)
		b2K = FALSE;
	if (CMSData.OBJECT[CMM_MODE_OBJ_GRAPHICS].tnl.MatchingOption == CMM_MATCHING_OPTION_3)
		b2K = FALSE;
	if (CMSData.OBJECT[CMM_MODE_OBJ_TEXT].tnl.MatchingOption == CMM_MATCHING_OPTION_3)
		b2K = FALSE;

	if (squaGraphicMode == ID_GRAPHICMODE_RASTER){
		if (dwStampFlag & ID_STAMPPRINT_ON)
		b2K = FALSE;
	}

#endif
	
	/* Win9x v8.00 SQA59差し戻し	*/
	{
		TCHAR	szTrayName[32];

		LMEMSET(szTrayName, 0, 32);
		ULIB_ADM_GetPps_TrayName(m_lpuift, lpdmProfile, szTrayName);
		if (szTrayName[0] != 0){
			if (!PDL_IsPrinterPaperNameSupported(lpCap)){
				ULIB_ADM_SetPps_TrayName(m_lpuift, lpdmProfile, NULL);
				bTrayName = FALSE;
			}
		}
	}
	
	/* Win2K v4.00 SQA108	*/
	if (sCategory == ID_CATEGORY_NUP) {
		bNup = L4_UTL_bIsPgLayoutPossible(m_lpuift);
	}

	/* Win2K v4.10 SQA40	*//* For Canofine	*/
	if (dwQualFlag & ID_CANOFINE_ON){
		if (!L4_UTL_bIsCanoFinePossible(m_lpuift, lpdmProfile))
			bCanoFine = FALSE;
	}

	/* 001010 NON@CSL Win9x/NT/2k v8.30/4.30/4.30 SQA67 */
	if (dwQualFlag & ID_VARIT_ON){
		if (!L4_UTL_bIsVaritPossible(m_lpuift)){
			bVarit = FALSE;
		}
	}

	/*_ シフト幅 */
	ULONG	ulMaxCopies = PDL_GetMaxCopies(lpCap);
	if ( (OFFSET_MIN > lpPDLFinishing->fshCopyForOffset) ||
		 (ulMaxCopies < lpPDLFinishing->fshCopyForOffset)) {
		bOffsetCopies = FALSE;
		lpPDLFinishing->fshCopyForOffset = OFFSET_MIN;
	}
	
//#if 1	/* 091116 SAI@IST mod start for LIPS4_i_QA V20.20-00044 */
	if ( (EMBEDDOCUMENTNUMBER_MIN > lpPDLFinishing->fshEmbedDocumentNumber) ||
		 (ulMaxCopies < lpPDLFinishing->fshEmbedDocumentNumber)) {
		bEmbedDocumentNumber = FALSE;
		lpPDLFinishing->fshEmbedDocumentNumber = EMBEDDOCUMENTNUMBER_MIN;
	}
//#endif
	
	/* パネル表示文字バイト単位の入力制限 */
	/* 090827 YKA@CIS For St.Ives */
/* 140623 QNC@SSS [v21.35][PreAlpha][Modify][CPCAUnicodeExtension] */
	if ( (lpPDLFinishing->fshUserName[0] != '\0') || (lpPDLFinishing->fshUserNameUTF8[0] != '\0') ) {
		if(!(PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_USERPANEL_SUPPORT))){
			bPanelDisplay = FALSE;
		}
		else{
			HRESULT	hr;
			size_t	cCh  = 0;
			char	sPanelString[PANELUSERNAMEBUFSIZE];

			if(PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_MULTIBYTE_SUPPORT)){
				if(PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_MULTIBYTE_MAX22SUPPORT)){
/* 140623 QNC@SSS [v21.35][PreAlpha][Add][CPCAUnicodeExtension] Start */
					if (PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_USERPANEL_UTF8_SUPPORT)) {
						hr = LSTRLEN_SAFE(lpPDLFinishing->fshUserNameUTF8, PANELUSERNAMESIZE_IPx01 + 1, &cCh);
					}
					else {
/* 140623 QNC@SSS [v21.35][PreAlpha][Add][CPCAUnicodeExtension] End */
						hr = STRLEN_SAFE(lpPDLFinishing->fshUserName, PANELUSERNAMESIZE_IPx01 + 1, &cCh);
					}
					if ( FAILED(hr) ) {
						bPanelDisplay = FALSE;
					}
				}
			}else{
/* 140623 QNC@SSS [v21.35][PreAlpha][Add][CPCAUnicodeExtension] Start */
				if (PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_USERPANEL_UTF8_SUPPORT)) {
					toExANSI(sPanelString, lpPDLFinishing->fshUserNameUTF8);
					if (!PCM_LIB_bIsString((LPSTR)sPanelString, PDLUTILSTR_WBYTE_22H)) {
						bPanelDisplay = FALSE;
					}
					hr = LSTRLEN_SAFE(lpPDLFinishing->fshUserNameUTF8, PANELUSERNAMESIZEx01 + 1, &cCh);
				}
				else {
/* 140623 QNC@SSS [v21.35][PreAlpha][Add][CPCAUnicodeExtension] End */
					StringCchCopyA(sPanelString, sizeof(sPanelString), lpPDLFinishing->fshUserName);
					if (!PCM_LIB_bIsString((LPSTR)sPanelString, PDLUTILSTR_WBYTE_22H)) {
						bPanelDisplay = FALSE;
					}
					hr = STRLEN_SAFE(lpPDLFinishing->fshUserName, PANELUSERNAMESIZEx01 + 1, &cCh);
				}
				if ( FAILED(hr) ) {
					bPanelDisplay = FALSE;
				}
			}
		}
	}

	/* 20080701 WQN@CSSSI [v1240][LIPS_v12.35_Beta-0028] End */
	/* 001016 NON@CSL Win9x/NT/2k v8.30/4.30/4.30 SQA103 JpegThrough */
#if !((WINVER < 0x0500) && defined(WINNT))
	if ( sQualJpegThrough == ID_JPEGTHROUGH_ON ){
		if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_QLTY_JPEGTHROUGH_SUPPORT) ){	/* JpegThrough ON */
			if (!L4_UTL_bIsJpegThroughPossible( m_lpuift, (LPDWORD)&dwReason ,lpdmProfile)){	/* 090630 WAT@IST LIPS4_i_QA V20.10-00036 */
				bJPGThrough = FALSE;
			}
		}
		else {
			bJPGThrough = FALSE;
		}
	}
#endif	/* !((WINVER < 0x0500) && defined(WINNT)) */
	
	/*_ 020619 KBT@CSL イメージモード非対応機種 */
	if(squaGraphicMode == ID_GRAPHICMODE_RASTER){
		if(!PDL_GetIPLID(lpCap)){
			bGraphicMode = FALSE;
		}
		/*_ 030728 NON@CSL Legacy v10.20_00028 9x/PCom:OFF ---> イメージモード不可 */
#if !defined WINNT
		if (sOptPriority==ID_PRIORITY_PRINTER) {
			bGraphicMode = FALSE;
		}
#endif	/*_ !defined WINNT */
	}
	
#ifdef WINNT
	/* LIPS4 WinNT/Win2000 v4.20 SQA0017 & 0018 000518	CHI	*/
	if (dwQualFlag & ID_METAFILEUI_ON)
		bMetaFile = L4_UTL_bFixMetaFile(m_lpuift);
#endif
#ifdef PAGECOMPOSER   /* 000602 FJK@CSL*/
/* 110820 JJJ@SSS [v20.65][Alpha][Add][Drv11_BR3_V20_65_a_FamiCom-0036] Start*/
//#if 1	/* 111008 CCR@SSS [v20.65][Beta][Delete][CR0027] NupWithDeviceBooklet Rollback */
	/* 121026 YAH@SSS [v20.90][PreAlpha][Delete][CommonUI_v20.90-0004] BookletNup ProfileWarning */
	if (!(PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_PAGE_DEVICEBOOKLETANDNUP_SUPPORT)) ) {
		if((sOptPriority!=ID_PRIORITY_PRINTER)&&(sCategory == ID_CATEGORY_BOOKLET) && 
			((sloDevCategory == ID_LAYOUT_2UP) || (sloDevCategory == ID_LAYOUT_4UP)))
			bBookletNup = FALSE;
	}
//#endif
/* 110820 JJJ@SSS [v20.65][Alpha][Add][Drv11_BR3_V20_65_a_FamiCom-0036] End*/
		/*20080221 SYC@CSSSI [v1230][LIPS_v12.25-0042] Start */
		/*Whether PCOM OFF+DEVICE BOOKLETNUP could be used should check Paper Size*/
		if( (sOptPriority==ID_PRIORITY_PRINTER)&&(sCategory == ID_CATEGORY_BOOKLET) &&
			((sloDevCategory == ID_LAYOUT_2UP) || (sloDevCategory == ID_LAYOUT_4UP)) ) {

				//short			sPapFormSID = 0;	/* 100707 MAT@CIS for PREfast : del */
				
				ULIB_ADM_GetLay_OutFormSID(m_lpuift, lpdmProfile, &sPapFormSID);
				/* v20.35_Alpha [DriverStretchBooklet] [Modify] Start. */
				/* ! 100302 ZYS@SSS */
//#if 0
//				switch ( sPapFormSID ){
//				case DMPAPER_A4:
//				case DMPAPER_LETTER:
//				case DMPAPER_B5:
//				case DMPAPER_A5:
//					break;
//				default:
//					bBookletNup = FALSE;
//				}
//#else
				switch ( sPapFormSID ){
				case DMPAPER_A3:
				case DMPAPER_TABLOID:
				case DMPAPER_B4:
				case DMPAPER_A4:
					break;
				default:
					bBookletNup = FALSE;
				}

//#endif
				/* v20.35_Alpha [DriverStretchBooklet] [Modify] End. */
		}
		/*20080221 SYC@CSSSI [v1230][LIPS_v12.25-0042] End */
#endif		

#ifdef PAGECOMPOSER
	if (L4_UTL_bFixLayoutOptions(m_lpuift))
		bLayoutOpt = FALSE;
	else
		bLayoutOpt = TRUE;
#endif

#ifdef PAGECOMPOSER		/* 030808 SGY@CSL V10.20 Legacy00061 Stamp*/
	if (dwStampFlag & ID_STAMPPRINT_ON){
		if(sOptPriority == ID_PRIORITY_PRINTER)
			bStamp = FALSE;
	}
#endif

	/*_ 040430 STM@CSL 「保存＋印刷」機能削除 */
	if (sdocDestination == ID_DST_STOREPRINT){
		bDestination = FALSE;
	}
	if (dwDocFlags & ID_PROOFPRINT_ON){
		bProof = FALSE;
	}
	
	/*_ 040517 YMS@CSL 複数BOX対応 */
	/*_ v10.50以前のドライバでユーザが作成した[お気に入り]を複数BOX対応機種で読み込むと */
	/*_ [ボックス番号]が何も選択されない状態になるための対応(LIPS開発内障害票LS4-1050-022) */
	if( PDLCMN_FunctionQuery(sPrnID, QUERY_BASE_MULTIBOXSUPPORT) ){
		byCursor = PCM_LIB_byGetSelectBox(lpCDM->pcCdevmode.pdlBoxNumber.BoxNumber);
		
		/*_ devmodeに何も入っていなければメッセージを表示し初期値をセットする */
		if (!byCursor) {
			bBox = FALSE;
		}
	}

	/*_ TonerSaveMode/InkSaveMode/DraftMode _*/
	if (L4_UTL_bFixTonerSave(m_lpuift, lpdmProfile))
		bTonerSave = FALSE;

	/*_ Matching Method _*/
	ULIB_ADM_GetQal_Cmsdata(m_lpuift, lpdmProfile,  &CMSData);
	if ( CMSData.IMGOBJ.tnl.MatchingOption == CMM_MATCHING_OPTION_4 ) {
		if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_QLTY_MATCHINGMODE_PHOTO) == FALSE ) {
			bMatchingMethod = FALSE;
		}
	}

//#if 1	/* 100629 ISA@IST add start for [V20.35] Spec Change [prj-drv-pdl-common:7305] */
	if ( PDL_GetColor(lpCap) != ID_STATE3_DISABLE ) {
		if ( lpdmProfile->dmICMMethod == DMICMMETHOD_NONE ) {
			if ( CMSData.IMGOBJ.tnl.MatchingOption == CMM_MATCHING_OPTION_3 ||
				 CMSData.GRPOBJ.tnl.MatchingOption == CMM_MATCHING_OPTION_3 ||
				 CMSData.TXTOBJ.tnl.MatchingOption == CMM_MATCHING_OPTION_3 ) {
				bMatchingMethod = FALSE;
			}
		}
	}
//#endif	/* 100629 ISA@IST add end for [V20.35] Spec Change [prj-drv-pdl-common:7305] */

	/* 090710 WAT@IST LIPS4_i_QA V20.10-00047 */
	if (PDL_GetColor(lpCap) == ID_STATE3_DISABLE){
		if ((lpdmProfile->dmICMMethod == DMICMMETHOD_NONE) &&
			(CMSData.IMGOBJ.tnl.ColorCorrectionMode == CMM_COLOR_CORRECTION_MODE_OFF) &&
			(CMSData.GRPOBJ.tnl.ColorCorrectionMode == CMM_COLOR_CORRECTION_MODE_OFF) &&
			(CMSData.TXTOBJ.tnl.ColorCorrectionMode == CMM_COLOR_CORRECTION_MODE_OFF)){
			;
		}else{
			bMatchingMethod = FALSE;
		}
	}
	/* 090710 WAT@IST LIPS4_i_QA V20.10-00047 */

	/* 090731 WAT@IST PCL_i_QA V20.15-00016 */
	if(!(dwQualFlag & ID_COLORSETTING_ON)){
		if( CMUSR_iGetSupportUIColorSampleFlag( m_lpuift ) != ID_UISAMPLE_NONE ){
			CMUSR_vSetSupportUIColorSampleFlag( m_lpuift, ID_UISAMPLE_STANDARD );
		}
	}
	/* 090731 WAT@IST PCL_i_QA V20.15-00016 */

	/*_ Monitor Profile _*/
	if (L4_UTL_bFixMonitorProfile(m_lpuift, lpdmProfile))
		bMonitorProfile = FALSE;

//#if 1	/* 100318 YKA@CIS [Iona] Spec Change */
	/* 部数印字 */
	if(lpPDLFinishing->fshFlags & ID_FSH_EMBEDDOCUMENT){
		if(!(PDLCMN_FunctionQuery(sPrnID, QUERY_FINI_EMBEDDOCUMENTSUPPORT))){
			bEmbedDocNum = FALSE;
		}
		else{
			if (PDLCMN_FunctionQuery(sPrnID, QUERY_FINI_HDD_OFF_DRIVER_COLLATE)) {
				if (!(lpPDLDevOption->dwUnitFlags & ID_UNIT_EQUIP_HDD)) {
					bEmbedDocNum = FALSE;
				}
			}
		}
	}

	/* 指定部数シフト */
	if (lpPDLFinishing->fshCopyForOffset != ID_COPYFOROFFSET){
		bShiftUnit = PDLCMN_FunctionQuery(sPrnID, QUERY_FINI_SHIFTUNIT_SUPPORT);
		if (PDLCMN_FunctionQuery(sPrnID, QUERY_FINI_HDD_OFF_DRIVER_COLLATE)) {
			if (!(lpPDLDevOption->dwUnitFlags & ID_UNIT_EQUIP_HDD)) {
				bShiftUnit = FALSE;
			}
		}
	}
//#endif	/* 100318 YKA@CIS [Iona] Spec Change */
	
	/*_ CCD-DDK対応 CMNDBGMSG >>> */
	{
		/*_ Basic 4 Sheets Common */
		PUIDBGProfile(("Change Profile and Check Value START >>>"));
#if (WINVER < 0x0500) && defined(WINNT)
		PUIDBGProfile(("b2K: %d",b2K));
#endif
		PUIDBGProfile(("bDestination: %d  bProof: %d  bBox: %d",bDestination,bProof,bBox));
		/*_ PageSetting */
		PUIDBGProfile(("bOutPaper: %d  bInformSize: %d  bOutformSize: %d  bStretch: %d  bUserPaper: %d",bOutPaper,bInformSize,bOutformSize,bStretch,bUserPaper));
		PUIDBGProfile(("bNup: %d  bStamp: %d  bLayoutOpt: %d",bNup,bStamp,bLayoutOpt));
		/*_ Finishing */
		PUIDBGProfile(("bMechanicalBin: %d  bDuplex: %d  bBooklet: %d  bFnMethodOff: %d bGroup: %d  bStaple: %d bStapleGroup: %d bFinishingMethodOff: %d",bMechanicalBin,bDuplex,bBooklet,bFnMethodOff,bGroup,bStaple, bStapleGroup, bFinishingMethodOff));
		PUIDBGProfile(("bStaplePos: %d  bRotate: %d  bOffSet: %d  bJobOffset: %d  bFaceUp: %d  bFold: %d",bStaplePos,bRotate,bOffSet,bJobOffset,bFaceUp,bFold));
		PUIDBGProfile(("bMechaSort: %d  bStack: %d  bUserSep: %d  Inserter: %d",bMechaSort,bStack,bUserSep,bInserter));
		PUIDBGProfile(("bFixBin: %d  bCopy: %d  bPunch: %d  bZFold: %d  bCFold: %d bSaddleFold: %d",bFixBin,bCopy,bPunch,bZFold,bCFold, bSaddleFold));/* 20060713 ZHF@CSSSI [v1125][CFold] Add bCFold process *//* 20080706 SYC@CSSSI [v1240][MultiFunctionFold] */
		PUIDBGProfile(("bMixPaper: %d  bForbid: %d  bPanelDisplay: %d  bBookletNup: %d",bMixPaper,bForbid,bPanelDisplay,bBookletNup));
		/*_ PaperSource */
		PUIDBGProfile(("bPaperType: %d  bManual2sided: %d  bOutputDest: %d  bPaperSourceMedia: %d",bPaperType,bManual2sided,bOutputDest,bPaperSourceMedia));
		PUIDBGProfile(("bDefaultSource: %d  bFirst: %d  b2nd: %d  bLast: %d  bInterLeaf: %d bOHPSupply: %d",bDefaultSource,bFirst,b2nd,bLast,bInterLeaf,bOHPSupply));	/* 20060425 LHW@CSSSI [v1110][LIPS_v11.00-0076] bOHPSupply */
		PUIDBGProfile(("bDefaultSourcePt: %d  bFirstPt: %d  b2ndPt: %d  bLastPt: %d  bInterLeafPt: %d",bDefaultSourcePt,bFirstPt,b2ndPt,bLastPt,bInterLeafPt));
		/*_ Quality */
		/* 20071203 ZYS@CSSSI [v1225][REMOVE_SAMPLEPRINT] Start */
		PUIDBGProfile(("bColor: %d",bColor));
		/* 20071203 ZYS@CSSSI [v1225][REMOVE_SAMPLEPRINT] End */
		PUIDBGProfile(("bGraphicMode: %d  bModeDetail: %d  bReso: %d  bColorDepth: %d  bColorHalfToneObj: %d", bGraphicMode, bModeDetail, bReso, bColorDepth, bColorHalfToneObj));
		PUIDBGProfile(("bMonoHalfToneObj: %d  bHalfToning: %d  bTTSub: %d  bVarit: %d bTonerSave: %d", bMonoHalfToneObj, bHalfToning, bTTSub, bVarit, bTonerSave));
		PUIDBGProfile(("bGrayOut: %d  bJPGThrough: %d  bLineClear: %d",bGrayOut, bJPGThrough, bLineClear));
		PUIDBGProfile(("bICM: %d  bMatchingMethod: %d  bMonitorProfile: %d  bCanoFine: %d", bICM, bMatchingMethod, bMonitorProfile, bCanoFine));
		PUIDBGProfile(("b50mm: %d  bBooklet50mm: %d bQualSelection: %d bAddNumPrint: %d bletPages: %d", b50mm, bBooklet50mm, bQualSelection, bAddNumPrint, bletPages));
		PUIDBGProfile(("bOffsetCopies: %d", bOffsetCopies));
		PUIDBGProfile(("bEmbedDocumentNumber: %d", bEmbedDocumentNumber));	/* 091116 SAI@IST add for LIPS4_i_QA V20.20-00044 */
/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] Start */
//@@@ [SUPPORT] Mod Start : KTO@CSL 060220 CCDコアGR障害票 E307GCL-0011
//		PUIDBGProfile(("bOvlay: %d bPerfectBind: %D", bOvlay, bPerfectBind));
		PUIDBGProfile(("bOvlay: %d bPerfectBind: %D bDeviceBooklet: %d", bOvlay, bPerfectBind, bDeviceBooklet));
		PUIDBGProfile(("bCltnAll: %d bCltnPartly: %d", bCltnAll, bCltnPartly));	/* 20080408 MFG@CSSSI [v1235][ClearToner] */
		PUIDBGProfile(("bBodyPaper: %d bFinishingPaper: %d", bBodyPaper, bFinishingPaper));	/* 20070907 LRH@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] */
//@@@ [SUPPORT] Mod End
/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] End */
		PUIDBGProfile(("bFixMonoColor: %d", bFixMonoColor)); /* 20060303 JZH@CSSSI [v1100][MONOCOLORFIX] */
		PUIDBGProfile(("bDestinationMsg: %d", bDestinationMsg));	/* 20060511 YUW@CSSSI [v1110][v11.02_00010] */ 
		PUIDBGProfile(("bPaperSelectionOHP: %d", bPaperSelection));	/* 20060515 ZYS@CSSSI [v1110][CSSSI_LIPS_v11.10-00032] */ 
		PUIDBGProfile(("bCreepCorrection: %d", bCreepCorrection));	/* 20060717 ZYS@CSSSI [v1125][IB535] */
		PUIDBGProfile(("bFrontBack: %d", bFrontBack));	/* 20061016 ZYS@CSSSI [v1200][E314_OPTION] */
		PUIDBGProfile(("bImgRotate: %d", bImgRotate));	/* 20070413 ZYS@CSSSI [v1210][FINISHINGMODES_CFOLD] */
		PUIDBGProfile(("bOutputFace: %d", bOutputFace));/* 20070413 ZYS@CSSSI [v1210][FINISHINGMODES_CFOLD] */
		PUIDBGProfile(("bTrimWidth: %d", bTrimWidth));  /* 20070703 LYN@CSSSI [v1215][LIPS_v12.10-0032] */
		PUIDBGProfile(("bAppBooklet: %d", bAppBooklet));	/* 20070627 ZYS@CSSSI [v1215][APPLICATION_BOOKLET] */
		PUIDBGProfile(("bKurumi: %d", bCoverPaper));			/* 20070907 ZYS@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] */
		PUIDBGProfile(("bTabNoShift: %d", bTabNoShift));	/* 20070912 WQN@CSSSI [v1215][CR0278][BOXHOLDQUEUE_TABNOSHIFT] */
		PUIDBGProfile(("bSetTrimWidth: %d", bSetTrimWidth));/* 20080401 CCR@CSSSI [v1235][E314_ThreeSideTrimmer] */
		PUIDBGProfile(("bEmbedDocNum: %d", bEmbedDocNum));	/* 100318 YKA@CIS [Iona] Spec Change */
		PUIDBGProfile(("bShiftUnit: %d", bShiftUnit));		/* 100318 YKA@CIS [Iona] Spec Change */
		PUIDBGProfile(("Change Profile and Check Value END   <<< "));
	}
	/*_ CCD-DDK対応 CMNDBGMSG <<< */

	if (!bMechanicalBin || !bOutPaper || !bDuplex || !bBooklet || !bGroup || !bStaple || !bStaplePos || !bRotate|| !bOffSet ||
		!bJobOffset || !bFaceUp || !bFold || !bFixBin || !bMechaSort || !bStack || !bUserSep ||
		!bInserter || !bReso || !bCopy || !bUserPaper || !bPunch || !bZFold || !bCFold ||!bColorDepth ||/* 20060713 ZHF@CSSSI [v1125][CFold] Add bCFold process */
		!bColorHalfToneObj || !bMonoHalfToneObj || !bGrayOut || !bColor || !bLineClear || !bTTSub || !bTonerDensity ||
		!bPaperType || !bManual2sided || !bOutputDest || !bPaperSourceMedia || !bNup || !bStamp || !bCanoFine || !bVarit ||
		!bMixPaper || !bForbid || !bPanelDisplay || !bJPGThrough || !bGraphicMode || !bBookletNup || !bLayoutOpt ||
		!bDefaultSource || !bFirst || !b2nd || !bInterLeaf || !bOHPSupply || !bDefaultSourcePt || !bFirstPt || !b2ndPt || !bLastPt ||	/* 20060425 LHW@CSSSI [v1110][LIPS_v11.00-0076] bOHPSupply */
		!bInterLeafPt || !bStretch || !bHalfToning || !bModeDetail || !bSpHalfTone || !bPanelDisplay || !bLast ||
		!bInformSize  || !bOutformSize || !bDestination || !bProof || !bBox || !bICM || !bPrintFromLastPage || !bPcCollate ||
		!bPcNup || !bLoOpt || !bletFlags || !bPaperSelection /* || !bCatalog */ || !bMatchingMethod || !bMonitorProfile || !bTonerSave ||	/* 20071203 ZYS@CSSSI [v1225][REMOVE_SAMPLEPRINT] */
		!b50mm || !bBooklet50mm || !bPerfectBind || !bDevCMS || !bQualSelection || !bAddNumPrint || !bletPages || !bOffsetCopies ||
/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] Start */
//@@@ [SUPPORT] Mod Start : KTO@CSL 060220 CCDコアGR障害票 E307GCL-0011
//		!bStapleGroup || !bOvlay
		!bStapleGroup || !bFinishingMethodOff || !bOvlay || !bDeviceBooklet/* 20080701 SYC@CSSSI [v1235][LIPS_v12.35_Beta-0021] */
//@@@ [SUPPORT] Mod End
		|| !bCltnAll || !bCltnPartly	/* 20080408 MFG@CSSSI [v1235][ClearToner] */
/* 20060222 JZH@CSSSI [v1100][修正依頼034][LIPS_v11.00-0047] End */
		|| !bFixMonoColor /* 20060307 JZH@CSSSI [v1100][MONOCOLORFIX] */
/* 101115 YUW@SSS [v20.50][Alpha2][Delete][Drv11_CCD3_5_1_V20_50_a1_0_LIPS0012] Start */
//#if 0
//		|| !bDestinationMsg	/* 20060511 YUW@CSSSI [v1110][v11.02_00010] */
//#endif
/* 101115 YUW@SSS [v20.50][Alpha2][Delete][Drv11_CCD3_5_1_V20_50_a1_0_LIPS0012] End */
		|| !bPaperSelectionOHP	/* 20060515 ZYS@CSSSI [v1110][CSSSI_LIPS_v11.10-00032] */
		|| !bCreepCorrection	/* 20060717 ZYS@CSSSI [v1125][IB535] */
		|| !bFrontBack	/* 20061016 ZYS@CSSSI [v1200][E314_OPTION] */
		|| !bSecureWatermark
		|| !bTranscription /* 20070206 LYN@CSSSI [v1205][TRANSCRIPTION_MODE3] */
		|| !bImgRotate || !bOutputFace	/* 20070413 ZYS@CSSSI [v1210][FINISHINGMODES_CFOLD] */
		|| !bTrimWidth					/* 20070703 LYN@CSSSI [v1215][LIPS_v12.10-0032_] */
		|| !bAppBooklet					/* 20070627 ZYS@CSSSI [v1215][APPLICATION_BOOKLET] */
		|| !bBodyPaper					/* 20070907 LRH@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] */
		|| !bFinishingPaper				/* 20070907 LRH@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] */
		|| !bCoverPaper					/* 20070907 ZYS@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] */
		|| !bTabNoShift					/* 20070912 WQN@CSSSI [v1215][CR0278][BOXHOLDQUEUE_TABNOSHIFT] */
		|| !bMetaFile					/* 20071204 LYN@CSSSI [v1225][LIPS_v12.20-0021] */
		|| !bSetTrimWidth				/* 20080401 CCR@CSSSI [v1235][E314_ThreeSideTrimmer] */
		|| !bVFold						/* 20080614 WQN@CSSSI [v1240][VFOLD] */
		|| !bSaddleFold					/* 20080706 SYC@CSSSI [v1240][MultiFunctionFold] */
		|| !bFnMethodOff				/* 090327 SAI@IST add for LIPS4_i_QA V20.00-00090 */
		|| !bSkipWhitePage				/* 090608 ISI@IST *//* MOD BDL_Dev V20.05-00046 [1] */
	   	|| !bGutter						/* 090608 ISI@IST *//* MOD BDL_Dev V20.05-00046 [2] */
		|| !bEmbedDocumentNumber		/* 091116 SAI@IST add for LIPS4_i_QA V20.20-00044 */
		||	(bTABSupport == FALSE)		/* 100317 ISI@IST *//* for V20.35 Dufftown */
		|| !bEmbedDocNum				/* 100318 YKA@CIS [Iona] Spec Change */
		|| !bShiftUnit					/* 100318 YKA@CIS [Iona] Spec Change */
		|| !bAutoColor					/* 100527 WAT@IST ColorUpdate */
		|| !bECOStaple					/* 140612 ZMJ@SSS [v21.35][PreAlpha][Add][ECOStaple] */
#if (WINVER < 0x0500) && defined(WINNT)
		|| !b2K
#endif
		) { /* 値変更 */
		
		/* メッセージ表示 */
		CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_022);
	
	/** ここから変更開始 **/

#if !defined(WINNT)
		/*_ 050922 KBT@CSL Win9x イメージモード時はCGLフラグを立てる */
		if(squaGraphicMode == ID_GRAPHICMODE_RASTER){
			if(!(dwDocFlags & ID_CANONGDILITE_ON)){
				dwDocFlags |= ID_CANONGDILITE_ON;
				ULIB_ADM_SetDoc_Flags(m_lpuift, lpdmProfile, &dwDocFlags);
			}
		}
#endif

		/* 020711 MYS@CSL LIPS4 LegacyDriver障害票 V890_490_00027 対策 */
		/* 出力用紙サイズ固定不可 */
		if (!bOutPaper){
			/* [出力用紙サイズ]を[原稿サイズと同じ]に設定する */
			short	inpapFormSID = 0, inpapFormFID = 0;
			CDM_SIZE	sizeFormSize;
			CDM_RECT	rectFormSize;
			
			/* [原稿サイズ]に設定されている用紙の情報を取得 */
			ULIB_ADM_GetLay_InFormSID(m_lpuift, lpdmProfile, &inpapFormSID);
			ULIB_ADM_GetLay_InFormFID(m_lpuift, lpdmProfile, &inpapFormFID);
			ULIB_ADM_GetLay_InSizeFormSize( m_lpuift, lpdmProfile, &sizeFormSize );
			ULIB_ADM_GetLay_InRectFormSize( m_lpuift, lpdmProfile, &rectFormSize );
			
			/* [出力用紙サイズ]にその情報を設定する */
			ULIB_ADM_SetLay_InFormSID(m_lpuift, lpdmProfile, &inpapFormSID);
			ULIB_ADM_SetLay_InFormFID(m_lpuift, lpdmProfile, &inpapFormFID);
			ULIB_ADM_SetLay_InSizeFormSize( m_lpuift, lpdmProfile, &sizeFormSize );
			ULIB_ADM_SetLay_InRectFormSize( m_lpuift, lpdmProfile, &rectFormSize );
			
			/* 出力用紙固定のフラグを落とす */
			dwloFlags &= ~ID_OUTPAPERFIXED_ON;
			ULIB_ADM_SetLay_Flags(m_lpuift, lpdmProfile, &dwloFlags);
		}

		/*_ 031212 UCH@CSL Legacy v10.30_00093 */
		if (!bOutformSize) {
			
			LPCLIBFORM			lpFormIF = NULL;
			
			m_lpuift->ULIB_QuerySubIFforDataAccess(m_lpuift, CUISIF_FORM, NULL, (LPVOID*)&lpFormIF);

			/* 出力用紙サイズをデフォルト(A4)の用紙に丸める */
			if (lpFormIF->SearchByFormID(lpFormIF, ULIB_CAP_GetDefaultPaper(m_lpuift)) ) {

				short sOutFormSID = 0, sOutFormFID = 0;
				CDM_SIZE	sizeFormSize;
				CDM_RECT	rectFormSize;
				
				sOutFormSID	= (short)lpFormIF->GetID(lpFormIF, CLIB_FORM_ID_SYSTEM);
				sOutFormFID	= (short)lpFormIF->GetID(lpFormIF, CLIB_FORM_ID_FORMDB);
				
				ULIB_ADM_SetLay_OutFormSID(m_lpuift, lpdmProfile, &sOutFormSID);
				ULIB_ADM_SetLay_OutFormFID(m_lpuift, lpdmProfile, &sOutFormFID);

				lpFormIF->GetSize(lpFormIF, &sizeFormSize, &rectFormSize);
				
				ULIB_ADM_SetLay_OutSizeFormSize(m_lpuift, lpdmProfile, &sizeFormSize);
				ULIB_ADM_SetLay_OutRectFormSize(m_lpuift, lpdmProfile, &rectFormSize);
			}
			lpFormIF->Release(lpFormIF);
		}

		/* 050601 KTO@CSL オーバレイOFFに設定 */
		if (!bOvlay) {
			sovlMode = ID_OVERLAYMODE_NONE;
			ULIB_ADM_SetOvl_Mode(m_lpuift, lpdmProfile, &sovlMode);
		}

		/* 20080408 MFG@CSSSI [v1235][ClearToner] Start */
		if (!bCltnAll) {
			lpCDM->pcCdevmode.pdlPageSetup.pageClearTonerMode = ID_PDLCLEARTONER_NONE;
		}

		if (!bCltnPartly) {
			lpCDM->pcCdevmode.pdlPageSetup.pageClearTonerMode = ID_PDLCLEARTONER_NONE;
		}
		/* 20080408 MFG@CSSSI [v1235][ClearToner] End */

		/* 製本不可	*/
		if (!bBooklet)		/* 991005 SZK@CSL */
		{
			short sPSelectionSet = 0;
			/* 051202 KTO@CSL Lプリンタ障害票L_FSh00002対応 製本不可時お気に入りの[給紙方法]を丸める */
			ULIB_ADM_GetPps_PaperSelection(m_lpuift, lpdmProfile, &sPSelectionSet);
			if (sPSelectionSet == ID_PS_ADDCOVER) {
				sPSelectionSet = ID_PS_ALLPAGES;

				ULIB_ADM_SetPps_PaperSelection(m_lpuift, lpdmProfile, &sPSelectionSet);
			}
			lpdmProfile->dmDuplex = DMDUP_SIMPLEX;
			dwloFlags &= ~ID_2SIDEDPRINT_ON;

			ULIB_ADM_SetLay_Flags(m_lpuift, lpdmProfile, &dwloFlags);
		}
		
		if (!bletFlags) {
			dwBkltFlags &= ~ID_BOOKLET_DIVIDESET;
			dwBkltFlags &= ~ID_BOOKLET_MARGIN_ON;
			ULIB_ADM_SetBok_letFlags(m_lpuift, lpdmProfile, &dwBkltFlags);
		}
		
		if (!bBooklet || !bNup){
			sCategory = ID_CATEGORY_NORMAL;
			
			ULIB_ADM_SetLay_Category(m_lpuift, lpdm, &sCategory);
		}
		
		/*_ とじしろ */
		if (!b50mm)	ULIB_ADM_SetLay_BindMargin(m_lpuift, lpdmProfile, &sMaxBindMarginMm);
		
		/*_ 製本とじしろ */
		if (!bBooklet50mm) {
			sMaxBindMarginMm *= 10;
			ULIB_ADM_SetBok_letBindMargin(m_lpuift, lpdmProfile, &sMaxBindMarginMm);
		}
		
		/* 回転不可能の場合、DEVMODEもOFFにする */
		/* ステイプル不可能の場合、ソートにする */
		if(!bRotate){
			dwfshFlags &= ~ID_ROTATE_ON;
		}
		
		if (!bFnMethodOff) {
			DWORD		dwInitFinishings = 0L;
			ULIB_ADM_GetFin_Flags(m_lpuift, lpDefdm, &dwInitFinishings);
			
			dwfshFlags |= ( dwInitFinishings & ID_SORT_ON );
		}
		if (!bGroup){
			DWORD		dwInitFinishings = 0L;

			ULIB_ADM_GetFin_Flags(m_lpuift, lpDefdm, &dwInitFinishings);
			
			dwfshFlags &= ~ID_GROUP_ON;
			dwfshFlags |= ( dwInitFinishings & ID_SORT_ON );
		}
		
		if(!bStapleGroup){
			DWORD		dwInitFinishings = 0L;

			ULIB_ADM_GetFin_Flags(m_lpuift, lpDefdm, &dwInitFinishings);
			
			dwfshFlags &= ~(ID_STAPLE_ON | ID_GROUP_ON);
			dwfshFlags |= ( dwInitFinishings & ID_SORT_ON );
		}
		
		if(!bStaple){
			DWORD		dwInitFinishings = 0L;

			ULIB_ADM_GetFin_Flags(m_lpuift, lpDefdm, &dwInitFinishings);
			/*_ ステイプルが無効でステイプルの設定になっていた場合は、デフォルトの設定値をセットする */
			dwfshFlags &= ~(ID_SORT_ON | ID_STAPLE_ON);
			dwfshFlags |= ( dwInitFinishings & ID_SORT_ON );
		}

		/* 140612 ZMJ@SSS [v21.35][PreAlpha][Delete][CommonUI_v21.35-0021][ECOStaple] Start */
		/* 140612 ZMJ@SSS [v21.35][PreAlpha][Add][ECOStaple] Start */
		//if(!bECOStaple){
			///* 140612 ZMJ@SSS [v21.35][PreAlpha][Add][CommonUI_v21.35-0021][ECOStaple] Start */
			//DWORD		dwInitFinishings = 0L;

			//ULIB_ADM_GetFin_Flags(m_lpuift, lpDefdm, &dwInitFinishings);
			///*_ ECOステイプルが無効でステイプルの設定になっていた場合は、デフォルトの設定値をセットする */
			//dwfshFlags &= ~(ID_SORT_ON | ID_STAPLE_ON);
			//dwfshFlags |= ( dwInitFinishings & ID_SORT_ON );
			///* 140612 ZMJ@SSS [v21.35][PreAlpha][Add][CommonUI_v21.35-0021][ECOStaple] End */
		//	lpPDLFinishing->fshFlags2 &= ~ID_FSH_ECOSTAPLE_ON;
		//}
		/* 140612 ZMJ@SSS [v21.35][PreAlpha][Add][ECOStaple] End */
		/* 140612 ZMJ@SSS [v21.35][PreAlpha][Delete][CommonUI_v21.35-0021][ECOStaple] End */

		/* パンチ・Z折り	*/
		if (!bPunch){
			dwfshFlags &= ~ID_PUNCH_ON;
		}
		if (!bZFold){
			dwfshFlags &= ~ID_ZFOLD_ON;
		}
		
		/* C折り	*//* 20060713 ZHF@CSSSI [v1125][CFold] */
		if (!bCFold){
			lpPDLFinishing->fshFlags &= ~ID_FSH_CFOLD_ON;
			lpPDLFinishing->fshFlags2 &= ~(ID_FSH_FOUR_ON|ID_FSH_ACCORDION_ON);
		}

/* 20080706 SYC@CSSSI [v1240][MultiFunctionFold] Start */
		if (!bSaddleFold) {
			lpPDLFinishing->fshFlags &= ~ID_SADDLEFOLD_ON;
		}
/* 20080706 SYC@CSSSI [v1240][MultiFunctionFold] End */

		/* 余白0mm */
		/*_ 全デバイスで余白0mm対応されているために不要に _*/
/*		if (!bMiniMargin){
			lpCDM->pdlDeviceSetting.devFlags &= ~ID_MINIMIZEMARGIN_ON;
		}*/
		
		if (!bFixBin){
			if (dwRsnFixBin & CMRE_NODEVICE){		/* FixBin Unsupport Device */
				sfshFixBin = ID_FIXBIN_AUTO;
				dwfshFlags &= ~ID_FIXEDBIN_ON;
				ULIB_ADM_SetFin_Flags(m_lpuift, lpdm, &dwfshFlags);
				ULIB_ADM_SetFin_FixbinNo(m_lpuift, lpdm, &sfshFixBin);
			}else{
				L4_UTL_bFix7BinSorterFinishing (m_lpuift);
			}
		}
		
		if (!bMechaSort){
			DWORD		dwInitFinishings = 0L;

			ULIB_ADM_GetFin_Flags(m_lpuift, lpDefdm, &dwInitFinishings);
			
			dwfshFlags &= ~ID_MECHANICAL_ON;
			dwfshFlags |= (dwInitFinishings & ID_MECHANICAL_ON);
		}

		if (!bStack){
			DWORD		dwInitFinishings = 0L;
			
			ULIB_ADM_GetFin_Flags(m_lpuift, lpDefdm, &dwInitFinishings);

			dwfshFlags &= ~ID_BINSTACK_ON;
			dwfshFlags |= (dwInitFinishings & ID_MECHANICAL_ON);
		}
		
		if (!bUserSep){
			DWORD		dwInitFinishings = 0L;
			
			ULIB_ADM_GetFin_Flags(m_lpuift, lpDefdm, &dwInitFinishings);
			
			dwfshFlags &= ~ID_BINUSERSEPARATE_ON;
			dwfshFlags |= (dwInitFinishings & ID_BINUSERSEPARATE_ON);
		}
		
		if (!bJobOffset){
			dwfshFlags &= ~ID_JOBOFFSET_ON;
		}
		
		if (!bFaceUp){
			dwfshFlags &= ~ID_FACEUP_ON;
		}
		
		/*_ 040610 UCH@CSL 最終ページから印刷 */
		if (!bPrintFromLastPage) {
			dwfshFlags &= ~ID_REVERSE_ON;
		}
		if (!bRotate || !bStaple || !bPunch || !bZFold || !bGroup || !bMechaSort || !bStack || !bUserSep
			|| !bJobOffset || !bFaceUp || !bStapleGroup || !bFnMethodOff  || !bECOStaple){	/* 140612 ZMJ@SSS [v21.35][PreAlpha][Add][CommonUI_v21.35-0021][ECOStaple] */
			
			ULIB_ADM_SetFin_Flags(m_lpuift, lpdmProfile, &dwfshFlags);
		}
		
		/* 中とじ不可能の場合、DEVMODEもOFFにする */
		if(!bFold){
			dwBkltFlags &= ~ID_CENTERFOLD_ON;
			ULIB_ADM_SetBok_letFlags(m_lpuift, lpdmProfile, &dwBkltFlags);
		}

		/* 20080614 WQN@CSSSI [v1240][VFOLD] Start */
		/* V折り不可の場合、DEVMODEもOFFにする */
		if (!bVFold) {
			lpCDM->pcCdevmode.pdlFinishing.fshFlags &= ~ID_VFOLD_ON;
			lpCDM->pcCdevmode.pdlFinishing.fshFlags2 &= ~ID_FSH_SADDLE_ON;	/* 090107 ISI@IST *//* MOD LIPSDEV_v20.00_0008 [1] */
		}
		/* 20080614 WQN@CSSSI [v1240][VFOLD] End */
		/* 000515 SZK@CSL PaperType Conflict */
		if (!bPaperType){
			sPaperType = ID_MEDIA_PLANE;
			ULIB_ADM_SetPps_PaperType(m_lpuift, lpdmProfile, &sPaperType);
			
			/* 011108 FJK@CSL Legacy障害票V860_460_00025 */
			if(PDL_IsPaperTypeSupported(lpCap)){
				if (sPaperSelection == ID_PS_TRANSPARENCY) {
					if ( !(L4_LIB_bCheckMediaType1(dwAvailMediaType1Array, (SHORT)ID_MEDIA_TRANSPARENCY)) ) {
						sPaperSelection = ID_PS_ALLPAGES;
						ULIB_ADM_SetPps_PaperSelection(m_lpuift, lpdmProfile, &sPaperSelection);
					}
				}
			}
		}

		/* 20060515 ZYS@CSSSI [v1110][CSSSI_LIPS_v11.10-00032] Start */
		if (!bPaperSelectionOHP) {
			short sPSelectionSet = 0;
			sPSelectionSet = ID_PS_ALLPAGES;
			ULIB_ADM_SetPps_PaperSelection(m_lpuift, lpdmProfile, &sPSelectionSet);

			lpdmProfile->dmDefaultSource = DMBIN_AUTO;
		}
		/* 20060515 ZYS@CSSSI [v1110][CSSSI_LIPS_v11.10-00032] End */

		/* 拡縮	*/
		if( !bStretch ){
			ULIB_ADM_GetLay_PrivScale(m_lpuift, lpdmProfile, &sloStretch);
			ULIB_ADM_SetLay_PrivScale(m_lpuift, lpdm, &sloStretch);
		}
		
		if( !bInserter ){
			dwPaperFlag &= ~ID_COVERINSERTER_ON;
			ULIB_ADM_SetPps_Flags(m_lpuift, lpdmProfile, &dwPaperFlag);
		}


		if( !bDuplex ){		/* 991005 SZK@CSL 両面不可の場合、loFlagsのみ丸める */
			lpdmProfile->dmDuplex = DMDUP_SIMPLEX;
			dwloFlags &= ~ID_2SIDEDPRINT_ON;
			ULIB_ADM_SetLay_Flags(m_lpuift, lpdmProfile, &dwloFlags);
		}
	/* 110106 XHD@SSS [v20.50][Beta1][Delete][Drv11_CCD3_5_1_V20_50_b1_0_LIPS-0005] Start */	
		//if( !bPerfectBind ){
		//	lpCDM->pcCdevmode.pdlLayout.loFlags &= ~ID_LAYOUT_PERFECTBIND;
			//lpdmProfile->dmDuplex = DMDUP_SIMPLEX;
			//dwloFlags &= ~ID_2SIDEDPRINT_ON;
			//ULIB_ADM_SetLay_Flags(m_lpuift, lpdmProfile, &dwloFlags);
		//}
	/* 110106 XHD@SSS [v20.50][Beta1][Delete][Drv11_CCD3_5_1_V20_50_b1_0_LIPS-0005] End */

		/* 20070907 LRH@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] Start */
		if (!bBodyPaper) {
			lpCDM->pcCdevmode.pdlFinishing.fshPBInPaper = DMPAPER_A4;
		}

		if (!bFinishingPaper) {
			lpCDM->pcCdevmode.pdlFinishing.fshPBOutPaper = DMPAPER_A4;
		}
		/* 20070907 LRH@CSSSI [v1220][CSSSI_LIPS_v12.15-00007] End */
		/* 部数	*/
		if( !bCopy ){
			lpdmProfile->dmCopies = lpDefdm->dmCopies;
		}

		if (!bGraphicMode) {
			squaGraphicMode = ID_GRAPHICMODE_PDL;
			ULIB_ADM_SetQal_GraphicsMode(m_lpuift, lpdmProfile, &squaGraphicMode);
		}

		if (!bLineClear)
			lpPDLQuality->quaRenderingFlags &= ~ID_LINECLEAR_ON;

		/* 解像度 */
		if( !bReso ){
			sQualPrivQuality = ID_RESO_600;
#ifdef WINNT
			lpdm->dmPrintQuality = ID_RESO_600;
#else
			lpdm->dmPrintQuality = DMRES_HIGH + PDL_GetDefaultResoShift(lpCap);
#endif
			ULIB_ADM_SetQal_PrivQuality(m_lpuift, lpdmProfile, &sQualPrivQuality);

		}

		if (!bICM) {
#if !defined (WINNT)
		/*_ 040607 NON@CSL Color Gear Lite対応(Lプリンタ)---ICMモード非サポート */
			dwQualFlag &= ~ID_ICM_ON;
			ULIB_ADM_SetQal_Flags(m_lpuift, lpdmProfile, &dwQualFlag);
			
			lpdm->dmICMMethod = DMICMMETHOD_NONE;
			lpdm->dmFields &= ~ DM_ICMMETHOD;
			
#endif
#if defined(WINNT) && (WINVER >= 0x0500)
			lpdm->dmICMMethod = DMICMMETHOD_NONE;
			lpdm->dmFields &= ~ DM_ICMMETHOD;
#endif
		}

		if (!bCanoFine){
			dwQualFlag &= ~(ID_CANOFINE_ON | ID_CANOFINE_IMAGEALL | ID_CANOFINE_VIVID);
			ULIB_ADM_SetQal_Flags(m_lpuift, lpdmProfile, &dwQualFlag);
		}
		
		/* 001010 NON@CSL Win9x/NT/2k v8.30/4.30/4.30 SQA00067 VARIT */	
		if (!bVarit){
			if(dwQualFlag & ID_VARIT_ON){
				dwQualFlag &= ~ID_VARIT_ON;
				ULIB_ADM_SetQal_Flags(m_lpuift, lpdmProfile, &dwQualFlag);
			}
		}

		/* 001016 NON@CSL Win9x/NT/2k v8.30/4.30/4.30 SQA103 JpegThrough */
		if (!bJPGThrough){
			sQualJpegThrough = ID_JPEGTHROUGH_AUTO;
			ULIB_ADM_SetQal_JPEGThrough	(m_lpuift, lpdmProfile, &sQualJpegThrough);
		}

		/* 030808 SGY@CSL V10.20 Legacy00061 Stamp*/
#ifdef PAGECOMPOSER
		if (!bStamp){
			dwStampFlag &= ~ID_STAMPPRINT_ON;
			dwloFlags &= ~ID_PCSTAMP_ON;
			ULIB_ADM_SetStm_Flags(m_lpuift, lpdmProfile, &dwStampFlag );
			ULIB_ADM_SetLay_Flags(m_lpuift, lpdmProfile, &dwloFlags);
		}
		
		if (!bPcCollate){
			dwloFlags &= ~ID_PCCOLLATE_ON;
			ULIB_ADM_SetLay_Flags(m_lpuift, lpdmProfile, &dwloFlags);
		}
#endif
		/*_ 040430 STM@CSL 「保存＋印刷」機能削除 */
		if (!bDestination){
			sdocDestination = ID_DST_PRINTER;
			ULIB_ADM_SetDoc_Destination(m_lpuift, lpdmProfile, &sdocDestination);
		}
/* 101115 YUW@SSS [v20.50][Alpha2][Delete][Drv11_CCD3_5_1_V20_50_a1_0_LIPS0012] Start */
/* Move down */
//#if 0
//		/* 20060511 YUW@CSSSI [v1110][v11.02_00010] Start */
//		if (!bDestinationMsg){
//			CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_035);
//			sdocDestination = ID_DST_PRINTER;
//			ULIB_ADM_SetDoc_Destination(m_lpuift, lpdmProfile, &sdocDestination);
//		}
//		/* 20060511 YUW@CSSSI [v1110][v11.02_00010] End */
//#endif
/* 101115 YUW@SSS [v20.50][Alpha2][Delete][Drv11_CCD3_5_1_V20_50_a1_0_LIPS0012] End */		
		if (!bProof){
			dwDocFlags &= ~ID_PROOFPRINT_ON;
			ULIB_ADM_SetDoc_Flags(m_lpuift, lpdmProfile, &dwDocFlags);
		}
		
		/*_ 040517 YMS@CSL 複数BOX対応 */
		/*_ devmodeに何も入っていなければ初期値をセットする */
		if (!bBox) {
			lpCDM->pcCdevmode.pdlBoxNumber.BoxNumber[0] |= 0x01;
		}
		
		/*_ 041214 KMK@CSL NTでのパネル優先がデフォルトの機種対応 */
		if (!bGrayOut){
			squaGrayOut = ID_GRAYOUT_TEXTGRAPHIC;
			ULIB_ADM_SetQal_Grayout(m_lpuift, lpdmProfile, &squaGrayOut);
		}
		
	} /* 値変更 */

	/* 101115 YUW@SSS [v20.50][Alpha2][Add][Drv11_CCD3_5_1_V20_50_a1_0_LIPS0012] Start */
	/* 20060511 YUW@CSSSI [v1110][v11.02_00010] Start */
	if (!bDestinationMsg){
		CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_035);
		sdocDestination = ID_DST_PRINTER;
		ULIB_ADM_SetDoc_Destination(m_lpuift, lpdmProfile, &sdocDestination);
	}
	/* 20060511 YUW@CSSSI [v1110][v11.02_00010] End */
	/* 101115 YUW@SSS [v20.50][Alpha2][Add][Drv11_CCD3_5_1_V20_50_a1_0_LIPS0012] End */	

	/* v10.00_00121 TT置換/TypeWing */
	if (dwQualFlag & ID_SUBFONT_ON) {
		lpPDLQuality->quaRenderingFlags &= ~ID_SETTTFONT_OFF;
	} else {
		lpPDLQuality->quaRenderingFlags |= ID_SETTTFONT_OFF;
	}
	if (dwQualFlag & ID_FONTCACHE_ON) {
		lpPDLQuality->quaRenderingFlags &= ~ID_SETTYPEWING_OFF;
	} else {
		lpPDLQuality->quaRenderingFlags |= ID_SETTYPEWING_OFF;
	}
	/* 050811 KTO@CSL E230-0002仕様変更 */
	if (!bDevCMS)
		lpPDLQuality->quaRenderingFlags &= ~ID_DEVICECMS_ON;

	/* 20060303 JZH@CSSSI [v1100][MONOCOLORFIX] Start */
	if (!bFixMonoColor){
		lpPDLFinishing->fshFlags &= ~ID_FIXEDCOLORMODE_ON;
	}
	/* 20060303 JZH@CSSSI [v1100][MONOCOLORFIX] End */

	/* 20060425 LHW@CSSSI [v1110][LIPS_v11.00-0076] Start */
	if(!bOHPSupply){
		sOHPSupply = DMBIN_MANUAL;
		ULIB_ADM_SetPps_OHP(m_lpuift, lpdmProfile, &sOHPSupply);
	}
	/* 20060425 LHW@CSSSI [v1110][LIPS_v11.00-0076] End */

	/* 101022 GPG@SSS [v20.50][Alpha][Add][CommonUI_v20.50_0070] Start */
	if(!PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_PAPE_FEEDBYMEDIA)){
		if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_BASE_DELETE_PS_CFG_SEET) == FALSE ) {
			/*if(sPaperSelection == ID_PS_ALLPAGES){*/	/* 101111 CHL@SSS [v20.50][Alpha][Add][CommonUI_v20.50_0078] */ /* 110111 JJJ@SSS [v20.50][Alpha][Delete][Drv11_CCD3_5_1_V20_50_b1_0_LIPS-0004] */
				short	sQuaGraphicMode = 0;
				ULIB_ADM_GetQal_GraphicsMode(m_lpuift, lpdmProfile, &sQuaGraphicMode);
				ULIB_ADM_GetPps_PaperType(m_lpuift, lpdmProfile, &sPaperType);
				if((sPapFormSID != DMPAPER_A4) &&
					(sPapFormSID != DMPAPER_LETTER)){
						if(sPaperType == ID_MEDIA_TRANSPARENCY){
							/* デフォルトの用紙タイプにまるめる */
							sPaperType = (BYTE)PDLCMN_bGetDefaultLBPPaperType(m_lpuift->ULIB_GetPrinterID(m_lpuift), sQuaGraphicMode);
							ULIB_ADM_SetPps_PaperType(m_lpuift, lpdmProfile, &sPaperType);
						}
				}
				if(bIsEnvelope(sPapFormSID) == TRUE){
					if(sPaperType != ID_MEDIA_ENVELOPE2){
						sPaperType = ID_MEDIA_ENVELOPE;
						ULIB_ADM_SetPps_PaperType(m_lpuift, lpdmProfile, &sPaperType);
					}
				}
				else if (bIsPostCard(sPapFormSID) == FALSE){
					if((sPaperType == ID_MEDIA_ENVELOPE) ||
						(sPaperType == ID_MEDIA_ENVELOPE2) || 
						(sPaperType == ID_MEDIA_POSTCARD)){
							switch(m_lpuift->ULIB_GetPrinterID(m_lpuift)){
							case IDM_LBP6650:
							case IDM_LBP8630:
							case IDM_LBP6700:
							case IDM_LBP6750:
							/* 110627 MFG@SSS [v20.65][PreAlpha][Add][Brighton] Start */
							case IDM_LBP9650C:
							case IDM_LBP9660C:	/* 130116 JJJ@SSS [v21.00][PreAlpha][Add][Nukualofa] */
							/* 110627 MFG@SSS [v20.65][PreAlpha][Add][Brighton] End */
							case IDM_LBP6710: /* 120203 XIA@SSS [v20.80][PreAlpha][Add][AP_2080_3-HamptonCourt][AddNewDevice_HamptonCourt] */
							/* 130117 ZMJ@SSS [v21.00][PreAlpha][Add][AddNewDevice_Turnberry] Start */
							case IDM_LBP8730:
							case IDM_LBP8710:
							/* 130117 ZMJ@SSS [v21.00][PreAlpha][Add][AddNewDevice_Turnberry] End */
	//						case IDM_LBP6780: /* 120203 XIA@SSS [v20.80][PreAlpha][Add][AP_2080_3-HamptonCourt][AddNewDevice_HamptonCourt] */
							case IDM_LBP351:/* 150310 DWY@SSS [v21.55][PreAlpha][Add][AddNewDevice_Yeran] */
								if (lpdmProfile->dmDefaultSource != DMBIN_MANUAL) {
									sPaperType = PDLCMN_bGetDefaultLBPPaperType(m_lpuift->ULIB_GetPrinterID(m_lpuift), sQuaGraphicMode);
									ULIB_ADM_SetPps_PaperType(m_lpuift, lpdmProfile, &sPaperType);
								}
								break;	
							default:
								break;
							}
					}
				}
		    /*}*/   /* 110111 JJJ@SSS [v20.50][Alpha][Delete][Drv11_CCD3_5_1_V20_50_b1_0_LIPS-0004] */
		}
	}
	/* 101022 GPG@SSS [v20.50][Alpha][Add][CommonUI_v20.50_0070] End */

	PUIDBGProfileOUT(("L4CmBsProc::bMessageCheckProfile() bRet=%d", bRet));
	PUIDBGProcOUT(("L4CmBsProc::bMessageCheckProfile() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]	bMsgChkFrontBackCoverSource
;[Action]
;[Format]
;[Input]
;[Output]
;BaseCording: 090610 ISI@IST 	MOD BDL_Dev V20.02-00005 Ref. PCSvMulti_PaperSourceTable::bAction(), 同期をとること。
********************************************************************/
BOOL L4CmBsProc::bMsgChkFrontBackCoverSource(LPUIINTERFACETABLE lpuift, LPDEVMODE lpdmProfile, LPDEVMODE lpdmCurrent)
{
	BOOL					bRet = FALSE;
	UINT					uCnt = 0, uCnt2 = 0, uCnt3 = 0, uNum = 0, uCurIndx = 0;
	short					sID = 0, sFormID = 0, sSystemID = 0;
	CmSv					*lpCmSv = NULL;
	CmSvMulti				*lpCmSvMulti = NULL;
	CmSvConflict			*lpCmSvCnf = NULL;
	LPSVLISTITEM			lpList = NULL;
	unsigned short			ushDefNumPS = 0, ushNumPSOpt = 0;
	LPCAP_PAPERSOURCE		lpPs = NULL;
	LPCAP_EXPPAPERSOURCE	lpPap = NULL;
	LPPDL_DEVICEOPTION		lpPDLDevOption = NULL;
	LPSHAREDO				lpShareDO = NULL;
	LPDEVMODE				lpdm = NULL;
	short					loCategory	= 0;
	short					optPriority	= 0;
	BOOL					bLocaleUSA = FALSE;
	PCSvMulti_PaperSourceTable	*pPCSvMulti_PSTbl = NULL;		/* 090610 ISI@IST */
	LPPDL_PAPERSOURCE		lpPdlPaperSource = NULL;
	int						nSrcCnt = 0, nSrcCntMax = 0;
	short					sCoverSource = 0;
	BOOL					bSetFrontAndBack = FALSE;
	short					sTMP_OutPaper = 0;
	CmSvCB_OutPutSize		*pCmSvCB_OutPutSize = NULL;
	CmSv					*lpCmSvOutPaper = NULL;
	
	lpdm			= CMUSR_lpGetCurDevmode(lpuift);		/* Get DEVMODE passed from application or system */

    LPPDL_PC_CDEVMODE lpPDLDevMode = GetPcDmPointer(((LPSHAREDEV)lpdmProfile));
    /* lpPdlFinishingは使用していない */

	lpPdlPaperSource = (LPPDL_PAPERSOURCE)&lpPDLDevMode->pdlPaperSource;
	
	int 	i = 0; /* @@@Canon 020924 CHO@CSI E307_D_PSR-0138 start */
	int		j = 0;	/* 030226 KOZ@NCT add for PS210K-103, V210R-019 */
	int		k = 0;	/* 030226 KOZ@NCT add for PS210K-103, V210R-019 */
	
	PUIDBGSv(("> BDLCmBsProc::bMsgChkFrontBackCoverSource\n"));

	lpCmSvCnf = CMSVUSR_lpGetConflict(m_lpuift);
	if(lpCmSvCnf == NULL){
		goto _EXIT;
	}
	
	lpCmSv = lpCmSvCnf->lpGetCmSv(IDSV_NCTRL_PAPERSOURCETABLE);
	if( lpCmSv == NULL ){
		goto _EXIT;
	}
	pPCSvMulti_PSTbl = dynamic_cast<PCSvMulti_PaperSourceTable *>(lpCmSv);

	if( (( lpPdlPaperSource->wInsertKind & ID_INSERTSHEET_FRONTCOVER ) == ID_INSERTSHEET_FRONTCOVER )	&&
		(( lpPdlPaperSource->wInsertKind & ID_INSERTSHEET_BACKCOVER ) == ID_INSERTSHEET_BACKCOVER	)		){
		
		bSetFrontAndBack = TRUE;
		nSrcCntMax = 2;
		sCoverSource = lpPdlPaperSource->sFrontSourceID;
		
	}else if(	(( lpPdlPaperSource->wInsertKind & ID_INSERTSHEET_FRONTCOVER ) == ID_INSERTSHEET_FRONTCOVER )	&&
				(( lpPdlPaperSource->wInsertKind & ID_INSERTSHEET_BACKCOVER ) != ID_INSERTSHEET_BACKCOVER	)		){
		nSrcCntMax = 1;
		sCoverSource = lpPdlPaperSource->sFrontSourceID;
	
	}else if(	(( lpPdlPaperSource->wInsertKind & ID_INSERTSHEET_FRONTCOVER ) != ID_INSERTSHEET_FRONTCOVER )	&&
				(( lpPdlPaperSource->wInsertKind & ID_INSERTSHEET_BACKCOVER ) == ID_INSERTSHEET_BACKCOVER	)		){
		nSrcCntMax = 1;
		sCoverSource = lpPdlPaperSource->sBackSourceID;
	}

	for( nSrcCnt = 0; nSrcCnt < nSrcCntMax; nSrcCnt++ ){
		
		if( (bSetFrontAndBack == TRUE) && (nSrcCnt == 1) ){
			sCoverSource = lpPdlPaperSource->sBackSourceID;
		}
	
		/* Get the Paper Source ID */
		for ( uCnt = 0; uCnt < pPCSvMulti_PSTbl->uNumOfItems; uCnt++ ) {
			if( pPCSvMulti_PSTbl->lpSvListItems[uCnt].uitmData == sCoverSource /* lpPdlPaperSource->sFrontSourceID */ ){
			/*	sID			= (short)lpSvListItems[uCnt].uitmData; */
				sID = (short)( pPCSvMulti_PSTbl->lpSvListItems[uCnt].uitmData );
				uCurIndx	= uCnt;
				bRet		= TRUE;
				break;
			}
		}

		/* "Insetionunit"はリストにないので、ここは通します*/
		switch( sCoverSource /* lpPdlPaperSource->sFrontSourceID */){
		case ID_DMBIN_MULTIINSERTER:
		case ID_DMBIN_MULTIINSERTER1:
		case ID_DMBIN_MULTIINSERTER2:
		case ID_DMBIN_MULTIINSERTER3:
			bRet = TRUE;
			break;
			
		default:	/*_ 何もしない */
			break;
		}

		if ( bRet == FALSE ){
			goto _EXIT;
		}

		bRet = FALSE; /* initialize */

		lpCmSv		= lpCmSvCnf->lpGetCmSv(IDSV_NCTRL_FORMTABLE);
		lpCmSvMulti	= dynamic_cast<CmSvMulti *>(lpCmSv);

		uNum	= lpCmSvMulti->uNumOfItems;
		lpList	= lpCmSvMulti->lpSvListItems;

		/* 090610 ISI@IST *//* lpArgv[2] = $TMP_OutPaper, ルールの内容を実装 */
		lpCmSvOutPaper = lpCmSvCnf->lpGetCmSv(IDSV_LAYOUT_OUTPAPER);
		pCmSvCB_OutPutSize = dynamic_cast<CmSvCB_OutPutSize *>(lpCmSvOutPaper);
		if ( strcmp(pCmSvCB_OutPutSize->lpSvListItems[pCmSvCB_OutPutSize->uValue].lpName, "DRAFT") == 0 ){
			sTMP_OutPaper = lpdmCurrent->dmPaperSize;
			switch( lpdmCurrent->dmPaperSize ){
			case DMPAPER_A0:
			case DMPAPER_A1:
			case DMPAPER_A2:
				sTMP_OutPaper = DMPAPER_A3;
				break;
				
			case DMPAPER_B1:
			case DMPAPER_B2:
			case DMPAPER_B3:
				sTMP_OutPaper = DMPAPER_B4;
				break;
			
			default:
				break;
			}
		}else{
			sTMP_OutPaper = pCmSvCB_OutPutSize->lpSvListItems[pCmSvCB_OutPutSize->uValue].uitmData;
			/* A0等はどうするのだろう？ */
		}

		/* Get the Paper Size ID (Form ID) */
		for ( uCnt = 0; uCnt < uNum; uCnt++ ) {
			/* if ( strcmp(lpArgv[2], lpList[uCnt].lpName) == 0 ) { */
			if(lpList[uCnt].uitmData == sTMP_OutPaper ){
				sFormID	= (INT)lpList[uCnt].uitmData;
				bRet	= TRUE;
				break;
			}
		}

		if ( bRet == FALSE ){
			goto _EXIT;
		}

		bRet = FALSE; /* initialize */

		/* 090610 ISI@IST lpShareDOはカレントで問題なし */
		lpShareDO		= CMUSR_lpGetCurDevoption(lpuift);	/* Get SHAREDO passed from application or system */
		lpPDLDevOption	= GetPdlDoPointer(lpShareDO);

		/* Default Paper Feed Step */
		ushDefNumPS = ULIB_CAP_GetNumPaperSource(lpuift);
		lpPs		= ULIB_CAP_GetPaperSourceInfo(lpuift);

		/* Optional Paper Feed Step */
		ushNumPSOpt = ULIB_CAP_GetNumExpPaperSource(lpuift);
		lpPap		= ULIB_CAP_GetExpPaperSourceInfo(lpuift);
		
		/* convert a FormID to a SystemID */
		sSystemID = sGetSystemIDbyFormID(lpuift, sFormID);
		/* 040514 KOZ@NCT add start */
		/* 用紙サイズが見つからない場合、FALSEを返します */
		if(sSystemID == 0){
			goto _EXIT;
		}
		/* 040514 KOZ@NCT add end */

		/* デバイス製本の場合、サイズが2倍になるので、ここで変換する。以下以外のサイズは、デバイス製本不可 */
		ULIB_ADM_GetLay_Category(lpuift, lpdm, &loCategory);
		ULIB_ACDO_GetOpt_Priority(lpuift, &optPriority);
		bLocaleUSA = PCUTILS_bSupportFunction(lpuift, IDR_FUNC_LOCALE_USA);
		
		/* v20.35_Alpha [DriverStretchBooklet] [Modify] ! 100224 ZYS@SSS */
		/* 100824 ZXP@SSS [v20.50][PreAlpha][STRETCH_DEVICE_BOOKLET][Modify] change function support flag */ 
		/*if ( PDLCMN_FunctionQuery(lpuift->ULIB_GetPrinterID(lpuift), QUERY_FINI_DEVICEBOOKLET_PAGESIZE_CONFLICT) == FALSE) {*/
		if ( PDLCMN_FunctionQuery(lpuift->ULIB_GetPrinterID(lpuift), QUERY_FINI_BOOKLETPRINTPROCESS_SUPPORT) == FALSE) {
			if ( ( loCategory == ID_CATEGORY_BOOKLET ) &&
				( optPriority == ID_PRIORITY_PRINTER)){
				sSystemID = PDLCMN_shConvertPaperSizeForDeviceBooklet(sSystemID, bLocaleUSA);
			}
		}

		/* 090221 SAI@IST for LIPS_v20.00_Alpha-0167 start */
		if( /* lpPdlPaperSource->sFrontSourceID */sCoverSource == ID_DMBIN_MULTIINSERTER ){
		/* if( strcmp(lpArgv[1], "INSERTIONUNIT") == 0 ) { */
			switch(lpuift->ULIB_GetPrinterID(lpuift)){
				case IDM_iRADVC7055:
				case IDM_iRADVC7000:	/* 090420 ISI@IST *//* mod E220_Esp1.1_D_PCL-0007 */
				case IDM_iRADVC7270:	/* 120208 GPG@SSS [v20.80][PreAlpha][Add][AddNewDevice_Chesterfield] */
					for(i = 0; Inserter_SuppourtSize_7055[i] != 0; i++){
						if( Inserter_SuppourtSize_7055[i] == (unsigned short)sSystemID ){
							bRet = TRUE;
							goto _CHECKUSER;		/* 090525 IKM@IST mod for PCL_i_QA_V20.02-00010, _EXIT to _CHECKUSER */
						}
					}
					break;
				case IDM_iRADV6075:
				case IDM_iRADV6065:
				case IDM_iRADV6000:
				case IDM_iRADV600A:	/* 090911 ISA@IST for add PCL EFI */
				/* 120220 CHL@SSS [v20.80][PreAlpha][Add][AddNewDevice_Bedford] Start */
				case IDM_iRADV6275:
				case IDM_iRADV6265:
				/* 120220 CHL@SSS [v20.80][PreAlpha][Add][AddNewDevice_Bedford] End */
					/* 130903 JJJ@SSS [v21.45][PreAlpha][Add][AddNewDevice_Ewloe] Start */
				case IDM_iRADV6575:
				case IDM_iRADV6565:
					/* 130903 JJJ@SSS [v21.45][PreAlpha][Add][AddNewDevice_Ewloe] End */
					for(i = 0; Inserter_SuppourtSize_Greater[i] != 0; i++){
						if( Inserter_SuppourtSize_Greater[i] == (unsigned short)sSystemID ){
							bRet = TRUE;
							goto _CHECKUSER;
						}
					}
					break;
				default:
					break;
			}

		}else if((/* lpPdlPaperSource->sFrontSourceID */sCoverSource == ID_DMBIN_MULTIINSERTER1)	||
				 (/* lpPdlPaperSource->sFrontSourceID */sCoverSource == ID_DMBIN_MULTIINSERTER2)	||
				 (/* lpPdlPaperSource->sFrontSourceID */sCoverSource == ID_DMBIN_MULTIINSERTER3)		){
				UINT *pSupportSizeTbl = NULL;
				
				switch(lpuift->ULIB_GetPrinterID(lpuift)){
				case IDM_iRADVC9065:
				case IDM_iRADVC9060:
				case IDM_iRADVC9000:	/* 090420 ISI@IST *//* mod E220_Esp1.1_D_PCL-0007 */
				case IDM_iRADV8105:
				case IDM_iRADV8095:
				case IDM_iRADV8000:
				/* 120220 CHL@SSS [v20.80][PreAlpha][Add][AddNewDevice_Bedford] Start */
				case IDM_iRADV8205:
				case IDM_iRADV8295:
				/* 120220 CHL@SSS [v20.80][PreAlpha][Add][AddNewDevice_Bedford] End */
				/* 130903 JJJ@SSS [v21.45][PreAlpha][Add][AddNewDevice_Ewloe] Start */
				case IDM_iRADV8505:
				case IDM_iRADV8595:
				/* 130903 JJJ@SSS [v21.45][PreAlpha][Add][AddNewDevice_Ewloe] End */
				case IDM_iRADVC9280:	/* 120208 GPG@SSS [v20.80][PreAlpha][Add][AddNewDevice_Chesterfield] */
				case IDM_iPRC800:     /* 130121 YAH@SSS [v21.00][PreAlpha][Add][AddNewDevice_Bath] */
					for(i = 0; Inserter12_SuppourtSize_9065[i] != 0; i++){
						if( Inserter12_SuppourtSize_9065[i] == (unsigned short)sSystemID ){
							bRet = TRUE;
							goto _CHECKUSER;		/* 090525 IKM@IST mod for PCL_i_QA_V20.02-00010, _EXIT to _CHECKUSER */
						}
					}
					break;
				case IDM_iPR1110:
				case IDM_iPRSJ100:	/* 090420 ISI@IST *//* mod E220_Esp1.1_D_PCL-0007 */
					for(i = 0; Inserter123_SuppourtSize_1110[i] != 0; i++){
						if( Inserter123_SuppourtSize_1110[i] == (unsigned short)sSystemID ){
							bRet = TRUE;
							goto _CHECKUSER;		/* 090525 IKM@IST mod for PCL_i_QA_V20.02-00010, _EXIT to _CHECKUSER */
						}
					}
					break;
//#if 1	/* 100316 KIS@CIS [V20.35] for Nottingham */
				case IDM_iPRC7010VP:
				case IDM_iPRC6010VP:
				case IDM_iPRC6010:
					if (sCoverSource == ID_DMBIN_MULTIINSERTER1) {
						pSupportSizeTbl = Inserter1_SuppourtSize_Nottingham;
					} else if (sCoverSource == ID_DMBIN_MULTIINSERTER2) {
						pSupportSizeTbl = Inserter2_SuppourtSize_Nottingham;
					}
					
					if (pSupportSizeTbl != NULL) {
						for(i = 0; pSupportSizeTbl[i] != 0; i++){
							if( pSupportSizeTbl[i] == (unsigned short)sSystemID ){
								bRet = TRUE;
								goto _CHECKUSER;
							}
						}
					}
					break;
//#endif	/* 100316 KIS@CIS [V20.35] for Nottingham */
				default:
					break;
				}
		}
		/* 090221 SAI@IST for LIPS_v20.00_Alpha-0167 end */

		/* 030226 KOZ@NCT add start for PS210K-103, V210R-019 */
		/* カセット１かつ封筒フィーダ付き。	*/
		/* このような差し替える給紙段についてはオプション・デフォルト給紙段より先にチェックする	*/
		if( lpPDLDevOption->dwPaperSource & ID_PS_ENVELOPEFEEDER ){
			/* 090219 IKM@IST add start for Sanahin Envelope Feeder 2 */
			short sPaperSourceID = DMBIN_CASSETTE2;
			
			if ( PDLCMN_FunctionQuery(lpuift->ULIB_GetPrinterID(lpuift), QUERY_DEVC_ENVELOPEFEEDER_1) ) {
				sPaperSourceID = DMBIN_CASSETTE1;
			}
			/* 090219 IKM@IST add end for Sanahin Envelope Feeder 2 */
			if( sID == sPaperSourceID || sID == DMBIN_AUTO){
				/* オプション給紙段	*/
				if( ushNumPSOpt ){
					for( k = 0; k < ushNumPSOpt; k++ ){
						for( i = 0; i < lpPap[k].usNumPapersource; i++ ){
							if( lpPap[k].pPaperSource[i].usID == (unsigned short)DMBIN_ENVELOPE ){
								for( j = 0; j < lpPap[k].pPaperSource[i].usNumPaper_1; j++ ){
									if( lpPap[k].pPaperSource[i].pPaper_1[j].usID == (unsigned short)sSystemID ){
										bRet = TRUE;
										goto _EXIT;
									}
								}
							}
						}
					}
				}
				if( sID == sPaperSourceID ){
					bRet = FALSE;	/* カセット１かつ封筒フィーダなのに用紙IDが一致しなかった	*/
					goto _EXIT;					
				}
			}
		}
		/* 030226 KOZ@NCT add end for PS210K-103, V210R-019 */

		for ( uCnt = 0; uCnt < ushDefNumPS; uCnt++, lpPs++ ) {
			if ( (unsigned short)sID == lpPs->usID ) {
				break;
			}
		}

		for ( uCnt = 0; uCnt < lpPs->usNumPaper_1; uCnt++ ) {

			if ( lpPs->pPaper_1[uCnt].usID == (unsigned short)sSystemID ) {	/* 050920 KOZ@IST modify for E250_SQ_D_BDL-0003 sFormID -> sSystemID */

				bRet = TRUE;

				goto _CHECKUSER;		/* 090525 IKM@IST mod for PCL_i_QA_V20.02-00010, _EXIT to _CHECKUSER */
			}
		}

		/* @@@Canon 020930 CHO@CSI E307_D_PSR-0138 start */
		if (sSystemID == DMPAPER_USER) {
			for (i = 0; lpPDLDevOption->stPaperSourceinfo[i].sPaperSourceID != 0; i++) { 
				if ( (lpPDLDevOption->stPaperSourceinfo[i].sPaperSizeID == PDLDMPAPER_LOCALSTANDARED) 
	/* 050128 TAM@IST MOD */
					|| ( (lpPDLDevOption->dwDevFuncFlags & ID_DEVFUNC_CHINESE_PAPER) &&	/* K紙サポートバージョン*/
					     ((lpPDLDevOption->stPaperSourceinfo[i].sPaperSizeID == DMPAPER_GBT_8K) ||	/* 8K または 16K */
					     (lpPDLDevOption->stPaperSourceinfo[i].sPaperSizeID == DMPAPER_GBT_16K)) ) ) {
	/* 050128 TAM@IST MOD */

					if( (lpPDLDevOption->stPaperSourceinfo[i].sPaperSourceID == sID )	||
						(sID == pPCSvMulti_PSTbl->lpSvListItems[0].uitmData			)			){
						bRet = TRUE;
						goto _EXIT;
					}
				}
			}
		}
		/* @@@Canon 020930 CHO@CSI E307_D_PSR-0138 end */
			
		if ( ushNumPSOpt ) {
			for ( uCnt = 0; uCnt < ushNumPSOpt; uCnt++ ) {
				for ( uCnt2 = 0; uCnt2 < lpPap[uCnt].usNumPapersource; uCnt2++ ) {
					if ( lpPap[uCnt].pPaperSource[uCnt2].usID == (unsigned short)sID ) {
						for ( uCnt3 = 0; uCnt3 < lpPap[uCnt].pPaperSource[uCnt2].usNumPaper_1; uCnt3++ ) {
							if ( lpPap[uCnt].pPaperSource[uCnt2].pPaper_1[uCnt3].usID == (unsigned short)sSystemID ) {
								if( LSTRCMP(pPCSvMulti_PSTbl->lpSvListItems[uCurIndx].lpStr, lpPap[uCnt].pPaperSource[uCnt2].szName) == 0 ) {
									bRet = TRUE;

									goto _CHECKUSER;		/* 090525 IKM@IST mod for PCL_i_QA_V20.02-00010, _EXIT to _CHECKUSER */
								}
							}
						}
					}
				}
			}
		}

	}	/* 	for( nSrcCnt = 0; nSrcCnt < nSrcCntMax; nSrcCnt++ ) */

/* 090525 IKM@IST add start for PCL_i_QA_V20.02-00010 */
_CHECKUSER:
	/* ユーザー定義用紙だった場合は長尺紙のチェックを行う。但し手差しではチェックしない。 */
	if( sSystemID == DMPAPER_USER && sID != DMBIN_MANUAL ){
		
		LPCLIBFORM		lpFormIF = NULL;
		SHORT			sMinW = 0, sMaxW = 0, sMinH = 0;
		BOOL			bFind = FALSE;
		CDM_SIZE32		size32;
		CDM_RECT32		rect32;
		
		PDLCMN_vGetLongPaperLIMIT( lpuift->ULIB_GetPrinterID(lpuift), &sMinW, &sMaxW, &sMinH );
		lpuift->ULIB_QuerySubIFforDataAccess(lpuift, CUISIF_FORM, NULL, (LPVOID*)&lpFormIF);
		
		/* チェック対象の用紙サイズの情報を取得 */
		lpFormIF->EnumStart(lpFormIF);
		do {
			if( sFormID ==lpFormIF->GetID(lpFormIF, CLIB_FORM_ID_FORMDB) ){
				bFind = TRUE;
				break;
			}
		} while (lpFormIF->EnumNext(lpFormIF));
		
		/* 長尺紙の判定 */
		if( bFind == TRUE ){	/* 090610 ISI@IST *//* mod typemis "=" -> "=="  */
			lpFormIF->GetSize32(lpFormIF, &size32, &rect32);
			
			if( (LONG)size32.cy >= sMinH ){
				bRet = FALSE;
			}
		}
		
		/* メモリー解放 */
		if (lpFormIF) {
			lpFormIF->Release(lpFormIF);
		}
	}
/* 090525 IKM@IST add end for PCL_i_QA_V20.02-00010 */

_EXIT:

	PUIDBGSv(("< BDLCmBsProc::bMsgChkFrontBackCoverSource\n"));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
; bMsgPrf_PaperTypeFTTA
;[Action]
;[Format]
; BOOL FAR PASCAL bMsgPrf_PaperTypeFTTA(LPUIINTERFACETABLE lpuift)
;[Input]
;[Output]
;[Return]
;[Memo]
;	coding : 100112 ISI@IST MOD LIPS4_i_QA V20.25-00017
********************************************************************/
BOOL L4CmBsProc::bMsgChkPaperTypeFTTA(LPDEVMODE lpdmProfile, LPDEVMODE lpdmCurrent)
{
	LPCAP_PRINTERCAP		lpCap = NULL;
	short					sPaperSelection = 0;
	BOOL					bPaperType = /* FALSE */TRUE;	/* 100119 ISI@IST *//* MOD LIPS4_i_QA V20.25-00051 */
	LPPDL_DEVICEOPTION		lpPDLDevOption = NULL;
	SHORT					shPrinterID = 0;
	BYTE					bySelNum = 0;
	short					sPaperType = 0;
	LPPDL_PAPERSOURCE		lpPdlPaperSource = NULL;
	LPPDL_CDEVMODE			lpCDM = NULL;
	BOOL					bIgnorePaperSelection = FALSE;	/* 140808 CHL@SSS [v21.40][PreAlpha][Add][UsabilityUp4SelectPaperType] */

	lpPDLDevOption = GetPdlDoPointer(m_lpuift->ULIB_GetDeviceOption(m_lpuift));
	shPrinterID = (short)m_lpuift->ULIB_GetPrinterID(m_lpuift);
	lpCap = (LPCAP_PRINTERCAP)m_lpuift->ULIB_GetPrinterCap(m_lpuift);
	if ( lpCap == NULL ) {		/* 100715 JUN@CIS for prefast */
		return FALSE;
	}
	
	lpCDM				= GetPdlDmPointer(((LPSHAREDEV)lpdmProfile));
	lpPdlPaperSource	= &(lpCDM->pcCdevmode.pdlPaperSource);
	
	ULIB_ADM_GetPps_PaperSelection(m_lpuift, lpdmProfile, &sPaperSelection);

	/* 140808 CHL@SSS [v21.40][PreAlpha][Add][UsabilityUp4SelectPaperType] Start */
	if(PDLCMN_FunctionQuery(shPrinterID, QUERY_PAPE_MFP_PSPT_SETSAMETIME)){
		if(sPaperSelection == ID_PS_ALLPAGES){
			bIgnorePaperSelection = TRUE;
		}
	}
	/* 140808 CHL@SSS [v21.40][PreAlpha][Add][UsabilityUp4SelectPaperType] End */

	if( (PDLCMN_FunctionQuery(shPrinterID, QUERY_PAPE_FEEDBYMEDIA)) == TRUE ){
		if(lpPdlPaperSource->psFlags & PAPERSRC_MEDIA){
			switch (sPaperSelection){
			case ID_PS_ALLPAGES:
			case ID_PS_INSERTSHEET:
				bPaperType = PDLCMN_bIsSelectedMediaType(lpPDLDevOption->stAvailableMedia.byAvailMFArray,
									lpPdlPaperSource->sPaperTypeID, lpPdlPaperSource->sLocalPaperType);
				break;
			case ID_PS_FIRSTLAST:
				bPaperType = PDLCMN_bIsSelectedMediaType(lpPDLDevOption->stAvailableMedia.byAvailMFArray,
									lpPdlPaperSource->s1stMediaID, lpPdlPaperSource->sLocal1stMedia);
				
				if ( bPaperType ) {
					bPaperType = PDLCMN_bIsSelectedMediaType(lpPDLDevOption->stAvailableMedia.byAvailMFArray,
										lpPdlPaperSource->sLastMediaID, lpPdlPaperSource->sLocalLastMedia);
				}
				
				if ( bPaperType ) {
					bPaperType = PDLCMN_bIsSelectedMediaType(lpPDLDevOption->stAvailableMedia.byAvailMFArray,
										lpPdlPaperSource->sPaperTypeID, lpPdlPaperSource->sLocalPaperType);
				}
				break;
			case ID_PS_FIRST2NDLAST:
				bPaperType = PDLCMN_bIsSelectedMediaType(lpPDLDevOption->stAvailableMedia.byAvailMFArray,
									lpPdlPaperSource->s1stMediaID, lpPdlPaperSource->sLocal1stMedia);
				
				if ( bPaperType ) {
					bPaperType = PDLCMN_bIsSelectedMediaType(lpPDLDevOption->stAvailableMedia.byAvailMFArray,
										lpPdlPaperSource->s2ndMediaID, lpPdlPaperSource->sLocal2ndMedia);
				}
				
				if ( bPaperType ) {
					bPaperType = PDLCMN_bIsSelectedMediaType(lpPDLDevOption->stAvailableMedia.byAvailMFArray,
										lpPdlPaperSource->sLastMediaID, lpPdlPaperSource->sLocalLastMedia);
				}
				
				if ( bPaperType ) {
					bPaperType = PDLCMN_bIsSelectedMediaType(lpPDLDevOption->stAvailableMedia.byAvailMFArray,
										lpPdlPaperSource->sPaperTypeID, lpPdlPaperSource->sLocalPaperType);
				}
				break;
			case ID_PS_ADDCOVER:
				bPaperType = PDLCMN_bIsSelectedMediaType(lpPDLDevOption->stAvailableMedia.byAvailMFArray,
									lpPdlPaperSource->s1stMediaID, lpPdlPaperSource->sLocal1stMedia);
				
				if ( bPaperType ) {
					bPaperType = PDLCMN_bIsSelectedMediaType(lpPDLDevOption->stAvailableMedia.byAvailMFArray,
										lpPdlPaperSource->sPaperTypeID, lpPdlPaperSource->sLocalPaperType);
				}
				break;
			case ID_PS_TRANSPARENCY:
				/* 用紙銘柄非対応機種のみ、FTTAでOHPフィルムが設定されているかを見る */
				if ( PDLCMN_FunctionQuery(shPrinterID, QUERY_PAPE_MEDIALIBRARY_SUPPORT) == FALSE ) {
					bySelNum = (BYTE)PDLCMN_nGetMediaType1Idx(ID_MEDIA_TRANSPARENCY);
					if ( PDLCMN_bCheckBitOtherArray2(lpPDLDevOption->stAvailableMedia.byAvailMFArray, bySelNum) == FALSE ) {
						/* bTransparency = FALSE; */
					}
				}
				bPaperType = PDLCMN_bIsSelectedMediaType(lpPDLDevOption->stAvailableMedia.byAvailMFArray,
									lpPdlPaperSource->sInterLeafMediaID, lpPdlPaperSource->sLocalInterLeaf);
				break;
			/* 030501 KOZ@NCT modify end */
			}
		}

		/* 140808 CHL@SSS [v21.40][PreAlpha][Add][UsabilityUp4SelectPaperType] Start */
		if(bIgnorePaperSelection){
			bPaperType = PDLCMN_bIsSelectedMediaType(lpPDLDevOption->stAvailableMedia.byAvailMFArray,
				lpPdlPaperSource->sPaperTypeID, lpPdlPaperSource->sLocalPaperType);

			if(lpPdlPaperSource->sPaperTypeID == ID_MEDIA_PRINTERDEFAULT){
				bPaperType = TRUE;
			}
		}
		/* 140808 CHL@SSS [v21.40][PreAlpha][Add][UsabilityUp4SelectPaperType] End */
	}
	/* LBP機の場合、用紙タイプは常に表示されますので、CAP情報より取得いたします。*/
	else{
		/* CAP情報から用紙タイプのサポート状況を求める処理を関数化しました。*/
		ULIB_ADM_GetPps_PaperType(m_lpuift, lpdmProfile, &sPaperType);

		if( bCheckPaperTypeAvailable(lpCap, sPaperType) == FALSE){
			bPaperType = FALSE;
		}
	}
	
	return bPaperType;

}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/07/12:0.00:
********************************************************************/
VOID L4SvBsProc::vUpdateTooltip()
{
	HWND			hCtrl_job = NULL, hCtrl_pcom = NULL;
	CmSvConflict	*lpCmSvConflict = NULL;
	short			docDst = 0;
	UINT			uStrID = 0;

	PUIDBGSvIN(("L4SvBsProc::vUpdateTooltip()"));

	lpCmSvConflict = CMSVUSR_lpGetConflict(m_lpuift);
	if (lpCmSvConflict == NULL) {
		return;
	}
	
	hCtrl_job	= GetDlgItem(m_hDlg, IDC_PB_JOBOPERATIONS_DETAILSSETTING);
	hCtrl_pcom	= GetDlgItem(m_hDlg, IDC_PB_CHANGE_PAGECOMPOSER);

	ULIB_ADM_GetDoc_Destination(m_lpuift, m_lpdm, &docDst);


	/* 20080620 ZDL@CSSSI [v1300][UI Extension] Start */
	if (CMUSR_iGetSupportUI2lineFlag(m_lpuift) == CM_UIORIGINAL){
		/* 20080620 ZDL@CSSSI [v1300][UI Extension] End */
		switch (docDst) {
		case ID_DST_SECUREDPRINT:
			uStrID = IDR_STR_TTP_SECUREDBUTTON;
			break;
		case ID_DST_STORE:
		case ID_DST_STOREPRINT:
		/*case ID_DST_HOLDQUEUE:		*//* 20060824 LYN@CSSSI [E314][HoldQueue] */
			uStrID = IDR_STR_TTP_MAILBOXSETUP;
			break;
		case ID_DST_PAGECOMPOSER:
			uStrID = IDR_STR_TTP_LOCKBUTTON;
			break;
		/* 20061027 LYN@CSSSI [CSSSI_LIPS_v12.00-00004] Start */
		case ID_DST_HOLDQUEUE:
			uStrID = IDR_STR_TOOLTIP_HOLDBUTTON;
			break;
		/* 20061027 LYN@CSSSI [CSSSI_LIPS_v12.00-00004] End */
		}
	/* 20080620 ZDL@CSSSI [v1300][UI Extension] Start */
	}
	/* 20080620 ZDL@CSSSI [v1300][UI Extension] End */

	if (uStrID) {
		m_lpTooltip->TTM_vSetString    (hCtrl_job,  uStrID);
		m_lpTooltip->TTM_vEnableTooltip(hCtrl_job,  TRUE);
		m_lpTooltip->TTM_vSetString    (hCtrl_pcom, uStrID);
		m_lpTooltip->TTM_vEnableTooltip(hCtrl_pcom, TRUE);
	}
	else {
		m_lpTooltip->TTM_vEnableTooltip(hCtrl_job, FALSE);
		m_lpTooltip->TTM_vEnableTooltip(hCtrl_pcom, FALSE);
	}

	PUIDBGSvOUT(("L4SvBsProc::vUpdateTooltip()"));

}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]基本4シート の OnApply で処理されるシート非依存の処理
;[Format]
;[Input]
;[Output]
;cording:03/01/29:0.00:YMS@CSL:CCD3
;[Memo] M2Y@CSL This class used by the Finishing class
********************************************************************/
BOOL L4SvBsProc::UTL_CheckSettingsOnApply()
{
	short			sPaperSize = 0;
	short			sCategory = 0;
	short			sNUpPages = 0;
	DWORD			fshFlags = 0L;
	LPPDL_DEVICEOPTION	lpPdldo = NULL;
	LPPDL_CDEVMODE		lpPdlDevMode = NULL;

	PUIDBGSvIN(("L4SvBsProc::UTL_CheckSettingsOnApply()"));

	lpPdldo = GetPdlDoPointer(m_lpuift->ULIB_GetDeviceOption(m_lpuift));
	lpPdlDevMode = GetPdlDmPointer(((LPSHAREDEV)m_lpdm));

	if((!lpPdldo) || (!lpPdlDevMode)){
		PUIDBGSvOUT(("L4SvBsProc::UTL_CheckSettingsOnApply() ADDRESS ERROR return FALSE L.%d", __LINE__));
		return FALSE;
	}

	ULIB_ADM_GetLay_OutFormSID(m_lpuift, m_lpdm, &sPaperSize);
	ULIB_ADM_GetLay_Category(m_lpuift, m_lpdm, &sCategory);
	ULIB_ADM_GetLay_NupPages(m_lpuift, m_lpdm, &sNUpPages);

	ULIB_ADM_GetFin_Flags(m_lpuift, m_lpdm, &fshFlags);

	PUIDBGSvOUT(("L4SvBsProc::UTL_CheckSettingsOnApply() return TRUE"));

	return TRUE;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:02/10/04:0.00:ITO:CCD3 [PCL6.00/4.00SDK_00004]
********************************************************************/
VOID L4SvBsProc::vForciblyDisableControls()
{
	PUIDBGSvIN(("L4SvBsProc::vForciblyDisableControls()"));

	/* Basic Class */
	L4SvProc::vForciblyDisableControls();

	if (!m_bForciblyDisable) {
		PUIDBGSvOUT(("L4SvBsProc::vForciblyDisableControls (NOP) return"));
		return;
	}

	PUIDBGSvOUT(("L4SvBsProc::vForciblyDisableControls()"));
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:04/12/20:0.00:ITO:CCD3 [PCL6.00/4.00SDK_00004]
********************************************************************/
VOID L4SvBsProc::vForciblyHideControls()
{
	PUIDBGSvIN(("L4SvBsProc::vForciblyHideControls()"));

	HWND	hDlgItem = GetDlgItem(m_hDlg, IDC_CB_JOBOPERATIONS);

	if (hDlgItem) {

		int iItemCnt = ComboBox_GetCount(hDlgItem);

		if (iItemCnt > 1) {
			int		idx	= 0;
			int		iCurSel = 0;
			LPSVLISTITEM	lpList	= NULL;
			WORD		wItem	= 0;

			idx = (iItemCnt - 1);

			for (; idx >= 0; idx --) {

				if (CB_ERR == (LRESULT)(lpList = (LPSVLISTITEM)ComboBox_GetItemData(hDlgItem, idx))) {
					break;
				}

				if (lpList == NULL){
					break;
				}

				switch (lpList->uitmData) {
				case ID_DST_PRINTER:		wItem = CDM_UI_ACED_JOBPRINT;		break;
				case ID_DST_SECUREDPRINT:	wItem = CDM_UI_SECURED;				break;
				case ID_DST_STORE:			wItem = CDM_UI_JOBSTORE;			break;	
				case ID_DST_PROMOTE:		wItem = CDM_UI_PROMOTE;				break;
				case ID_DST_PAGECOMPOSER:	wItem = CDM_UI_JOBEDITANDPREVIEW;	break;
				case ID_DST_INTRPT:			wItem = CDM_UI_INTERRUPT;			break;
				case ID_DST_PRIORITY:		wItem = CDM_UI_PROMOTE;				break;
				case ID_DST_HOLDQUEUE:		wItem = CDM_UI_HOLDQUEUE;			break;				/* 20060822 LYN@CSSSI [E314][HoldQueue] */
				default:					wItem = 0; 							break;
				}

				if (wItem) {

					if (UTL_bIsVisibleDocPropItem(m_lpuift, wItem) == FALSE) {
						
						/* アイテムの抜き取り */
						ComboBox_DeleteString(hDlgItem, idx);

						/* カレント選択が無くなってしまった場合、先頭を選択する */
						iCurSel = ComboBox_GetCurSel(hDlgItem);
						if (iCurSel < 0) ComboBox_SetCurSel(hDlgItem, 0);
					}
				}
			}
			
		}
	}

	/* Basic Class */
	L4SvProc::vForciblyHideControls();

	PUIDBGSvOUT(("L4SvBsProc::vForciblyHideControls()"));
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:03/04/07:0.00:NON@CSL
********************************************************************/
BOOL L4SvBsProc::bForciblyOnCommand()
{
	BOOL	bRet = FALSE;

	PUIDBGSvIN(("L4SvBsProc::bForciblyOnCommand()"));

	switch (m_wCtrlID) {
	case IDC_CB_JOBOPERATIONS:
		bRet = CMSV_bForciblyOnCommandComboBox(m_lpuift, m_hDlg, SID_UNSPECIFIED, m_wCtrlID, bIsEnableItemOnOpenDocProp);
		break;
	}

	PUIDBGSvOUT(("L4SvBsProc::bForciblyOnCommand() bRet=%d", bRet));

	return bRet;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:03/04/07:0.00:NON@CSL
********************************************************************/
BOOL L4SvBsProc::bForciblyOnDraw()
{
	BOOL	bRet = FALSE;

	PUIDBGSvIN(("L4SvBsProc::bForciblyOnDraw()"));

	switch (m_wCtrlID) {
	case IDC_CB_JOBOPERATIONS:
		bRet = CMSV_bForciblyOnDrawItemComboBox(m_lpuift, m_hDlg, SID_UNSPECIFIED, m_wCtrlID, bIsEnableItemOnOpenDocProp, m_lpdis);
		break;
	}

	PUIDBGSvOUT(("L4SvBsProc::bForciblyOnDraw() bRet=%d", bRet));

	return bRet;
}

/* 20120514 XIA@SSS [v20.85][PreAlpha][NF_2085_1-BasicSheet][Add] Start */
/******************************* <-68->   1tab = 4space ************
;[Name]	L4SvBsProc::CheckPageSheetData
;[Action]	
;[Format]	L4SvBsProc::CheckPageSheetData()
;[Output]	
;cording:03/01/15:0.00:EBI@CSL:CCD3
********************************************************************/
short L4SvBsProc::CheckPageSheetData()
{
	short				errItem = 0, shInFormFID = 0;
	int					iScaling = -1;
	int					nVal = 0, nMin = MINSCALE_25, nMax = MAXSCALE_200;
	BOOL				bErr = FALSE;
	UINT				uMsgID = 0;
	LPCAP_PRINTERCAP	lpCap = NULL;
	CmSvConflict*		lpCnf = NULL;
	/* 120131 XIA@SSS [v20.75][Alpha][CR0014][DriverSort] Start */
	LPPDL_DEVICEOPTION	lpPDLDevOption = GetPdlDoPointer(m_lpdo);
	/* 120131 XIA@SSS [v20.75][Alpha][CR0014][DriverSort] End */

	PUIDBGProcIN(("L4SvBsProc::CheckPageSheetData()"));

	lpCap	= (LPCAP_PRINTERCAP)(m_lpuift->ULIB_GetPrinterCap(m_lpuift));
	if ( lpCap == NULL ) {		/* 100715 JUN@CIS for prefast */
		return 0;
	}
	
	lpCnf	= CMSVUSR_lpGetConflict(m_lpuift);
	ULIB_ADM_GetLay_InFormFID(m_lpuift, m_lpdm, &shInFormFID);

	/*_ IDC_ED_COPY		[部数]に不正値が入っていないか */
	if (lpCnf->CNF_bIsSvVisible(m_lpuift, IDSV_DEVMODE_COPY)){
		nVal = GetDlgItemInt( m_hDlg, IDC_ED_COPY, &bErr, TRUE );

		if( (bErr == FALSE) || (nVal < 1) || ( nVal > (short)PDL_GetMaxCopies(lpCap) ) ){
			errItem = IDC_ED_COPY;

			CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_092);
			SetFocus( GetDlgItem( m_hDlg, IDC_ED_COPY ));
			SendDlgItemMessage( m_hDlg, errItem, EM_SETSEL, 0, MAKELONG(-1, -1) );

			PUIDBGProc(("Error %d", __LINE__));
			PUIDBGProcOUT(("L4SvBsProc::CheckPageSheetData() errItem=%d", errItem));
			return errItem;
		}
		
		/* 120131 XIA@SSS [v20.75][Alpha][CR0014][DriverSort] Start */
		if ( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_HDDOFF_MULTIBINDER4COLLATE) &&
			((lpPDLDevOption->dwUnitFlags & ID_UNIT_EQUIP_HDD) != ID_UNIT_EQUIP_HDD)  &&
			(nVal >= MULTIBINDER_SUPPORT_MAX_400) ) {
				errItem = IDC_ED_COPY;

				CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_264);
				SetFocus( GetDlgItem( m_hDlg, IDC_ED_COPY ));
				SendDlgItemMessage( m_hDlg, errItem, EM_SETSEL, 0, MAKELONG(-1, -1) );

				PUIDBGProc(("Error %d", __LINE__));
				PUIDBGProcOUT(("L4SvBsProc::CheckPageSheetData() errItem=%d", errItem));
				return errItem; 		    
		}
		/* 120131 XIA@SSS [v20.75][Alpha][CR0014][DriverSort] End */		
	}

	/*_ IDC_ED_SCALING	[倍率を指定]に不正値が入っていないか */
	if (lpCnf->CNF_bIsSvVisible(m_lpuift, IDSV_LAYOUT_SCALINGRATIO)) {	/*_ 倍率チェックボックスON */
		nVal = GetDlgItemInt( m_hDlg, IDC_ED_SCALING, &bErr, TRUE );
		/*_ 030926 UCH@CSL 大判用紙禁則メッセージ対応 */
		if( (bErr == FALSE) || (nVal < nMin) || (nVal > nMax) ) {
			errItem = IDC_ED_SCALING;
			uMsgID = PMSID_093;
		}
		else if ( bPaperAssumeSupported( shInFormFID, lpCap ) ) {
			iScaling = iCheckAssumedPaper_ScalingRatio( nVal );
			if ( iScaling == 0 ){
				PUIDBGProc(("Error %d", __LINE__));
				PUIDBGProcOUT(("L4SvBsProc::CheckPageSheetData() errItem=%d", errItem));
				return errItem;
			} else {
				errItem = IDC_ED_SCALING;
				uMsgID = PMSID_215;
				nMin = iScaling;
			}
		}
		else{
			PUIDBGProc(("Error %d", __LINE__));
			PUIDBGProcOUT(("L4SvBsProc::CheckPageSheetData() errItem=%d", errItem));
			return errItem;
		}
#if defined(_WIN64)	/*_ @@@ _*/
			CMUSR_iMessageBoxEx(m_lpuift, m_hDlg, uMsgID, IntToPtr (nMin));
#else
			CMUSR_iMessageBoxEx(m_lpuift, m_hDlg, uMsgID, (LPVOID)nMin);
#endif
			SetFocus( GetDlgItem( m_hDlg, IDC_ED_SCALING ));
			SendDlgItemMessage( m_hDlg, errItem, EM_SETSEL, 0, MAKELONG(-1, -1) );
	}

	PUIDBGProcOUT(("L4SvBsProc::CheckPageSheetData() errItem=%d", errItem));

	return errItem;
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;	bPaperAssumeSupported( )
;[Action]
; ある用紙が仮想用紙をサポートしているかを調べる
;[Format]
; static BOOL  bPaperAssumeSupported(iPaper, lpCap)
;[Input]
; int		iPaper		用紙タイプ
; LPVOID	lpCap
;[Output]
; TRUE		サポートしている
; FALSE		サポートしていない
;coding:99/08/26:_.__:YAO@CSL:CCD2
;update:03/09/25:_.__:UCH@CSL:CCD3_________________________________
;bugfix	:__/__/__:_.__:___:_________________________________________
;sysfix	:__/__/__:_.__:___:_________________________________________
;[Main Topic]
;[memo]
********************************************************************/
BOOL L4SvBsProc::bPaperAssumeSupported(int iPaper,LPVOID lpCap)
{
	BOOL		result = FALSE;
	USHORT		i;
	LPCAP_PAPER	pPaper = PDL_GetPaper(lpCap);

	PUIDBGProcIN(("L4SvBsProc::bPaperAssumeSupported()"));

	if( lpCap != NULL ) {
		for( i = 0; i < PDL_GetPapeNum(lpCap); i++, pPaper++ ){
			if( pPaper->usID == (USHORT)iPaper ){
				if( pPaper->LayoutFlag.fAssumed)
					result = TRUE;
				break;
			}
		}
	}

	PUIDBGProcOUT(("L4SvBsProc::bPaperAssumeSupported() result=%d", result));
	
	return( result );
}

/******************************* <-68->   1tab = 4space ************
;[Name]
;	iCheckAssumedPaper_ScalingRatio( )
;[Action]
; 選択された仮想用紙の有効印字領域から最大拡大率を計算する
;[Format]
; int  iCheckAssumedPaper_ScalingRatio( int nVal )
;[Input]
; int nVal 	入力された拡縮率
;
;[Output]
; 0			コントローラの許容範囲内の倍率指定がされている
;						( 禁則の必要なし )
;
; 0以外		指定可能な最大拡縮率を返す(%)
;
;coding:03/09/26:_.__:UCH@CSL:CCD3
;bugfix	:__/__/__:_.__:___:_________________________________________
;sysfix	:__/__/__:_.__:___:_________________________________________
;[Main Topic]
;[memo]
********************************************************************/
int L4SvBsProc::iCheckAssumedPaper_ScalingRatio( int nVal )
{
	CDM_SIZE	sFormSize;
	short		sPrivQuality = 0;
	int			iRet = 0;
	long		lsizeX=0,lsizeY=0,lsizeXtmp=0,lsizeYtmp=0;
	
	PUIDBGProcIN(("L4SvBsProc::iCheckAssumedPaper_ScalingRatio()"));

	sFormSize.cx = 0;sFormSize.cy = 0;
	ULIB_ADM_GetLay_InSizeFormSize(m_lpuift, m_lpdm, &sFormSize);
	ULIB_ADM_GetQal_PrivQuality(m_lpuift, m_lpdm, &sPrivQuality);

	lsizeX = (long)sFormSize.cx;	lsizeX = lsizeX*sPrivQuality /254;	/*_ Dot単位に換算 */
	lsizeY = (long)sFormSize.cy;	lsizeY = lsizeY*sPrivQuality /254;	/*_ Dot単位に換算 */

	/*_ 拡縮後のサイズ */
	lsizeXtmp = (lsizeX * nVal) / 100;
	lsizeYtmp = (lsizeY * nVal) / 100;

	/*_ 拡縮後のサイズがコントローラの許容範囲を超えていたら、最大拡縮率を計算する */
	if ( (lsizeXtmp > LOGICPOSMAX) || (lsizeYtmp > LOGICPOSMAX) ) {
		lsizeXtmp = LOGICPOSMAX * 100 / lsizeX;
		lsizeYtmp = LOGICPOSMAX * 100 / lsizeY;
		iRet = ( lsizeXtmp > lsizeYtmp ) ? lsizeYtmp : lsizeXtmp;
	}

	PUIDBGProcOUT(("L4SvBsProc::iCheckAssumedPaper_ScalingRatio() iRet=%d", iRet));

	return iRet;
}

/* 090317 WAT@IST LIPS4_i_QA V20.00-00017 */
/******************************* <-68->   1tab = 4space ************
;[Name] bOnCmd_PaperSize_ComboBox
;[Action] Process actions when the Page Size combo box is toggled
;[Format] BOOL PageSetupProc::bOnCmd_PaperSize_ComboBox()
;[Input] None
;[Output] BOOL - always returns TRUE
;coding:
********************************************************************/
BOOL L4SvBsProc::bOnCmd_PaperSize_ComboBox()
{
	HWND			hDlgItem = NULL;
	LPSVLISTITEM	lPaperSize = NULL;
	INT				iFormID, sPaperID;
	short			sPrinterID;
	LPCLIBFORM		lpFormIF = NULL;
	CDM_SIZE32	size32;
	CDM_RECT32	rect32;
	SHORT	sMinW = 0, sMaxW = 0, sMinH = 0;
	BOOL				bDispLongPaperMsg = FALSE;
	BOOL		bResult = FALSE;
	BOOL		bDispDuplexMsg = FALSE, bDispSaddleMsg = FALSE;	/* 120220 CHL@SSS [v20.80][PreAlpha][Add][AddNewDevice_Bedford] */
	LPPDL_DEVICEOPTION	lpPDLDevOption = (LPPDL_DEVICEOPTION)m_lpuift->ULIB_GetPrivateDeviceOption(m_lpuift);	/* 100819 ISA@IST add for [V20.40] E219 Spec Change */

	sPrinterID	= (short)m_lpuift->ULIB_GetPrinterID(m_lpuift);

		hDlgItem	= GetDlgItem(m_hDlg, m_wCtrlID);
		if (!hDlgItem){
			return FALSE;
		}
		PDLCMN_vGetLongPaperLIMIT( sPrinterID, &sMinW, &sMaxW, &sMinH );
		lPaperSize = (LPSVLISTITEM)ComboBox_GetItemData(hDlgItem, ComboBox_GetCurSel(hDlgItem));

		m_lpuift->ULIB_QuerySubIFforDataAccess(m_lpuift, CUISIF_FORM, NULL, (LPVOID*)&lpFormIF);
		lpFormIF->EnumStart(lpFormIF);
		do {
			iFormID	 = (short)lpFormIF->GetID(lpFormIF, CLIB_FORM_ID_FORMDB);
#if defined WINNT	/* ユーザー定義判断 */
			if ( ((DMPAPER_CCDUSER_TOP <= iFormID) && (iFormID < DMPAPER_CCDUSER_LAST))
#else
			if ( ((DMPAPER_CCDUSER9X_TOP <= iFormID) && (iFormID < DMPAPER_CCDUSER9X_LAST))
#endif
				|| (iFormID == DMPAPER_USER) ) {
				lpFormIF->GetSize32(lpFormIF, &size32, &rect32);
				if (iFormID == lPaperSize->uitmData) {
					lpFormIF->GetSize32(lpFormIF, &size32, &rect32);

					/* 090519 WAT@IST LIPS4_i_QA V20.00-00161 */
					bResult = PDLCMN_bIsUDP2SidedAvailable(sPrinterID, (LONG)size32.cy, (LONG)size32.cx);
					if( bResult == FALSE ) {
						CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_112);
						bDispDuplexMsg = TRUE;	/* 120220 CHL@SSS [v20.80][PreAlpha][Add][AddNewDevice_Bedford] */
					}
					/* 090519 WAT@IST LIPS4_i_QA V20.00-00161 */

					if( bResult == TRUE ) {	/* 100120 WAT@IST LIPS4_i_QA V20.25-00010 */
						/* 150414 TYL@SSS [v21.50][Beta][Modify][Drv_V21_50_b_LIPS-0005][Case2] Start */
						/* 141121 JJJ@SSS [v21.45][PreAlpha][Modify][AddNewDevice_Ewloe] Start */
// 						bResult = PDLCMN_bIsUDPNotSaddleFoldAvailable(sPrinterID, (LONG)size32.cy, (LONG)size32.cx);
						//BOOL bFinXV2 = FALSE;
						/* 150311 JJJ@SSS [v21.50][Alpha][Modify][Drv_V21_50_a_FamiCom-0046] Start */
						/* Modify the FinXV2's judgment method */
						//if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_FINXV2_SUPPORT) ){
							//if( PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher) || PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_1, lpPDLDevOption->dwFinisher) ){
								//bFinXV2 = TRUE;
							//}
						//}else if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_3RDFINXV2_SUPPORT) ){
							//if( PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_3, lpPDLDevOption->dwFinisher) || PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_3, lpPDLDevOption->dwFinisher) ){
								//bFinXV2 = TRUE;
							//}
						//}

// 						if (PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_FINXV2_SUPPORT)){
// 							switch (m_lpuift->ULIB_GetPrinterID(m_lpuift)){
// 							case IDM_iRADV8595:
// 							case IDM_iRADV8500S:
// 								if (PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_3, lpPDLDevOption->dwFinisher) || 
// 									PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_3, lpPDLDevOption->dwFinisher)){
// 										bFinXV2 = TRUE;
// 								}
// 								break;
// 							case IDM_iRADV6565:
// 							case IDM_iRADV6575:
// 							case IDM_iRADV6500S:
// 								if (PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher) || 
// 									PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_1, lpPDLDevOption->dwFinisher)){
// 										bFinXV2 = TRUE;
// 								}
// 								break;
// 							default:
// 								break;
// 							}
// 						}
						/* 150311 JJJ@SSS [v21.50][Alpha][Modify][Drv_V21_50_a_FamiCom-0046] End */

						//if (bFinXV2 == FALSE){
							//bResult = PDLCMN_bIsUDPNotSaddleFoldAvailable(sPrinterID, (LONG)size32.cy, (LONG)size32.cx);
						//}else{
							//bResult = PDLCMN_bIsUDPNotSaddleFoldAvailable_FinXV2(sPrinterID, (LONG)size32.cy, (LONG)size32.cx);
						//}
						/* 141121 JJJ@SSS [v21.45][PreAlpha][Modify][AddNewDevice_Ewloe] End */
						if ( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_SADDLEMSG_FINISHER_DIVISION) ) {
							bResult = PDLCMN_bIsUDPNotSaddleFoldAvailableV2(sPrinterID, lpPDLDevOption, (LONG)size32.cy, (LONG)size32.cx);
						}else {
							bResult = PDLCMN_bIsUDPNotSaddleFoldAvailable(sPrinterID, (LONG)size32.cy, (LONG)size32.cx);
						}
						/* 150414 TYL@SSS [v21.50][Beta][Modify][Drv_V21_50_b_LIPS-0005][Case2] End */
						
						if( bResult == FALSE ) {
//#if 0						/* 120131 GZW@SSS [v20.80][PreAlpha][modify][FoldWithTrimming]   start */
//							CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_208);
//#else
							if ( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_FINI_FOLDTRIMONLY_SUPPORT) ) {
								/* 150409 TYL@SSS [v21.50][Beta][Add][Drv_V21_50_b_LIPS-0005][Case4] Start */
								if ((m_lpuift->ULIB_GetPrinterID(m_lpuift) == IDM_iRADV8505) && PCUTILS_bSupportFunction(m_lpuift, IDR_FUNC_JAPANESEINPUTSTYLE)) {
									CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_208);
								}else {
									CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_265);
								}
								/* 150409 TYL@SSS [v21.50][Beta][Add][Drv_V21_50_b_LIPS-0005][Case4] End */
							} else {
								/* 140403 ZMJ@SSS [v21.30][PreAlpha][Add][Drv_V21_30_pa_FamiCom-0002][AddNewDevice_Honiara2] Start */
								if ( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_FINT1_SUPPORT) ) {
									if( PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_2, lpPDLDevOption->dwFinisher) || PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_2, lpPDLDevOption->dwFinisher) ){
										;
									} else{
										CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_208);
									}
								} else{
								/* 140403 ZMJ@SSS [v21.30][PreAlpha][Add][Drv_V21_30_pa_FamiCom-0002][AddNewDevice_Honiara2] End */
									CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_208);
								}
							}
//#endif						/* 120131 GZW@SSS [v20.80][PreAlpha][modify][FoldWithTrimming]   end */
							bDispSaddleMsg = TRUE;	/* 120220 CHL@SSS [v20.80][PreAlpha][Add][AddNewDevice_Bedford] */
						}
					}	/* 100120 WAT@IST LIPS4_i_QA V20.25-00010 */

					/* 120220 CHL@SSS [v20.80][PreAlpha][Add][AddNewDevice_Bedford] Start */
					if( !bDispDuplexMsg && !bDispSaddleMsg ){
						/* 121008 MFG@SSS [v20.90][PreAlpha][Modify][Pushcha_YG3] Start */
						if((lpPDLDevOption->dwDevFuncFlags2 & ID_DEVFUNC2_FINYG3_SUPPORT) == ID_DEVFUNC2_FINYG3_SUPPORT){
							if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_FINYG3_SUPPORT) ){
								/* The warning message could pop up only when Finisher is YG3. */
								if( PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_2, lpPDLDevOption->dwFinisher) || PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_2, lpPDLDevOption->dwFinisher) ){
									BOOL	bStapleResult	= PDLCMN_bIsUDPStapleAvailable(sPrinterID, (LONG)size32.cy, (LONG)size32.cx);
									BOOL	bPunchResult	= PDLCMN_bIsUDPPunchAvailable(sPrinterID, (LONG)size32.cy, (LONG)size32.cx);
									if( !bStapleResult || !bPunchResult ) {
										CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_266);
									}
								}
							}else if( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_FINYG3_3RDFIN_SUPPORT) ){
								/* The warning message could pop up only when Finisher is YG3. */
								if( PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_3, lpPDLDevOption->dwFinisher) || PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_3, lpPDLDevOption->dwFinisher) ){
									BOOL	bStapleResult	= PDLCMN_bIsUDPStapleAvailable(sPrinterID, (LONG)size32.cy, (LONG)size32.cx);
									BOOL	bPunchResult	= PDLCMN_bIsUDPPunchAvailable(sPrinterID, (LONG)size32.cy, (LONG)size32.cx);
									if( !bStapleResult || !bPunchResult ) {
										CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_275);
									}
								}
							}
						}
						/* 121008 MFG@SSS [v20.90][PreAlpha][Modify][Pushcha_YG3] End */
						/* 130124 GPG@SSS [v21.00][PreAlpha][Add][Bath2] Start */
						else if ( PDLCMN_FunctionQuery(m_lpuift->ULIB_GetPrinterID(m_lpuift), QUERY_DEVC_FINYN_SUPPORT) ) {
							if (PDLCMN_bFinisherYNSeleted(sPrinterID, lpPDLDevOption)) {	/* 150310 TYL@SSS [v21.50][Alpha][Add][Drv_V21_50_a_FamiCom-0042] */
							//if( PDLCMN_bIsEqualFinisherID(ID_FSH_FINISHER_1, lpPDLDevOption->dwFinisher) || PDLCMN_bIsEqualFinisherID(ID_FSH_SADDLEFINISHER_1, lpPDLDevOption->dwFinisher) ){	/* 150310 TYL@SSS [v21.50][Alpha][Delete][Drv_V21_50_a_FamiCom-0042] */
								BOOL	bStapleSupport	= PDLCMN_bIsUDPStapleAvailable(sPrinterID, (LONG)size32.cy, (LONG)size32.cx);
								BOOL	bPunchSupport	= PDLCMN_bIsUDPPunchAvailable(sPrinterID, (LONG)size32.cy, (LONG)size32.cx);
								if( !bStapleSupport && !bPunchSupport ) {  /* 130517 GPG@SSS [v21.00][Beta][Modify][Change || to &&][CommonUI v21.00_CR0065] */
									CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_266);
								/* 130520 GPG@SSS [v21.00][Beta][Add][CommonUI v21.00_CR0065] Start */
								} else if (!bStapleSupport) {
									CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_286);
								}
								/* 130520 GPG@SSS [v21.00][Beta][Add][CommonUI v21.00_CR0065] End */
							}
						}
						/* 130124 GPG@SSS [v21.00][PreAlpha][Add][Bath2] End */
					}
					/* 120220 CHL@SSS [v20.80][PreAlpha][Add][AddNewDevice_Bedford] End */
				}
			}
		} while (lpFormIF->EnumNext(lpFormIF));

		if (lpFormIF) {
			lpFormIF->Release(lpFormIF);
		}

//#if 1	/* 100819 ISA@IST add start for [V20.40] E219 Spec Change */
	{
		hDlgItem	= GetDlgItem(m_hDlg, m_wCtrlID);
		if ( !hDlgItem ){
			return FALSE;
		}
		lPaperSize = (LPSVLISTITEM)ComboBox_GetItemData(hDlgItem, ComboBox_GetCurSel(hDlgItem));

		m_lpuift->ULIB_QuerySubIFforDataAccess(m_lpuift, CUISIF_FORM, NULL, (LPVOID*)&lpFormIF);
		if (lpFormIF != NULL) {
			lpFormIF->EnumStart(lpFormIF);
			do {
				iFormID	 = (short)lpFormIF->GetID(lpFormIF, CLIB_FORM_ID_FORMDB);
				if (iFormID == lPaperSize->uitmData) {
					lpFormIF->GetSize32(lpFormIF, &size32, &rect32);
					/* 120416 CHL@SSS [v20.80][Alpha][Modify][CR0022][Bedford YG3 はがき相当のユーザー定義用紙が排紙可能になる] */
					/* 120220 CHL@SSS [v20.80][PreAlpha][Modify][AddNewDevice_Bedford] */
					/* Bedford Paper Related */
					bResult = PDLCMN_bIsFinisherAvailable(sPrinterID, lpPDLDevOption, (LONG)size32.cy, (LONG)size32.cx);
					//bResult = PDLCMN_bIsFinisherAvailable(sPrinterID, lpPDLDevOption, (LONG)size32.cy, (LONG)size32.cx, (short)iFormID);

					if( bResult == FALSE ) {
						CMUSR_iMessageBox(m_lpuift, m_hDlg, PMSID_245);
					}
				}
			} while (lpFormIF->EnumNext(lpFormIF));
			
			lpFormIF->Release(lpFormIF);
		}
	}
//#endif	/* 100819 ISA@IST add end for [V20.40] E219 Spec Change */

	return TRUE;
}
/* 090317 WAT@IST LIPS4_i_QA V20.00-00017 */

/* 20120514 XIA@SSS [v20.85][PreAlpha][NF_2085_1-BasicSheet][Add] End */

/******************************* <-68->   1tab = 4space ************
;[Name]
;[Action]
;[Format]
;[Input]
;[Output]
;cording:03/04/07:0.00:NON@CSL
********************************************************************/
static BOOL CALLBACK bIsEnableItemOnOpenDocProp(LPUIINTERFACETABLE lpuift, WORD wCtrlID, LPSVLISTITEM lpList)
{
	WORD		wItem = 0;
	BOOL		bRet = FALSE;


	PUIDBGSvIN(("bIsEnableItemOnOpenDocProp()"));

	bRet = lpList->bEnable;

	switch (wCtrlID) {

	case IDC_CB_JOBOPERATIONS:

		switch (lpList->uitmData) {
		case ID_DST_PRINTER:		wItem = CDM_UI_ACED_JOBPRINT;		UIDBGPrint("CDM_UI_ACED_JOBPRINT\n");		break;
		case ID_DST_STORE:			wItem = CDM_UI_JOBSTORE;			UIDBGPrint("CDM_UI_JOBSTORE\n");			break;
		case ID_DST_INTRPT:			wItem = CDM_UI_INTERRUPT;			UIDBGPrint("CDM_UI_INTERRUPT\n");			break;
		case ID_DST_SECUREDPRINT:	wItem = CDM_UI_SECURED;				UIDBGPrint("CDM_UI_SECURED\n"); 			break;
		case ID_DST_PAGECOMPOSER:	wItem = CDM_UI_JOBEDITANDPREVIEW;	UIDBGPrint("CDM_UI_JOBEDITANDPREVIEW\n");	break;
		case ID_DST_PROMOTE:		wItem = CDM_UI_PROMOTE;				UIDBGPrint("CDM_UI_PROMOTE\n");				break;		/* 100519 KIS@CIS LIPS4_i_QA V20.35-00078 - mod ID_DST_PRIORITY to ID_DST_PROMOTE */
		case ID_DST_HOLDQUEUE:		wItem = CDM_UI_HOLDQUEUE;			UIDBGPrint("CDM_UI_HOLDQUEUE\n");			break;		/* 20060822 LYN@CSSSI [E314][HoldQueue] */
		default:					wItem = 0;							UIDBGPrint("Other\n");						break;
		}
		break;
	}

	if (wItem) {
		if (UTL_bIsEnableDocPropItem(lpuift, wItem) == FALSE) {
			bRet = FALSE;
		}
	}

	PUIDBGSvOUT(("bIsEnableItemOnOpenDocProp() bRet=%d", bRet));

	return bRet;
}
