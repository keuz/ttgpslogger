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
#include <e32math.h>
#include <e32std.h>
#include <aknlists.h>
#include <aknnavide.h>
#include <aknnotewrappers.h> 
#include <aknsbasicbackgroundcontrolcontext.h>
#include <barsread.h>
#include <eikedwin.h> 
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
#include "TTGPSLoggerPositionData.h"
#include "TTGPSLoggerPositionObserver.h"
#include "TTGPSLoggerViewerContainer.h"
#include "TTGPSLoggerViewerView.h"

CTTGPSLoggerViewerContainer::CTTGPSLoggerViewerContainer(CTTGPSLoggerViewerView* aParent) :
	iParent(aParent),
	iInitialFlag(FALSE),
	iEdwin(NULL),
	iPositionData(NULL),
    iNavigationDecorator(NULL)
	{
	}

CTTGPSLoggerViewerContainer::~CTTGPSLoggerViewerContainer()
	{
    TTGPSLoggerCommon::PopNavigationDecoratorL(iNavigationDecorator);
    iEdwinText.Close();
	delete iNavigationDecorator;
	delete iPositionData;
	delete iEdwin;
    delete iItemNameModId;
    delete iItemNameModName;
    delete iItemNameLon;
    delete iItemNameLat;
    delete iItemNameAlt;
    delete iItemNameSeaAlt;
    delete iItemNameGeoSep;
    delete iItemNameHAcc;
    delete iItemNameVAcc;
    delete iItemNameHSpeed;
    delete iItemNameVSpeed;
    delete iItemNameTCourse;
    delete iItemNameMCourse;
    delete iItemNameHeading;
    delete iItemNameMHeading;
    delete iItemNameHDOP;
    delete iItemNameVDOP;
    delete iItemNamePDOP;
    delete iItemNameTDOP;
    delete iItemNameSatNumInView;
    delete iItemNameSatNumUsed;
    delete iItemNameSatTime;
    delete iMsgCopyClipboard;
    delete iBgContext;
	}

CTTGPSLoggerViewerContainer* CTTGPSLoggerViewerContainer::NewLC(CTTGPSLoggerViewerView* aParent, const TRect& aRect)
	{
	CTTGPSLoggerViewerContainer* self = new (ELeave) CTTGPSLoggerViewerContainer(aParent);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
	}

CTTGPSLoggerViewerContainer* CTTGPSLoggerViewerContainer::NewL(CTTGPSLoggerViewerView* aParent, const TRect& aRect)
	{
	CTTGPSLoggerViewerContainer* self = CTTGPSLoggerViewerContainer::NewLC(aParent, aRect);
	CleanupStack::Pop(); // self;
	return self;
	}

