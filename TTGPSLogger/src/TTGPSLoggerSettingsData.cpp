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
#include <e32std.h>
#include <f32file.h>
#include <s32file.h>
#include <aknappui.h>
#include <pathinfo.h>
#include <TTGPSLogger.rsg>
#include "TTGPSLogger.hrh"
#include "TTGPSLoggerSettingsData.h"

#ifdef DSYSTEM
const TBool ETTGPSLoggerSettingsGeneralSystemInitial = ETrue;
#endif
#ifdef POSMETH
const TInt ETTGPSLoggerSettingsGeneralPositioningMethodInitial = 0;
#endif
const TInt ETTGPSLoggerSettingsDisplayMeasurementSystemInitial = CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayMeasurementSystemMetric;
const TInt ETTGPSLoggerSettingsDisplayCoordinateFormatInitial = CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsDisplayCoordinateFormatDDDMMSSSS;
const TBool ETTGPSLoggerSettingsDisplayShortcutsInitial = ETrue;
const TBool ETTGPSLoggerSettingsGpxSegmentInitial = EFalse;
const TInt ETTGPSLoggerSettingsOutputAutostartInitial = CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsOutputAutostartDisabled;
//const CAknMemorySelectionDialog::TMemory ETTGPSLoggerSettingsOutputMemoryInitial = CAknMemorySelectionDialog::EPhoneMemory;
const TInt ETTGPSLoggerSettingsOutputMemoryInitial = 0;
const TInt ETTGPSLoggerSettingsOutputIntervalsInitial = CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsOutputIntervals1Sec;
const TInt ETTGPSLoggerSettingsOutputAutoPauseHSpeedInitial = CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsOutputAutoPauseSpeedNone;
const TInt ETTGPSLoggerSettingsOutputAutoPauseSpeedInitial = CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsOutputAutoPauseSpeedNone;
const TInt ETTGPSLoggerSettingsOutputAutoPauseHAccInitial = CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsOutputAutoPauseAccNone;
const TInt ETTGPSLoggerSettingsOutputAutoPauseHDOPInitial = CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsOutputAutoPauseDOPNone;
const TInt ETTGPSLoggerSettingsOutputAutoPausePDOPInitial = CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsOutputAutoPauseDOPNone;
const TInt ETTGPSLoggerSettingsKMLPmNameInitial = CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsKMLPmNamePoint;

//RDebug::Print(_L("%d \n"), __LINE__);

CTTGPSLoggerSettingsData::CTTGPSLoggerSettingsData() :
    iDisplayItems(ETTGPSLoggerSettingsDisplayItemsNum),
    iNMEASentences(ETTGPSLoggerSettingsNMEASentencesNum),
    iGPXTags(ETTGPSLoggerSettingsGPXTagsNum),
    iKMLPmDescription(ETTGPSLoggerSettingsKMLPmDescriptionNum)
	{
	}

CTTGPSLoggerSettingsData::~CTTGPSLoggerSettingsData()
	{
	iDisplayItems.Reset();
    iOutputDirectory.Close();
    iNMEAExt.Close();
    iNMEASentences.Reset();
    iGPXExt.Close();
    iGPXAuthor.Close();
    iGPXEmail.Close();
    iGPXUrl.Close();
    iGPXUrlName.Close();
    iGPXTags.Reset();
    iKMLExt.Close();
    iKMLPmDescription.Reset();
	}

