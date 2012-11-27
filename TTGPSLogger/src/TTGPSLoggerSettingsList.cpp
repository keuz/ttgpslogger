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
#include <caknmemoryselectionsettingitem.h> 
#include <e32base.h>
#include <e32debug.h>
#include <e32std.h>
#include <akntextsettingpage.h>
#include <avkon.hrh>
#include <TTGPSLogger.rsg>
#include "TTGPSLogger.hrh"
#include "TTGPSLoggerAppUi.h"
#include "TTGPSLoggerCheckBoxSettingItem.h"
#include "TTGPSLoggerEngine.h"
#include "TTGPSLoggerSettingsView.h"
#include "TTGPSLoggerSettingsData.h"
#include "TTGPSLoggerSettingsList.h"

CTTGPSLoggerSettingsList::CTTGPSLoggerSettingsList(CTTGPSLoggerSettingsView* aParent, TInt aTabIndex) :
    iParent(aParent),
    iTabIndex(aTabIndex)
	{
	}

CTTGPSLoggerSettingsList::~CTTGPSLoggerSettingsList()
	{
	}

CTTGPSLoggerSettingsList* CTTGPSLoggerSettingsList::NewLC(CTTGPSLoggerSettingsView* aParent, const TRect& aRect, TInt aTabIndex)
	{
	CTTGPSLoggerSettingsList* self = new (ELeave) CTTGPSLoggerSettingsList(aParent, aTabIndex);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
	}

CTTGPSLoggerSettingsList* CTTGPSLoggerSettingsList::NewL(CTTGPSLoggerSettingsView* aParent, const TRect& aRect, TInt aTabIndex)
	{
	CTTGPSLoggerSettingsList* self = CTTGPSLoggerSettingsList::NewLC(aParent, aRect, aTabIndex);
	CleanupStack::Pop(); // self;
	return self;
	}

void CTTGPSLoggerSettingsList::ConstructL(const TRect& aRect)
	{
	switch (iTabIndex)
	    {
	    case ETTGPSLoggerSettingsTabGeneral:
	        {
	        CAknSettingItemList::ConstructFromResourceL(R_TTGP_SETTINGITEMLIST_GENERAL);
	        break;
	        }
	    case ETTGPSLoggerSettingsTabDisplay:
	        {
	        CAknSettingItemList::ConstructFromResourceL(R_TTGP_SETTINGITEMLIST_DISPLAY);
	        break;
	        }
	    case ETTGPSLoggerSettingsTabOutput:
	        {
	        CAknSettingItemList::ConstructFromResourceL(R_TTGP_SETTINGITEMLIST_OUTPUT);
	        break;
	        }
        case ETTGPSLoggerSettingsTabNMEA:
            {
            CAknSettingItemList::ConstructFromResourceL(R_TTGP_SETTINGITEMLIST_NMEA);
            break;
            }
        case ETTGPSLoggerSettingsTabGPX:
            {
            CAknSettingItemList::ConstructFromResourceL(R_TTGP_SETTINGITEMLIST_GPX);
            break;
            }
        case ETTGPSLoggerSettingsTabKML:
            {
            CAknSettingItemList::ConstructFromResourceL(R_TTGP_SETTINGITEMLIST_KML);
            break;
            }
	    }
    SetMopParent(iParent);
    LoadSettingsL();
    SetRect(aRect);
    ActivateL();
	}

