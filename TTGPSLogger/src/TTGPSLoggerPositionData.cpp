/*
 * TTGPSLogger
 * 
 * TTGPSLogger, a GPS logger for Symbian S60 smartphones.
 * Copyright (C) 2009 TTINPUT <ttinputdiary@ovi.com>
 * Copyright (C) 2012 Amacri <amacri@tiscali.it>
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
#include <eikenv.h>
#include <e32base.h>
#include <e32std.h>
#include <lbs.h>
#include <TTGPSLogger.rsg>
#include "TTGPSLoggerPositionData.h"

CTTGPSLoggerPositionData::CTTGPSLoggerPositionData() :
    iIsValid(EFalse)
	{
	}

CTTGPSLoggerPositionData::~CTTGPSLoggerPositionData()
	{
	iNMEASentences.Close();
	iNMEASentencesWork.Close();
	}

CTTGPSLoggerPositionData* CTTGPSLoggerPositionData::NewLC()
	{
	CTTGPSLoggerPositionData* self = new (ELeave) CTTGPSLoggerPositionData();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CTTGPSLoggerPositionData* CTTGPSLoggerPositionData::NewL()
	{
	CTTGPSLoggerPositionData* self = CTTGPSLoggerPositionData::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CTTGPSLoggerPositionData::ConstructL()
	{
	iNMEASentences.CreateL(8192);
	iNMEASentencesWork.CreateL(1024);
	}

void CTTGPSLoggerPositionData::Init()
    {
    TRealX realNaN;
    realNaN.SetNaN();
    
    iIsValid = EFalse;
    iPosition = TPosition();
    iPositionModuleInfo = TPositionModuleInfo();
    realNaN.GetTReal(iHorizontalSpeed);
    realNaN.GetTReal(iVerticalSpeed);
    realNaN.GetTReal(iTrueCourse);
    realNaN.GetTReal(iMagneticCourse);
    realNaN.GetTReal(iHeading);
    realNaN.GetTReal(iMagneticHeading);
    iNMEASentences.Zero();
    iNMEASentencesWork.Zero();
    iSatelliteNumInView = 0;
    iSatelliteNumUsed = 0;
    iSatelliteTime = 0;
    realNaN.GetTReal(iSatelliteHorizontalDoP);
    realNaN.GetTReal(iSatelliteVerticalDoP);
    realNaN.GetTReal(iSatelliteTimeDoP);
    realNaN.GetTReal(iSatellitePositionDoP);
    realNaN.GetTReal(iSatelliteSeaLevelAltitude);
    realNaN.GetTReal(iSatelliteGeoidalSeparation);
    }

void CTTGPSLoggerPositionData::SetPositionInfoL(TPositionModuleInfo aPositionModuleInfo, HPositionGenericInfo* aPositionInfo)
    {
    TBool flag(EFalse);
    TInt err(KErrNone);
	TRealX realNaN;
	realNaN.SetNaN();
    
    Init();
    iIsValid = ETrue;
    aPositionInfo->GetPosition(iPosition);
    iPositionModuleInfo = aPositionModuleInfo;
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldHorizontalSpeed);
    if (flag)
        {
        err = aPositionInfo->GetValue(EPositionFieldHorizontalSpeed, iHorizontalSpeed);
        if (err != KErrNone)
            realNaN.GetTReal(iHorizontalSpeed);
        }
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldVerticalSpeed);
    if (flag)
        {
        err = aPositionInfo->GetValue(EPositionFieldVerticalSpeed, iVerticalSpeed);
        if ((err != KErrNone) || (iVerticalSpeed>500)) // Amacri
            realNaN.GetTReal(iVerticalSpeed);
        }
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldTrueCourse);
    if (flag)
        {
        err = aPositionInfo->GetValue(EPositionFieldTrueCourse, iTrueCourse);
        if (err != KErrNone)
            realNaN.GetTReal(iTrueCourse);
        }
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldMagneticCourse);
    if (flag)
        {
        err = aPositionInfo->GetValue(EPositionFieldMagneticCourse, iMagneticCourse);
        if (err != KErrNone)
            realNaN.GetTReal(iMagneticCourse);
        }
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldHeading);
    if (flag)
        {
        err = aPositionInfo->GetValue(EPositionFieldHeading, iHeading);
        if (err != KErrNone)
            realNaN.GetTReal(iHeading);
        }
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldMagneticHeading);
    if (flag)
        {
        err = aPositionInfo->GetValue(EPositionFieldMagneticHeading, iMagneticHeading);
        if (err != KErrNone)
            realNaN.GetTReal(iMagneticHeading);
        }
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldNMEASentences);
    if (flag)
        {
        TUint8 num = 0;
        err = aPositionInfo->GetValue(EPositionFieldNMEASentences, num);
        if (err == KErrNone)
            {
            for (TInt i1 = 0; i1 < num; i1++)
                {
                iNMEASentencesWork.Zero();
                err = aPositionInfo->GetValue(EPositionFieldNMEASentencesStart + i1, iNMEASentencesWork);
                if (err == KErrNone &&
                    iNMEASentencesWork.Length() > 0 &&
                    (iNMEASentences.Length() + iNMEASentencesWork.Length()) < 8192)
                    {
                    iNMEASentences.Append(iNMEASentencesWork);
                    if (iNMEASentencesWork[iNMEASentencesWork.Length()-1] != '\n' &&
                        iNMEASentences.Length() < 8192)
                        {
                        iNMEASentences.Append(_L8("\n"));
                        }
                    }
                }
            }
        }
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldSatelliteNumInView);
    if (flag)
        {
        err = aPositionInfo->GetValue(EPositionFieldSatelliteNumInView, iSatelliteNumInView);
        if (err != KErrNone)
        	iSatelliteNumInView=0;
        }
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldSatelliteNumUsed);
    if (flag)
        {
        err = aPositionInfo->GetValue(EPositionFieldSatelliteNumUsed, iSatelliteNumUsed);
        if (err != KErrNone)
        	iSatelliteNumUsed=0;
        }
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldSatelliteTime);
    if (flag)
        {
        err = aPositionInfo->GetValue(EPositionFieldSatelliteTime, iSatelliteTime);
        if (err != KErrNone)
        	iSatelliteTime=0;
        }
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldSatelliteHorizontalDoP);
    if (flag)
        {
        err = aPositionInfo->GetValue(EPositionFieldSatelliteHorizontalDoP, iSatelliteHorizontalDoP);
        if (err != KErrNone)
        	iSatelliteHorizontalDoP=0;
        }
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldSatelliteVerticalDoP);
    if (flag)
        {
        err = aPositionInfo->GetValue(EPositionFieldSatelliteVerticalDoP, iSatelliteVerticalDoP);
        if (err != KErrNone)
			realNaN.GetTReal(iSatelliteVerticalDoP);
        }
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldSatelliteTimeDoP);
    if (flag)
        {
        err = aPositionInfo->GetValue(EPositionFieldSatelliteTimeDoP, iSatelliteTimeDoP);
        if (err != KErrNone)
			realNaN.GetTReal(iSatelliteTimeDoP);
        }
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldSatellitePositionDoP);
    if (flag)
        {
        err = aPositionInfo->GetValue(EPositionFieldSatellitePositionDoP, iSatellitePositionDoP);
        if (err != KErrNone)
			realNaN.GetTReal(iSatellitePositionDoP);
        }
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldSatelliteSeaLevelAltitude);
    if (flag)
        {
        err = aPositionInfo->GetValue(EPositionFieldSatelliteSeaLevelAltitude, iSatelliteSeaLevelAltitude);
        if (err != KErrNone)
			realNaN.GetTReal(iSatelliteSeaLevelAltitude);
        }
    flag = aPositionInfo->IsFieldAvailable(EPositionFieldSatelliteGeoidalSeparation);
    if (flag)
        {
        err = aPositionInfo->GetValue(EPositionFieldSatelliteGeoidalSeparation, iSatelliteGeoidalSeparation);
        if (err != KErrNone)
			realNaN.GetTReal(iSatelliteGeoidalSeparation);
        }
    }

void CTTGPSLoggerPositionData::SetPositionInfoL(CTTGPSLoggerPositionData* aPositionInfo)
    {
    iIsValid = aPositionInfo->IsValid();
    iPosition = aPositionInfo->Position();
    iPositionModuleInfo = aPositionInfo->PositionModuleInfo();
    iHorizontalSpeed = aPositionInfo->HorizontalSpeed();
    iVerticalSpeed = aPositionInfo->VerticalSpeed();
    iTrueCourse = aPositionInfo->TrueCourse();
    iMagneticCourse = aPositionInfo->MagneticCourse();
    iHeading = aPositionInfo->Heading();
    iMagneticHeading = aPositionInfo->MagneticHeading();
    iNMEASentences.Zero();
    iNMEASentences.Copy(aPositionInfo->GetNMEASentences());
    iNMEASentencesWork.Zero();
    iSatelliteNumInView = aPositionInfo->SatelliteNumInView();
    iSatelliteNumUsed = aPositionInfo->SatelliteNumUsed();
    iSatelliteTime = aPositionInfo->SatelliteTime();
    iSatelliteHorizontalDoP = aPositionInfo->SatelliteHorizontalDoP();
    iSatelliteVerticalDoP = aPositionInfo->SatelliteVerticalDoP();
    iSatelliteTimeDoP = aPositionInfo->SatelliteTimeDoP();
    iSatellitePositionDoP = aPositionInfo->SatellitePositionDoP();
    iSatelliteSeaLevelAltitude = aPositionInfo->SatelliteSeaLevelAltitude();
    iSatelliteGeoidalSeparation = aPositionInfo->SatelliteGeoidalSeparation();
    }

TBool CTTGPSLoggerPositionData::IsValid()
    {
    return iIsValid;
    }
TPosition CTTGPSLoggerPositionData::Position()
    {
    return iPosition;
    }
TPositionModuleInfo CTTGPSLoggerPositionData::PositionModuleInfo()
    {
    return iPositionModuleInfo;
    }
TReal32 CTTGPSLoggerPositionData::HorizontalSpeed()
    {
    return iHorizontalSpeed;
    }
TReal32 CTTGPSLoggerPositionData::VerticalSpeed()
    {
    return iVerticalSpeed;
    }
TReal32 CTTGPSLoggerPositionData::TrueCourse()
    {
    return iTrueCourse;
    }
TReal32 CTTGPSLoggerPositionData::MagneticCourse()
    {
    return iMagneticCourse;
    }
TReal32 CTTGPSLoggerPositionData::Heading()
    {
    return iHeading;
    }
TReal32 CTTGPSLoggerPositionData::MagneticHeading()
    {
    return iMagneticHeading;
    }
TDesC8& CTTGPSLoggerPositionData::GetNMEASentences()
    {
    return iNMEASentences;
    }
TInt8 CTTGPSLoggerPositionData::SatelliteNumInView()
    {
    return iSatelliteNumInView;
    }
TInt8 CTTGPSLoggerPositionData::SatelliteNumUsed()
    {
    return iSatelliteNumUsed;
    }
TTime CTTGPSLoggerPositionData::SatelliteTime()
    {
    return iSatelliteTime;
    }
TReal32 CTTGPSLoggerPositionData::SatelliteHorizontalDoP()
    {
    return iSatelliteHorizontalDoP;
    }
TReal32 CTTGPSLoggerPositionData::SatelliteVerticalDoP()
    {
    return iSatelliteVerticalDoP;
    }
TReal32 CTTGPSLoggerPositionData::SatelliteTimeDoP()
    {
    return iSatelliteTimeDoP;
    }
TReal32 CTTGPSLoggerPositionData::SatellitePositionDoP()
    {
    return iSatellitePositionDoP;
    }
TReal32 CTTGPSLoggerPositionData::SatelliteSeaLevelAltitude()
    {
    return iSatelliteSeaLevelAltitude;
    }
TReal32 CTTGPSLoggerPositionData::SatelliteGeoidalSeparation()
    {
    return iSatelliteGeoidalSeparation;
    }
