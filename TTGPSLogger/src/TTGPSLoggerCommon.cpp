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
#include <e32std.h>
#include <e32math.h>
#include <tz.h>
#include <aknappui.h>
#include <akntitle.h>
#include <eikspane.h>
#include <aknnavi.h>
#include <aknnavide.h> 
#include <aknnavilabel.h>
#include <utf.h>
#include "TTGPSLoggerAppUi.h"
#include "TTGPSLoggerCommon.h"
#include "TTGPSLoggerEngine.h"
#include "TTGPSLoggerSettingsData.h"

void TTGPSLoggerCommon::AppendFormatLongitude(TDes8& aBuf, TReal64 aLongitude)
	{
    TBuf<32> buf;
    TBuf8<96> buf8;
    AppendFormatLongitude(buf, aLongitude);
    CnvUtfConverter::ConvertFromUnicodeToUtf8(buf8, buf);
    aBuf.Append(buf8);
	}
void TTGPSLoggerCommon::AppendFormatLongitude(TDes& aBuf, TReal64 aLongitude)
    {
    _LIT(KFormatE, "E");
    _LIT(KFormatW, "W");
    
    AppendFormatCoordinate(aBuf, aLongitude);
    if (aBuf.Length() > 0)
        {
        if (aLongitude > 0.0)
            {
            aBuf.Append(KFormatE);
            }
        else
            {
            aBuf.Append(KFormatW);
            }
        }
    }

void TTGPSLoggerCommon::AppendFormatLatitude(TDes8& aBuf, TReal64 aLatitude)
	{
    TBuf<32> buf;
    TBuf8<96> buf8;
    AppendFormatLatitude(buf, aLatitude);
    CnvUtfConverter::ConvertFromUnicodeToUtf8(buf8, buf);
    aBuf.Append(buf8);
	}
void TTGPSLoggerCommon::AppendFormatLatitude(TDes& aBuf, TReal64 aLatitude)
    {
    _LIT(KFormatN, "N");
    _LIT(KFormatS, "S");
    
    AppendFormatCoordinate(aBuf, aLatitude);
    if (aBuf.Length() > 0)
        {
        if (aLatitude > 0.0)
            {
            aBuf.Append(KFormatN);
            }
        else
            {
            aBuf.Append(KFormatS);
            }
        }
    }

void TTGPSLoggerCommon::AppendFormatCoordinate(TDes& aBuf, TReal64 aCoordinate)
    {
    _LIT(KFormatDDDMMSSSS, "%d\xb0%02d'%02d\.%02d\"");
    _LIT(KFormatDDDMMMMMM, "%d\xb0%02d\.%04d'");
    _LIT(KFormatDDDDDDDD,  "%d\.%05d\xb0");
    
    if (!Math::IsNaN(aCoordinate))
        {
        TReal64 workReal(0.0);
        TReal coordinateAbs = Abs(aCoordinate);
        TReal degreeReal = coordinateAbs;
        TInt32 degreeInt(0);
        TInt32 degreeFracInt(0);
        TInt32 minInt(0);
        TInt32 minFracInt(0);
        TInt32 secondInt(0);
        TInt32 secondFracInt(0);
        TInt err = Math::Int(degreeInt, coordinateAbs);
        if (err != KErrNone)
            {
            return;
            }
        err = Math::Frac(workReal, coordinateAbs);
        if (err != KErrNone)
            {
            return;
            }
        err = Math::Int(degreeFracInt, workReal * 100000.0);
        if (err != KErrNone)
            {
            return;
            }
        TReal minReal = workReal * 60.0;
        err = Math::Int(minInt, minReal);
        if (err != KErrNone)
            {
            return;
            }
        err = Math::Frac(workReal, minReal);
        if (err != KErrNone)
            {
            return;
            }
        err = Math::Int(minFracInt, workReal * 10000.0);
        if (err != KErrNone)
            {
            return;
            }
        TReal secordReal = workReal * 60.0;
        err = Math::Int(secondInt, secordReal);
        if (err != KErrNone)
            {
            return;
            }
        err = Math::Frac(workReal, secordReal);
        if (err != KErrNone)
            {
            return;
            }
        err = Math::Int(secondFracInt, workReal * 100.0);
        if (err != KErrNone)
            {
            return;
            }
        
        TInt coordinateFormat(static_cast<CTTGPSLoggerAppUi*>(CEikonEnv::Static()->EikAppUi())->Engine()->Settings()->DisplayCoordinateFormat());
        switch (coordinateFormat)
            {
            case CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayCoordinateFormatDDDMMSSSS:
                aBuf.AppendFormat(KFormatDDDMMSSSS, degreeInt, minInt, secondInt, secondFracInt);
                break;
            case CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayCoordinateFormatDDDMMMMMM:
                aBuf.AppendFormat(KFormatDDDMMMMMM, degreeInt, minInt, minFracInt);
                break;
            case CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayCoordinateFormatDDDDDDDD:
                aBuf.AppendFormat(KFormatDDDDDDDD,  degreeInt, degreeFracInt);
                break;
            }
        }
    }

