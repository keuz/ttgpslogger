/*
 * TTGPSLogger
 * 
 * TTGPSLogger, a GPS logger for Symbian S60 smartphones.
 * Copyright (C) 2009 TTINPUT <ttinputdiary@ovi.com>
 * 
 * http://ttinputdiary.vox.com/
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include <e32base.h>
#include <e32debug.h>
#include <e32std.h>
#include <aknlistquerydialog.h>
#include <aknnotewrappers.h>
#include <aknselectionlist.h>
#include <apgtask.h>
#include <aknmessagequerydialog.h>
#include <eikbtgpc.h> 
#include <lbs.h>
#include <lbserrors.h>
#include <lbssatellite.h>
#include <aknkeylock.h>
#include <avkon.hrh>
#include <TTGPSLogger.rsg>

#if defined(__S60_50__) || defined(__S60_32__)
#include <csatelliteinfoui.h>
#endif // defined(__S60_50__) || defined(__S60_32__)
#ifdef __S60_50__
#include <akntoolbar.h>
#endif // __S60_50__

#include "TTGPSLogger.hrh"
#include "TTGPSLoggerCommon.h"
#include "TTGPSLoggerPositionObserver.h"
#include "TTGPSLoggerRecorderObserver.h"
#include "TTGPSLoggerAppUi.h"
#include "TTGPSLoggerPositionRecorder.h"
#include "TTGPSLoggerPositionRequester.h"
#include "TTGPSLoggerRecorderFormat.h"
#include "TTGPSLoggerRecorderFormatNMEA.h"
#include "TTGPSLoggerRecorderFormatGPX.h"
#include "TTGPSLoggerRecorderFormatKML.h"
#include "TTGPSLoggerSettingsData.h"
#include "TTGPSLoggerEngine.h"

CTTGPSLoggerEngine::CTTGPSLoggerEngine(CTTGPSLoggerAppUi* aAppUi) :
    iAppUi(aAppUi),
    iSettingsData(NULL),
	iPositionRequester(NULL),
	iPositionRecorder(NULL),
    iRecorderFormatNMEA(NULL),
	iRecorderFormatGPX(NULL),
	iRecorderFormatKML(NULL)
	{
	}

CTTGPSLoggerEngine::~CTTGPSLoggerEngine()
	{
	delete iPositionRequester;
    delete iPositionRecorder;
    delete iRecorderFormatNMEA;
    delete iRecorderFormatGPX;
    delete iRecorderFormatKML;
    delete iSettingsData;
	}

CTTGPSLoggerEngine* CTTGPSLoggerEngine::NewLC(CTTGPSLoggerAppUi* aAppUi)
	{
	CTTGPSLoggerEngine* self = new (ELeave) CTTGPSLoggerEngine(aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CTTGPSLoggerEngine* CTTGPSLoggerEngine::NewL(CTTGPSLoggerAppUi* aAppUi)
	{
	CTTGPSLoggerEngine* self = CTTGPSLoggerEngine::NewLC(aAppUi);
	CleanupStack::Pop(); // self;
	return self;
	}

void CTTGPSLoggerEngine::ConstructL()
	{

	RDebug::Print(_L("%d \n"), __LINE__);
	iSettingsData = CTTGPSLoggerSettingsData::NewL();
	RDebug::Print(_L("%d \n"), __LINE__);
    iSettingsData->LoadL();
    RDebug::Print(_L("%d \n"), __LINE__);
    iPositionRecorder = CTTGPSLoggerPositionRecorder::NewL(this);
    RDebug::Print(_L("%d \n"), __LINE__);
    iRecorderFormatNMEA = CTTGPSLoggerRecorderFormatNMEA::NewL(this);
    RDebug::Print(_L("%d \n"), __LINE__);
    iRecorderFormatGPX = CTTGPSLoggerRecorderFormatGPX::NewL(this);
    RDebug::Print(_L("%d \n"), __LINE__);
    iRecorderFormatKML = CTTGPSLoggerRecorderFormatKML::NewL(this);
    RDebug::Print(_L("%d \n"), __LINE__);
    iPositionRequester = CTTGPSLoggerPositionRequester::NewL();
    RDebug::Print(_L("%d \n"), __LINE__, __func__);
    CEikonEnv::Static()->ReadResourceAsDes16L(iMsgStartError1, R_TTGP_TBUF_MSG_START_ERROR1);
    RDebug::Print(_L("%d \n"), __LINE__);
    iPositionRequester->StartL();
    RDebug::Print(_L("%d \n"), __LINE__);
    
    switch (Settings()->OutputAutostart())
        {
        case CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsOutputAutostartDisabled:
            break;
        case CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsOutputAutostartNMEA:
            MenuStartNowNmeaL();
            break;
        case CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsOutputAutostartGPX:
            MenuStartNowGpxL();
            break;
        case CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsOutputAutostartKML:
            MenuStartNowKmlL();
            break;
        default:
            break;
        }
	}

void CTTGPSLoggerEngine::AddPositionObserver(MTTGPSLoggerPositionObserver* aObserver)
	{
	if (iPositionRequester)
		{
		iPositionRequester->AddPositionObserver(aObserver);
		}
	}
void CTTGPSLoggerEngine::DeletePositionObserver(MTTGPSLoggerPositionObserver* aObserver)
    {
    if (iPositionRequester)
        {
        iPositionRequester->DeletePositionObserver(aObserver);
        }
    }
void CTTGPSLoggerEngine::AddRecorderObserver(MTTGPSLoggerRecorderObserver* aObserver)
    {
    if (iPositionRecorder)
        {
        iPositionRecorder->AddRecorderObserver(aObserver);
        }
    }
void CTTGPSLoggerEngine::DeleteRecorderObserver(MTTGPSLoggerRecorderObserver* aObserver)
    {
    if (iPositionRecorder)
        {
        iPositionRecorder->DeleteRecorderObserver(aObserver);
        }
    }

void CTTGPSLoggerEngine::MenuHide()
    {
    TApaTask task(CEikonEnv::Static()->WsSession());
    task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
    task.SendToBackground();
    }

void CTTGPSLoggerEngine::MenuStartNowL()
    {
    TInt selectIndex(0);
    HBufC* title = CEikonEnv::Static()->AllocReadResourceLC(R_TTGP_TBUF32_DIALOG_MENUSTART1_TITLE_STARTNOW);
    CDesC16Array* itemArray = CEikonEnv::Static()->ReadDesC16ArrayResourceL(R_TTGP_ARRAY_DIALOG_MENUSTART1);
    CleanupStack::PushL(itemArray);
    CAknListQueryDialog* dialog = new (ELeave) CAknListQueryDialog(&selectIndex);
    dialog->PrepareLC(R_TTGP_DIALOG_LISTQUERY);
    dialog->QueryHeading()->SetTextL(*title);
    dialog->SetItemTextArray(itemArray);
    dialog->SetOwnershipType(ELbmDoesNotOwnItemArray);
    TInt answer = dialog->RunLD();
    CleanupStack::PopAndDestroy(); // itemArray
    CleanupStack::PopAndDestroy(); // title
    if (answer)
        {
        switch (selectIndex)
            {
            case 0:
                MenuStartNowNmeaL();
                break;
            case 1:
                MenuStartNowGpxL();
                break;
            case 2:
                MenuStartNowKmlL();
                break;
            default:
                break;
            }
        }
    }

void CTTGPSLoggerEngine::MenuStartAfterL()
    {
    TInt selectIndex(0);
    HBufC* title = CEikonEnv::Static()->AllocReadResourceLC(R_TTGP_TBUF32_DIALOG_MENUSTART1_TITLE_STARTAFTER);
    CDesC16Array* itemArray = CEikonEnv::Static()->ReadDesC16ArrayResourceL(R_TTGP_ARRAY_DIALOG_MENUSTART1);
    CleanupStack::PushL(itemArray);
    CAknListQueryDialog* dialog = new (ELeave) CAknListQueryDialog(&selectIndex);
    dialog->PrepareLC(R_TTGP_DIALOG_LISTQUERY);
    dialog->QueryHeading()->SetTextL(*title);
    dialog->SetItemTextArray(itemArray);
    dialog->SetOwnershipType(ELbmDoesNotOwnItemArray);
    TInt answer = dialog->RunLD();
    CleanupStack::PopAndDestroy(); // itemArray
    CleanupStack::PopAndDestroy(); // title
    if (answer)
        {
        TTime startTime = InputStartTime();
        if (startTime > 0)
            {
            switch (selectIndex)
                {
                case 0:
                    MenuStartNmeaL(startTime);
                    break;
                case 1:
                    MenuStartGpxL(startTime);
                    break;
                case 2:
                    MenuStartKmlL(startTime);
                    break;
                default:
                    break;
                }
            }
        }
    }

void CTTGPSLoggerEngine::MenuStartNmeaL(TTime aTime)
    {
    iPositionRecorder->StartL(iRecorderFormatNMEA, aTime);
    iPositionRequester->AddPositionObserver(iPositionRecorder);
    SetSoftKeyL();
    SetToolbarL();
    }
void CTTGPSLoggerEngine::MenuStartNowNmeaL()
    {
    TTime startTime;
    startTime.UniversalTime();
    MenuStartNmeaL(startTime);
    SetToolbarL();
    }
void CTTGPSLoggerEngine::MenuStartAfterNmeaL()
    {
    TTime startTime = InputStartTime();
    if (startTime > 0)
        {
        MenuStartNmeaL(startTime);
        }
    }

void CTTGPSLoggerEngine::MenuStartGpxL(TTime aTime)
    {
    iPositionRecorder->StartL(iRecorderFormatGPX, aTime);
    iPositionRequester->AddPositionObserver(iPositionRecorder);
    SetSoftKeyL();
    SetToolbarL();
    }
void CTTGPSLoggerEngine::MenuStartNowGpxL()
    {
    TTime startTime;
    startTime.UniversalTime();
    MenuStartGpxL(startTime);
    }
void CTTGPSLoggerEngine::MenuStartAfterGpxL()
    {
    TTime startTime = InputStartTime();
    if (startTime > 0)
        {
        MenuStartGpxL(startTime);
        }
    }

void CTTGPSLoggerEngine::MenuStartKmlL(TTime aTime)
    {
    iPositionRecorder->StartL(iRecorderFormatKML, aTime);
    iPositionRequester->AddPositionObserver(iPositionRecorder);
    SetSoftKeyL();
    SetToolbarL();
    }
void CTTGPSLoggerEngine::MenuStartNowKmlL()
    {
    TTime startTime;
    startTime.UniversalTime();
    MenuStartKmlL(startTime);
    }
void CTTGPSLoggerEngine::MenuStartAfterKmlL()
    {
    TTime startTime = InputStartTime();
    if (startTime > 0)
        {
        MenuStartKmlL(startTime);
        }
    }

void CTTGPSLoggerEngine::MenuRecordL()
    {
    iPositionRecorder->RecordL();
    }

void CTTGPSLoggerEngine::MenuPauseL()
    {
    iPositionRecorder->PauseL();
    SetSoftKeyL();
    SetToolbarL();
    }

void CTTGPSLoggerEngine::MenuResumeL()
    {
    iPositionRecorder->ResumeL();
    SetSoftKeyL();
    SetToolbarL();
    }

void CTTGPSLoggerEngine::MenuStopL()
    {
    iPositionRequester->DeletePositionObserver(iPositionRecorder);
    iPositionRecorder->StopL();
    SetSoftKeyL();
    SetToolbarL();
    }

void CTTGPSLoggerEngine::MenuSatelliteL()
    {
#if defined(__S60_50__) || defined(__S60_32__)
    _LIT(KRequestor, "TTGPSLogger");
    
    CSatelliteInfoUI* iSatelliteInfo = CSatelliteInfoUI::NewL();
    iSatelliteInfo->SetLaunchView(CSatelliteInfoUI::ESatelliteFirmamentView);
    iSatelliteInfo->ExecuteLD(KRequestor);
    iSatelliteInfo = NULL;
#endif // defined(__S60_50__) || defined(__S60_32__)
    }

void CTTGPSLoggerEngine::MenuGPSStatusL()
	{
	TTGPSLoggerCommon::SetTextStatusPaneL(R_TTGP_TBUF32_STATUSPANE_TITLE_GPSSTATUS);
	AppUi()->ActivateLocalViewL(TUid::Uid(ETTGPSLoggerViewGPSStatusId));
	}

void CTTGPSLoggerEngine::MenuNMEAViewerL()
	{
	TTGPSLoggerCommon::SetTextStatusPaneL(R_TTGP_TBUF32_STATUSPANE_TITLE_NMEAVIEWER);
	AppUi()->ActivateLocalViewL(TUid::Uid(ETTGPSLoggerViewNMEAViewerId));
	}

void CTTGPSLoggerEngine::MenuSettingsL()
    {
    TTGPSLoggerCommon::SetTextStatusPaneL(R_TTGP_TBUF32_STATUSPANE_TITLE_SETTINGS);
    AppUi()->ActivateLocalViewL(TUid::Uid(ETTGPSLoggerViewSettingsId));
    /*
    TInt selectedIndex = 0;
    CDesC16ArrayFlat *itemArray = CCoeEnv::Static()->ReadDesCArrayResourceL(R_TTGP_ARRAY_SETTINGSTOP);
    CleanupStack::PushL(itemArray);
    CAknSelectionListDialog* dialog = CAknSelectionListDialog::NewL(selectedIndex, itemArray, R_TTGP_MENUBAR_SETTINGSTOP);
    dialog->PrepareLC(R_TTGP_DIALOG_SETTINGSTOP);
    TInt answer = 0;
    answer = dialog->RunLD();
    if (answer)
        {
        switch (selectedIndex)
            {
            case 0:
                iSettingsData->SetSettingsId(ETTGPSLoggerSettingGeneral);
                AppUi()->ActivateLocalViewL(TUid::Uid(ETTGPSLoggerViewSettingsId));
                break;
            case 1:
                iSettingsData->SetSettingsId(ETTGPSLoggerSettingNMEA);
                AppUi()->ActivateLocalViewL(TUid::Uid(ETTGPSLoggerViewSettingsId));
                break;
            case 2:
                iSettingsData->SetSettingsId(ETTGPSLoggerSettingGPX);
                AppUi()->ActivateLocalViewL(TUid::Uid(ETTGPSLoggerViewSettingsId));
                break;
            case 3:
                iSettingsData->SetSettingsId(ETTGPSLoggerSettingKML);
                AppUi()->ActivateLocalViewL(TUid::Uid(ETTGPSLoggerViewSettingsId));
                break;
            default:
                break;
            }
        }
    else
        {
        AppUi()->ActivateLocalViewL(TUid::Uid(ETTGPSLoggerViewMainId));
        }
    
    CleanupStack::PopAndDestroy(itemArray);
    */
    }

