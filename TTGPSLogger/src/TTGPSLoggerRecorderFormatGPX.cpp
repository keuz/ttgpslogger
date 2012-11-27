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
#include <bafl/sysutil.h>

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
    TBuf<KSysUtilVersionTextLength> version;
    TPtrC16 modelNamePtrC;
    TBuf<KSysUtilVersionTextLength> prInfo;

    aHeader.Zero();
    aHeader.Append(_L8("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"));
#ifdef GPXXSL
    aHeader.Append(_L8("<?xml-stylesheet href=\"gpx.xsl\" type=\"text/xsl\"?>\n"));
#endif
    aHeader.Append(_L8("<gpx version=\"1.0\" creator=\"TTGPSLogger Version 0.5"));
/*
    if( User::LeaveIfError( SysUtil::GetDeviceTypeInfoL()->GetModelName( modelNamePtrC ) ) == CDeviceTypeInformation::KDefaultValue )
          {
          // We have a default attribute value, do something else
          aHeader.Append(_L8(" - Phone "));
          }
else
          {
          // We have a device creator supplied attribute value.
          aHeader.Append(_L8(" - Mode name "));
          aHeader.Append(modelNamePtrC);
          }

if( User::LeaveIfError( SysUtil::GetDeviceTypeInfoL()->GetDefaultDeviceName( modelNamePtrC ) ) != CDeviceTypeInformation::KDefaultValue ) {
          aHeader.Append(_L8(" - Default device name "));
          aHeader.Append(modelNamePtrC);
          }
if( User::LeaveIfError( SysUtil::GetDeviceTypeInfoL()->GetManufacturerName( modelNamePtrC ) ) != CDeviceTypeInformation::KDefaultValue ) {
          aHeader.Append(_L8(" - Manufacturer "));
          aHeader.Append(modelNamePtrC);
          }
if( User::LeaveIfError( SysUtil::GetDeviceTypeInfoL()->GetModelCode( modelNamePtrC ) ) != CDeviceTypeInformation::KDefaultValue ) {
          aHeader.Append(_L8(" - Model Code "));
          aHeader.Append(modelNamePtrC);
          }
if( User::LeaveIfError( SysUtil::GetDeviceTypeInfoL()->GetOSVersion( modelNamePtrC ) ) != CDeviceTypeInformation::KDefaultValue ) {
          aHeader.Append(_L8(" - OS Version "));
          aHeader.Append(modelNamePtrC);
          }
if( User::LeaveIfError( SysUtil::GetDeviceTypeInfoL()->GetRevisionID( modelNamePtrC ) ) != CDeviceTypeInformation::KDefaultValue ) {
          aHeader.Append(_L8(" - Revision ID "));
          aHeader.Append(modelNamePtrC);
          }
if( User::LeaveIfError( SysUtil::GetDeviceTypeInfoL()->GetUIPlatformName( modelNamePtrC ) ) != CDeviceTypeInformation::KDefaultValue ) {
          aHeader.Append(_L8(" - UI Platform Name "));
          aHeader.Append(modelNamePtrC);
          }
if( User::LeaveIfError( SysUtil::GetDeviceTypeInfoL()->GetUIPlatformVersion( modelNamePtrC ) ) != CDeviceTypeInformation::KDefaultValue ) {
          aHeader.Append(_L8(" - UI Platform Version "));
          aHeader.Append(modelNamePtrC);
          }
if( SysUtil::GetSWVersion( version ) == KErrNone )
     {     // Use the version string.     ...
          aHeader.Append(_L8(" - SW Version "));
          aHeader.Append(version);
     }
if( SysUtil::GetLangSWVersion( version ) == KErrNone )
     {     // Use the version string.     ...
          aHeader.Append(_L8(" - Language SW Version "));
          aHeader.Append(version);
     }
if( SysUtil::GetLangVersion( version ) == KErrNone )
     {     // Use the version string.     ...
          aHeader.Append(_L8(" - Language Version "));
          aHeader.Append(version);
     }
if ( SysUtil::GetPRInformation( prInfo ) == KErrNone )
    {
    // Use the version string.
          aHeader.Append(_L8(" - Product Release Information "));
          aHeader.Append(prInfo);
    }
*/
    aHeader.Append(_L8("\"\n xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.topografix.com/GPX/1/0\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/0 http://www.topografix.com/GPX/1/0/gpx.xsd\">\n"));

    if (iEngine->Settings()->GPXAuthor().Length()) {
        aHeader.Append(_L8("  <author>"));
        aHeader.Append(iEngine->Settings()->GPXAuthor());
        aHeader.Append(_L8("</author>\n"));
    }
    if (iEngine->Settings()->GPXEmail().Length() ) {
        aHeader.Append(_L8("  <email>"));
        aHeader.Append(iEngine->Settings()->GPXEmail());
        aHeader.Append(_L8("</email>\n"));
    }
    if (iEngine->Settings()->GPXUrl().Length() ) {
        aHeader.Append(_L8("  <url>"));
        aHeader.Append(iEngine->Settings()->GPXUrl());
        aHeader.Append(_L8("</url>\n"));
    }
    if (iEngine->Settings()->GPXUrlName().Length() ) {
        aHeader.Append(_L8("  <urlname>"));
        aHeader.Append(iEngine->Settings()->GPXUrlName());
        aHeader.Append(_L8("</urlname>\n"));
    }

    aHeader.Append(_L8("  <trk>\n"));
    aHeader.Append(_L8("    <trkseg>\n"));
    }

