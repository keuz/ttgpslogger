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
#ifndef TTGPSLOGGERSETTINGSLIST_H
#define TTGPSLOGGERSETTINGSLIST_H

#include <aknsettingitemlist.h>

class CAknSettingItem;
class CTTGPSLoggerSettingsView;

class CTTGPSLoggerSettingsList : public CAknSettingItemList
	{
public:
    static CTTGPSLoggerSettingsList* NewL(CTTGPSLoggerSettingsView* aParent, const TRect& aRect, TInt aTabIndex);
    static CTTGPSLoggerSettingsList* NewLC(CTTGPSLoggerSettingsView* aParent, const TRect& aRect, TInt aTabIndex);
	~CTTGPSLoggerSettingsList();

private:
	CTTGPSLoggerSettingsList(CTTGPSLoggerSettingsView* aParent, TInt aTabIndex);
	void ConstructL(const TRect& aRect);

public:
    // From CAknSettingItemList
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);
    void ChangeSelectedItemL();

private:
    // From CCoeControl
    void HandleResourceChange(TInt aType);
    // From CAknSettingItemList
    void SizeChanged();
    CAknSettingItem* CreateSettingItemL(TInt aId);
    void EditItemL(TInt aIndex, TBool aCalledFromMenu);

private:
    CTTGPSLoggerSettingsView* iParent;
    TInt iTabIndex;
	};

#endif // TTGPSLOGGERSETTINGSLIST_H