void TTGPSLoggerCommon::AppendFormatAltitude(TDes8& aBuf, TReal32 aAltitude)
	{
    TBuf<16> buf;
    TBuf8<48> buf8;
    AppendFormatAltitude(buf, aAltitude);
    CnvUtfConverter::ConvertFromUnicodeToUtf8(buf8, buf);
    aBuf.Append(buf8);
	}
void TTGPSLoggerCommon::AppendFormatAltitude(TDes& aBuf, TReal32 aAltitude)
    {
    AppendFormatFeetsMetres(aBuf, aAltitude);
    }

void TTGPSLoggerCommon::AppendFormatSpeed(TDes8& aBuf, TReal32 aSpeed)
	{
    TBuf<16> buf;
    TBuf8<48> buf8;
    AppendFormatSpeed(buf, aSpeed);
    CnvUtfConverter::ConvertFromUnicodeToUtf8(buf8, buf);
    aBuf.Append(buf8);
	}
void TTGPSLoggerCommon::AppendFormatSpeed(TDes& aBuf, TReal32 aSpeed)
    {
    AppendFormatMphKmph(aBuf, aSpeed);
    }
void TTGPSLoggerCommon::AppendFormatSpeed(TDes& aBuf, TReal32 aSpeed, TReal32 aVerticalSpeed)
    {
    _LIT(KFormatVerticalHead, "(V. ");
    _LIT(KFormatVerticalTail, ")");
    
    AppendFormatMphKmph(aBuf, aSpeed);
    if (!Math::IsNaN(aVerticalSpeed))
        {
        aBuf.Append(KFormatVerticalHead);
        AppendFormatMphKmph(aBuf, aVerticalSpeed);
        aBuf.Append(KFormatVerticalTail);
        }
    }

void TTGPSLoggerCommon::AppendFormatCourse(TDes8& aBuf, TReal32 aTrueCourse, TReal32 aHeading)
	{
    TBuf<16> buf;
    TBuf8<48> buf8;
    AppendFormatCourse(buf, aTrueCourse, aHeading);
    CnvUtfConverter::ConvertFromUnicodeToUtf8(buf8, buf);
    aBuf.Append(buf8);
	}
void TTGPSLoggerCommon::AppendFormatCourse(TDes& aBuf, TReal32 aTrueCourse, TReal32 aHeading)
	{
    _LIT(KFormatCourse, "%.2f");
    
    if (!Math::IsNaN(aTrueCourse))
        {
        aBuf.AppendFormat(KFormatCourse, aTrueCourse);
        }
    else if (!Math::IsNaN(aHeading))
        {
        aBuf.AppendFormat(KFormatCourse, aHeading);
        }
	}

void TTGPSLoggerCommon::AppendFormatAccuracy(TDes8& aBuf, TReal32 aAccuracy)
	{
    TBuf<16> buf;
    TBuf8<48> buf8;
    AppendFormatAccuracy(buf, aAccuracy);
    CnvUtfConverter::ConvertFromUnicodeToUtf8(buf8, buf);
    aBuf.Append(buf8);
	}
void TTGPSLoggerCommon::AppendFormatAccuracy(TDes& aBuf, TReal32 aAccuracy)
    {
    AppendFormatFeetsMetres(aBuf, aAccuracy);
    }
void TTGPSLoggerCommon::AppendFormatAccuracy(TDes& aBuf, TReal32 aAccuracy, TReal32 aVerticalAccuracy)
    {
    _LIT(KFormatVerticalHead, "(V. ");
    _LIT(KFormatVerticalTail, ")");
    
    AppendFormatFeetsMetres(aBuf, aAccuracy);
    if (!Math::IsNaN(aVerticalAccuracy))
        {
        aBuf.Append(KFormatVerticalHead);
        AppendFormatFeetsMetres(aBuf, aVerticalAccuracy);
        aBuf.Append(KFormatVerticalTail);
        }
    }

