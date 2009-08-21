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
#include <TTGPSLogger.rsg>
#include "TTGPSLogger.hrh"
#include "TTGPSLogger.pan"
#include "TTGPSLoggerCommon.h"
#include "TTGPSLoggerAppUi.h"
#include "TTGPSLoggerEngine.h"
#include "TTGPSLoggerSettingsContainer.h"
#include "TTGPSLoggerSettingsView.h"

CTTGPSLoggerSettingsView::CTTGPSLoggerSettingsView()
	{
	}

CTTGPSLoggerSettingsView::~CTTGPSLoggerSettingsView()
	{
	}

CTTGPSLoggerSettingsView* CTTGPSLoggerSettingsView::NewLC()
	{
	CTTGPSLoggerSettingsView* self = new (ELeave) CTTGPSLoggerSettingsView();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CTTGPSLoggerSettingsView* CTTGPSLoggerSettingsView::NewL()
	{
	CTTGPSLoggerSettingsView* self = CTTGPSLoggerSettingsView::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CTTGPSLoggerSettingsView::ConstructL()
    {
    BaseConstructL(R_TTGP_VIEW_SETTINGS);
    }

TUid CTTGPSLoggerSettingsView::Id() const
    {
    return TUid::Uid(ETTGPSLoggerViewSettingsId);
    }

void CTTGPSLoggerSettingsView::HandleCommandL(TInt aCommand)
    {
    switch (aCommand)
        {
        case EEikCmdExit:
        case EAknSoftkeyExit:
            {
            AppUi()->Exit();
            break;
            }
        case EAknSoftkeySelect:
            {
            if (iSettingsContainer)
                {
                iSettingsContainer->ChangeSelectedItemL();
                }
            break;
            }
        case EAknSoftkeyCancel:
            {
            TTGPSLoggerCommon::SetTextStatusPaneL(R_TTGP_TBUF32_STATUSPANE_TITLE_MAIN);
            AppUi()->ActivateLocalViewL(TUid::Uid(ETTGPSLoggerViewMainId));
            break;
            }
        default:
            Panic(ETTGPSLoggerUi);
            break;
        }
    }

void CTTGPSLoggerSettingsView::DoActivateL(const TVwsViewId& aPrevViewId, TUid aCustomMessageId, const TDesC8& aCustomMessage)
    {
    if (!iSettingsContainer)
        {
        iSettingsContainer = CTTGPSLoggerSettingsContainer::NewL(this, ClientRect());
        AppUi()->AddToStackL(iSettingsContainer);
        }
    }

void CTTGPSLoggerSettingsView::DoDeactivate()
    {
    if (iSettingsContainer)
        {
        AppUi()->RemoveFromStack(iSettingsContainer);
        delete iSettingsContainer;
        iSettingsContainer = NULL;
        }
    }

void CTTGPSLoggerSettingsView::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
    {
    }
