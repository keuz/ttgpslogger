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
#include <e32debug.h>
#include <e32math.h>
#include <e32std.h>
#include <aknlists.h>
#include <barsread.h>
#include <aknnavide.h>
#include <lbs.h>
#include <TTGPSLogger.rsg>

#ifdef __S60_50__
#include <akntoolbar.h>
#include <akntoolbarobserver.h>
#endif // __S60_50__

#include "TTGPSLogger.hrh"
#include "TTGPSLoggerCommon.h"
#include "TTGPSLoggerEngine.h"
#include "TTGPSLoggerAppUi.h"
#include "TTGPSLoggerMainView.h"
#include "TTGPSLoggerPositionData.h"
#include "TTGPSLoggerPositionObserver.h"
#include "TTGPSLoggerRecorderObserver.h"
#include "TTGPSLoggerMainContainer.h"
#include "TTGPSLoggerSettingsData.h"

CTTGPSLoggerMainContainer::CTTGPSLoggerMainContainer(CTTGPSLoggerMainView* aParent) :
	iParent(aParent),
	iListBox(NULL),
	iListBoxItemArray(NULL),
	iRecordStatusItemArray(NULL),
	iPositionStatusItemArray(NULL),
    iNavigationDecorator(NULL)
	{
	}

CTTGPSLoggerMainContainer::~CTTGPSLoggerMainContainer()
	{
    TTGPSLoggerCommon::PopNavigationDecoratorL(iNavigationDecorator);
	delete iNavigationDecorator;
	delete iListBox;
	delete iRecordStatusItemArray;
	delete iPositionStatusItemArray;
	iItemArrayItemBuf.Close();
	iItemArrayItemValueBuf.Close();
    delete iListboxItemHeadFileName;
    delete iListboxItemHeadFilePoint;
    delete iListboxItemHeadLon;
    delete iListboxItemHeadLat;
    delete iListboxItemHeadAlt;
    delete iListboxItemHeadSeaAlt;
    delete iListboxItemHeadHSpeed;
    delete iListboxItemHeadVSpeed;
    delete iListboxItemHeadCourse;
    delete iListboxItemHeadHAcc;
    delete iListboxItemHeadVAcc;
    delete iListboxItemHeadHDOP;
    delete iListboxItemHeadVDOP;
    delete iListboxItemHeadPDOP;
    delete iListboxItemHeadSatNum;
    delete iListboxItemHeadSatTime;
    delete iListboxItemHeadSysTime;
    delete iNavigationStateNone;
    delete iNavigationStateWaiting;
    delete iNavigationStateRecording;
    delete iNavigationStatePausing;
	}

CTTGPSLoggerMainContainer* CTTGPSLoggerMainContainer::NewLC(CTTGPSLoggerMainView* aParent, const TRect& aRect)
	{
	CTTGPSLoggerMainContainer* self = new (ELeave) CTTGPSLoggerMainContainer(aParent);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
	}

CTTGPSLoggerMainContainer* CTTGPSLoggerMainContainer::NewL(CTTGPSLoggerMainView* aParent, const TRect& aRect)
	{
	CTTGPSLoggerMainContainer* self = CTTGPSLoggerMainContainer::NewLC(aParent, aRect);
	CleanupStack::Pop(); // self;
	return self;
	}