CTTGPSLoggerSettingsData* CTTGPSLoggerSettingsData::NewLC()
	{
	CTTGPSLoggerSettingsData* self = new (ELeave) CTTGPSLoggerSettingsData();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CTTGPSLoggerSettingsData* CTTGPSLoggerSettingsData::NewL()
	{
	CTTGPSLoggerSettingsData* self = CTTGPSLoggerSettingsData::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CTTGPSLoggerSettingsData::ConstructL()
	{
#ifdef DSYSTEM
    iGeneralSystem = ETTGPSLoggerSettingsGeneralSystemInitial;
#endif
#ifdef POSMETH
    iPositioningMethod = ETTGPSLoggerSettingsGeneralPositioningMethodInitial;
#endif

    iDisplayMeasurementSystem = ETTGPSLoggerSettingsDisplayMeasurementSystemInitial;
    iDisplayCoordinateFormat = ETTGPSLoggerSettingsDisplayCoordinateFormatInitial;
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialLon);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialLat);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialAlt);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialSeaAlt);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialHSpeedWithVSpeed);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialHSpeed);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialVSpeed);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialCourse);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialHAccWithVAcc);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialHAcc);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialVAcc);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialHDOPWithVDOP);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialHDOP);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialVDOP);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialPDOP);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialSatNum);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialSatTime);
    iDisplayItems.AppendL(ETTGPSLoggerSettingsDisplayItemsInitialSysTime);
    iDisplayShortcuts = ETTGPSLoggerSettingsDisplayShortcutsInitial;
    
    iOutputAutostart = ETTGPSLoggerSettingsOutputAutostartInitial;
    iOutputMemory = ETTGPSLoggerSettingsOutputMemoryInitial;
	iOutputDirectory.CreateL(KMaxPath);
    CEikonEnv::Static()->ReadResourceAsDes16L(iOutputDirectory, R_TTGP_TBUF32_SETTINGS_DISPLAY_DIRECTORY);
    iOutputIntervals = ETTGPSLoggerSettingsOutputIntervalsInitial;
    iOutputAutoPauseHSpeed = ETTGPSLoggerSettingsOutputAutoPauseHSpeedInitial;
    iOutputAutoPauseSpeed = ETTGPSLoggerSettingsOutputAutoPauseSpeedInitial;
    iOutputAutoPauseHAcc = ETTGPSLoggerSettingsOutputAutoPauseHAccInitial;
    iOutputAutoPauseHDOP = ETTGPSLoggerSettingsOutputAutoPauseHDOPInitial;
    iOutputAutoPausePDOP = ETTGPSLoggerSettingsOutputAutoPausePDOPInitial;
    
    iNMEAExt.CreateL(KMaxPath);
    CEikonEnv::Static()->ReadResourceAsDes16L(iNMEAExt, R_TTGP_TBUF32_SETTINGS_NMEA_EXT);
    iNMEASentences.AppendL(ETTGPSLoggerSettingsNMEASentencesInitialGPGGA);
    iNMEASentences.AppendL(ETTGPSLoggerSettingsNMEASentencesInitialGPGLL);
    iNMEASentences.AppendL(ETTGPSLoggerSettingsNMEASentencesInitialGPGSA);
    iNMEASentences.AppendL(ETTGPSLoggerSettingsNMEASentencesInitialGPGST);
    iNMEASentences.AppendL(ETTGPSLoggerSettingsNMEASentencesInitialGPGSV);
    iNMEASentences.AppendL(ETTGPSLoggerSettingsNMEASentencesInitialGPRMC);
    iNMEASentences.AppendL(ETTGPSLoggerSettingsNMEASentencesInitialGPVTG);
    iNMEASentences.AppendL(ETTGPSLoggerSettingsNMEASentencesInitialGPZDA);
    iNMEASentences.AppendL(ETTGPSLoggerSettingsNMEASentencesInitialOthers);
    
    iGPXExt.CreateL(KMaxPath);
    CEikonEnv::Static()->ReadResourceAsDes16L(iGPXExt, R_TTGP_TBUF32_SETTINGS_GPX_EXT);
    iGPXAuthor.CreateL(KMaxPath);
    CEikonEnv::Static()->ReadResourceAsDes16L(iGPXAuthor, R_TTGP_TBUF32_SETTINGS_GPX_AUTHOR);
    iGPXEmail.CreateL(KMaxPath);
    CEikonEnv::Static()->ReadResourceAsDes16L(iGPXEmail, R_TTGP_TBUF32_SETTINGS_GPX_EMAIL);
    iGPXUrl.CreateL(KMaxPath);
    CEikonEnv::Static()->ReadResourceAsDes16L(iGPXUrl, R_TTGP_TBUF32_SETTINGS_GPX_URL);
    iGPXUrlName.CreateL(KMaxPath);
    CEikonEnv::Static()->ReadResourceAsDes16L(iGPXUrlName, R_TTGP_TBUF32_SETTINGS_GPX_URLNAME);
    iGPXTags.AppendL(ETTGPSLoggerSettingsGPXTagsInitialTime);
    iGPXTags.AppendL(ETTGPSLoggerSettingsGPXTagsInitialEle);
    iGPXTags.AppendL(ETTGPSLoggerSettingsGPXTagsInitialSeaAlt);
    iGPXTags.AppendL(ETTGPSLoggerSettingsGPXTagsInitialCourse);
    iGPXTags.AppendL(ETTGPSLoggerSettingsGPXTagsInitialSpeed);
    iGPXTags.AppendL(ETTGPSLoggerSettingsGPXTagsInitialUseVSpeed);
    iGPXTags.AppendL(ETTGPSLoggerSettingsGPXTagsInitialFix);
    iGPXTags.AppendL(ETTGPSLoggerSettingsGPXTagsInitialSat);
    iGPXTags.AppendL(ETTGPSLoggerSettingsGPXTagsInitialHdop);
    iGPXTags.AppendL(ETTGPSLoggerSettingsGPXTagsInitialVdop);
    iGPXTags.AppendL(ETTGPSLoggerSettingsGPXTagsInitialPdop);
    
    iKMLExt.CreateL(KMaxPath);
    CEikonEnv::Static()->ReadResourceAsDes16L(iKMLExt, R_TTGP_TBUF32_SETTINGS_KML_EXT);
    iKMLPmName = ETTGPSLoggerSettingsKMLPmNameInitial;
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialPoint);
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialLon);
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialLat);
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialAlt);
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialSeaAlt);
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialHSpeed);
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialVSpeed);
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialCourse);
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialHAcc);
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialVAcc);
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialHDOP);
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialVDOP);
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialPDOP);
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialSatNum);
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialSatTimeUTC);
    iKMLPmDescription.AppendL(ETTGPSLoggerSettingsKMLPmDescriptionInitialSatTimeLocal);
	}