void CTTGPSLoggerViewerContainer::ConstructL(const TRect& aRect)
	{
    CreateWindowL();
    SetMopParent(iParent);
    
    TResourceReader rr;
    iEikonEnv->CreateResourceReaderLC(rr, R_TTGP_EDWIN_VIEWER);
    iEdwin = new (ELeave)CEikEdwin;
    iEdwin->SetContainerWindowL(*this);
    iEdwin->ConstructFromResourceL(rr);
    iEdwin->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff, CEikScrollBarFrame::EOn);
    CleanupStack::PopAndDestroy(); // rr
    
    {
    iBgContext = CAknsBasicBackgroundControlContext::NewL(
    		KAknsIIDQsnBgAreaMain, 
            //KAknsIIDQsnBgScreen, 
            TRect(0, 0, 0, 0), // changed in SizeChanged()
            EFalse);
    iEdwin->SetSkinBackgroundControlContextL(iBgContext);

    TInt err(0);
    TRgb color;
    err = AknsUtils::GetCachedColor( AknsUtils::SkinInstance(), color,
            KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG6);
    if (err == KErrNone)
    	{
        CCharFormatLayer* charformatLayer = CEikonEnv::NewDefaultCharFormatLayerL();
        TCharFormat charFormat;
        TCharFormatMask charFormatMask;
        charformatLayer->Sense(charFormat, charFormatMask);
        charFormat.iFontPresentation.iTextColor = color;
        charFormatMask.SetAttrib(EAttColor);
        charformatLayer->SetL(charFormat, charFormatMask);
    	iEdwin->SetCharFormatLayer(charformatLayer);
    	}
    }
    
    iEdwinText.CreateL(8192);
    iPositionData = CTTGPSLoggerPositionData::NewL();
    iNavigationDecorator = TTGPSLoggerCommon::PushNavigationDecoratorL();
    TTGPSLoggerCommon::SetTextNavigationPane(_L("Reading..."));
    
    iItemNameModId = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_MODID);
    iItemNameModName = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_MODNAME);
    iItemNameLon = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_LON);
    iItemNameLat = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_LAT);
    iItemNameAlt = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_ALT);
    iItemNameSeaAlt = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_SEAALT);
    iItemNameGeoSep = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_GEOSEP);
    iItemNameHAcc = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_HACC);
    iItemNameVAcc = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_VACC);
    iItemNameHSpeed = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_HSPEED);
    iItemNameVSpeed = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_VSPEED);
    iItemNameTCourse = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_TCOURSE);
    iItemNameMCourse = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_MCOURSE);
    iItemNameHeading = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_HEADING);
    iItemNameMHeading = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_MHEADING);
    iItemNameHDOP = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_HDOP);
    iItemNameVDOP = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_VDOP);
    iItemNamePDOP = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_PDOP);
    iItemNameTDOP = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_TDOP);
    iItemNameSatNumInView = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_SATNUMINVIEW);
    iItemNameSatNumUsed = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_SATNUMUSED);
    iItemNameSatTime = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_GPSSTATUSVIEW_ITEM_SATTIME);
    iMsgCopyClipboard = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF_MSG_COPY_CLIPBOARD);
    
    SetRect(aRect);
    ActivateL();
	}

TKeyResponse CTTGPSLoggerViewerContainer::OfferKeyEventL(const TKeyEvent &aKeyEvent, TEventCode aType)
    {
    if (aType == EEventKey)
    	{
        switch (aKeyEvent.iCode)
            {
            case EKeyOK:
            	{
                CopyL();
                return EKeyWasConsumed;
            	}
            case EKeyUpArrow:
            	{
            	iEdwin->MoveDisplayL(TCursorPosition::EFLineUp);
                return EKeyWasConsumed;
            	}
            case EKeyDownArrow:
            	{
            	iEdwin->MoveDisplayL(TCursorPosition::EFLineDown);
                return EKeyWasConsumed;
            	}
            case EKeyLeftArrow:
            case EKeyRightArrow:
            	{
        		if (iParent->Id() == TUid::Uid(ETTGPSLoggerViewGPSStatusId))
        			{
                	TTGPSLoggerCommon::SetTextStatusPaneL(R_TTGP_TBUF32_STATUSPANE_TITLE_NMEAVIEWER);
                    static_cast<CTTGPSLoggerAppUi*>(CEikonEnv::Static()->EikAppUi())->ActivateLocalViewL(TUid::Uid(ETTGPSLoggerViewNMEAViewerId));
        			}
        		else if (iParent->Id() == TUid::Uid(ETTGPSLoggerViewNMEAViewerId))
        			{
                	TTGPSLoggerCommon::SetTextStatusPaneL(R_TTGP_TBUF32_STATUSPANE_TITLE_GPSSTATUS);
                    static_cast<CTTGPSLoggerAppUi*>(CEikonEnv::Static()->EikAppUi())->ActivateLocalViewL(TUid::Uid(ETTGPSLoggerViewGPSStatusId));
        			}
                return EKeyWasConsumed;
            	}
            }
    	}

    return iEdwin->OfferKeyEventL(aKeyEvent, aType);
    }

void CTTGPSLoggerViewerContainer::HandleResourceChange(TInt aType)
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

