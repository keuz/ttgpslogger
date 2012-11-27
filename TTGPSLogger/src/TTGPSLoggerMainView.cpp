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
#include <eikmenup.h> 
#include <TTGPSLogger.rsg>

#ifdef __S60_50__
#include <akntoolbar.h>
#include <akntoolbarobserver.h>
#endif // __S60_50__

#include "TTGPSLogger.hrh"
#include "TTGPSLoggerPositionObserver.h"
#include "TTGPSLoggerRecorderObserver.h"
#include "TTGPSLoggerAppUi.h"
#include "TTGPSLoggerEngine.h"
#include "TTGPSLoggerMainContainer.h"
#include "TTGPSLoggerMainView.h"

CTTGPSLoggerMainView::CTTGPSLoggerMainView()
	{
	}

CTTGPSLoggerMainView::~CTTGPSLoggerMainView()
	{
	}

CTTGPSLoggerMainView* CTTGPSLoggerMainView::NewLC()
	{
	CTTGPSLoggerMainView* self = new (ELeave) CTTGPSLoggerMainView();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CTTGPSLoggerMainView* CTTGPSLoggerMainView::NewL()
	{
	CTTGPSLoggerMainView* self = CTTGPSLoggerMainView::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CTTGPSLoggerMainView::ConstructL()
	{
    BaseConstructL(R_TTGP_VIEW_MAIN);
#ifdef __S60_50__
    CAknToolbar* toolbar = Toolbar();
    if (toolbar)
        {
        toolbar->SetToolbarObserver(this);
        }
    else
        {
        toolbar = CAknToolbar::NewL(R_TTGP_TOOLBAR_MAIN);
        if (toolbar)
            {
            SetToolbar(toolbar);
            toolbar->SetToolbarObserver(this);
            }
        }
#endif // __S60_50__
	}

TUid CTTGPSLoggerMainView::Id() const
    {
    return TUid::Uid(ETTGPSLoggerViewMainId);
    }

void CTTGPSLoggerMainView::HandleCommandL(TInt aCommand)
    {
    switch (aCommand)
        {
        default:
            {
            AppUi()->HandleCommandL(aCommand);
            break;
            }
        };
    }

void CTTGPSLoggerMainView::DoActivateL(const TVwsViewId& aPrevViewId, TUid aCustomMessageId, const TDesC8& aCustomMessage)
    {
    if (!iContainer)
        {
        iContainer = CTTGPSLoggerMainContainer::NewL(this, ClientRect());
        if (static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine())
            {
            static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine()->AddPositionObserver(iContainer);
            static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine()->AddRecorderObserver(iContainer);
            static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine()->SetSoftKeyL();
            static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine()->SetToolbarL();
            }
        AppUi()->AddToStackL(iContainer);
        }
    }

void CTTGPSLoggerMainView::DoDeactivate()
    {
    if (iContainer)
        {
        AppUi()->RemoveFromStack(iContainer);
        if (static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine())
            {
            static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine()->DeletePositionObserver(iContainer);
            static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine()->DeleteRecorderObserver(iContainer);
            }
        delete iContainer;
        iContainer = NULL;
        }
    }

void CTTGPSLoggerMainView::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
    {
	if (aResourceId == R_TTGP_MENUPANE_MAIN)
	    {
	    static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine()->DynInitMenuPaneL(aResourceId, aMenuPane);
	    }
	else if (aResourceId == R_TTGP_MENUPANE_MAIN_START)
		{
		static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine()->DynInitMenuPaneL(aResourceId, aMenuPane);
		}
	else if (aResourceId == R_TTGP_MENUPANE_MAIN_TOOL)
		{
		static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine()->DynInitMenuPaneL(aResourceId, aMenuPane);
		}
    }

#ifdef __S60_50__
void CTTGPSLoggerMainView::DynInitToolbarL(TInt aResourceId, CAknToolbar *aToolbar)
    {
    if (aToolbar)
        {
        static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine()->DynInitToolbarL(aResourceId, aToolbar);
        }
    }

void CTTGPSLoggerMainView::OfferToolbarEventL(TInt aCommand)
    {
    CAknToolbar* toolbar = Toolbar();
    if (toolbar)
        {
        static_cast<CTTGPSLoggerAppUi*>(AppUi())->HandleCommandL(aCommand);
        }
    }
#endif // __S60_50__