void CTTGPSLoggerSettingsData::CheckL()
	{
	iNMEASentences[ETTGPSLoggerSettingsNMEASentencesIndexGPGGA] = ETrue;
	iGPXTags[ETTGPSLoggerSettingsGPXTagsIndexTime] = ETrue;
	}

void CTTGPSLoggerSettingsData::LoadL()
    {
    TInt err(KErrNone);
    
    HBufC* settingsFileName = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_SETTINGS_FILE_NAME);
    CleanupStack::PushL(settingsFileName);
    //RDebug::Print(_L("%d \n"), __LINE__);
    User::LeaveIfError(CCoeEnv::Static()->FsSession().CreatePrivatePath(EDriveC));
    //RDebug::Print(_L("%d \n"), __LINE__);
    User::LeaveIfError(CCoeEnv::Static()->FsSession().PrivatePath(iSettingsFileName));
    //RDebug::Print(_L("%d \n"), __LINE__);
    iSettingsFileName.Append(*settingsFileName);
    //RDebug::Print(_L("%d \n"), __LINE__);
    CleanupStack::PopAndDestroy(settingsFileName);
    
    RFile file;
    CleanupClosePushL(file);
    //RDebug::Print(_L("%d \n"), __LINE__);
    err = file.Open(CCoeEnv::Static()->FsSession(), iSettingsFileName, EFileRead | EFileShareAny);
    //RDebug::Print(_L("%S %d \n"), &iSettingsFileName, __LINE__);
    if (err == KErrNone)
        {
        //RDebug::Print(_L("%d \n"), __LINE__);
        RFileReadStream readStream(file);
        CleanupClosePushL(readStream);
        
        iDisplayMeasurementSystem = readStream.ReadInt32L();
        iDisplayCoordinateFormat = readStream.ReadInt32L();
        TInt displayItemsCount = readStream.ReadInt32L();
        for (TInt i1 = 0; i1 < displayItemsCount; i1++)
        	{
        	iDisplayItems[i1] = readStream.ReadInt32L();
        	}
        iDisplayShortcuts = readStream.ReadInt32L();
#ifdef DSYSTEM
        iGeneralSystem = readStream.ReadInt32L();
#endif
#ifdef POSMETH
        iPositioningMethod = readStream.ReadInt32L();
#endif
        iOutputAutostart = readStream.ReadInt32L();
        iOutputMemory = readStream.ReadInt32L();
        //TInt outputMemory = readStream.ReadInt32L();
        //iOutputMemory =  (CAknMemorySelectionDialog::TMemory)outputMemory;
        readStream >> iOutputDirectory;
        iOutputIntervals = readStream.ReadInt32L();
        iOutputDistance = readStream.ReadInt32L();
        iOutputAutoPauseHSpeed = readStream.ReadInt32L();
        iOutputAutoPauseSpeed = readStream.ReadInt32L();
        iOutputAutoPauseHAcc = readStream.ReadInt32L();
        iOutputAutoPauseHDOP = readStream.ReadInt32L();
        iOutputAutoPausePDOP = readStream.ReadInt32L();
        readStream >> iNMEAExt;
        TInt nmeaSentencesCount = readStream.ReadInt32L();
        for (TInt i1 = 0; i1 < nmeaSentencesCount; i1++)
        	{
        	iNMEASentences[i1] = readStream.ReadInt32L();
        	}
        readStream >> iGPXExt;
        readStream >> iGPXAuthor;
        readStream >> iGPXEmail;
        readStream >> iGPXUrl;
        readStream >> iGPXUrlName;
        iGpxSegment = readStream.ReadInt32L();
        TInt gpxTagsCount = readStream.ReadInt32L();
        for (TInt i1 = 0; i1 < gpxTagsCount; i1++)
        	{
        	iGPXTags[i1] = readStream.ReadInt32L();
        	}
        readStream >> iKMLExt;
        iKMLPmName = readStream.ReadInt32L();
        TInt kmlPmDescriptionCount = readStream.ReadInt32L();
        for (TInt i1 = 0; i1 < kmlPmDescriptionCount; i1++)
        	{
        	iKMLPmDescription[i1] = readStream.ReadInt32L();
        	}

        CleanupStack::PopAndDestroy(); // readStream
        }
    //RDebug::Print(_L("%d \n"), __LINE__);
    CleanupStack::PopAndDestroy(); // file
    }