CAknSettingItem* CTTGPSLoggerSettingsList::CreateSettingItemL(TInt aId)
    {
    CAknSettingItem* item = NULL;
    CTTGPSLoggerSettingsData* settings = static_cast<CTTGPSLoggerAppUi*>(CEikonEnv::Static()->EikAppUi())->Engine()->Settings();
    
    switch (iTabIndex)
        {

    	case ETTGPSLoggerSettingsTabGeneral:
            {
            switch (aId)
                {
#ifdef DSYSTEM
                case ETTGPSLoggerSettingItemGeneralSystem:
                    {
                    item = new (ELeave) CAknBinaryPopupSettingItem(aId, settings->GetGeneralSystem());
                    break;
                    }
#endif
#ifdef POSMETH
                case ETTGPSLoggerSettingItemGeneralPositioningMethod:
                    {
                    item = new (ELeave) CTTGPSLoggerPositioningMethod(aId, settings->GetPositioningMethod());
                    break;
                    }
#endif
                }
			break;
			}
    	
    	case ETTGPSLoggerSettingsTabDisplay:
            {
            switch (aId)
                {
                case ETTGPSLoggerSettingItemDisplayMeasurementSystem:
                    {
                    item = new (ELeave) CAknEnumeratedTextPopupSettingItem(aId, settings->GetDisplayMeasurementSystem());
                    break;
                    }
                case ETTGPSLoggerSettingItemDisplayCoordinateFormat:
                    {
                    item = new (ELeave) CAknEnumeratedTextPopupSettingItem(aId, settings->GetDisplayCoordinateFormat());
                    break;
                    }
                case ETTGPSLoggerSettingItemDisplayItems:
                    {
                    item = new (ELeave) CTTGPSLoggerCheckBoxSettingItem(aId, settings->GetDisplayItems());
                    break;
                    }
                case ETTGPSLoggerSettingItemDisplayShortcuts:
                    {
                    item = new (ELeave) CAknBinaryPopupSettingItem(aId, settings->GetDisplayShortcuts());
                    break;
                    }
                }
            break;
            }
        case ETTGPSLoggerSettingsTabOutput:
            {
            switch (aId)
                {
                case ETTGPSLoggerSettingItemOutputAutostart:
                    {
                    item = new (ELeave) CAknEnumeratedTextPopupSettingItem(aId, settings->GetOutputAutostart());
                    break;
                    }
                case ETTGPSLoggerSettingItemOutputMemory:
                    {
    		    	//item = new (ELeave) CAknMemorySelectionSettingItem(aId, settings->GetOutputMemory());
					item = new (ELeave) CTTGPSLoggerSelectMemorySettingItem(aId, settings->GetOutputMemory());
                    break;
                    }
                case ETTGPSLoggerSettingItemOutputIntervals:
                    {
                    item = new (ELeave) CAknEnumeratedTextPopupSettingItem(aId, settings->GetOutputIntervals());
                    break;
                    }
                case ETTGPSLoggerSettingItemOutputDistance:
                    {
                    item = new (ELeave) CAknEnumeratedTextPopupSettingItem(aId, settings->GetOutputDistance());
                    break;
                    }
                case ETTGPSLoggerSettingItemOutputAutoPauseHSpeed:
                    {
                    item = new (ELeave) CAknEnumeratedTextPopupSettingItem(aId, settings->GetOutputAutoPauseHSpeed());
                    break;
                    }
                case ETTGPSLoggerSettingItemOutputAutoPauseSpeed:
                    {
                    item = new (ELeave) CAknEnumeratedTextPopupSettingItem(aId, settings->GetOutputAutoPauseSpeed());
                    break;
                    }
                case ETTGPSLoggerSettingItemOutputAutoPauseHAcc:
                    {
                    item = new (ELeave) CAknEnumeratedTextPopupSettingItem(aId, settings->GetOutputAutoPauseHAcc());
                    break;
                    }
                case ETTGPSLoggerSettingItemOutputAutoPauseHDOP:
                    {
                    item = new (ELeave) CAknEnumeratedTextPopupSettingItem(aId, settings->GetOutputAutoPauseHDOP());
                    break;
                    }
                case ETTGPSLoggerSettingItemOutputAutoPausePDOP:
                    {
                    item = new (ELeave) CAknEnumeratedTextPopupSettingItem(aId, settings->GetOutputAutoPausePDOP());
                    break;
                    }
                }
            break;
            }
        case ETTGPSLoggerSettingsTabNMEA:
            {
            switch (aId)
                {
                case ETTGPSLoggerSettingItemNMEAExt:
                    {
                    item = new (ELeave) CAknTextSettingItem(aId, settings->GetNMEAExt());
                    item->SetSettingPageFlags(CAknTextSettingPage::EPredictiveTextEntryPermitted);
                    break;
                    }
                case ETTGPSLoggerSettingItemNMEASentences:
                    {
                    item = new (ELeave) CTTGPSLoggerCheckBoxSettingItem(aId, settings->GetNMEASentences());
                    break;
                    }
                }
            break;
            }
        case ETTGPSLoggerSettingsTabGPX:
            {
            switch (aId)
                {
                case ETTGPSLoggerSettingItemGPXExt:
                    {
                    item = new (ELeave) CAknTextSettingItem(aId, settings->GetGPXExt());
                    item->SetSettingPageFlags(CAknTextSettingPage::EPredictiveTextEntryPermitted);
                    break;
                    }
                case ETTGPSLoggerSettingItemGPXTags:
                    {
                    item = new (ELeave) CTTGPSLoggerCheckBoxSettingItem(aId, settings->GetGPXTags());
                    break;
                    }
                case ETTGPSLoggerSettingItemGPXAuthor:
                    {
                    item = new (ELeave) CAknTextSettingItem(aId, settings->GetGPXAuthor());
                    item->SetSettingPageFlags(CAknTextSettingPage::EPredictiveTextEntryPermitted);
                    break;
                    }
                case ETTGPSLoggerSettingItemGPXEmail:
                    {
                    item = new (ELeave) CAknTextSettingItem(aId, settings->GetGPXEmail());
                    item->SetSettingPageFlags(CAknTextSettingPage::EPredictiveTextEntryPermitted);
                    break;
                    }
                case ETTGPSLoggerSettingItemGPXUrl:
                    {
                    item = new (ELeave) CAknTextSettingItem(aId, settings->GetGPXUrl());
                    item->SetSettingPageFlags(CAknTextSettingPage::EPredictiveTextEntryPermitted);
                    break;
                    }
                case ETTGPSLoggerSettingItemGPXUrlName:
                    {
                    item = new (ELeave) CAknTextSettingItem(aId, settings->GetGPXUrlName());
                    item->SetSettingPageFlags(CAknTextSettingPage::EPredictiveTextEntryPermitted);
                    break;
                    }
                case ETTGPSLoggerSettingItemGPXSegment:
                    {
                    item = new (ELeave) CAknBinaryPopupSettingItem(aId, settings->GetGpxSegment());
                    break;
                    }
                }
            break;
            }
        case ETTGPSLoggerSettingsTabKML:
            {
            switch (aId)
                {
                case ETTGPSLoggerSettingItemKMLExt:
                    {
                    item = new (ELeave) CAknTextSettingItem(aId, settings->GetKMLExt());
                    item->SetSettingPageFlags(CAknTextSettingPage::EPredictiveTextEntryPermitted);
                    break;
                    }
                case ETTGPSLoggerSettingItemKMLPmName:
                    {
                    item = new (ELeave) CAknEnumeratedTextPopupSettingItem(aId, settings->GetKMLPmName());
                    break;
                    }
                case ETTGPSLoggerSettingItemKMLPmDescription:
                    {
                    item = new (ELeave) CTTGPSLoggerCheckBoxSettingItem(aId, settings->GetKMLPmDescription());
                    break;
                    }
                }
            break;
            }
        }
    
    return item;
    }

