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
#ifndef TTGPSLOGGERPOSITIONDATA_H
#define TTGPSLOGGERPOSITIONDATA_H

class CBase;
class HPositionGenericInfo;
class TPosition;
class TPositionModuleInfo;

class CTTGPSLoggerPositionData : public CBase
	{
	
public:
    static CTTGPSLoggerPositionData* NewL();
    static CTTGPSLoggerPositionData* NewLC();
	~CTTGPSLoggerPositionData();

private:
    CTTGPSLoggerPositionData();
	void ConstructL();

public:
    void Init();
    void SetPositionInfoL(TPositionModuleInfo aPositionModuleInfo, HPositionGenericInfo* aPositionInfo);
    void SetPositionInfoL(CTTGPSLoggerPositionData* aPositionInfo);
    TBool IsValid();
    TPositionModuleInfo PositionModuleInfo();
    TPosition Position();
    TReal32 HorizontalSpeed();
    TReal32 VerticalSpeed();
    TReal32 TrueCourse();
    TReal32 MagneticCourse();
    TReal32 Heading();
    TReal32 MagneticHeading();
    TDesC8& GetNMEASentences();
    TInt8 SatelliteNumInView();
    TInt8 SatelliteNumUsed();
    TTime SatelliteTime();
    TReal32 SatelliteHorizontalDoP();
    TReal32 SatelliteVerticalDoP();
    TReal32 SatelliteTimeDoP();
    TReal32 SatellitePositionDoP();
    TReal32 SatelliteSeaLevelAltitude();
    TReal32 SatelliteGeoidalSeparation();

private:
    TBool iIsValid;
    TPositionModuleInfo iPositionModuleInfo;
    TPosition iPosition;
    TReal32 iHorizontalSpeed;
    TReal32 iVerticalSpeed;
    TReal32 iTrueCourse;
    TReal32 iMagneticCourse;
    TReal32 iHeading;
    TReal32 iMagneticHeading;
    RBuf8 iNMEASentences;
    RBuf8 iNMEASentencesWork;
    TInt8 iSatelliteNumInView;
    TInt8 iSatelliteNumUsed;
    TTime iSatelliteTime;
    TReal32 iSatelliteHorizontalDoP;
    TReal32 iSatelliteVerticalDoP;
    TReal32 iSatelliteTimeDoP;
    TReal32 iSatellitePositionDoP;
    TReal32 iSatelliteSeaLevelAltitude;
    TReal32 iSatelliteGeoidalSeparation;
	};

#endif // TTGPSLOGGERSETTINGSDATA_H