void CTTGPSLoggerMainContainer::ConstructL(const TRect& aRect)
	{
     // Create a window for this application view
    CreateWindowL();
    SetMopParent(iParent);
    
    iListboxItemHeadFileName = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_FILENAME);
    iListboxItemHeadFilePoint = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_FILEPOINT);
    iListboxItemHeadLon = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_LON);
    iListboxItemHeadLat = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_LAT);
    iListboxItemHeadAlt = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_ALT);
    iListboxItemHeadSeaAlt = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_SEAALT);
    iListboxItemHeadHSpeed = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_HSPEED);
    iListboxItemHeadVSpeed = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_VSPEED);
    iListboxItemHeadCourse = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_COURSE);
    iListboxItemHeadHAcc = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_HACC);
    iListboxItemHeadVAcc = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_VACC);
    iListboxItemHeadHDOP = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_HDOP);
    iListboxItemHeadVDOP = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_VDOP);
    iListboxItemHeadPDOP = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_PDOP);
    iListboxItemHeadSatNum = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_SATNUM);
    iListboxItemHeadSatTime = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_SATTIME);
    iListboxItemHeadSysTime = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_LISTBOXITEM_SYSTIME);
    
    iNavigationStateNone = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_NAVISTATE_NONE);
    iNavigationStateWaiting = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_NAVISTATE_WAITING);
    iNavigationStateRecording = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_NAVISTATE_RECORDING);
    iNavigationStatePausing = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_NAVISTATE_PAUSING);

    TResourceReader rr;
    iEikonEnv->CreateResourceReaderLC(rr, R_TTGP_VIEW_MAIN_LISTBOX);
    iListBox = new (ELeave) CAknSingleHeadingStyleListBox();
    iListBox->SetContainerWindowL(*this);
    iListBox->ConstructFromResourceL(rr);
    iListBox->CreateScrollBarFrameL(ETrue);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOn, CEikScrollBarFrame::EAuto);
    iListBox->View()->ItemDrawer()->SetSkinEnabledL(ETrue);
    CleanupStack::PopAndDestroy(); // rr
    
    iListBoxItemArray = new (ELeave) CDesC16ArrayFlat(10);
    iRecordStatusItemArray = new (ELeave) CDesC16ArrayFlat(10);
    iPositionStatusItemArray = new (ELeave) CDesC16ArrayFlat(10);
	iItemArrayItemBuf.CreateL(256);
	iItemArrayItemValueBuf.CreateL(256);
    InitListBoxL();
    iListBox->Model()->SetItemTextArray(iListBoxItemArray);
    iListBox->Model()->SetOwnershipType(ELbmOwnsItemArray);
    iListBox->HandleItemAdditionL();
    iListBox->DrawDeferred();
    
    iNavigationDecorator = TTGPSLoggerCommon::PushNavigationDecoratorL();
    
    SetRect(aRect);
    ActivateL();
	}

TKeyResponse CTTGPSLoggerMainContainer::OfferKeyEventL(const TKeyEvent &aKeyEvent, TEventCode aType)
    {
    if (aType == EEventKey)
        {
        if (static_cast<CTTGPSLoggerAppUi*>(CEikonEnv::Static()->EikAppUi())->Engine())
            {
            if (static_cast<CTTGPSLoggerAppUi*>(CEikonEnv::Static()->EikAppUi())->Engine()->HandleShortcutKeyL(aKeyEvent.iCode) == EKeyWasConsumed)
            	return EKeyWasConsumed;
            }
        }

    return iListBox->OfferKeyEventL(aKeyEvent, aType);
    }

void CTTGPSLoggerMainContainer::HandleResourceChange(TInt aType)
    {
    CCoeControl::HandleResourceChange(aType);

    switch(aType)
    	{
    	case KEikDynamicLayoutVariantSwitch:
    		{
            TRect rect;
            AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, rect);
            SetRect(rect);
			break;
    		}
    	default:
    		break;
    	}
    }

void CTTGPSLoggerMainContainer::SizeChanged()
    {
    iListBox->SetRect(Rect());
    }

TInt CTTGPSLoggerMainContainer::CountComponentControls() const
    {
    return 1;
    }

CCoeControl* CTTGPSLoggerMainContainer::ComponentControl(TInt aIndex) const
    {
    CCoeControl* control = NULL;
    switch ( aIndex )
        {
        case 0:
            control = iListBox;
            break;
        default:
            control = NULL;
            break;
        }
    return control;
    }

void CTTGPSLoggerMainContainer::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    CCoeControl::HandlePointerEventL(aPointerEvent);
    }

void CTTGPSLoggerMainContainer::PositionUpdatedL(CTTGPSLoggerPositionData* aPositionData)
    {
    SetPositionStatusItemArrayL(aPositionData);
    UpdateListBoxL();
    }

void CTTGPSLoggerMainContainer::PositionUpdatedPartialL(CTTGPSLoggerPositionData* aPositionData)
    {
    PositionUpdatedL(aPositionData);
    }