void CTTGPSLoggerSettingsData::SaveL()
    {
    TInt err(KErrNone);
    
    RFile file;
    CleanupClosePushL(file);
    err = file.Replace(CCoeEnv::Static()->FsSession(), iSettingsFileName, EFileWrite | EFileShareAny);
    if (err == KErrNone)
        {
        RFileWriteStream writeStream(file);
        CleanupClosePushL(writeStream);
        
        writeStream.WriteInt32L(iDisplayMeasurementSystem);
        writeStream.WriteInt32L(iDisplayCoordinateFormat);
        TInt displayItemsCount = iDisplayItems.Count();
    	writeStream.WriteInt32L(displayItemsCount);
        for (TInt i1 = 0; i1 < displayItemsCount; i1++)
        	{
        	writeStream.WriteInt32L(iDisplayItems[i1]);
        	}
    	writeStream.WriteInt32L(iDisplayShortcuts);
#ifdef DSYSTEM
    	writeStream.WriteInt32L(iGeneralSystem);
#endif
#ifdef POSMETH
    	writeStream.WriteInt32L(iPositioningMethod);
#endif
        writeStream.WriteInt32L(iOutputAutostart);
        writeStream.WriteInt32L(iOutputMemory);
        writeStream << iOutputDirectory;
        writeStream.WriteInt32L(iOutputIntervals);
        writeStream.WriteInt32L(iOutputDistance);
        writeStream.WriteInt32L(iOutputAutoPauseHSpeed);
        writeStream.WriteInt32L(iOutputAutoPauseSpeed);
        writeStream.WriteInt32L(iOutputAutoPauseHAcc);
        writeStream.WriteInt32L(iOutputAutoPauseHDOP);
        writeStream.WriteInt32L(iOutputAutoPausePDOP);
        writeStream << iNMEAExt;
        TInt nmeaSentencesCount = iNMEASentences.Count();
    	writeStream.WriteInt32L(nmeaSentencesCount);
        for (TInt i1 = 0; i1 < nmeaSentencesCount; i1++)
        	{
        	writeStream.WriteInt32L(iNMEASentences[i1]);
        	}
        writeStream << iGPXExt;
        writeStream << iGPXAuthor;
        writeStream << iGPXEmail;
        writeStream << iGPXUrl;
        writeStream << iGPXUrlName;
    	writeStream.WriteInt32L(iGpxSegment);
        TInt gpxTagsCount = iGPXTags.Count();
    	writeStream.WriteInt32L(gpxTagsCount);
        for (TInt i1 = 0; i1 < gpxTagsCount; i1++)
        	{
        	writeStream.WriteInt32L(iGPXTags[i1]);
        	}
        writeStream << iKMLExt;
        writeStream.WriteInt32L(iKMLPmName);
        TInt kmlPmDescriptionCount = iKMLPmDescription.Count();
    	writeStream.WriteInt32L(kmlPmDescriptionCount);
        for (TInt i1 = 0; i1 < kmlPmDescriptionCount; i1++)
        	{
        	writeStream.WriteInt32L(iKMLPmDescription[i1]);
        	}
        
        CleanupStack::PopAndDestroy(); // writeStream
        }
    CleanupStack::PopAndDestroy(); // file
    }