void CTTGPSLoggerViewerContainer::SizeChanged()
    {
    //iEdwin->SetExtentToWholeScreen();
    TRect rect = Rect();
    if (iBgContext)
    	{
        //iBgContext->SetBitmap(KAknsIIDQsnBgScreen);
        iBgContext->SetRect(rect);
		if (&Window())
			{
			iBgContext->SetParentPos(PositionRelativeToScreen());
			}
    	}
    if (iEdwin->ScrollBarFrame())
    	{
    	rect.SetWidth(rect.Width() - iEdwin->ScrollBarFrame()->VerticalScrollBar()->ScrollBarBreadth());
        iEdwin->SetRect(rect);
    	}
    }

TInt CTTGPSLoggerViewerContainer::CountComponentControls() const
    {
    return 1;
    }

CCoeControl* CTTGPSLoggerViewerContainer::ComponentControl(TInt aIndex) const
    {
    CCoeControl* control = NULL;
    switch (aIndex)
        {
        case 0:
            control = iEdwin;
            break;
        default:
            control = NULL;
            break;
        }
    return control;
    }

void CTTGPSLoggerViewerContainer::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    CCoeControl::HandlePointerEventL(aPointerEvent);
    }

void CTTGPSLoggerViewerContainer::PositionUpdatedL(CTTGPSLoggerPositionData* aPositionData)
    {
    iPositionData->SetPositionInfoL(aPositionData);
    if (!iInitialFlag) 
    	{
    	ReloadL();
    	iInitialFlag = TRUE;
    	}
    }

void CTTGPSLoggerViewerContainer::PositionUpdatedPartialL(CTTGPSLoggerPositionData* aPositionData)
    {
    PositionUpdatedL(aPositionData);
    }

void CTTGPSLoggerViewerContainer::PositionQualityLossL()
    {
    iPositionData->Init();
    }

void CTTGPSLoggerViewerContainer::PositionTimedOutL()
    {
    iPositionData->Init();
    }

void CTTGPSLoggerViewerContainer::PositionErrorL(TInt aErrorCode)
    {
    iPositionData->Init();
    }

void CTTGPSLoggerViewerContainer::CopyL()
	{
	iEdwin->SelectAllL();
	iEdwin->ClipboardL(CEikEdwin::ECopy);
	iEdwin->ClearSelectionL();
    CAknInformationNote *note = new (ELeave) CAknInformationNote(EFalse);
    note->ExecuteLD(*iMsgCopyClipboard);
/*	
	CClipboard* cb = CClipboard::NewForWritingLC(CCoeEnv::Static()->FsSession());
	cb->StreamDictionary().At(KClipboardUidTypePlainText);
	CPlainText* pt = CPlainText::NewL();
	CleanupStack::PushL(pt);
	pt->InsertL(0, iEdwinText);
	pt->CopyToStoreL(cb->Store(), cb->StreamDictionary(), 0, pt->DocumentLength());
	CleanupStack::PopAndDestroy(); // pt
	cb->CommitL();
	CleanupStack::PopAndDestroy(); // cb
*/
	}