void TTGPSLoggerCommon::AppendFormatDoP(TDes8& aBuf, TReal32 aDoP)
	{
    TBuf<16> buf;
    TBuf8<48> buf8;
    AppendFormatDoP(buf, aDoP);
    CnvUtfConverter::ConvertFromUnicodeToUtf8(buf8, buf);
    aBuf.Append(buf8);
	}
void TTGPSLoggerCommon::AppendFormatDoP(TDes& aBuf, TReal32 aDoP)
    {
    _LIT(KFormatDoP, "%.2f");
    
    if (!Math::IsNaN(aDoP))
        {
        aBuf.AppendFormat(KFormatDoP, aDoP);
        }
    }
void TTGPSLoggerCommon::AppendFormatDoP(TDes& aBuf, TReal32 aDoP, TReal32 aVerticalDoP)
    {
    _LIT(KFormatVerticalHead, "(V. ");
    _LIT(KFormatVerticalTail, ")");
    _LIT(KFormatDoP, "%.2f");
    
    if (!Math::IsNaN(aDoP))
        {
        aBuf.AppendFormat(KFormatDoP, aDoP);
        }
    if (!Math::IsNaN(aVerticalDoP))
        {
        aBuf.Append(KFormatVerticalHead);
        aBuf.AppendFormat(KFormatDoP, aVerticalDoP);
        aBuf.Append(KFormatVerticalTail);
        }
    }

void TTGPSLoggerCommon::AppendFormatSatelliteNum(TDes8& aBuf, TInt8 aInView, TInt8 aUsed)
	{
    TBuf<16> buf;
    TBuf8<48> buf8;
    AppendFormatSatelliteNum(buf, aInView, aUsed);
    CnvUtfConverter::ConvertFromUnicodeToUtf8(buf8, buf);
    aBuf.Append(buf8);
	}
void TTGPSLoggerCommon::AppendFormatSatelliteNum(TDes& aBuf, TInt8 aInView, TInt8 aUsed)
    {
    _LIT(KFormatNum, "%d/%d");
    
    aBuf.AppendFormat(KFormatNum, aUsed, aInView);
    }

void TTGPSLoggerCommon::AppendFormatFilePoint(TDes& aBuf, TInt aFilePoint, TInt aFileSize)
    {
    _LIT(KFormatMB, "%d(%.2f MB)");
    _LIT(KFormatKB, "%d(%.2f KB)");
    _LIT(KFormatB, "%d(%d B)");
    
    if (aFileSize >= 1024 * 1024)
        {
        aBuf.AppendFormat(KFormatMB, aFilePoint, aFileSize / (1024.0 * 1024.0));
        }
    else if (aFileSize >= 1024)
        {
        aBuf.AppendFormat(KFormatKB, aFilePoint, aFileSize / 1024.0);
        }
    else
        {
        aBuf.AppendFormat(KFormatB, aFilePoint, aFileSize);
        }
    }

/*
void TTGPSLoggerCommon::AppendFormatTimeStampL(TDes& aBuf, const TTime& aTime)
    {
    _LIT(KFormatTimeStamp, "%-B%:0%J%:1%T%:2%S%:3%+B %D%M%Y%/0%1%/1%2%/2%3%/3");
    
    RTz tzServer;
    User::LeaveIfError(tzServer.Connect());
    CleanupClosePushL(tzServer);
    
    TTime tmLocal = aTime;
    TInt err = tzServer.ConvertToLocalTime(tmLocal);
    if (err == KErrNone)
        {
        TBuf<32> buf;
        tmLocal.FormatL(buf, KFormatTimeStamp);
        aBuf.Append(buf);
        }
    CleanupStack::PopAndDestroy(); // tzServer
    }
*/

void TTGPSLoggerCommon::AppendFormatTimeDate8L(TDes8& aBuf, const TTime& aTime, const TBool aConvLocalFlag)
	{
    TBuf<32> buf;
    TBuf8<96> buf8;
    AppendFormatTimeDateL(buf, aTime, aConvLocalFlag);
    CnvUtfConverter::ConvertFromUnicodeToUtf8(buf8, buf);
    aBuf.Append(buf8);
	}
