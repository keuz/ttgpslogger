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
#include <avkon.hrh>
#include <TTGPSLogger.rsg>

#ifdef __S60_50__
#include <akntoolbar.h>
#include <akntoolbarobserver.h>
#endif // __S60_50__

#include "TTGPSLogger.hrh"
#include "TTGPSLogger.pan"
#include "TTGPSLoggerCommon.h"
#include "TTGPSLoggerApplication.h"
#include "TTGPSLoggerAppUi.h"
#include "TTGPSLoggerEngine.h"
#include "TTGPSLoggerMainView.h"
#include "TTGPSLoggerSettingsView.h"
#include "TTGPSLoggerViewerView.h"

CTTGPSLoggerAppUi::CTTGPSLoggerAppUi() :
    iEngine(NULL),
    iMainView(NULL),
    iSettingsView(NULL)
	{
	}

CTTGPSLoggerAppUi::~CTTGPSLoggerAppUi()
	{
    delete iEngine;
    iEngine = NULL;
	}

void CTTGPSLoggerAppUi::ConstructL()
    {
    BaseConstructL(EAknEnableSkin | EAknEnableMSK);

    iEngine = CTTGPSLoggerEngine::NewL(this);
    iMainView = CTTGPSLoggerMainView::NewL();
    iSettingsView = CTTGPSLoggerSettingsView::NewL();
    iGPSStatusView = CTTGPSLoggerViewerView::NewL(TUid::Uid(ETTGPSLoggerViewGPSStatusId));
    iNMEAViewerView = CTTGPSLoggerViewerView::NewL(TUid::Uid(ETTGPSLoggerViewNMEAViewerId));
    AddViewL(iMainView);
    AddViewL(iSettingsView);
    AddViewL(iGPSStatusView);
    AddViewL(iNMEAViewerView);
    TTGPSLoggerCommon::SetTextStatusPaneL(R_TTGP_TBUF32_STATUSPANE_TITLE_MAIN);
    ActivateLocalViewL(iMainView->Id());
    }

void CTTGPSLoggerAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
		case EAknCmdTaskSwapper:
            break;
		case EEikCmdExit:
		case EAknSoftkeyExit:
			{
			Engine()->MenuHide();
            Engine()->MenuStopL();
			Exit();
			break;
			}
		case ETTGPSLoggerCBAHide:
            Engine()->MenuHide();
            break;
        case ETTGPSLoggerCommandStartNow:
		case ETTGPSLoggerCBAStartNow:
        case ETTGPSLoggerTBMainStartNow:
			Engine()->MenuStartNowL();
			break;
		case ETTGPSLoggerCommandStartNMEA:
			Engine()->MenuStartAfterNmeaL();
			break;
		case ETTGPSLoggerCommandStartGPX:
			Engine()->MenuStartAfterGpxL();
			break;
		case ETTGPSLoggerCommandStartKML:
			Engine()->MenuStartAfterKmlL();
			break;
        case ETTGPSLoggerCommandStartNowNMEA:
            Engine()->MenuStartNowNmeaL();
            break;
        case ETTGPSLoggerCommandStartNowGPX:
            Engine()->MenuStartNowGpxL();
            break;
        case ETTGPSLoggerCommandStartNowKML:
            Engine()->MenuStartNowKmlL();
            break;
        case ETTGPSLoggerCommandRecord:
        case ETTGPSLoggerCBARecord:
        case ETTGPSLoggerTBMainRecord:
			Engine()->MenuRecordL();
			break;
		case ETTGPSLoggerCommandPause:
		case ETTGPSLoggerCBAPause:
        case ETTGPSLoggerTBMainPause:
			Engine()->MenuPauseL();
			break;
		case ETTGPSLoggerCommandResume:
		case ETTGPSLoggerCBAResume:
        case ETTGPSLoggerTBMainResume:
			Engine()->MenuResumeL();
			break;
		case ETTGPSLoggerCommandStop:
		case ETTGPSLoggerCBAStop:
        case ETTGPSLoggerTBMainStop:
			Engine()->MenuStopL();
			break;
        case ETTGPSLoggerCommandToolSatellite:
            Engine()->MenuSatelliteL();
            break;
        case ETTGPSLoggerCommandToolGPSStatus:
            Engine()->MenuGPSStatusL();
            break;
        case ETTGPSLoggerCommandToolNMEAViewer:
            Engine()->MenuNMEAViewerL();
            break;
		case ETTGPSLoggerCommandSettings:
			Engine()->MenuSettingsL();
			break;
		case ETTGPSLoggerCommandHelpAbout:
            Engine()->MenuAboutL();
            break;
		case ETTGPSLoggerCommandHelpShortcuts:
            Engine()->MenuShortcutsL();
            break;
        case ETTGPSLoggerTBMainLock:
            Engine()->MenuLockL();
            break;
		default:
			Panic(ETTGPSLoggerUi);
			break;
		}
	}