#ifdef DSYSTEM
TBool& CTTGPSLoggerSettingsData::GetGeneralSystem()
    {
    return iGeneralSystem;
    }
TInt& CTTGPSLoggerSettingsData::GetPositioningMethod()
    {
    return iPositioningMethod;
    }
#endif
TInt& CTTGPSLoggerSettingsData::GetDisplayMeasurementSystem()
    {
    return iDisplayMeasurementSystem;
    }
TInt& CTTGPSLoggerSettingsData::GetDisplayCoordinateFormat()
    {
    return iDisplayCoordinateFormat;
    }
CArrayFix<TInt>& CTTGPSLoggerSettingsData::GetDisplayItems()
	{
    return iDisplayItems;
	}
TBool& CTTGPSLoggerSettingsData::GetDisplayShortcuts()
    {
    return iDisplayShortcuts;
    }
TBool& CTTGPSLoggerSettingsData::GetGpxSegment()
    {
    return iGpxSegment;
    }
TInt& CTTGPSLoggerSettingsData::GetOutputAutostart()
    {
    return iOutputAutostart;
    }
//CAknMemorySelectionDialog::TMemory& CTTGPSLoggerSettingsData::GetOutputMemory()
TInt& CTTGPSLoggerSettingsData::GetOutputMemory()
    {
    return iOutputMemory;
    }
TDes& CTTGPSLoggerSettingsData::GetOutputDirectory()
    {
    return iOutputDirectory;
    }
TInt& CTTGPSLoggerSettingsData::GetOutputIntervals()
    {
    return iOutputIntervals;
    }
TInt& CTTGPSLoggerSettingsData::GetOutputDistance()
    {
    return iOutputDistance;
    }
TInt& CTTGPSLoggerSettingsData::GetOutputAutoPauseHSpeed()
    {
    return iOutputAutoPauseHSpeed;
    }
TInt& CTTGPSLoggerSettingsData::GetOutputAutoPauseSpeed()
    {
    return iOutputAutoPauseSpeed;
    }
TInt& CTTGPSLoggerSettingsData::GetOutputAutoPauseHAcc()
    {
    return iOutputAutoPauseHAcc;
    }
TInt& CTTGPSLoggerSettingsData::GetOutputAutoPauseHDOP()
    {
    return iOutputAutoPauseHDOP;
    }
TInt& CTTGPSLoggerSettingsData::GetOutputAutoPausePDOP()
    {
    return iOutputAutoPausePDOP;
    }
TDes& CTTGPSLoggerSettingsData::GetNMEAExt()
    {
    return iNMEAExt;
    }
CArrayFix<TInt>& CTTGPSLoggerSettingsData::GetNMEASentences()
	{
    return iNMEASentences;
	}
TDes& CTTGPSLoggerSettingsData::GetGPXExt()
    {
    return iGPXExt;
    }
TDes& CTTGPSLoggerSettingsData::GetGPXAuthor()
    {
    return iGPXAuthor;
    }
TDes& CTTGPSLoggerSettingsData::GetGPXEmail()
    {
    return iGPXEmail;
    }
TDes& CTTGPSLoggerSettingsData::GetGPXUrl()
    {
    return iGPXUrl;
    }
TDes& CTTGPSLoggerSettingsData::GetGPXUrlName()
    {
    return iGPXUrlName;
    }
CArrayFix<TInt>& CTTGPSLoggerSettingsData::GetGPXTags()
	{
    return iGPXTags;
	}
TDes& CTTGPSLoggerSettingsData::GetKMLExt()
    {
    return iKMLExt;
    }
TInt& CTTGPSLoggerSettingsData::GetKMLPmName()
    {
    return iKMLPmName;
    }
CArrayFix<TInt>& CTTGPSLoggerSettingsData::GetKMLPmDescription()
	{
    return iKMLPmDescription;
	}
#ifdef DSYSTEM
TBool CTTGPSLoggerSettingsData::GeneralSystem() const
    {
    return iGeneralSystem;
    }