void TTGPSLoggerCommon::AppendFormatTimeDateL(TDes& aBuf, const TTime& aTime, const TBool aConvLocalFlag)
	{
    _LIT(KFormat, "%-B%:0%J%:1%T%:2%S%:3%+B %D%M%Y%/0%1%/1%2%/2%3%/3");
    AppendFormatTimeStampL(aBuf, KFormat, aTime, aConvLocalFlag);
	}
void TTGPSLoggerCommon::AppendFormatDateTime8L(TDes8& aBuf, const TTime& aTime, const TBool aConvLocalFlag)
	{
    TBuf<32> buf;
    TBuf8<96> buf8;
    AppendFormatDateTimeL(buf, aTime, aConvLocalFlag);
    CnvUtfConverter::ConvertFromUnicodeToUtf8(buf8, buf);
    aBuf.Append(buf8);
	}
void TTGPSLoggerCommon::AppendFormatDateTimeL(TDes& aBuf, const TTime& aTime, const TBool aConvLocalFlag)
	{
    _LIT(KFormat, "%D%M%Y%/0%1%/1%2%/2%3%/3 %-B%:0%J%:1%T%:2%S%:3%+B");
    AppendFormatTimeStampL(aBuf, KFormat, aTime, aConvLocalFlag);
	}
void TTGPSLoggerCommon::AppendFormatTimeStampL(TDes& aBuf, const TDesC& aFormat, const TTime& aTime, const TBool aConvLocalFlag)
    {
    TTime tmLocal = aTime;
    if (aConvLocalFlag)
    	{
        RTz tzServer;
        User::LeaveIfError(tzServer.Connect());
        CleanupClosePushL(tzServer);
        TInt err = tzServer.ConvertToLocalTime(tmLocal);
        CleanupStack::PopAndDestroy(); // tzServer
        if (err != KErrNone)
        	return;
    	}
    TBuf<32> buf;
    tmLocal.FormatL(buf, aFormat);
    aBuf.Append(buf);
    }

void TTGPSLoggerCommon::AppendFormatGPXTimeStampL(TDes8& aBuf, const TTime& aTime)
    {
    _LIT(KFormatTimeStamp, "%F%Y-%M-%DT%H:%T:%SZ");
    
    TBuf<32> buf;
    TBuf8<96> buf8;
    aTime.FormatL(buf, KFormatTimeStamp);
    CnvUtfConverter::ConvertFromUnicodeToUtf8(buf8, buf);
    aBuf.Append(buf8);
    }

void TTGPSLoggerCommon::SetTextStatusPaneL(const TInt aResourceId)
    {
    HBufC* title = CEikonEnv::Static()->AllocReadResourceL(aResourceId);
    if (title)
        {
        CleanupStack::PushL(title);
        TTGPSLoggerCommon::SetTextStatusPaneL(*title);
        CleanupStack::PopAndDestroy(title);
        }
    }

void TTGPSLoggerCommon::SetTextStatusPaneL(const TDesC& aBuf)
    {
    CEikStatusPane* sp = static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi())->StatusPane();
    CAknTitlePane* titlePane = static_cast<CAknTitlePane*>(sp->ControlL(TUid::Uid(EEikStatusPaneUidTitle)));
    if (titlePane)
        titlePane->SetTextL(aBuf);
    }

void TTGPSLoggerCommon::SetTextNavigationPane(const TDesC& aBuf)
    {
    CEikStatusPane* sp = static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi())->StatusPane();
    CAknNavigationControlContainer* naviPane = static_cast<CAknNavigationControlContainer*>(sp->ControlL(TUid::Uid(EEikStatusPaneUidNavi)));
    CAknNavigationDecorator* naviDecorator = naviPane->Top();
    if (!naviDecorator)
        return;
    CAknNaviLabel* naviLabel = static_cast<CAknNaviLabel*>(naviDecorator->DecoratedControl());
    if (naviLabel)
        {
        if (naviLabel->Text()->Compare(aBuf) != 0)
            {
            naviLabel->SetTextL(aBuf);
            naviPane->Pop();
            naviPane->PushL(*naviDecorator);
            }
        }
    }