void CTTGPSLoggerEngine::MenuAboutL()
    {
    HBufC* title = CEikonEnv::Static()->AllocReadResourceLC(R_TTGP_TBUF32_ABOUT_TITLE);
    HBufC* text = CEikonEnv::Static()->AllocReadResourceLC(R_TTGP_TBUF_ABOUT_TEXT);
    CAknMessageQueryDialog* dialog = new (ELeave) CAknMessageQueryDialog();
    CleanupStack::PushL(dialog);
    dialog->PrepareLC(R_TTGP_DIALOG_MESSAGEQUERY_OK);
    dialog->QueryHeading()->SetTextL(*title);
    dialog->SetMessageTextL(*text);
    CleanupStack::Pop(); // dialog
    dialog->RunLD();
    CleanupStack::PopAndDestroy(); // text
    CleanupStack::PopAndDestroy(); // title
    }

void CTTGPSLoggerEngine::MenuShortcutsL()
    {
    HBufC* title = CEikonEnv::Static()->AllocReadResourceLC(R_TTGP_TBUF32_SHORTCUTS_TITLE);
    HBufC* text = CEikonEnv::Static()->AllocReadResourceLC(R_TTGP_TBUF_SHORTCUTS_TEXT);
    CAknMessageQueryDialog* dialog = new (ELeave) CAknMessageQueryDialog();
    CleanupStack::PushL(dialog);
    dialog->PrepareLC(R_TTGP_DIALOG_MESSAGEQUERY_OK);
    dialog->QueryHeading()->SetTextL(*title);
    dialog->SetMessageTextL(*text);
    CleanupStack::Pop(); // dialog
    dialog->RunLD();
    CleanupStack::PopAndDestroy(); // text
    CleanupStack::PopAndDestroy(); // title
    }