void CTTGPSLoggerMainContainer::PositionQualityLossL()
    {
    SetPositionStatusItemArrayL(NULL);
    UpdateListBoxL();
    }

void CTTGPSLoggerMainContainer::PositionTimedOutL()
    {
    SetPositionStatusItemArrayL(NULL);
    UpdateListBoxL();
    }

void CTTGPSLoggerMainContainer::PositionErrorL(TInt aErrorCode)
    {
    SetPositionStatusItemArrayL(NULL);
    UpdateListBoxL();
    }

void CTTGPSLoggerMainContainer::RecorderUpdatedL(const TDesC& aStatus)
    {
    TTGPSLoggerCommon::SetTextNavigationPane(aStatus);
    iRecordStatusItemArray->Reset();
    UpdateListBoxL();
    }

void CTTGPSLoggerMainContainer::RecorderUpdatedL(const TDesC& aStatus, const TDesC& aFileName, TInt aFilePoint, TInt aFileSize)
    {
    TTGPSLoggerCommon::SetTextNavigationPane(aStatus);
    iRecordStatusItemArray->Reset();
    AppendItemArrayL(iRecordStatusItemArray, *iListboxItemHeadFileName, aFileName);
    TBuf<64> buf;
    TTGPSLoggerCommon::AppendFormatFilePoint(buf, aFilePoint, aFileSize);
    AppendItemArrayL(iRecordStatusItemArray, *iListboxItemHeadFilePoint, buf);
    UpdateListBoxL();
    }

void CTTGPSLoggerMainContainer::SetPositionStatusItemArrayL(CTTGPSLoggerPositionData* aPositionData)
    {
    _LIT(KValueNull, " ");
    
    if (!static_cast<CTTGPSLoggerAppUi*>(CEikonEnv::Static()->EikAppUi())->Engine())
    	{
    	return;
    	}
    CTTGPSLoggerEngine* engine = static_cast<CTTGPSLoggerAppUi*>(CEikonEnv::Static()->EikAppUi())->Engine();
    
    iPositionStatusItemArray->Reset();
    if (aPositionData && aPositionData->SatelliteTime() > 0)
        {
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexLon))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatLongitude(iItemArrayItemValueBuf, aPositionData->Position().Longitude());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadLon, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexLat))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatLatitude(iItemArrayItemValueBuf, aPositionData->Position().Latitude());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadLat, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexAlt))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatAltitude(iItemArrayItemValueBuf, aPositionData->Position().Altitude());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadAlt, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexSeaAlt))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatAltitude(iItemArrayItemValueBuf, aPositionData->SatelliteSeaLevelAltitude());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadSeaAlt, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexHSpeedWithVSpeed))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatSpeed(iItemArrayItemValueBuf, aPositionData->HorizontalSpeed(), aPositionData->VerticalSpeed());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadHSpeed, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexHSpeed))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatSpeed(iItemArrayItemValueBuf, aPositionData->HorizontalSpeed());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadHSpeed, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexVSpeed))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatSpeed(iItemArrayItemValueBuf, aPositionData->VerticalSpeed());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadVSpeed, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexCourse))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatCourse(iItemArrayItemValueBuf, aPositionData->TrueCourse(), aPositionData->Heading());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadCourse, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexHAccWithVAcc))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatAccuracy(iItemArrayItemValueBuf, aPositionData->Position().HorizontalAccuracy(), aPositionData->Position().VerticalAccuracy());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadHAcc, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexHAcc))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatAccuracy(iItemArrayItemValueBuf, aPositionData->Position().HorizontalAccuracy());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadHAcc, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexVAcc))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatAccuracy(iItemArrayItemValueBuf, aPositionData->Position().VerticalAccuracy());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadVAcc, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexHDOPWithVDOP))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatDoP(iItemArrayItemValueBuf, aPositionData->SatelliteHorizontalDoP(), aPositionData->SatelliteVerticalDoP());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadHDOP, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexHDOP))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatDoP(iItemArrayItemValueBuf, aPositionData->SatelliteHorizontalDoP());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadHDOP, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexVDOP))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatDoP(iItemArrayItemValueBuf, aPositionData->SatelliteVerticalDoP());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadVDOP, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexPDOP))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatDoP(iItemArrayItemValueBuf, aPositionData->SatellitePositionDoP());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadPDOP, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexSatNum))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatSatelliteNum(iItemArrayItemValueBuf, aPositionData->SatelliteNumInView(), aPositionData->SatelliteNumUsed());
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadSatNum, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexSatTime))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatTimeDateL(iItemArrayItemValueBuf, aPositionData->SatelliteTime(), TRUE);
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadSatTime, iItemArrayItemValueBuf);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexSysTime))
        	{
            iItemArrayItemValueBuf.Zero();
            TTGPSLoggerCommon::AppendFormatTimeDateL(iItemArrayItemValueBuf, aPositionData->Position().Time(), TRUE);
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadSysTime, iItemArrayItemValueBuf);
        	}
        }
    else
        {
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexLon))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadLon, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexLat))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadLat, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexAlt))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadAlt, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexSeaAlt))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadSeaAlt, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexHSpeedWithVSpeed))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadHSpeed, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexHSpeed))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadHSpeed, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexVSpeed))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadVSpeed, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexCourse))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadCourse, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexHAccWithVAcc))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadHAcc, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexHAcc))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadHAcc, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexVAcc))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadVAcc, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexHDOPWithVDOP))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadHDOP, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexHDOP))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadHDOP, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexVDOP))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadVDOP, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexPDOP))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadPDOP, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexSatNum))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadSatNum, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexSatTime))
        	{
            AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadSatTime, KValueNull);
        	}
        if (engine->Settings()->DisplayItems(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayItemsIndexSysTime))
        	{
            if (aPositionData)
            	{
                iItemArrayItemValueBuf.Zero();
                TTGPSLoggerCommon::AppendFormatTimeDateL(iItemArrayItemValueBuf, aPositionData->Position().Time(), TRUE);
                AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadSysTime, iItemArrayItemValueBuf);
            	}
            else
            	{
                AppendItemArrayL(iPositionStatusItemArray, *iListboxItemHeadSysTime, KValueNull);
                }
        	}
        }
    }

