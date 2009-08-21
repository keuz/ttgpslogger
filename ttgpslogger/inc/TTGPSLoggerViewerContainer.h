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
#ifndef TTGPSLOGGERVIEWERCONTAINER_H
#define TTGPSLOGGERVIEWERCONTAINER_H

#include <coecntrl.h>

class CCoeControl;
class CEikEdwin;
class CTTGPSLoggerViewerView;
class CTTGPSLoggerPositionData;
class MTTGPSLoggerPositionObserver;
class CAknNavigationDecorator;
class CAknsBasicBackgroundControlContext;

class CTTGPSLoggerViewerContainer : public CCoeControl, public MTTGPSLoggerPositionObserver
	{
public:
    static CTTGPSLoggerViewerContainer* NewL(CTTGPSLoggerViewerView* aParent, const TRect& aRect);
    static CTTGPSLoggerViewerContainer* NewLC(CTTGPSLoggerViewerView* aParent, const TRect& aRect);
	~CTTGPSLoggerViewerContainer();

private:
	CTTGPSLoggerViewerContainer(CTTGPSLoggerViewerView* aParent);
	void ConstructL(const TRect& aRect);

public:
    // From CCoeControl
    TKeyResponse OfferKeyEventL(const TKeyEvent &aKeyEvent, TEventCode aType);
    void HandleResourceChange(TInt aType);
    void HandlePointerEventL(const TPointerEvent& aPointerEvent);
    // From MTTGPSLoggerPositionObserver
    void PositionUpdatedL(CTTGPSLoggerPositionData* aPositionData);
    void PositionUpdatedPartialL(CTTGPSLoggerPositionData* aPositionData);
    void PositionQualityLossL();
    void PositionTimedOutL();
    void PositionErrorL(TInt aErrorCode);
    
    void CopyL();
    void ReloadL();

private:
    // From CoeControl      
    void SizeChanged();
    TInt CountComponentControls() const;
    CCoeControl* ComponentControl(TInt aIndex) const;

private:
	TBool iInitialFlag;
    CTTGPSLoggerViewerView* iParent;
    CEikEdwin* iEdwin;
    RBuf iEdwinText;
    CTTGPSLoggerPositionData* iPositionData;
    CAknNavigationDecorator* iNavigationDecorator;
    CAknsBasicBackgroundControlContext *iBgContext;

    HBufC* iItemNameModId;
    HBufC* iItemNameModName;
    HBufC* iItemNameLon;
    HBufC* iItemNameLat;
    HBufC* iItemNameAlt;
    HBufC* iItemNameSeaAlt;
    HBufC* iItemNameGeoSep;
    HBufC* iItemNameHAcc;
    HBufC* iItemNameVAcc;
    HBufC* iItemNameHSpeed;
    HBufC* iItemNameVSpeed;
    HBufC* iItemNameTCourse;
    HBufC* iItemNameMCourse;
    HBufC* iItemNameHeading;
    HBufC* iItemNameMHeading;
    HBufC* iItemNameHDOP;
    HBufC* iItemNameVDOP;
    HBufC* iItemNamePDOP;
    HBufC* iItemNameTDOP;
    HBufC* iItemNameSatNumInView;
    HBufC* iItemNameSatNumUsed;
    HBufC* iItemNameSatTime;
    HBufC* iMsgCopyClipboard;
	};

#endif // TTGPSLOGGERVIEWERCONTAINER_H
