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
#include <e32math.h>
#include <e32std.h>
#include <coemain.h>
#include <lbs.h>
#include "TTGPSLoggerCommon.h"
#include "TTGPSLoggerEngine.h"
#include "TTGPSLoggerPositionData.h"
#include "TTGPSLoggerSettingsData.h"
#include "TTGPSLoggerRecorderFormat.h"
#include "TTGPSLoggerRecorderFormatGPX.h"

CTTGPSLoggerRecorderFormatGPX::CTTGPSLoggerRecorderFormatGPX(CTTGPSLoggerEngine* aEngine) :
    iEngine(aEngine)
    {
    }

CTTGPSLoggerRecorderFormatGPX::~CTTGPSLoggerRecorderFormatGPX()
    {
    }

CTTGPSLoggerRecorderFormatGPX* CTTGPSLoggerRecorderFormatGPX::NewLC(CTTGPSLoggerEngine* aEngine)
    {
    CTTGPSLoggerRecorderFormatGPX* self =
            new (ELeave) CTTGPSLoggerRecorderFormatGPX(aEngine);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CTTGPSLoggerRecorderFormatGPX* CTTGPSLoggerRecorderFormatGPX::NewL(CTTGPSLoggerEngine* aEngine)
    {
    CTTGPSLoggerRecorderFormatGPX* self =
    CTTGPSLoggerRecorderFormatGPX::NewLC(aEngine);
    CleanupStack::Pop(); // self;
    return self;
    }

void CTTGPSLoggerRecorderFormatGPX::ConstructL()
    {
    }

void CTTGPSLoggerRecorderFormatGPX::SetFileExtL(TDes& aFileExt)
    {
    aFileExt.Copy(iEngine->Settings()->GPXExt());
    }

void CTTGPSLoggerRecorderFormatGPX::SetHeaderL(TDes8& aHeader, const TDesC& aFileName)
    {
    aHeader.Zero();
    aHeader.Append(_L8("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"));
    aHeader.Append(_L8("<gpx version=\"1.0\" creator=\"TTGPSLogger Version 0.4 beta\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.topografix.com/GPX/1/0\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/0 http://www.topografix.com/GPX/1/0/gpx.xsd\">\n"));
    aHeader.Append(_L8("  <trk>\n"));
    aHeader.Append(_L8("    <trkseg>\n"));
    }

void CTTGPSLoggerRecorderFormatGPX::SetBodyL(TDes8& aBody, const TDesC& aFileName, const TInt aFilePoint, CTTGPSLoggerPositionData* aPositionData)
    {
    TReal64 speed;
    TTGPSLoggerCommon::SetNaN(speed);
    if (!Math::IsNaN(aPositionData->HorizontalSpeed()) &&
        !Math::IsNaN(aPositionData->VerticalSpeed()))
        {
        TReal64 tmp = aPositionData->HorizontalSpeed() * aPositionData->HorizontalSpeed() +
                      aPositionData->VerticalSpeed() * aPositionData->VerticalSpeed();
        Math::Sqrt(speed, tmp);
        }
    else if (!Math::IsNaN(aPositionData->VerticalSpeed()))
        {
        speed = aPositionData->VerticalSpeed();
        }
    else if (!Math::IsNaN(aPositionData->HorizontalSpeed()))
        {
        speed = aPositionData->HorizontalSpeed();
        }
    
    aBody.Zero();
    if (!Math::IsNaN(aPositionData->Position().Longitude()) && !Math::IsNaN(aPositionData->Position().Latitude()))
        {
        aBody.AppendFormat(_L8("      <trkpt lon=\"%.6f\" lat=\"%.6f\">\n"), aPositionData->Position().Longitude(), aPositionData->Position().Latitude());
        if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexTime))
        	{
            aBody.Append(_L8("        <time>"));
            TTGPSLoggerCommon::AppendFormatGPXTimeStampL(aBody, aPositionData->SatelliteTime());
            aBody.Append(_L8("</time>\n"));
        	}
        if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexEle) && !Math::IsNaN(aPositionData->Position().Altitude()))
            {
            aBody.AppendFormat(_L8("        <ele>%.1f</ele>\n"), aPositionData->Position().Altitude());
            }
        if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexCourse))
        	{
            if (!Math::IsNaN(aPositionData->TrueCourse()))
                {
                aBody.AppendFormat(_L8("        <course>%.2f</course>\n"), aPositionData->TrueCourse());
                }
            else if (!Math::IsNaN(aPositionData->Heading()))
                {
                aBody.AppendFormat(_L8("        <course>%.2f</course>\n"), aPositionData->Heading());
                } 
        	}
        if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexSpeed) && !Math::IsNaN(speed))
            {
            aBody.AppendFormat(_L8("        <speed>%.2f</speed>\n"), speed);
            }
        if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexFix))
        	{
            if (!Math::IsNaN(aPositionData->Position().Altitude()))
                {
                aBody.Append(_L8("        <fix>3d</fix>\n"));
                }
            else
                {
                aBody.Append(_L8("        <fix>2d</fix>\n"));
                }
        	}
        if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexSat) && !Math::IsNaN(aPositionData->SatelliteNumUsed()))
            {
            aBody.AppendFormat(_L8("        <sat>%d</sat>\n"), aPositionData->SatelliteNumUsed());
            }
        if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexHdop) && !Math::IsNaN(aPositionData->SatelliteHorizontalDoP()))
            {
            aBody.AppendFormat(_L8("        <hdop>%.2f</hdop>\n"), aPositionData->SatelliteHorizontalDoP());
            }
        if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexVdop) && !Math::IsNaN(aPositionData->SatelliteVerticalDoP()))
            {
            aBody.AppendFormat(_L8("        <vdop>%.2f</vdop>\n"), aPositionData->SatelliteVerticalDoP());
            }
        if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexPdop) && !Math::IsNaN(aPositionData->SatellitePositionDoP()))
            {
            aBody.AppendFormat(_L8("        <pdop>%.2f</pdop>\n"), aPositionData->SatellitePositionDoP());
            }
        aBody.Append(_L8("      </trkpt>\n"));
        }
    
    }

void CTTGPSLoggerRecorderFormatGPX::SetFooterL(TDes8& aFooter)
    {
    aFooter.Zero();
    aFooter.Append(_L8("    </trkseg>\n"));
    aFooter.Append(_L8("  </trk>\n"));
    aFooter.Append(_L8("</gpx>\n"));
    }