void CTTGPSLoggerEngine::MenuLockL()
    {
    RAknKeyLock lock;
    if (lock.Connect() == KErrNone)
        {
        lock.EnableKeyLock();
        lock.Close();
        }
    }

TKeyResponse CTTGPSLoggerEngine::HandleShortcutKeyL(TUint aCode)
    {
    if (aCode == EKeyOK)
    	{
        switch(iPositionRecorder->Status())
            {
            case CTTGPSLoggerPositionRecorder::EStatusNone:
                {
                MenuStartNowL();
                return EKeyWasConsumed;
                }
            case CTTGPSLoggerPositionRecorder::EStatusWaiting:
                {
                MenuStopL();
                return EKeyWasConsumed;
                }
            case CTTGPSLoggerPositionRecorder::EStatusRecording:
            case CTTGPSLoggerPositionRecorder::EStatusAutoPausing:
                {
                MenuPauseL();
                return EKeyWasConsumed;
                }
            case CTTGPSLoggerPositionRecorder::EStatusPausing:
                {
                MenuResumeL();
                return EKeyWasConsumed;
                }
            default:
                break;
            }
    	}
    
    if (!Settings()->DisplayShortcuts())
    	{
        return EKeyWasNotConsumed;
    	}
    
    if      (aCode == '1')
        {
        if (iPositionRecorder->Status() == CTTGPSLoggerPositionRecorder::EStatusNone)
        	{
        	MenuStartNowL();
            return EKeyWasConsumed;
        	}
        }
    else if (aCode == '2')
        {
        if (iPositionRecorder->Status() == CTTGPSLoggerPositionRecorder::EStatusNone)
        	{
        	MenuStartAfterL();
            return EKeyWasConsumed;
        	}
        }
    else if (aCode == '3')
        {
        switch(iPositionRecorder->Status())
        	{
            case CTTGPSLoggerPositionRecorder::EStatusWaiting:
            	{
                MenuStopL();
                return EKeyWasConsumed;
            	}
            case CTTGPSLoggerPositionRecorder::EStatusRecording:
            case CTTGPSLoggerPositionRecorder::EStatusPausing:
            case CTTGPSLoggerPositionRecorder::EStatusAutoPausing:
            	{
                TInt selectIndex(0);
                HBufC* title = NULL;
                CDesC16Array* itemArray = NULL;
                if (iPositionRecorder->Status() == CTTGPSLoggerPositionRecorder::EStatusRecording ||
                	iPositionRecorder->Status() == CTTGPSLoggerPositionRecorder::EStatusAutoPausing)
                	{
                    title = CEikonEnv::Static()->AllocReadResourceLC(R_TTGP_TBUF32_DIALOG_PAUSESTOP_TITLE);
                    itemArray = CEikonEnv::Static()->ReadDesC16ArrayResourceL(R_TTGP_ARRAY_DIALOG_PAUSESTOP);
                    CleanupStack::PushL(itemArray);
                	}
                else
                	{
                    title = CEikonEnv::Static()->AllocReadResourceLC(R_TTGP_TBUF32_DIALOG_RESUMESTOP_TITLE);
                    itemArray = CEikonEnv::Static()->ReadDesC16ArrayResourceL(R_TTGP_ARRAY_DIALOG_RESUMESTOP);
                    CleanupStack::PushL(itemArray);
                	}
                CAknListQueryDialog* dialog = new (ELeave) CAknListQueryDialog(&selectIndex);
                dialog->PrepareLC(R_TTGP_DIALOG_LISTQUERY);
                dialog->QueryHeading()->SetTextL(*title);
                dialog->SetItemTextArray(itemArray);
                dialog->SetOwnershipType(ELbmDoesNotOwnItemArray);
                TInt answer = dialog->RunLD();
                CleanupStack::PopAndDestroy(); // itemArray
                CleanupStack::PopAndDestroy(); // title
                if (answer)
                	{
                    if (iPositionRecorder->Status() == CTTGPSLoggerPositionRecorder::EStatusRecording ||
                    	iPositionRecorder->Status() == CTTGPSLoggerPositionRecorder::EStatusAutoPausing)
                    	{
                    	switch (selectIndex)
                    		{
                    		case 0:
                    			{
                    			MenuPauseL();
                    			break;
                    			}
                    		case 1:
                    			{
                    			MenuStopL();
                    			break;
                    			}
                    		default:
                    			break;
                    		}
                    	}
                    else
                    	{
                    	switch (selectIndex)
                    		{
                    		case 0:
                    			{
                    			MenuResumeL();
                    			break;
                    			}
                    		case 1:
                    			{
                    			MenuStopL();
                    			break;
                    			}
                    		default:
                    			break;
                    		}
                    	}
            	    }
                
                return EKeyWasConsumed;
            	}
            case CTTGPSLoggerPositionRecorder::EStatusNone:
            default:
                break;
            }
        }
    else if (aCode == 'n')
        {
        if (iPositionRecorder->Status() == CTTGPSLoggerPositionRecorder::EStatusNone)
        	{
        	MenuStartNowNmeaL();
            return EKeyWasConsumed;
        	}
        }
    else if (aCode == 'N')
        {
        if (iPositionRecorder->Status() == CTTGPSLoggerPositionRecorder::EStatusNone)
        	{
        	MenuStartAfterNmeaL();
            return EKeyWasConsumed;
        	}
        }
    else if (aCode == 'g')
        {
        if (iPositionRecorder->Status() == CTTGPSLoggerPositionRecorder::EStatusNone)
        	{
        	MenuStartNowGpxL();
            return EKeyWasConsumed;
        	}
        }
    else if (aCode == 'G')
        {
        if (iPositionRecorder->Status() == CTTGPSLoggerPositionRecorder::EStatusNone)
        	{
        	MenuStartAfterGpxL();
            return EKeyWasConsumed;
        	}
        }
    else if (aCode == 'k')
        {
        if (iPositionRecorder->Status() == CTTGPSLoggerPositionRecorder::EStatusNone)
        	{
        	MenuStartNowKmlL();
            return EKeyWasConsumed;
        	}
        }
    else if (aCode == 'K')
        {
        if (iPositionRecorder->Status() == CTTGPSLoggerPositionRecorder::EStatusNone)
        	{
        	MenuStartAfterKmlL();
            return EKeyWasConsumed;
        	}
        }
    else if (aCode == '*' || aCode == 'l' || aCode == 'L')
        {
        MenuLockL();
        return EKeyWasConsumed;
        }
    else if (aCode == '0' || aCode == ' ')
        {
        switch(iPositionRecorder->Status())
        	{
            case CTTGPSLoggerPositionRecorder::EStatusRecording:
            case CTTGPSLoggerPositionRecorder::EStatusAutoPausing:
            	{
                MenuRecordL();
                return EKeyWasConsumed;
            	}
            case CTTGPSLoggerPositionRecorder::EStatusNone:
            case CTTGPSLoggerPositionRecorder::EStatusWaiting:
            case CTTGPSLoggerPositionRecorder::EStatusPausing:
            default:
            	break;
        	}
        }
    else if (aCode == '#' || aCode == 't' || aCode == 'T')
        {
        switch(iPositionRecorder->Status())
        	{
            case CTTGPSLoggerPositionRecorder::EStatusNone:
            case CTTGPSLoggerPositionRecorder::EStatusWaiting:
            case CTTGPSLoggerPositionRecorder::EStatusRecording:
            case CTTGPSLoggerPositionRecorder::EStatusPausing:
            case CTTGPSLoggerPositionRecorder::EStatusAutoPausing:
            	{
                TInt selectIndex(0);
                HBufC* title = NULL;
                CDesC16Array* itemArray = NULL;
                title = CEikonEnv::Static()->AllocReadResourceLC(R_TTGP_TBUF32_DIALOG_TOOL_TITLE);
#if !defined(__S60_50__) && !defined(__S60_32__)
                itemArray = CEikonEnv::Static()->ReadDesC16ArrayResourceL(R_TTGP_ARRAY_DIALOG_TOOL_V31);
#else
                itemArray = CEikonEnv::Static()->ReadDesC16ArrayResourceL(R_TTGP_ARRAY_DIALOG_TOOL_V32);
#endif // defined(__S60_50__) || defined(__S60_32__)
                CleanupStack::PushL(itemArray);
                CAknListQueryDialog* dialog = new (ELeave) CAknListQueryDialog(&selectIndex);
                dialog->PrepareLC(R_TTGP_DIALOG_LISTQUERY);
                dialog->QueryHeading()->SetTextL(*title);
                dialog->SetItemTextArray(itemArray);
                dialog->SetOwnershipType(ELbmDoesNotOwnItemArray);
                TInt answer = dialog->RunLD();
                CleanupStack::PopAndDestroy(); // itemArray
                CleanupStack::PopAndDestroy(); // title
                if (answer)
                	{
#if !defined(__S60_50__) && !defined(__S60_32__)
                	switch (selectIndex)
                		{
                		case 0:
                			{
                			MenuGPSStatusL();
                			break;
                			}
                		case 1:
                			{
                			MenuNMEAViewerL();
                			break;
                			}
                		default:
                			break;
                		}
#else
                	switch (selectIndex)
                		{
                		case 0:
                			{
                			MenuSatelliteL();
                			break;
                			}
                		case 1:
                			{
                			MenuGPSStatusL();
                			break;
                			}
                		case 2:
                			{
                			MenuNMEAViewerL();
                			break;
                			}
                		default:
                			break;
                		}
#endif // defined(__S60_50__) || defined(__S60_32__)
                	}
                return EKeyWasConsumed;
            	}
            default:
                break;
            }
        }
    else if (aCode == 's' || aCode == 'S')
        {
        switch(iPositionRecorder->Status())
        	{
            case CTTGPSLoggerPositionRecorder::EStatusNone:
            	{
            	if (aCode == 's')
            		{
                    MenuStartNowL();
            		}
            	else
            		{
                    MenuStartAfterL();
            		}
                return EKeyWasConsumed;
            	}
            case CTTGPSLoggerPositionRecorder::EStatusWaiting:
            case CTTGPSLoggerPositionRecorder::EStatusRecording:
            case CTTGPSLoggerPositionRecorder::EStatusPausing:
            case CTTGPSLoggerPositionRecorder::EStatusAutoPausing:
            	{
                MenuStopL();
                return EKeyWasConsumed;
            	}
            default:
            	break;
        	}
        }
    else if (aCode == 'p' || aCode == 'P')
        {
        switch(iPositionRecorder->Status())
        	{
            case CTTGPSLoggerPositionRecorder::EStatusRecording:
            case CTTGPSLoggerPositionRecorder::EStatusAutoPausing:
            	{
                MenuPauseL();
                return EKeyWasConsumed;
            	}
            case CTTGPSLoggerPositionRecorder::EStatusNone:
            case CTTGPSLoggerPositionRecorder::EStatusWaiting:
            case CTTGPSLoggerPositionRecorder::EStatusPausing:
            default:
            	break;
        	}
        }
    else if (aCode == 'r' || aCode == 'R')
        {
        switch(iPositionRecorder->Status())
        	{
            case CTTGPSLoggerPositionRecorder::EStatusPausing:
            	{
                MenuResumeL();
                return EKeyWasConsumed;
            	}
            case CTTGPSLoggerPositionRecorder::EStatusNone:
            case CTTGPSLoggerPositionRecorder::EStatusWaiting:
            case CTTGPSLoggerPositionRecorder::EStatusRecording:
            case CTTGPSLoggerPositionRecorder::EStatusAutoPausing:
            default:
            	break;
        	}
        }
    
    return EKeyWasNotConsumed;
    }

