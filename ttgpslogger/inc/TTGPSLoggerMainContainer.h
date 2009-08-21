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
#ifndef TTGPSLOGGERMAINCONTAINER_H
#define TTGPSLOGGERMAINCONTAINER_H

#include <coecntrl.h>

class CAknSingleHeadingStyleListBox;
class CCoeControl;
class CTTGPSLoggerMainView;
class CTTGPSLoggerPositionData;
class MTTGPSLoggerPositionObserver;
class MTTGPSLoggerRecorderObserver;
class CAknNavigationDecorator;

class CTTGPSLoggerMainContainer : public CCoeControl, public MTTGPSLoggerPositionObserver, public MTTGPSLoggerRecorderObserver
	{
public:
    static CTTGPSLoggerMainContainer* NewL(CTTGPSLoggerMainView* aParent, const TRect& aRect);
    static CTTGPSLoggerMainContainer* NewLC(CTTGPSLoggerMainView* aParent, const TRect& aRect);
	~CTTGPSLoggerMainContainer();

private:
	CTTGPSLoggerMainContainer(CTTGPSLoggerMainView* aParent);
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
    // From MTTGPSLoggerRecorderObserver
    void RecorderUpdatedL(const TDesC& aStatus);
    void RecorderUpdatedL(const TDesC& aStatus, const TDesC& aFileName, TInt aFilePoint, TInt aFileSize);

private:
    // From CoeControl
    void SizeChanged();
    TInt CountComponentControls() const;
    CCoeControl* ComponentControl(TInt aIndex) const;
    
    void SetPositionStatusItemArrayL(CTTGPSLoggerPositionData* aPositionData);
    void AppendItemArrayL(CDesC16ArrayFlat* aArray, const TDesC& aLabel, const TDesC& aValue);
    void InitListBoxL();
    void UpdateListBoxL();

private:
    CTTGPSLoggerMainView* iParent;
	CAknSingleHeadingStyleListBox* iListBox;
	CDesC16ArrayFlat *iListBoxItemArray;
    CAknNavigationDecorator* iNavigationDecorator;
    CDesC16ArrayFlat *iRecordStatusItemArray;
    CDesC16ArrayFlat *iPositionStatusItemArray;
    RBuf iItemArrayItemBuf;
    RBuf iItemArrayItemValueBuf;
    HBufC* iListboxItemHeadFileName;
    HBufC* iListboxItemHeadFilePoint;
    HBufC* iListboxItemHeadLon;
    HBufC* iListboxItemHeadLat;
    HBufC* iListboxItemHeadAlt;
    HBufC* iListboxItemHeadSeaAlt;
    HBufC* iListboxItemHeadHSpeed;
    HBufC* iListboxItemHeadVSpeed;
    HBufC* iListboxItemHeadCourse;
    HBufC* iListboxItemHeadHAcc;
    HBufC* iListboxItemHeadVAcc;
    HBufC* iListboxItemHeadHDOP;
    HBufC* iListboxItemHeadVDOP;
    HBufC* iListboxItemHeadPDOP;
    HBufC* iListboxItemHeadSatNum;
    HBufC* iListboxItemHeadSatTime;
    HBufC* iListboxItemHeadSysTime;
    HBufC* iNavigationStateNone;
    HBufC* iNavigationStateWaiting;
    HBufC* iNavigationStateRecording;
    HBufC* iNavigationStatePausing;
	};

#endif // TTGPSLOGGERMAINCONTAINER_H
