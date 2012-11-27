/*
 * TTGPSLogger
 * 
 * TTGPSLogger, a GPS logger for Symbian S60 smartphones.
 * Copyright (C) 2009 TTINPUT <ttinputdiary@ovi.com>
 * 
 * Updated by amacri@tiscali.it
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
#define DUMP_DISKS

#include <e32base.h>
#include <e32std.h>
#include <akncheckboxsettingpage.h>
#include <aknsettingitemlist.h>
#include <aknsettingpage.h>
#include <TTGPSLogger.rsg>
#include <aknnotewrappers.h>
#include <aknmessagequerydialog.h>
#include <pathinfo.h>
#include <bautils.h>
//#include "TTGPSLogger.hrh"
#include "TTGPSLoggerCheckBoxSettingItem.h"
#include "TTGPSLoggerAppUi.h"
#include "TTGPSLoggerEngine.h"
#include "TTGPSLoggerCommon.h"
#include <CAknFileSelectionDialog.h>
#define C_APPUI static_cast<CTTGPSLoggerAppUi*>(CEikonEnv::Static()->EikAppUi())
#include <lbs.h>
#include "TTGPSLoggerPositionRequester.h"

CTTGPSLoggerCheckBoxSettingItem::CTTGPSLoggerCheckBoxSettingItem(TInt aIdentifier, CArrayFix<TInt>& aValueArray) :
    CAknEnumeratedTextSettingItem(aIdentifier),
    iValueArray(aValueArray)
	{
	}

CTTGPSLoggerCheckBoxSettingItem::~CTTGPSLoggerCheckBoxSettingItem()
	{
	if (iSelectionItemList)
		{
		iSelectionItemList->ResetAndDestroy();
		delete iSelectionItemList;
		}
	}

void CTTGPSLoggerCheckBoxSettingItem::CompleteConstructionL()
    {
    CAknEnumeratedTextSettingItem::CompleteConstructionL();

	iSelectionItemList = new (ELeave) CSelectionItemList(32);
	TInt count = EnumeratedTextArray()->Count();
	TInt numValueArray = iValueArray.Count();

	for (TInt i1 = 0; i1 < count; i1++)
		{
		CSelectableItem* item = new (ELeave) CSelectableItem(*(EnumeratedTextArray()->At(i1)->Value()), EFalse);
		CleanupStack::PushL(item);
		
		item->ConstructL();
		if (i1 < numValueArray)
			{
			item->SetSelectionStatus(iValueArray[i1]);
			}
		
		CleanupStack::Pop(item);
		iSelectionItemList->AppendL(item);
		}
    }

void CTTGPSLoggerCheckBoxSettingItem::LoadL()
    {
	TInt numValueArray = iValueArray.Count();
	for (TInt i1 = 0; i1 < numValueArray; i1++)
		{
		(*iSelectionItemList)[i1]->SetSelectionStatus(iValueArray[i1]);
		}
    }

void CTTGPSLoggerCheckBoxSettingItem::EditItemL(TBool aCalledFromMenu)
    {
    TInt res = SettingPageResourceId();
    
	CAknCheckBoxSettingPage* settingPage = new (ELeave) CAknCheckBoxSettingPage(SettingPageResourceId(), iSelectionItemList);
	CleanupStack::PushL(settingPage);
	SetSettingPage(settingPage);
	SettingPage()->SetSettingPageObserver(this);
	//SettingPage()->SetSettingPageObserver(static_cast<MAknSettingPageObserver*>(this));
	SetUpStandardSettingPageL();
	CleanupStack::Pop(settingPage);
	TBool err = SettingPage()->ExecuteLD(CAknSettingPage::EUpdateWhenAccepted);
	SetSettingPage(NULL);
	if (err)
		{
		UpdateListBoxTextL();
		}
    }

void CTTGPSLoggerCheckBoxSettingItem::StoreL()
    {
	TInt numValueArray = iValueArray.Count();
	for (TInt i1 = 0; i1 < numValueArray; i1++)
		{
		iValueArray[i1] = (*iSelectionItemList)[i1]->SelectionStatus();
		}
    }

const TDesC& CTTGPSLoggerCheckBoxSettingItem::SettingTextL()
    {
    _LIT(KText1Space, " ");
    
    return KText1Space;
    }

void CTTGPSLoggerCheckBoxSettingItem::HandleSettingPageEventL(CAknSettingPage *aSettingPage, TAknSettingPageEvent aEventType)
    {

	HBufC* iMsgSettingChanged = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF_MSG_SETTING_CHANGED);
	CAknConfirmationNote *note = new (ELeave) CAknConfirmationNote(EFalse);
	note->ExecuteLD(*iMsgSettingChanged );
    delete iMsgSettingChanged;

    CAknEnumeratedTextSettingItem::HandleSettingPageEventL(aSettingPage, aEventType);
	if (aEventType == MAknSettingPageObserver::EEventSettingOked)
		{
		//
		}
	else if (aEventType == MAknSettingPageObserver::EEventSettingCancelled)
		{
		LoadL();
		}
    }


//------------------------------------------------------------------------------------------------------------------------

const TDesC& CTTGPSLoggerSelectMemorySettingItem::SettingTextL() // show text
    {
	iDispl.Zero();
    CDesC16Array *itemArray = CCoeEnv::Static()->ReadDesCArrayResourceL(R_TTGP_ARRAY_DIALOG_MEMORY1);
    CleanupStack::PushL(itemArray);
	iDispl.Copy((*itemArray)[iMemory]);
	iDispl.Append(_L(" ["));
	switch (iMemory)
		{
		case 2:
			iDispl.Append(PathInfo::MemoryCardRootPath());
			break;
		case 3:
			iDispl.Append(_L("f:\\"));
			break;
		default: // 1
			iDispl.Append(PathInfo::PhoneMemoryRootPath());
			break;
		}
	iDispl.Append(_L("]"));
	CleanupStack::PopAndDestroy(); // itemArray
	return(iDispl);
    }

CTTGPSLoggerSelectMemorySettingItem::CTTGPSLoggerSelectMemorySettingItem(TInt aIdentifier, TInt& aMemory):
		CAknSettingItem( aIdentifier ),
		iMemory(aMemory)
	{
	}

CTTGPSLoggerSelectMemorySettingItem::~CTTGPSLoggerSelectMemorySettingItem()
	{
	}

void CTTGPSLoggerSelectMemorySettingItem::EditItemL(TBool aCalledFromMenu) // edit text
    {
    CDesC16Array *itemArray = CCoeEnv::Static()->ReadDesCArrayResourceL(R_TTGP_ARRAY_DIALOG_MEMORY1);
    CleanupStack::PushL(itemArray);
	if(!TTGPSLoggerCommon::CheckDiskL(EDriveF, EFalse))
		itemArray->Delete(3);
	if(!TTGPSLoggerCommon::CheckDiskL(EDriveE, EFalse))
		itemArray->Delete(2);
	if(!TTGPSLoggerCommon::CheckDiskL(EDriveC, EFalse))
		itemArray->Delete(1);
    TInt selectIndex;
    selectIndex=iMemory;
    HBufC* title = NULL;
	title = CEikonEnv::Static()->AllocReadResourceLC(R_TTGP_TBUF32_DIALOG_MEMORY_TITLE);
    CAknListQueryDialog* dialog = new (ELeave) CAknListQueryDialog(&selectIndex);
    dialog->PrepareLC(R_TTGP_DIALOG_LISTQUERY);
    dialog->QueryHeading()->SetTextL(*title);
    dialog->SetItemTextArray(itemArray);
    dialog->SetOwnershipType(ELbmDoesNotOwnItemArray);
    TInt answer = dialog->RunLD();
    CleanupStack::PopAndDestroy(); // title
    if (answer)
		switch (selectIndex)
			{
			case 0: // (Drive info)
				{
				// drive info
				TTGPSLoggerCommon::CheckDiskL(-1, ETrue);
				break;
				}
			case 1: // Phone Memory
				if(TTGPSLoggerCommon::CheckDiskL(EDriveC, ETrue))
					iMemory=selectIndex;
				break;
			case 2: // Memory Card
				if(TTGPSLoggerCommon::CheckDiskL(EDriveE, ETrue))
					iMemory=selectIndex;
				break;
			case 3: // Mass Memory
				if(TTGPSLoggerCommon::CheckDiskL(EDriveF, ETrue))
					iMemory=selectIndex;
				break;
			}
	UpdateListBoxTextL();
    CleanupStack::PopAndDestroy(); // itemArray
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef POSMETH

const TDesC& CTTGPSLoggerPositioningMethod::SettingTextL() // show text
    {
	CDesC16ArrayFlat *itemArray = new (ELeave) CDesC16ArrayFlat(5);
	CleanupStack::PushL(itemArray); // in case the appends leave
	RPositionServer iPositionServer;
	RPositioner iPositioner;
	User::LeaveIfError(iPositionServer.Connect());
	User::LeaveIfError(iPositioner.Open(iPositionServer));
	TUint aModules = 0;

	// Get the available numbers of modules
	User::LeaveIfError(iPositionServer.GetNumModules(aModules));
    TPositionModuleInfo info;

	// Prepare itemArray and itemArrayDetails
	itemArray->AppendL(_L("(Default)"));
    for(TUint i = 0; i < aModules; i++)
    	{
    	if ((iPositionServer.GetModuleInfoByIndex(i,info) == KErrNone) && (info.IsAvailable()))
    		{
    		TBuf<200> smallBuffer;
    		smallBuffer.Zero();
    		info.GetModuleName(smallBuffer);
    		itemArray->AppendL(smallBuffer);
    		}
    	}	
	iPositioner.Close();
	iPositionServer.Close();

	iDispl.Zero();
	if ((iPosMethod>aModules)||(iPosMethod<0))
		iPosMethod=0; // Default
	iDispl.Copy((*itemArray)[iPosMethod]);
	CleanupStack::PopAndDestroy(); // itemArray
	return(iDispl);
    }

CTTGPSLoggerPositioningMethod::CTTGPSLoggerPositioningMethod(TInt aIdentifier, TInt& aPosMethod):
		CAknSettingItem( aIdentifier ),
		iPosMethod(aPosMethod)
	{
	}

CTTGPSLoggerPositioningMethod::~CTTGPSLoggerPositioningMethod()
	{
	}

void CTTGPSLoggerPositioningMethod::EditItemL(TBool aCalledFromMenu) // edit text
    {
	CDesC16ArrayFlat *itemArray = new (ELeave) CDesC16ArrayFlat(20);
	CDesC16ArrayFlat *itemArrayDetails = new (ELeave) CDesC16ArrayFlat(20);
	CleanupStack::PushL(itemArray); // in case the appends leave
	CleanupStack::PushL(itemArrayDetails); // in case the appends leave
	RPositionServer iPositionServer;
	RPositioner iPositioner;
	User::LeaveIfError(iPositionServer.Connect());
	User::LeaveIfError(iPositioner.Open(iPositionServer));
	itemArray->AppendL(_L("(Info on all modules)"));
	itemArray->AppendL(_L("(Default)"));
	TUint aModules = 0;
	TInt defaultModule=-1;

	// Get the available numbers of modules
	User::LeaveIfError(iPositionServer.GetNumModules(aModules));
    TPositionModuleInfo info;
	TPositionModuleId aModuleId;
	iPositionServer.GetDefaultModuleId(aModuleId);

	// Prepare itemArray and itemArrayDetails
    for(TUint i = 0; i < aModules; i++)
    	{
    	if ((iPositionServer.GetModuleInfoByIndex(i,info) == KErrNone) && (info.IsAvailable()))
    		{
    		TBuf<200> smallBuffer;
    		smallBuffer.Zero();
    		info.GetModuleName(smallBuffer);
    		itemArray->AppendL(smallBuffer);

			TBuf<200> buffer;
			buffer.Zero();
			info.GetModuleName(buffer);
			buffer.Append(_L(" ("));
			if (info.DeviceLocation() & TPositionModuleInfo::EDeviceExternal)
				buffer.Append(_L("External "));
			if (info.DeviceLocation() & TPositionModuleInfo::EDeviceInternal)
				buffer.Append(_L("Internal "));
			if (info.DeviceLocation() & TPositionModuleInfo::EDeviceUnknown)
				buffer.Append(_L("Unknown "));
			if (info.TechnologyType()& TPositionModuleInfo::ETechnologyAssisted)
				buffer.Append(_L("Assisted "));
			if (info.TechnologyType()& TPositionModuleInfo::ETechnologyTerminal)
				buffer.Append(_L("GPS, "));
			if (info.TechnologyType()& TPositionModuleInfo::ETechnologyNetwork)
				buffer.Append(_L("Network, "));
			if (info.TechnologyType()& TPositionModuleInfo::ETechnologyUnknown)
				buffer.Append(_L("Unknown tech, "));
			if (buffer.Mid(buffer.Length()-2, 2).Compare( _L(", ")) == KErrNone)
				buffer.Replace(buffer.Length()-2, 2, _L(")"));
			if (buffer.Mid(buffer.Length()-1, 1).Compare( _L(" "))  == KErrNone)
				buffer.Replace(buffer.Length()-1, 1, _L(")"));
			if ( info.ModuleId() == aModuleId)
				{
				buffer.Append(_L(" [Default]"));
				defaultModule=i;
				}
    		itemArrayDetails->AppendL(buffer);    		
    		}
    	}	
	iPositioner.Close();
	iPositionServer.Close();

	// Present list of positioning methods
	TInt selectIndex;
    selectIndex=iPosMethod+1;
    HBufC* title = NULL;
	title = CEikonEnv::Static()->AllocReadResourceLC(R_TTGP_TBUF32_DIALOG_POSITIONING_TITLE); // Select positioning method:
    CAknListQueryDialog* dialog = new (ELeave) CAknListQueryDialog(&selectIndex);
    dialog->PrepareLC(R_TTGP_DIALOG_LISTQUERY);
    dialog->QueryHeading()->SetTextL(*title);
    dialog->SetItemTextArray(itemArray);
    dialog->SetOwnershipType(ELbmDoesNotOwnItemArray);
    TInt answer = dialog->RunLD();
    CleanupStack::PopAndDestroy(); // title
    // Check answer
    if (answer)
    	{
        if (selectIndex==0) // (Info on all modules)
    		{
    		TBuf<1000> aText;
    		aText.Zero();
    		for (TInt i=0;i<itemArrayDetails->Count();i++)
    			{
    			aText.AppendFormat(_L("%d. "), i+1);
    			aText.Append(itemArrayDetails->MdcaPoint(i));
    			if (i<itemArrayDetails->Count()-1)
    				aText.Append(_L("\n"));
    			}
    		HBufC* title = CEikonEnv::Static()->AllocReadResourceLC(R_TTGP_TBUF32_AVAILABLEPOSITIONING_TITLE); // Available modules:
    		CAknMessageQueryDialog* dialog = new (ELeave) CAknMessageQueryDialog();
    		CleanupStack::PushL(dialog);
    		dialog->PrepareLC(R_TTGP_DIALOG_MESSAGEQUERY_OK);
    		dialog->QueryHeading()->SetTextL(*title);
    		dialog->SetMessageTextL(aText);
    		CleanupStack::Pop(); // dialog
    		dialog->RunLD();
    		CleanupStack::PopAndDestroy(); // title
    		}
        else
        if (selectIndex==1) // Default module
        	{
        	if (defaultModule>-1)
        		{
        		// Present default module name
        		HBufC* title = CEikonEnv::Static()->AllocReadResourceLC(R_TTGP_TBUF32_SELECTEDPOSITIONING_TITLE); // Selected module:
        		CAknMessageQueryDialog* dialog = new (ELeave) CAknMessageQueryDialog();
        		CleanupStack::PushL(dialog);
        		dialog->PrepareLC(R_TTGP_DIALOG_MESSAGEQUERY_OK);
        		dialog->QueryHeading()->SetTextL(*title);
        		dialog->SetMessageTextL(itemArrayDetails->MdcaPoint(defaultModule));
        		CleanupStack::Pop(); // dialog
        		dialog->RunLD();
        		CleanupStack::PopAndDestroy(); // title
        		}
    		iPosMethod=0; // default module selected
    		C_APPUI->Engine()->Requester()->SetPositioningMethod(iPosMethod);
    		C_APPUI->Engine()->Requester()->StartL();
        	}
        else
        if (selectIndex>1) // Custom selection of the module
        	{
    		HBufC* title = CEikonEnv::Static()->AllocReadResourceLC(R_TTGP_TBUF32_SELECTEDPOSITIONING_TITLE); // Selected module:
    		CAknMessageQueryDialog* dialog = new (ELeave) CAknMessageQueryDialog();
    		CleanupStack::PushL(dialog);
    		dialog->PrepareLC(R_TTGP_DIALOG_MESSAGEQUERY_OK);
    		dialog->QueryHeading()->SetTextL(*title);
    		dialog->SetMessageTextL(itemArrayDetails->MdcaPoint(selectIndex-2));
    		CleanupStack::Pop(); // dialog
    		dialog->RunLD();
    		CleanupStack::PopAndDestroy(); // title
    		iPosMethod=selectIndex-1;
    		C_APPUI->Engine()->Requester()->SetPositioningMethod(iPosMethod);
    		C_APPUI->Engine()->Requester()->StartL();
    		}
    	UpdateListBoxTextL();
    	}
    CleanupStack::PopAndDestroy(); // itemArrayDetails
    CleanupStack::PopAndDestroy(); // itemArray
    }

#endif
