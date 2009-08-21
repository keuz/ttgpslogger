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
#include <e32std.h>
#include <akncheckboxsettingpage.h>
#include <aknsettingitemlist.h>
#include <aknsettingpage.h>
#include <TTGPSLogger.rsg>
//#include "TTGPSLogger.hrh"
#include "TTGPSLoggerCheckBoxSettingItem.h"

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
