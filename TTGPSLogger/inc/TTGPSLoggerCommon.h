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
#ifndef TTGPSLOGGERCOMMON_H
#define TTGPSLOGGERCOMMON_H

class CAknNavigationDecorator;

class TTGPSLoggerCommon
	{
public:
    static void AppendFormatLongitude(TDes8& aBuf, TReal64 aLongitude);
    static void AppendFormatLongitude(TDes& aBuf, TReal64 aLongitude);
    static void AppendFormatLatitude(TDes8& aBuf, TReal64 aLatitude);
    static void AppendFormatLatitude(TDes& aBuf, TReal64 aLatitude);
    static void AppendFormatAltitude(TDes8& aBuf, TReal32 aAltitude);
    static void AppendFormatAltitude(TDes& aBuf, TReal32 aAltitude);
    static void AppendFormatSpeed(TDes8& aBuf, TReal32 aSpeed);
    static void AppendFormatSpeed(TDes& aBuf, TReal32 aSpeed);
    static void AppendFormatSpeed(TDes& aBuf, TReal32 aSpeed, TReal32 aVerticalSpeed);
    static void AppendFormatCourse(TDes8& aBuf, TReal32 aTrueCourse, TReal32 aHeading);
    static void AppendFormatCourse(TDes& aBuf, TReal32 aTrueCourse, TReal32 aHeading);
    static void AppendFormatAccuracy(TDes8& aBuf, TReal32 aAccuracy);
    static void AppendFormatAccuracy(TDes& aBuf, TReal32 aAccuracy);
    static void AppendFormatAccuracy(TDes& aBuf, TReal32 aAccuracy, TReal32 aVerticalAccuracy);
    static void AppendFormatDoP(TDes8& aBuf, TReal32 aDoP);
    static void AppendFormatDoP(TDes& aBuf, TReal32 aDoP);
    static void AppendFormatDoP(TDes& aBuf, TReal32 aDoP, TReal32 aVerticalDoP);
    static void AppendFormatSatelliteNum(TDes8& aBuf, TInt8 aInView, TInt8 aUsed);
    static void AppendFormatSatelliteNum(TDes& aBuf, TInt8 aInView, TInt8 aUsed);
    static void AppendFormatFilePoint(TDes& aBuf, TInt aFilePoint, TInt aFileSize);
    static void AppendFormatTimeDate8L(TDes8& aBuf, const TTime& aTime, const TBool aConvLocalFlag);
    static void AppendFormatTimeDateL(TDes& aBuf, const TTime& aTime, const TBool aConvLocalFlag);
    static void AppendFormatDateTime8L(TDes8& aBuf, const TTime& aTime, const TBool aConvLocalFlag);
    static void AppendFormatDateTimeL(TDes& aBuf, const TTime& aTime, const TBool aConvLocalFlag);
    static void AppendFormatTimeStampL(TDes& aBuf, const TDesC& aFormat, const TTime& aTime, const TBool aConvLocalFlag);
    static void AppendFormatGPXTimeStampL(TDes8& aBuf, const TTime& aTime);
    static void SetTextStatusPaneL(const TInt aResourceId);
    static void SetTextStatusPaneL(const TDesC& aBuf);
    static void SetTextNavigationPane(const TDesC& aBuf);
    static CAknNavigationDecorator* PushNavigationDecoratorL();
    static void PopNavigationDecoratorL(CAknNavigationDecorator* naviDecorator);
    static TUint GetS60PlatformVersionL();
    static void SetNaN(TReal32& aValue);
    static void SetNaN(TReal64& aValue);
    static TInt ConvertTDes8ToTDes(TDes& aBuf, const TDesC8& aBuf8);
    static TInt ConvertTDesToTDes8(TDes8& aBuf8, const TDesC& aBuf);
private:
    static void AppendFormatCoordinate(TDes& aBuf, TReal64 aCoordinate);
    static void AppendFormatFeetsMetres(TDes& aBuf, TReal32 aValue);
    static void AppendFormatMphKmph(TDes& aBuf, TReal32 aValue);
	};

#endif // TTGPSLOGGERCOMMON_H
