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
#include <eikmenup.h> 
#include <TTGPSLogger.rsg>

#ifdef __S60_50__
#include <akntoolbar.h>
#include <akntoolbarobserver.h>
#endif // __S60_50__

#include "TTGPSLogger.hrh"
#include "TTGPSLogger.pan"
#include "TTGPSLoggerPositionObserver.h"
#include "TTGPSLoggerRecorderObserver.h"
#include "TTGPSLoggerAppUi.h"
#include "TTGPSLoggerCommon.h"
#include "TTGPSLoggerEngine.h"
#include "TTGPSLoggerViewerContainer.h"
#include "TTGPSLoggerViewerView.h"

//RDebug::Print(_L("%d\n"), __LINE__);

CTTGPSLoggerViewerView::CTTGPSLoggerViewerView(TUid aUid) :
    iUid(aUid),
    iContainer(NULL)
	{
	}

CTTGPSLoggerViewerView::~CTTGPSLoggerViewerView()
	{
	}

CTTGPSLoggerViewerView* CTTGPSLoggerViewerView::NewLC(TUid aUid)
	{
	CTTGPSLoggerViewerView* self = new (ELeave) CTTGPSLoggerViewerView(aUid);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CTTGPSLoggerViewerView* CTTGPSLoggerViewerView::NewL(TUid aUid)
	{
	CTTGPSLoggerViewerView* self = CTTGPSLoggerViewerView::NewLC(aUid);
	CleanupStack::Pop(); // self;
	return self;
	}

void CTTGPSLoggerViewerView::ConstructL()
	{
    BaseConstructL(R_TTGP_VIEW_VIEWER);
#ifdef __S60_50__
    //RDebug::Print(_L("%d\n"), __LINE__);
    //RDebug::Print(_L("%d\n"), __LINE__);
    CAknToolbar* toolbar = Toolbar();
    //RDebug::Print(_L("%d\n"), __LINE__);
    if (toolbar)
        {
        //RDebug::Print(_L("%d\n"), __LINE__);
        toolbar->SetToolbarObserver(this);
        //RDebug::Print(_L("%d\n"), __LINE__);
        }
    else
        {
        //RDebug::Print(_L("%d\n"), __LINE__);
        toolbar = CAknToolbar::NewL(R_TTGP_TOOLBAR_VIEWER);
        //RDebug::Print(_L("%d\n"), __LINE__);
        if (toolbar)
            {
            //RDebug::Print(_L("%d\n"), __LINE__);
            SetToolbar(toolbar);
            //RDebug::Print(_L("%d\n"), __LINE__);
            toolbar->SetToolbarObserver(this);
            //RDebug::Print(_L("%d\n"), __LINE__);
            }
        }
#endif // __S60_50__
	}

TUid CTTGPSLoggerViewerView::Id() const
    {
    return iUid;
    }

void CTTGPSLoggerViewerView::HandleCommandL(TInt aCommand)
    {
    switch (aCommand)
        {
        case EEikCmdExit:
        case EAknSoftkeyExit:
            {
            AppUi()->Exit();
            break;
            }
        case EAknSoftkeyBack:
            {
            TTGPSLoggerCommon::SetTextStatusPaneL(R_TTGP_TBUF32_STATUSPANE_TITLE_MAIN);
            AppUi()->ActivateLocalViewL(TUid::Uid(ETTGPSLoggerViewMainId));
            break;
            }
        case ETTGPSLoggerCBACopy:
        case ETTGPSLoggerTBViewerCopy:
            {
            if (iContainer)
                {
                iContainer->CopyL();
                }
            break;
            }
        case ETTGPSLoggerCBAReload:
            {
            if (iContainer)
                {
                iContainer->ReloadL();
                }
            break;
            }
        case ETTGPSLoggerTBViewerNMEASt:
            {
        	TTGPSLoggerCommon::SetTextStatusPaneL(R_TTGP_TBUF32_STATUSPANE_TITLE_NMEAVIEWER);
            static_cast<CTTGPSLoggerAppUi*>(CEikonEnv::Static()->EikAppUi())->ActivateLocalViewL(TUid::Uid(ETTGPSLoggerViewNMEAViewerId));
            break;
            }
        case ETTGPSLoggerTBViewerGPSSt:
            {
        	TTGPSLoggerCommon::SetTextStatusPaneL(R_TTGP_TBUF32_STATUSPANE_TITLE_GPSSTATUS);
            static_cast<CTTGPSLoggerAppUi*>(CEikonEnv::Static()->EikAppUi())->ActivateLocalViewL(TUid::Uid(ETTGPSLoggerViewGPSStatusId));
            break;
            }
        default:
            Panic(ETTGPSLoggerUi);
            break;
        }
    }

void CTTGPSLoggerViewerView::DoActivateL(const TVwsViewId& aPrevViewId, TUid aCustomMessageId, const TDesC8& aCustomMessage)
    {
    if (!iContainer)
        {
        iContainer = CTTGPSLoggerViewerContainer::NewL(this, ClientRect());
        if (static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine())
            {
            static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine()->AddPositionObserver(iContainer);
            }
        AppUi()->AddToStackL(iContainer);
        }
    }

void CTTGPSLoggerViewerView::DoDeactivate()
    {
    if (iContainer)
        {
        AppUi()->RemoveFromStack(iContainer);
        if (static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine())
            {
            static_cast<CTTGPSLoggerAppUi*>(AppUi())->Engine()->DeletePositionObserver(iContainer);
            }
        delete iContainer;
        iContainer = NULL;
        }
    }

/*
void CTTGPSLoggerViewerView::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
    {
    }
*/

#ifdef __S60_50__
void CTTGPSLoggerViewerView::DynInitToolbarL(TInt aResourceId, CAknToolbar *aToolbar)
    {
    if (aToolbar)
        {
		if (iUid == TUid::Uid(ETTGPSLoggerViewGPSStatusId))
			{
			aToolbar->HideItem(ETTGPSLoggerTBViewerNMEASt, EFalse, EFalse);
			aToolbar->HideItem(ETTGPSLoggerTBViewerGPSSt, ETrue, EFalse);
			}
		else if (iUid == TUid::Uid(ETTGPSLoggerViewNMEAViewerId))
			{
			aToolbar->HideItem(ETTGPSLoggerTBViewerNMEASt, ETrue, EFalse);
			aToolbar->HideItem(ETTGPSLoggerTBViewerGPSSt, EFalse, EFalse);
			}
	    aToolbar->HideItem(ETTGPSLoggerTBViewerCopy, EFalse, ETrue);
        }
    }

void CTTGPSLoggerViewerView::OfferToolbarEventL(TInt aCommand)
    {
    CAknToolbar* toolbar = Toolbar();
    if (toolbar)
        {
        HandleCommandL(aCommand);
        }
    }
#endif // __S60_50__