#endif
TInt CTTGPSLoggerSettingsData::PositioningMethod() const
    {
    return iPositioningMethod;
    }
TInt CTTGPSLoggerSettingsData::DisplayMeasurementSystem() const
    {
    return iDisplayMeasurementSystem;
    }
TInt CTTGPSLoggerSettingsData::DisplayCoordinateFormat() const
    {
    return iDisplayCoordinateFormat;
    }
TInt CTTGPSLoggerSettingsData::DisplayItems(TInt aIndex) const
    {
    if (aIndex < 0 || aIndex >= iDisplayItems.Count())
    	{
    	return EFalse;
    	}
    else
    	{
    	return iDisplayItems[aIndex];
    	}
    }
TBool CTTGPSLoggerSettingsData::DisplayShortcuts() const
    {
    return iDisplayShortcuts;
    }
TBool CTTGPSLoggerSettingsData::GpxSegment() const
    {
    return iGpxSegment;
    }
TInt CTTGPSLoggerSettingsData::OutputAutostart() const
    {
    return iOutputAutostart;
    }
TInt CTTGPSLoggerSettingsData::OutputIntervals() const
    {
    TInt result(0);
    switch (iOutputIntervals)
        {
        case ETTGPSLoggerSettingsOutputIntervalsNone:
            result = 0; break;
        case ETTGPSLoggerSettingsOutputIntervals1Sec:
            result = 1; break;
        case ETTGPSLoggerSettingsOutputIntervals5Sec:
            result = 5; break;
        case ETTGPSLoggerSettingsOutputIntervals10Sec:
            result = 10; break;
        case ETTGPSLoggerSettingsOutputIntervals15Sec:
            result = 15; break;
        case ETTGPSLoggerSettingsOutputIntervals20Sec:
            result = 20; break;
        case ETTGPSLoggerSettingsOutputIntervals30Sec:
            result = 30; break;
        case ETTGPSLoggerSettingsOutputIntervals60Sec:
            result = 60; break;
        }
    return result;
    }
void CTTGPSLoggerSettingsData::OutputDirectory(TDes& aDes) const
    {
	switch (iOutputMemory)
		{
		case 2:
			aDes.Copy(PathInfo::MemoryCardRootPath());
			break;
		case 3:
			aDes.Copy(_L("f:\\"));
			break;
		default:
			aDes.Copy(PathInfo::PhoneMemoryRootPath());
			break;
		}
    aDes.Append(PathInfo::OthersPath());
    aDes.Append(iOutputDirectory);
    aDes.Append(KPathDelimiter);
    }
TInt CTTGPSLoggerSettingsData::OutputDistance() const
    {
    TInt result(0);
    switch (iOutputDistance)
        {
        case ETTGPSLoggerSettingsOutputDistanceNone:
            result = 0; break;
        case ETTGPSLoggerSettingsOutputDistance10m:
            result = 10; break;
        case ETTGPSLoggerSettingsOutputDistance20m:
            result = 20; break;
        case ETTGPSLoggerSettingsOutputDistance25m:
            result = 25; break;
        case ETTGPSLoggerSettingsOutputDistance50m:
            result = 50; break;
        case ETTGPSLoggerSettingsOutputDistance100m:
            result = 100; break;
        case ETTGPSLoggerSettingsOutputDistance200m:
            result = 200; break;
        case ETTGPSLoggerSettingsOutputDistance250m:
            result = 250; break;
        case ETTGPSLoggerSettingsOutputDistance500m:
            result = 500; break;
        case ETTGPSLoggerSettingsOutputDistance1000m:
            result = 1000; break;
        }
    return result;
    }
TInt CTTGPSLoggerSettingsData::OutputAutoPauseHSpeed() const
    {
    TInt result(-1);
    switch (iOutputAutoPauseHSpeed)
        {
        case ETTGPSLoggerSettingsOutputAutoPauseSpeedNone:
            result = -1; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed0:
            result = 0; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed1:
            result = 1; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed2:
            result = 2; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed3:
            result = 3; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed4:
            result = 4; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed5:
            result = 5; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed10:
            result = 10; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed20:
            result = 20; break;
        }
    return result;
    }
