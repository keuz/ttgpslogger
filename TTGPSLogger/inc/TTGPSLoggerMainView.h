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
#ifndef TTGPSLOGGERMAINVIEW_H
#define TTGPSLOGGERMAINVIEW_H

#include <aknview.h>

class CAknToolbar;
class CTTGPSLoggerAppUi;
class CTTGPSLoggerEngine;
class CTTGPSLoggerMainContainer;
class MAknToolbarObserver;

#ifdef __S60_50__
class CTTGPSLoggerMainView : public CAknView, public MAknToolbarObserver
#else
class CTTGPSLoggerMainView : public CAknView
#endif // __S60_50__
	{
public:
    static CTTGPSLoggerMainView* NewL();
    static CTTGPSLoggerMainView* NewLC();
	~CTTGPSLoggerMainView();

private:
	CTTGPSLoggerMainView();
	void ConstructL();

public:
    // From CAknView
    TUid Id() const;
    void HandleCommandL(TInt aCommand);
    void DoActivateL(const TVwsViewId& aPrevViewId, TUid aCustomMessageId, const TDesC8& aCustomMessage);
    void DoDeactivate();
    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);
    
#ifdef __S60_50__
    //From MAknToolbarObserver
    void DynInitToolbarL(TInt aResourceId, CAknToolbar *aToolbar);
    void OfferToolbarEventL(TInt aCommand);
#endif // __S60_50__

private:
	CTTGPSLoggerMainContainer* iContainer;
	};

#endif // TTGPSLOGGERMAINVIEW_H
