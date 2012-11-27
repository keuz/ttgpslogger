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
#ifndef TTGPSLOGGERSETTINGSCONTAINER_H
#define TTGPSLOGGERSETTINGSCONTAINER_H

#include <coecntrl.h>

class CCoeControl;
class CAknNavigationDecorator;
class CAknTabGroup;
class CTTGPSLoggerSettingsView;
class CTTGPSLoggerSettingsList;

class CTTGPSLoggerSettingsContainer : public CCoeControl
	{
	
public:
    static CTTGPSLoggerSettingsContainer* NewL(CTTGPSLoggerSettingsView* aParent, const TRect& aRect);
    static CTTGPSLoggerSettingsContainer* NewLC(CTTGPSLoggerSettingsView* aParent, const TRect& aRect);
	~CTTGPSLoggerSettingsContainer();

private:
    CTTGPSLoggerSettingsContainer(CTTGPSLoggerSettingsView* aParent);
	void ConstructL(const TRect& aRect);

public:
    // From CCoeControl
    TKeyResponse OfferKeyEventL(const TKeyEvent &aKeyEvent, TEventCode aType);
    void HandleResourceChange(TInt aType);
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);
    void SizeChanged();
    TInt CountComponentControls() const;
    CCoeControl* ComponentControl(TInt aIndex) const;
    
    void ChangeSelectedItemL();
    
private:
    // From CCoeControl
    void Draw(const TRect& aRect) const;

private:
    CTTGPSLoggerSettingsView* iParent;
    CAknNavigationDecorator* iNaviDecorator;
    CAknTabGroup* iTabGroup;
    CTTGPSLoggerSettingsList* iSettingsListGeneral;
    CTTGPSLoggerSettingsList* iSettingsListDisplay;
    CTTGPSLoggerSettingsList* iSettingsListOutput;
    CTTGPSLoggerSettingsList* iSettingsListNMEA;
    CTTGPSLoggerSettingsList* iSettingsListGPX;
    CTTGPSLoggerSettingsList* iSettingsListKML;
	};

#endif // TTGPSLOGGERSETTINGSCONTAINER_H