TInt CTTGPSLoggerSettingsData::OutputAutoPauseSpeed() const
    {
    TInt result(-1);
    switch (iOutputAutoPauseSpeed)
        {
        case ETTGPSLoggerSettingsOutputAutoPauseSpeedNone:
            result = -1; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed0:
            result = 0; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed1:
            result = 1; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed2:
            result = 2; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed3:
            result = 3; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed4:
            result = 4; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed5:
            result = 5; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed10:
            result = 10; break;
        case ETTGPSLoggerSettingsOutputAutoPauseSpeed20:
            result = 20; break;
        }
    return result;
    }
TInt CTTGPSLoggerSettingsData::OutputAutoPauseHAcc() const
    {
    TInt result(0);
    switch (iOutputAutoPauseHAcc)
        {
        case ETTGPSLoggerSettingsOutputAutoPauseAccNone:
            result = 0; break;
        case ETTGPSLoggerSettingsOutputAutoPauseAcc10:
            result = 10; break;
        case ETTGPSLoggerSettingsOutputAutoPauseAcc20:
            result = 20; break;
        case ETTGPSLoggerSettingsOutputAutoPauseAcc30:
            result = 30; break;
        case ETTGPSLoggerSettingsOutputAutoPauseAcc40:
            result = 40; break;
        case ETTGPSLoggerSettingsOutputAutoPauseAcc50:
            result = 50; break;
        case ETTGPSLoggerSettingsOutputAutoPauseAcc100:
            result = 100; break;
        }
    return result;
    }
TInt CTTGPSLoggerSettingsData::OutputAutoPauseHDOP() const
    {
    TInt result(0);
    switch (iOutputAutoPauseHDOP)
        {
        case ETTGPSLoggerSettingsOutputAutoPauseDOPNone:
            result = 0; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP2:
            result = 2; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP3:
            result = 3; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP4:
            result = 4; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP5:
            result = 5; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP6:
            result = 6; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP7:
            result = 7; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP8:
            result = 8; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP9:
            result = 9; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP10:
            result = 10; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP20:
            result = 20; break;
        }
    return result;
    }
TInt CTTGPSLoggerSettingsData::OutputAutoPausePDOP() const
    {
    TInt result(0);
    switch (iOutputAutoPausePDOP)
        {
        case ETTGPSLoggerSettingsOutputAutoPauseDOPNone:
            result = 0; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP2:
            result = 2; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP3:
            result = 3; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP4:
            result = 4; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP5:
            result = 5; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP6:
            result = 6; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP7:
            result = 7; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP8:
            result = 8; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP9:
            result = 9; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP10:
            result = 10; break;
        case ETTGPSLoggerSettingsOutputAutoPauseDOP20:
            result = 20; break;
        }
    return result;
    }
const TDesC& CTTGPSLoggerSettingsData::NMEAExt() const
    {
    return iNMEAExt;
    }
TInt CTTGPSLoggerSettingsData::NMEASentences(TInt aIndex) const
    {
    if (aIndex < 0 || aIndex >= iNMEASentences.Count())
    	{
    	return EFalse;
    	}
    else
    	{
    	return iNMEASentences[aIndex];
    	}
    }
const TDesC& CTTGPSLoggerSettingsData::GPXExt() const
    {
    return iGPXExt;
    }
const TDesC& CTTGPSLoggerSettingsData::GPXAuthor() const
    {
    return iGPXAuthor;
    }
const TDesC& CTTGPSLoggerSettingsData::GPXEmail() const
    {
    return iGPXEmail;
    }
const TDesC& CTTGPSLoggerSettingsData::GPXUrl() const
    {
    return iGPXUrl;
    }
const TDesC& CTTGPSLoggerSettingsData::GPXUrlName() const
    {
    return iGPXUrlName;
    }
TInt CTTGPSLoggerSettingsData::GPXTags(TInt aIndex) const
    {
    if (aIndex < 0 || aIndex >= iGPXTags.Count())
    	{
    	return EFalse;
    	}
    else
    	{
    	return iGPXTags[aIndex];
    	}
    }
const TDesC& CTTGPSLoggerSettingsData::KMLExt() const
    {
    return iKMLExt;
    }
TInt CTTGPSLoggerSettingsData::KMLPmName() const
    {
    return iKMLPmName;
    }
TInt CTTGPSLoggerSettingsData::KMLPmDescription(TInt aIndex) const
    {
    if (aIndex < 0 || aIndex >= iKMLPmDescription.Count())
    	{
    	return EFalse;
    	}
    else
    	{
    	return iKMLPmDescription[aIndex];
    	}
    }