void CTTGPSLoggerSettingsList::EditItemL(TInt aIndex, TBool aCalledFromMenu)
    {
    CAknSettingItemList::EditItemL(aIndex, aCalledFromMenu);
    (*SettingItemArray())[aIndex]->StoreL();
    if ((*SettingItemArray())[aIndex]->Identifier() == ETTGPSLoggerSettingItemNMEASentences ||
        (*SettingItemArray())[aIndex]->Identifier() == ETTGPSLoggerSettingItemGPXTags)
    	{
    	static_cast<CTTGPSLoggerAppUi*>(CEikonEnv::Static()->EikAppUi())->Engine()->Settings()->CheckL();
        (*SettingItemArray())[aIndex]->LoadL();
    	}
    static_cast<CTTGPSLoggerAppUi*>(CEikonEnv::Static()->EikAppUi())->Engine()->Settings()->SaveL();
    }

void CTTGPSLoggerSettingsList::HandleResourceChange(TInt aType)
    {
    CAknSettingItemList::HandleResourceChange(aType);
    SetRect(iParent->ClientRect());
    }

TKeyResponse CTTGPSLoggerSettingsList::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
    {
    if (aKeyEvent.iCode == EKeyLeftArrow || aKeyEvent.iCode == EKeyRightArrow)
        {
        return EKeyWasNotConsumed;
        }
    return CAknSettingItemList::OfferKeyEventL(aKeyEvent, aType);
    }

void CTTGPSLoggerSettingsList::SizeChanged()
    {
    if (ListBox())
        {
        ListBox()->SetRect(Rect());
        }
    }

void CTTGPSLoggerSettingsList::ChangeSelectedItemL()
    {
    if (ListBox()->CurrentItemIndex() >= 0)
        {
        EditItemL(ListBox()->CurrentItemIndex(), ETrue);
        }
    }