CAknNavigationDecorator* TTGPSLoggerCommon::PushNavigationDecoratorL()
    {
    CEikStatusPane* sp = static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi())->StatusPane();
    CAknNavigationControlContainer* naviPane = static_cast<CAknNavigationControlContainer*>(sp->ControlL(TUid::Uid(EEikStatusPaneUidNavi)));
    CAknNavigationDecorator* naviDecorator = naviPane->CreateNavigationLabelL();
    CleanupStack::PushL(naviDecorator);
    naviPane->PushL(*naviDecorator);
    CleanupStack::Pop(naviDecorator);
    
    return naviDecorator;
    }

void TTGPSLoggerCommon::PopNavigationDecoratorL(CAknNavigationDecorator* naviDecorator)
    {
    CEikStatusPane* sp = static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi())->StatusPane();
    CAknNavigationControlContainer* naviPane = static_cast<CAknNavigationControlContainer*>(sp->ControlL(TUid::Uid(EEikStatusPaneUidNavi)));
    naviPane->Pop(naviDecorator);
    }

TUint TTGPSLoggerCommon::GetS60PlatformVersionL()
    {
    /*
    _LIT(KS60ProductIDFile, "Series60v*.sis");
    _LIT(KROMInstallDir, "z:\\system\\install\\");
    TFindFile fileFinder(CCoeEnv::Static()->FsSession());
    CDir* result;
    
    User::LeaveIfError(fileFinder.FindWildByDir(KS60ProductIDFile, KROMInstallDir, result));
    CleanupStack::PushL(result);
    User::LeaveIfError(result->Sort(ESortByName | EDescending));
    TUint major = (*result)[0].iName[9] - '0';
    TUint minor = (*result)[0].iName[11] - '0';
    CleanupStack::PopAndDestroy();  // result
    
    return major * 10 + minor;
    // 30 : S60 3rd Edition
    // 31 : S60 3rd Edition FP1 
    // 32 : S60 3rd Edition FP2
    // 50 : S60 5th Edition
*/
    }

void TTGPSLoggerCommon::AppendFormatFeetsMetres(TDes& aBuf, TReal32 aValue)
    {
    _LIT(KFormatImperialNoNaN, "%.1f ft");
    _LIT(KFormatMetricNoNaN, "%.1f m");
    
    TInt measurementSystem(static_cast<CTTGPSLoggerAppUi*>(CEikonEnv::Static()->EikAppUi())->Engine()->Settings()->DisplayMeasurementSystem());
    if (measurementSystem == CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayMeasurementSystemImperial)
        {
        if (!Math::IsNaN(aValue))
            {
            aBuf.AppendFormat(KFormatImperialNoNaN, aValue / 0.3048);
            }
        }
    else
        {
        if (!Math::IsNaN(aValue))
            {
            aBuf.AppendFormat(KFormatMetricNoNaN, aValue);
            }
        }
    }

void TTGPSLoggerCommon::AppendFormatMphKmph(TDes& aBuf, TReal32 aValue)
    {
    _LIT(KFormatImperialNoNaN, "%.2f mph");
    _LIT(KFormatMetricNoNaN, "%.2f km/h");
    
    TInt measurementSystem(static_cast<CTTGPSLoggerAppUi*>(CEikonEnv::Static()->EikAppUi())->Engine()->Settings()->DisplayMeasurementSystem());
    if (measurementSystem == CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayMeasurementSystemImperial)
        {
        if (!Math::IsNaN(aValue))
            {
            aBuf.AppendFormat(KFormatImperialNoNaN, (aValue * 3600.0 / 1609.344));
            }
        }
    else
        {
        if (!Math::IsNaN(aValue))
            {
            aBuf.AppendFormat(KFormatMetricNoNaN, (aValue * 3.6));
            }
        }
    }

void TTGPSLoggerCommon::SetNaN(TReal32& aValue)
	{
	TRealX realNaN;
	realNaN.SetNaN();
	realNaN.GetTReal(aValue);
	}
void TTGPSLoggerCommon::SetNaN(TReal64& aValue)
	{
	TRealX realNaN;
	realNaN.SetNaN();
	realNaN.GetTReal(aValue);
	}

TInt TTGPSLoggerCommon::ConvertTDes8ToTDes(TDes& aBuf, const TDesC8& aBuf8)
	{
	return CnvUtfConverter::ConvertToUnicodeFromUtf8(aBuf, aBuf8);
	}

TInt TTGPSLoggerCommon::ConvertTDesToTDes8(TDes8& aBuf8, const TDesC& aBuf)
	{
	return CnvUtfConverter::ConvertFromUnicodeToUtf8(aBuf8, aBuf);
	}