void CTTGPSLoggerRecorderFormatGPX::SetBodyL(TDes8& aBody, const TDesC& aFileName, const TInt aFilePoint, CTTGPSLoggerPositionData* aPositionData)
    {
        TRealFormat numberFormat;
        TBuf8<50> valA;
        TBuf<50> valB;
        TReal speed;
        TReal tmpVal;
        
        // set culture independant decimal separator
        numberFormat.iPoint = '.';
        numberFormat.iTriLen = 0;
        numberFormat.iType = KRealFormatFixed | KDoNotUseTriads;
        
        TTGPSLoggerCommon::SetNaN(speed);
        if (!Math::IsNaN(aPositionData->HorizontalSpeed()) &&
            !Math::IsNaN(aPositionData->VerticalSpeed()) &&
            iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexUseVSpeed))
            {
            TReal tmp = aPositionData->HorizontalSpeed() * aPositionData->HorizontalSpeed() +
                          aPositionData->VerticalSpeed() * aPositionData->VerticalSpeed();
            Math::Sqrt(speed, tmp);
            }
        else if (!Math::IsNaN(aPositionData->VerticalSpeed()) &&
            iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexUseVSpeed))
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
            
            // create lon + lat line
            numberFormat.iPlaces = 6;
            tmpVal = aPositionData->Position().Longitude();
            valA.Num(tmpVal, numberFormat);
            tmpVal = aPositionData->Position().Latitude();
            valB.Num(tmpVal, numberFormat);
            aBody.AppendFormat(_L8("      <trkpt lon=\"")); aBody.Append(valA); aBody.AppendFormat(_L8("\" lat=\"")); aBody.Append(valB); aBody.AppendFormat(_L8("\">\n"));
            // aBody.AppendFormat(_L8("      <trkpt lon=\"%.6f\" lat=\"%.6f\">\n"), aPositionData->Position().Longitude(), aPositionData->Position().Latitude());

            if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexSeaLevel) && !Math::IsNaN(aPositionData->SatelliteSeaLevelAltitude()))
                {
                numberFormat.iPlaces = 1;
                tmpVal = aPositionData->SatelliteSeaLevelAltitude();
                valA.Num(tmpVal, numberFormat);
                aBody.AppendFormat(_L8("        <ele>")); aBody.Append(valA); aBody.Append(_L8("</ele>\n"));
                }
            else if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexEle) && !Math::IsNaN(aPositionData->Position().Altitude()))
				{
				numberFormat.iPlaces = 1;
                tmpVal = aPositionData->Position().Altitude();
				valA.Num(tmpVal, numberFormat);
				aBody.AppendFormat(_L8("        <ele>%.1f</ele>\n"), aPositionData->Position().Altitude());
				}
            if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexTime))
                    {
                        aBody.Append(_L8("        <time>"));
                        TTGPSLoggerCommon::AppendFormatGPXTimeStampL(aBody, aPositionData->SatelliteTime());
                        aBody.Append(_L8("</time>\n"));
                }
            if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexCourse))
                    {
                    numberFormat.iPlaces = 2;
                    TTGPSLoggerCommon::SetNaN(tmpVal);
                
                    if (!Math::IsNaN(aPositionData->TrueCourse()))
                    {
                    tmpVal = aPositionData->TrueCourse();
                    //aBody.AppendFormat(_L8("        <course>%.2f</course>\n"), aPositionData->TrueCourse());
                    }
                else if (!Math::IsNaN(aPositionData->Heading()))
                    {
                    tmpVal = aPositionData->Heading();
                    //aBody.AppendFormat(_L8("        <course>%.2f</course>\n"), aPositionData->Heading());
                    } 
                
                valA.Num(tmpVal, numberFormat);
                if (!Math::IsNaN(tmpVal))
                    {
                    aBody.AppendFormat(_L8("        <course>")); aBody.Append(valA); aBody.Append(_L8("</course>\n"));
                    }
                
                    }
            if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexSpeed) && !Math::IsNaN(speed))
                {
                numberFormat.iPlaces = 2;
                tmpVal = speed;
                valA.Num(tmpVal, numberFormat);
                aBody.AppendFormat(_L8("        <speed>")); aBody.Append(valA); aBody.Append(_L8("</speed>\n"));
                //aBody.AppendFormat(_L8("        <speed>%.2f</speed>\n"), speed);
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
                numberFormat.iPlaces = 0;
                tmpVal = aPositionData->SatelliteNumUsed();
                valA.Num(tmpVal, numberFormat);
                aBody.AppendFormat(_L8("        <sat>")); aBody.Append(valA); aBody.Append(_L8("</sat>\n"));
                //aBody.AppendFormat(_L8("        <sat>%d</sat>\n"), aPositionData->SatelliteNumUsed());
                }
            if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexHdop) && !Math::IsNaN(aPositionData->SatelliteHorizontalDoP()))
                {
                numberFormat.iPlaces = 2;
                tmpVal = aPositionData->SatelliteHorizontalDoP();
                valA.Num(tmpVal, numberFormat);
                aBody.AppendFormat(_L8("        <hdop>")); aBody.Append(valA); aBody.Append(_L8("</hdop>\n"));
                //aBody.AppendFormat(_L8("        <hdop>%.2f</hdop>\n"), aPositionData->SatelliteHorizontalDoP());
                }
            if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexVdop) && !Math::IsNaN(aPositionData->SatelliteVerticalDoP()))
                {
                numberFormat.iPlaces = 2;
                tmpVal = aPositionData->SatelliteVerticalDoP();
                valA.Num(tmpVal, numberFormat);
                aBody.AppendFormat(_L8("        <vdop>")); aBody.Append(valA); aBody.Append(_L8("</vdop>\n"));
                //aBody.AppendFormat(_L8("        <vdop>%.2f</vdop>\n"), aPositionData->SatelliteVerticalDoP());
                }
            if (iEngine->Settings()->GPXTags(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsGPXTagsIndexPdop) && !Math::IsNaN(aPositionData->SatellitePositionDoP()))
                {
                numberFormat.iPlaces = 2;
                tmpVal = aPositionData->SatellitePositionDoP();
                valA.Num(tmpVal, numberFormat);
                aBody.AppendFormat(_L8("        <pdop>")); aBody.Append(valA); aBody.Append(_L8("</pdop>\n"));
                //aBody.AppendFormat(_L8("        <pdop>%.2f</pdop>\n"), aPositionData->SatellitePositionDoP());
                }
            aBody.Append(_L8("      </trkpt>\n"));
            }
        
    }

void CTTGPSLoggerRecorderFormatGPX::SetSegmentL(TDes8& aBody)
    {
		if ( iEngine->Settings()->GpxSegment() ) {
			aBody.Append(_L8("    </trkseg>\n"));
			aBody.Append(_L8("    <trkseg>\n"));
		}
    }

void CTTGPSLoggerRecorderFormatGPX::SetFooterL(TDes8& aFooter)
    {
    aFooter.Zero();
    aFooter.Append(_L8("    </trkseg>\n"));
    aFooter.Append(_L8("  </trk>\n"));
    aFooter.Append(_L8("</gpx>\n"));
    }
