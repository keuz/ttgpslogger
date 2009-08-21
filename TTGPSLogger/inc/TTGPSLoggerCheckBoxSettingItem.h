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
#ifndef TTGPSLOGGERCHECKBOXSETTINGITEM_H
#define TTGPSLOGGERCHECKBOXSETTINGITEM_H

#include <aknsettingpage.h>

class CAknEnumeratedTextSettingItem;
class CSelectionItemList;
class MAknSettingPageObserver;

class CTTGPSLoggerCheckBoxSettingItem : public CAknEnumeratedTextSettingItem
	{
public:
	CTTGPSLoggerCheckBoxSettingItem(TInt aIdentifier, CArrayFix<TInt>& aValueArray);
	~CTTGPSLoggerCheckBoxSettingItem();

public:
    // From CAknSettingItem
	void CompleteConstructionL();
	void LoadL();
	void EditItemL(TBool aCalledFromMenu);
	void StoreL();
	const TDesC& SettingTextL();
    // From MAknSettingPageObserver
	void HandleSettingPageEventL(CAknSettingPage *aSettingPage, TAknSettingPageEvent aEventType);
	
private:
	CArrayFix<TInt>& iValueArray;
	CSelectionItemList* iSelectionItemList;
	};

#endif // TTGPSLOGGERCHECKBOXSETTINGITEM_H