void CTTGPSLoggerMainContainer::InitListBoxL()
    {
    _LIT(KValueNull, " ");
    
    SetPositionStatusItemArrayL(NULL);
    iListBoxItemArray->Reset();
    for (int i1 = 0; i1 < iPositionStatusItemArray->MdcaCount(); i1++)
        {
        iListBoxItemArray->AppendL(iPositionStatusItemArray->MdcaPoint(i1));
        }
    }

void CTTGPSLoggerMainContainer::UpdateListBoxL()
    {
    TInt count = iListBoxItemArray->Count();
    iListBoxItemArray->Reset();
    
    for (int i1 = 0; i1 < iRecordStatusItemArray->MdcaCount(); i1++)
        {
        iListBoxItemArray->AppendL(iRecordStatusItemArray->MdcaPoint(i1));
        }
    for (int i1 = 0; i1 < iPositionStatusItemArray->MdcaCount(); i1++)
        {
        iListBoxItemArray->AppendL(iPositionStatusItemArray->MdcaPoint(i1));
        }
    
    if (count < iListBoxItemArray->Count())
        {
        iListBox->HandleItemAdditionL();
        }
    else if (count > iListBoxItemArray->Count())
        {
        iListBox->HandleItemRemovalL();
        }
    else
        {
        iListBox->DrawDeferred();
        }
    }

void CTTGPSLoggerMainContainer::AppendItemArrayL(CDesC16ArrayFlat* aArray, const TDesC& aLabel, const TDesC& aValue)
    {
   	_LIT(KTab, "\t");
   	
   	iItemArrayItemBuf.Zero();
    iItemArrayItemBuf.Append(aLabel);
   	iItemArrayItemBuf.Append(KTab);
   	iItemArrayItemBuf.Append(aValue);
   	iItemArrayItemBuf.Append(KTab);
   	iItemArrayItemBuf.Append(KTab);
    
    aArray->AppendL(iItemArrayItemBuf);
    }
