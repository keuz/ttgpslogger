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
#include <aknappui.h>
#include <coemain.h>
#include <lbs.h>
#include <TTGPSLogger.rsg>
#include "TTGPSLoggerCommon.h"
#include "TTGPSLoggerEngine.h"
#include "TTGPSLoggerPositionData.h"
#include "TTGPSLoggerSettingsData.h"
#include "TTGPSLoggerRecorderFormat.h"
#include "TTGPSLoggerRecorderFormatKML.h"

CTTGPSLoggerRecorderFormatKML::CTTGPSLoggerRecorderFormatKML(CTTGPSLoggerEngine* aEngine) :
    iEngine(aEngine),
    iDescriptionItemArray(32)
    {
    }

CTTGPSLoggerRecorderFormatKML::~CTTGPSLoggerRecorderFormatKML()
    {
    iWorkBuf8.Close();
    iWorkBuf.Close();
    iDescriptionItemArray.Reset();
    }

CTTGPSLoggerRecorderFormatKML* CTTGPSLoggerRecorderFormatKML::NewLC(CTTGPSLoggerEngine* aEngine)
    {
    CTTGPSLoggerRecorderFormatKML* self = new (ELeave) CTTGPSLoggerRecorderFormatKML(aEngine);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CTTGPSLoggerRecorderFormatKML* CTTGPSLoggerRecorderFormatKML::NewL(CTTGPSLoggerEngine* aEngine)
    {
    CTTGPSLoggerRecorderFormatKML* self = CTTGPSLoggerRecorderFormatKML::NewLC(aEngine);
    CleanupStack::Pop(); // self;
    return self;
    }

void CTTGPSLoggerRecorderFormatKML::ConstructL()
    {
    iWorkBuf8.CreateL(KMaxPath * 3);
    iWorkBuf.CreateL(1024);
    //iDescriptionItemArray = CEikonEnv::Static()->ReadDesC8ArrayResourceL(R_TTGP_ARRAYPOPEDUPTEXTS_KML_PMDESCRIPTION);
    TBuf8<64> buf;
    //RDebug::Print(_L("%d\n"), __LINE__);
    CDesC16ArrayFlat* itemArray = CEikonEnv::Static()->ReadDesC16ArrayResourceL(R_TTGP_ARRAYPOPEDUPTEXTS_KML_PMDESCRIPTION);
    CleanupStack::PushL(itemArray);
    //RDebug::Print(_L("%d\n"), __LINE__);
    for (int i1 = 0; i1 < itemArray->Count(); i1++)
    	{
        TTGPSLoggerCommon::ConvertTDesToTDes8(buf, itemArray->MdcaPoint(i1));
        iDescriptionItemArray.AppendL(buf);
    	}
    //RDebug::Print(_L("%d\n"), __LINE__);
    CleanupStack::PopAndDestroy(); // itemArray
    //RDebug::Print(_L("%d\n"), __LINE__);
    }

void CTTGPSLoggerRecorderFormatKML::SetFileExtL(TDes& aFileExt)
    {
    aFileExt.Copy(iEngine->Settings()->KMLExt());
    }

void CTTGPSLoggerRecorderFormatKML::SetHeaderL(TDes8& aHeader, const TDesC& aFileName)
    {
    aHeader.Zero();
    aHeader.Append(_L8("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"));
    aHeader.Append(_L8("<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n"));
    aHeader.Append(_L8("  <Document>\n"));
    TTGPSLoggerCommon::ConvertTDesToTDes8(iWorkBuf8, aFileName);

    aHeader.AppendFormat(_L8("  <name>%S</name>\n"), &iWorkBuf8);
    }

void CTTGPSLoggerRecorderFormatKML::SetBodyL(TDes8& aBody, const TDesC& aFileName, const TInt aFilePoint, CTTGPSLoggerPositionData* aPositionData)
    {
    TRealFormat numberFormat;
    TReal tmpVal;
    TBuf8<50> valA;
    TBuf<50> valB;
    TBuf<50> valC;
    TReal64 speed;
    
    // set culture independant decimal separator
    numberFormat.iPoint = '.';
    numberFormat.iTriLen = 0;
    numberFormat.iType = KRealFormatFixed | KDoNotUseTriads;
    
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
        aBody.Append(_L8("    <Placemark>\n"));
        switch (iEngine->Settings()->KMLPmName())
        	{
        	case CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmNamePoint:
        		{
                aBody.AppendFormat(_L8("      <name>%d</name>\n"), aFilePoint);
        		break;
        		}
        	case CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmNameTimeUTC:
        		{
                aBody.Append(_L8("      <name>"));
                TTGPSLoggerCommon::AppendFormatDateTime8L(aBody, aPositionData->SatelliteTime(), FALSE);
                aBody.Append(_L8("</name>\n"));
        		break;
        		}
        	case CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmNameTimeLocal:
        		{
                aBody.Append(_L8("      <name>"));
                TTGPSLoggerCommon::AppendFormatDateTime8L(aBody, aPositionData->SatelliteTime(), TRUE);
                aBody.Append(_L8("</name>\n"));
        		break;
        		}
        	case CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmNameNone:
        	default:
        		break;
        	}
        aBody.Append(_L8("      <TimeStamp>\n"));
        aBody.Append(_L8("        <when>"));
        TTGPSLoggerCommon::AppendFormatGPXTimeStampL(aBody, aPositionData->SatelliteTime());
        aBody.Append(_L8("</when>\n"));
        aBody.Append(_L8("      </TimeStamp>\n"));
        aBody.Append(_L8("      <Point>\n"));
        
        // create lon + lat line
        numberFormat.iPlaces = 6;
        tmpVal = aPositionData->Position().Longitude();
        valA.Num(tmpVal, numberFormat);
        tmpVal = aPositionData->Position().Latitude();
        valB.Num(tmpVal, numberFormat);
        
        aBody.AppendFormat(_L8("        <coordinates>"));aBody.Append(valA);aBody.AppendFormat(_L8(","));aBody.Append(valB);
        if (!Math::IsNaN(aPositionData->Position().Altitude()))
            //{
            //aBody.AppendFormat(_L8("        <coordinates>%.6f,%.6f</coordinates>\n"), aPositionData->Position().Longitude(), aPositionData->Position().Latitude());
            //}
        //else
            {
            //aBody.AppendFormat(_L8("        <coordinates>%.6f,%.6f,%.1f</coordinates>\n"), aPositionData->Position().Longitude(), aPositionData->Position().Latitude(), aPositionData->Position().Altitude());
            numberFormat.iPlaces = 1;
            tmpVal = aPositionData->Position().Altitude();
            valC.Num(tmpVal, numberFormat);
            aBody.AppendFormat(_L8(","));aBody.Append(valC);
            }
        aBody.AppendFormat(_L8("</coordinates>\n"));
        
        aBody.Append(_L8("      </Point>\n"));
        aBody.Append(_L8("      <description><![CDATA[<table>"));
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexPoint))
        	{
            aBody.AppendFormat(_L8("<tr><td>%S</td><td>%d</td></tr>"), &(iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexPoint]), aFilePoint);
        	}
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexLon))
        	{
            aBody.AppendFormat(_L8("<tr><td>%S</td><td>"), &iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexLon]);
            TTGPSLoggerCommon::AppendFormatLongitude(aBody, aPositionData->Position().Longitude());
            aBody.Append(_L8("</td></tr>"));
        	}
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexLat))
        	{
            aBody.AppendFormat(_L8("<tr><td>%S</td><td>"), &iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexLat]);
            TTGPSLoggerCommon::AppendFormatLatitude(aBody, aPositionData->Position().Latitude());
            aBody.Append(_L8("</td></tr>"));
        	}
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexAlt))
        	{
            if (!Math::IsNaN(aPositionData->Position().Altitude()))
            	{
                aBody.AppendFormat(_L8("<tr><td>%S</td><td>"), &iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexAlt]);
                TTGPSLoggerCommon::AppendFormatAltitude(aBody, aPositionData->Position().Altitude());
                aBody.Append(_L8("</td></tr>"));
            	}
        	}
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexSeaAlt))
        	{
            if (!Math::IsNaN(aPositionData->SatelliteSeaLevelAltitude()))
            	{
                aBody.AppendFormat(_L8("<tr><td>%S</td><td>"), &iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexSeaAlt]);
                TTGPSLoggerCommon::AppendFormatAltitude(aBody, aPositionData->SatelliteSeaLevelAltitude());
                aBody.Append(_L8("</td></tr>"));
            	}
        	}
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexHSpeed))
        	{
            if (!Math::IsNaN(aPositionData->HorizontalSpeed()))
            	{
                aBody.AppendFormat(_L8("<tr><td>%S</td><td>"), &iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexHSpeed]);
                TTGPSLoggerCommon::AppendFormatSpeed(aBody, aPositionData->HorizontalSpeed());
                aBody.Append(_L8("</td></tr>"));
            	}
        	}
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexVSpeed))
        	{
            if (!Math::IsNaN(aPositionData->VerticalSpeed()))
            	{
                aBody.AppendFormat(_L8("<tr><td>%S</td><td>"), &iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexVSpeed]);
                TTGPSLoggerCommon::AppendFormatSpeed(aBody, aPositionData->VerticalSpeed());
                aBody.Append(_L8("</td></tr>"));
            	}
        	}
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexCourse))
        	{
            if (!Math::IsNaN(aPositionData->TrueCourse()) || !Math::IsNaN(aPositionData->Heading()))
            	{
                aBody.AppendFormat(_L8("<tr><td>%S</td><td>"), &iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexCourse]);
                TTGPSLoggerCommon::AppendFormatCourse(aBody, aPositionData->TrueCourse(), aPositionData->Heading());
                aBody.Append(_L8("</td></tr>"));
            	}
        	}
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexHAcc))
        	{
            if (!Math::IsNaN(aPositionData->Position().HorizontalAccuracy()))
            	{
                aBody.AppendFormat(_L8("<tr><td>%S</td><td>"), &iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexHAcc]);
                TTGPSLoggerCommon::AppendFormatAccuracy(aBody, aPositionData->Position().HorizontalAccuracy());
                aBody.Append(_L8("</td></tr>"));
            	}
        	}
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexVAcc))
        	{
            if (!Math::IsNaN(aPositionData->Position().VerticalAccuracy()))
            	{
                aBody.AppendFormat(_L8("<tr><td>%S</td><td>"), &iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexVAcc]);
                TTGPSLoggerCommon::AppendFormatAccuracy(aBody, aPositionData->Position().VerticalAccuracy());
                aBody.Append(_L8("</td></tr>"));
            	}
        	}
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexHDOP))
        	{
            if (!Math::IsNaN(aPositionData->SatelliteHorizontalDoP()))
            	{
                aBody.AppendFormat(_L8("<tr><td>%S</td><td>"), &iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexHDOP]);
                TTGPSLoggerCommon::AppendFormatDoP(aBody, aPositionData->SatelliteHorizontalDoP());
                aBody.Append(_L8("</td></tr>"));
            	}
        	}
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexVDOP))
        	{
            if (!Math::IsNaN(aPositionData->SatelliteVerticalDoP()))
            	{
                aBody.AppendFormat(_L8("<tr><td>%S</td><td>"), &iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexVDOP]);
                TTGPSLoggerCommon::AppendFormatDoP(aBody, aPositionData->SatelliteVerticalDoP());
                aBody.Append(_L8("</td></tr>"));
            	}
        	}
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexPDOP))
        	{
            if (!Math::IsNaN(aPositionData->SatellitePositionDoP()))
            	{
                aBody.AppendFormat(_L8("<tr><td>%S</td><td>"), &iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexPDOP]);
                TTGPSLoggerCommon::AppendFormatDoP(aBody, aPositionData->SatellitePositionDoP());
                aBody.Append(_L8("</td></tr>"));
            	}
        	}
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexSatNum))
        	{
            if (!Math::IsNaN(aPositionData->SatelliteNumInView()) && !Math::IsNaN(aPositionData->SatelliteNumUsed()))
            	{
                aBody.AppendFormat(_L8("<tr><td>%S</td><td>"), &iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexSatNum]);
                TTGPSLoggerCommon::AppendFormatSatelliteNum(aBody, aPositionData->SatelliteNumInView(), aPositionData->SatelliteNumUsed());
                aBody.Append(_L8("</td></tr>"));
            	}
        	}
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexSatTimeUTC))
        	{
            aBody.AppendFormat(_L8("<tr><td>%S</td><td>"), &iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexSatTimeUTC]);
            TTGPSLoggerCommon::AppendFormatDateTime8L(aBody, aPositionData->SatelliteTime(), FALSE);
            aBody.Append(_L8("</td></tr>"));
        	}
        if (iEngine->Settings()->KMLPmDescription(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexSatTimeLocal))
        	{
            aBody.AppendFormat(_L8("<tr><td>%S</td><td>"), &iDescriptionItemArray[CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmDescriptionIndexSatTimeLocal]);
            TTGPSLoggerCommon::AppendFormatDateTime8L(aBody, aPositionData->SatelliteTime(), TRUE);
            aBody.Append(_L8("</td></tr>"));
        	}
        aBody.Append(_L8("</table>]]></description>\n"));
        aBody.Append(_L8("    </Placemark>\n"));
        }
    }

void CTTGPSLoggerRecorderFormatKML::SetSegmentL(TDes8& aBody)
    {
    }

void CTTGPSLoggerRecorderFormatKML::SetFooterL(TDes8& aFooter)
    {
    aFooter.Zero();
    aFooter.Append(_L8("  </Document>\n"));
    aFooter.Append(_L8("</kml>\n"));
    }
