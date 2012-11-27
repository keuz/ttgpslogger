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
#include <e32base.h>
#include <e32std.h>
#include <aknappui.h>
#include <aknnavi.h>
#include <aknnavide.h> 
#include <akntabgrp.h>
#include <barsread.h>
#include <TTGPSLogger.rsg>
#include "TTGPSLogger.hrh"
#include "TTGPSLoggerCommon.h"
#include "TTGPSLoggerSettingsView.h"
#include "TTGPSLoggerSettingsList.h"
#include "TTGPSLoggerSettingsContainer.h"

CTTGPSLoggerSettingsContainer::CTTGPSLoggerSettingsContainer(CTTGPSLoggerSettingsView* aParent) :
	iParent(aParent)
	{
	}

CTTGPSLoggerSettingsContainer::~CTTGPSLoggerSettingsContainer()
	{
    CEikStatusPane* sp = static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi())->StatusPane();
    CAknNavigationControlContainer* naviPane = static_cast<CAknNavigationControlContainer*>(sp->ControlL(TUid::Uid(EEikStatusPaneUidNavi)));
    
    naviPane->Pop(iNaviDecorator);
    delete iNaviDecorator;
    
    delete iSettingsListGeneral;
    delete iSettingsListDisplay;
    delete iSettingsListOutput;
    delete iSettingsListNMEA;
    delete iSettingsListGPX;
    delete iSettingsListKML;
	}

CTTGPSLoggerSettingsContainer* CTTGPSLoggerSettingsContainer::NewLC(CTTGPSLoggerSettingsView* aParent, const TRect& aRect)
	{
	CTTGPSLoggerSettingsContainer* self = new (ELeave) CTTGPSLoggerSettingsContainer(aParent);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
	}

CTTGPSLoggerSettingsContainer* CTTGPSLoggerSettingsContainer::NewL(CTTGPSLoggerSettingsView* aParent, const TRect& aRect)
	{
	CTTGPSLoggerSettingsContainer* self = CTTGPSLoggerSettingsContainer::NewLC(aParent, aRect);
	CleanupStack::Pop(); // self;
	return self;
	}

void CTTGPSLoggerSettingsContainer::ConstructL(const TRect& aRect)
	{
    // Create a window for this application view
    CreateWindowL();
    SetMopParent(iParent);
    
    CEikStatusPane* sp = static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi())->StatusPane();
    CAknNavigationControlContainer* naviPane = static_cast<CAknNavigationControlContainer*>(sp->ControlL(TUid::Uid(EEikStatusPaneUidNavi)));
    
    TResourceReader rr;
    CEikonEnv::Static()->CreateResourceReaderLC(rr, R_TTGP_NAVIDECORATOR_SETTINGS);
    iNaviDecorator = naviPane->ConstructNavigationDecoratorFromResourceL(rr);
    CleanupStack::PopAndDestroy(); // rr
    iTabGroup = static_cast<CAknTabGroup*>(iNaviDecorator->DecoratedControl());
    iTabGroup->SetActiveTabByIndex(0);
    naviPane->PushL(*iNaviDecorator);
    
    iSettingsListNMEA = CTTGPSLoggerSettingsList::NewL(iParent, aRect, 3);
    iSettingsListGeneral = CTTGPSLoggerSettingsList::NewL(iParent, aRect, 0);
    iSettingsListDisplay = CTTGPSLoggerSettingsList::NewL(iParent, aRect, 1);
    iSettingsListOutput = CTTGPSLoggerSettingsList::NewL(iParent, aRect, 2);
    iSettingsListGPX = CTTGPSLoggerSettingsList::NewL(iParent, aRect, 4);
    iSettingsListKML = CTTGPSLoggerSettingsList::NewL(iParent, aRect, 5);
    iSettingsListGeneral->MakeVisible(ETrue);
    iSettingsListDisplay->MakeVisible(EFalse);
    iSettingsListOutput->MakeVisible(EFalse);
    iSettingsListNMEA->MakeVisible(EFalse);
    iSettingsListGPX->MakeVisible(EFalse);
    iSettingsListKML->MakeVisible(EFalse);
    iSettingsListGeneral->DrawDeferred();
    
    SetRect(aRect);
    ActivateL();
    //DrawNow();
	}