void CTTGPSLoggerViewerContainer::ReloadL()
	{
	if (iPositionData->IsValid())
		{
		if (iPositionData->SatelliteTime() > 0)
			{
		    iEdwinText.Zero();
			TTGPSLoggerCommon::AppendFormatDateTimeL(iEdwinText, iPositionData->SatelliteTime(), TRUE);
		    TTGPSLoggerCommon::SetTextNavigationPane(iEdwinText);
			}
		else
			{
		    TTGPSLoggerCommon::SetTextNavigationPane(_L(" "));
			}
	    iEdwinText.Zero();
		if (iParent->Id() == TUid::Uid(ETTGPSLoggerViewGPSStatusId))
			{
			TBuf<128> buf;

		    iEdwinText.AppendFormat(_L("%S,%d\f"),   iItemNameModId, iPositionData->PositionModuleInfo().ModuleId());
		    iPositionData->PositionModuleInfo().GetModuleName(buf);
		    iEdwinText.AppendFormat(_L("%S,%S\f"),   iItemNameModName, &buf);
		    iEdwinText.AppendFormat(_L("%S,%.6f\f"), iItemNameLon, iPositionData->Position().Longitude());
		    iEdwinText.AppendFormat(_L("%S,%.6f\f"), iItemNameLat, iPositionData->Position().Latitude());
		    iEdwinText.AppendFormat(_L("%S,%.1f\f"), iItemNameAlt, iPositionData->Position().Altitude());
		    iEdwinText.AppendFormat(_L("%S,%.1f\f"), iItemNameSeaAlt, iPositionData->SatelliteSeaLevelAltitude());
		    iEdwinText.AppendFormat(_L("%S,%.1f\f"), iItemNameGeoSep, iPositionData->SatelliteGeoidalSeparation());
		    iEdwinText.AppendFormat(_L("%S,%.1f\f"), iItemNameHAcc, iPositionData->Position().HorizontalAccuracy());
		    iEdwinText.AppendFormat(_L("%S,%.1f\f"), iItemNameVAcc, iPositionData->Position().VerticalAccuracy());
		    iEdwinText.AppendFormat(_L("%S,%.2f\f"), iItemNameHSpeed, iPositionData->HorizontalSpeed());
		    iEdwinText.AppendFormat(_L("%S,%.2f\f"), iItemNameVSpeed, iPositionData->VerticalSpeed());
		    iEdwinText.AppendFormat(_L("%S,%.2f\f"), iItemNameTCourse, iPositionData->TrueCourse());
		    iEdwinText.AppendFormat(_L("%S,%.2f\f"), iItemNameMCourse, iPositionData->MagneticCourse());
		    iEdwinText.AppendFormat(_L("%S,%.2f\f"), iItemNameHeading, iPositionData->Heading());
		    iEdwinText.AppendFormat(_L("%S,%.2f\f"), iItemNameMHeading, iPositionData->MagneticHeading());
		    iEdwinText.AppendFormat(_L("%S,%.2f\f"), iItemNameHDOP, iPositionData->SatelliteHorizontalDoP());
		    iEdwinText.AppendFormat(_L("%S,%.2f\f"), iItemNameVDOP, iPositionData->SatelliteVerticalDoP());
		    iEdwinText.AppendFormat(_L("%S,%.2f\f"), iItemNamePDOP, iPositionData->SatellitePositionDoP());
		    iEdwinText.AppendFormat(_L("%S,%.2f\f"), iItemNameTDOP, iPositionData->SatelliteTimeDoP());
		    iEdwinText.AppendFormat(_L("%S,%d\f"), iItemNameSatNumInView, iPositionData->SatelliteNumInView());
		    iEdwinText.AppendFormat(_L("%S,%d\f"), iItemNameSatNumUsed, iPositionData->SatelliteNumUsed());
		    if (iPositionData->SatelliteTime() > 0)
		    	{
			    iEdwinText.AppendFormat(_L("%S,"), iItemNameSatTime);
		    	TTGPSLoggerCommon::AppendFormatDateTimeL(iEdwinText, iPositionData->SatelliteTime(), TRUE);
			    iEdwinText.Append(_L("\f"));
		    	}
		    else
		    	{
			    iEdwinText.AppendFormat(_L("%S,\f"), iItemNameSatTime);
		    	}
			}
		else if (iParent->Id() == TUid::Uid(ETTGPSLoggerViewNMEAViewerId))
			{
			TTGPSLoggerCommon::ConvertTDes8ToTDes(iEdwinText, iPositionData->GetNMEASentences());
			for (int i1 = 0; i1 < iEdwinText.Length(); i1++)
				{
				if (iEdwinText[i1] == '\n')
					{
					iEdwinText[i1] = '\f';
					}
				}
			}
		//iEdwin->SetCursorPosL(0, EFalse);
		iEdwin->SetTextL(&iEdwinText);
		iEdwin->DrawDeferred();
		}
	else
		{
	    TTGPSLoggerCommon::SetTextNavigationPane(_L("Reading..."));
	    iEdwinText.Zero();
		iEdwin->SetCursorPosL(0, EFalse);
		iEdwin->SetTextL(&iEdwinText);
		iEdwin->DrawDeferred();
		}
	}