void CTTGPSLoggerEngine::SetSoftKeyL()
    {
    CEikButtonGroupContainer* cba = CEikButtonGroupContainer::Current();
    if (!cba)
    	return;
    
    switch(iPositionRecorder->Status())
        {
        case CTTGPSLoggerPositionRecorder::EStatusNone:
            {
            cba->SetCommandSetL(R_TTGP_SOFTKEYS_OPTIONS_STARTNOW_HIDE);
            cba->DrawNow();
            break;
            }
        case CTTGPSLoggerPositionRecorder::EStatusWaiting:
            {
            cba->SetCommandSetL(R_TTGP_SOFTKEYS_OPTIONS_STOP_HIDE);
            cba->DrawNow();
            break;
            }
        case CTTGPSLoggerPositionRecorder::EStatusRecording:
        case CTTGPSLoggerPositionRecorder::EStatusAutoPausing:
            {
            if (Settings()->OutputIntervals() == 0 && Settings()->OutputDistance() == 0)
                {
                cba->SetCommandSetL(R_TTGP_SOFTKEYS_OPTIONS_RECORD_HIDE);
                }
            else
                {
                cba->SetCommandSetL(R_TTGP_SOFTKEYS_OPTIONS_PAUSE_HIDE);
                }
            cba->DrawNow();
            break;
            }
        case CTTGPSLoggerPositionRecorder::EStatusPausing:
            {
            cba->SetCommandSetL(R_TTGP_SOFTKEYS_OPTIONS_RESUME_HIDE);
            cba->DrawNow();
            break;
            }
        default:
            break;
        }
    }