TKeyResponse CTTGPSLoggerSettingsContainer::OfferKeyEventL(const TKeyEvent &aKeyEvent, TEventCode aType)
    {
    TKeyResponse res = EKeyWasNotConsumed;
    
    switch (aKeyEvent.iCode)
        {
        case EKeyRightArrow:
            {
            if (iTabGroup)
                {
                TInt i1 = iTabGroup->ActiveTabIndex();
                i1 = (i1 >= (iTabGroup->TabCount() - 1)) ? (iTabGroup->TabCount() - 1) : (i1 + 1);
                iTabGroup->SetActiveTabByIndex(i1);
                iSettingsListGeneral->MakeVisible(EFalse);
                iSettingsListDisplay->MakeVisible(EFalse);
                iSettingsListOutput->MakeVisible(EFalse);
                iSettingsListNMEA->MakeVisible(EFalse);
                iSettingsListGPX->MakeVisible(EFalse);
                iSettingsListKML->MakeVisible(EFalse);
                if (ComponentControl(i1))
                    {
                    ComponentControl(i1)->MakeVisible(ETrue);
                    }
                DrawNow();
                res = EKeyWasConsumed;
                }
            break;
            }
        case EKeyLeftArrow:
            {
            if (iTabGroup)
                {
                TInt i1 = iTabGroup->ActiveTabIndex();
                i1 = (i1 <= 0) ? 0 : (i1 - 1);
                iTabGroup->SetActiveTabByIndex(i1);
                iSettingsListGeneral->MakeVisible(EFalse);
                iSettingsListDisplay->MakeVisible(EFalse);
                iSettingsListOutput->MakeVisible(EFalse);
                iSettingsListNMEA->MakeVisible(EFalse);
                iSettingsListGPX->MakeVisible(EFalse);
                iSettingsListKML->MakeVisible(EFalse);
                if (ComponentControl(i1))
                    {
                    ComponentControl(i1)->MakeVisible(ETrue);
                    }
                DrawNow();
                res = EKeyWasConsumed;
                }
            break;
            }
        default:
            {
            if (iTabGroup)
                {
                if (ComponentControl(iTabGroup->ActiveTabIndex()))
                    {
                    static_cast<CTTGPSLoggerSettingsList*>(ComponentControl(iTabGroup->ActiveTabIndex()))->OfferKeyEventL(aKeyEvent, aType);
                    }
                }
            break;
            }
        }
    
    return res;
    }

TInt CTTGPSLoggerSettingsContainer::CountComponentControls() const
    {
    TInt count = 0;
    if (iTabGroup)
        {
        count = 5;
        }
    return count;
    }

CCoeControl* CTTGPSLoggerSettingsContainer::ComponentControl(TInt aIndex) const
    {
    CCoeControl* control = NULL;
    switch (aIndex)
        {
        case ETTGPSLoggerSettingsTabGeneral:
            {
            control = iSettingsListGeneral;
            break;
            }
        case ETTGPSLoggerSettingsTabDisplay:
            {
            control = iSettingsListDisplay;
            break;
            }
        case ETTGPSLoggerSettingsTabOutput:
            {
            control = iSettingsListOutput;
            break;
            }
        case ETTGPSLoggerSettingsTabNMEA:
            {
            control = iSettingsListNMEA;
            break;
            }
        case ETTGPSLoggerSettingsTabGPX:
            {
            control = iSettingsListGPX;
            break;
            }
        case ETTGPSLoggerSettingsTabKML:
            {
            control = iSettingsListKML;
            break;
            }
        default:
            {
            control = NULL;
            break;
            }
        }
    return control;
    }

void CTTGPSLoggerSettingsContainer::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    CCoeControl::HandlePointerEventL(aPointerEvent);
    }


void CTTGPSLoggerSettingsContainer::HandleResourceChange(TInt aType)
    {
    CCoeControl::HandleResourceChange(aType);

    switch(aType)
    	{
    	case KEikDynamicLayoutVariantSwitch:
    		{
    		TRect rect;
            AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, rect);
            SetRect(rect);
            //AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EScreen, rect);
            //iSettingsListGeneral->SetRect(rect);
            //iSettingsListDisplay->SetRect(rect);
            //iSettingsListOutput->SetRect(rect);
            //iSettingsListNMEA->SetRect(rect);
            //iSettingsListGPX->SetRect(rect);
            //iSettingsListKML->SetRect(rect);
			break;
    		}
    	default:
    		break;
    	}
    }

void CTTGPSLoggerSettingsContainer::SizeChanged()
    {
    CCoeControl::SizeChanged();
    //CEikStatusPane* sp = static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi())->StatusPane();
    //CAknNavigationControlContainer* naviPane = static_cast<CAknNavigationControlContainer*>(sp->ControlL(TUid::Uid(EEikStatusPaneUidNavi)));
    //naviPane->SetRect(Rect());
    //iSettingsListGeneral->SetRect(Rect());
    //iSettingsListDisplay->SetRect(Rect());
    //iSettingsListOutput->SetRect(Rect());
    //iSettingsListNMEA->SetRect(Rect());
    //iSettingsListGPX->SetRect(Rect());
    //iSettingsListKML->SetRect(Rect());
    }

void CTTGPSLoggerSettingsContainer::ChangeSelectedItemL()
    {
    if (iTabGroup)
        {
        if (ComponentControl(iTabGroup->ActiveTabIndex()))
            {
            static_cast<CTTGPSLoggerSettingsList*>(ComponentControl(iTabGroup->ActiveTabIndex()))->ChangeSelectedItemL();
            }
        }
    }

void CTTGPSLoggerSettingsContainer::Draw(const TRect& /*aRect*/) const
{
    CWindowGc& gc = SystemGc();
    gc.Clear(Rect());
    }