void CTTGPSLoggerEngine::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
    {
    if (aResourceId == R_TTGP_MENUPANE_MAIN)
        {
        switch (iPositionRecorder->Status())
            {
            case CTTGPSLoggerPositionRecorder::EStatusNone:
                {
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandStart, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandStartNow, ETrue);
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandRecord, ETrue);
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandPause, ETrue);
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandResume, ETrue);
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandStop, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandTool, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandSettings, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandHelp, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandAbout, ETrue);
                break;
                }
            case CTTGPSLoggerPositionRecorder::EStatusWaiting:
                {
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandStart, ETrue);
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandStartNow, ETrue);
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandRecord, ETrue);
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandPause, ETrue);
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandResume, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandStop, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandTool, ETrue);
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandSettings, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandHelp, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandAbout, ETrue);
                break;
                }
            case CTTGPSLoggerPositionRecorder::EStatusRecording:
            case CTTGPSLoggerPositionRecorder::EStatusAutoPausing:
                {
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandStart, ETrue);
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandStartNow, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandRecord, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandPause, ETrue);
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandResume, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandStop, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandTool, ETrue);
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandSettings, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandHelp, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandAbout, ETrue);
                break;
                }
            case CTTGPSLoggerPositionRecorder::EStatusPausing:
                {
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandStart, ETrue);
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandStartNow, ETrue);
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandRecord, ETrue);
                aMenuPane->SetItemDimmed(ETTGPSLoggerCommandPause, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandResume, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandStop, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandTool, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandSettings, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandHelp, ETrue);
                //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandAbout, ETrue);
                break;
                }
            default:
                break;
            }
        }
    else if (aResourceId == R_TTGP_MENUPANE_MAIN_START)
        {
        //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandStartNMEA, ETrue);
        //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandStartGPX, ETrue);
        //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandStartKML, ETrue);
        }
    else if (aResourceId == R_TTGP_MENUPANE_MAIN_TOOL)
        {
#if !defined(__S60_50__) && !defined(__S60_32__)
        aMenuPane->SetItemDimmed(ETTGPSLoggerCommandToolSatellite, ETrue);
#endif // defined(__S60_50__) || defined(__S60_32__)
        //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandToolSatellite, ETrue);
        //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandToolGPSStatus, ETrue);
        //aMenuPane->SetItemDimmed(ETTGPSLoggerCommandToolNMEAViewer, ETrue);
        }
    }

void CTTGPSLoggerEngine::SetToolbarL()
    {
#ifdef __S60_50__
    CAknToolbar* toolbar = AppUi()->CurrentFixedToolbar();
    if (toolbar)
        {
        DynInitToolbarL(0, toolbar);
        }
#endif // __S60_50__
    }

#ifdef __S60_50__
void CTTGPSLoggerEngine::DynInitToolbarL(TInt aResourceId, CAknToolbar *aToolbar)
    {
    CAknToolbar* toolbar = aToolbar;
    if (toolbar)
        {
        switch (iPositionRecorder->Status())
            {
            case CTTGPSLoggerPositionRecorder::EStatusNone:
                {
                toolbar->HideItem(ETTGPSLoggerTBMainLock, EFalse, ETrue);
                toolbar->HideItem(ETTGPSLoggerTBMainStartNow, EFalse, EFalse);
                toolbar->HideItem(ETTGPSLoggerTBMainRecord, ETrue, EFalse);
                toolbar->HideItem(ETTGPSLoggerTBMainPause, ETrue, EFalse);
                toolbar->HideItem(ETTGPSLoggerTBMainResume, ETrue, EFalse);
                toolbar->SetItemDimmed(ETTGPSLoggerTBMainStop, ETrue, EFalse);
                toolbar->HideItem(ETTGPSLoggerTBMainStop, EFalse, EFalse);
                break;
                }
            case CTTGPSLoggerPositionRecorder::EStatusWaiting:
                {
                toolbar->HideItem(ETTGPSLoggerTBMainLock, EFalse, ETrue);
                toolbar->HideItem(ETTGPSLoggerTBMainStartNow, ETrue, EFalse);
                toolbar->HideItem(ETTGPSLoggerTBMainRecord, ETrue, EFalse);
                toolbar->HideItem(ETTGPSLoggerTBMainPause, ETrue, EFalse);
                toolbar->HideItem(ETTGPSLoggerTBMainResume, ETrue, EFalse);
                toolbar->SetItemDimmed(ETTGPSLoggerTBMainStop, EFalse, EFalse);
                toolbar->HideItem(ETTGPSLoggerTBMainStop, EFalse, EFalse);
                break;
                }
            case CTTGPSLoggerPositionRecorder::EStatusRecording:
            case CTTGPSLoggerPositionRecorder::EStatusAutoPausing:
                {
                toolbar->HideItem(ETTGPSLoggerTBMainLock, EFalse, ETrue);
                toolbar->HideItem(ETTGPSLoggerTBMainStartNow, ETrue, EFalse);
                if (Settings()->OutputIntervals() == 0 && Settings()->OutputDistance() == 0)
                    {
                    toolbar->HideItem(ETTGPSLoggerTBMainRecord, EFalse, EFalse);
                    toolbar->HideItem(ETTGPSLoggerTBMainPause, ETrue, EFalse);
                    }
                else
                    {
                    toolbar->HideItem(ETTGPSLoggerTBMainRecord, ETrue, EFalse);
                    toolbar->HideItem(ETTGPSLoggerTBMainPause, EFalse, EFalse);
                    }
                toolbar->HideItem(ETTGPSLoggerTBMainResume, ETrue, EFalse);
                toolbar->SetItemDimmed(ETTGPSLoggerTBMainStop, EFalse, EFalse);
                toolbar->HideItem(ETTGPSLoggerTBMainStop, EFalse, EFalse);
                break;
                }
            case CTTGPSLoggerPositionRecorder::EStatusPausing:
                {
                toolbar->HideItem(ETTGPSLoggerTBMainLock, EFalse, ETrue);
                toolbar->HideItem(ETTGPSLoggerTBMainStartNow, ETrue, EFalse);
                toolbar->HideItem(ETTGPSLoggerTBMainRecord, ETrue, EFalse);
                toolbar->HideItem(ETTGPSLoggerTBMainPause, ETrue, EFalse);
                toolbar->HideItem(ETTGPSLoggerTBMainResume, EFalse, EFalse);
                toolbar->SetItemDimmed(ETTGPSLoggerTBMainStop, EFalse, EFalse);
                toolbar->HideItem(ETTGPSLoggerTBMainStop, EFalse, EFalse);
                break;
                }
            default:
                break;
            }
        }
    }
#endif // __S60_50__

TTime CTTGPSLoggerEngine::InputStartTime()
    {
    TTime inputDate;
    TTime inputTime;
    inputDate.HomeTime();
    inputTime.HomeTime();
    CAknMultiLineDataQueryDialog* dialog = CAknMultiLineDataQueryDialog::NewL(inputDate, inputTime);
    if (dialog->ExecuteLD(R_TTGP_DIALOG_MENUSTART2))
        {
        TTime nowDateTime;
        TTime inputDateTime = inputDate.Int64() + inputTime.Int64();
        nowDateTime.HomeTime();
        if (inputDateTime < nowDateTime)
            {
            CAknInformationNote *note = new (ELeave) CAknInformationNote(EFalse);
            note->ExecuteLD(iMsgStartError1);
            return 0;
            }
        else
            {
            TLocale locale;
            TTimeIntervalSeconds universalTimeOffset(locale.UniversalTimeOffset());
            return inputDateTime - universalTimeOffset;
            }
        }
    else
        {
        return 0;
        }
    }
