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
#ifndef TTGPSLOGGERSETTINGSDATA_H
#define TTGPSLOGGERSETTINGSDATA_H

#include <caknmemoryselectiondialog.h>

class CBase;

class CTTGPSLoggerSettingsData : public CBase
	{
public:
    enum CTTGPSLoggerSettingsDiaplayMeasurementSystem
        {
        ETTGPSLoggerSettingsDisplayMeasurementSystemMetric = 0,
        ETTGPSLoggerSettingsDisplayMeasurementSystemImperial
        };
    enum CTTGPSLoggerSettingsDiaplayCoordinateFormat
        {
        ETTGPSLoggerSettingsDisplayCoordinateFormatDDDMMSSSS = 0,
        ETTGPSLoggerSettingsDisplayCoordinateFormatDDDMMMMMM,
        ETTGPSLoggerSettingsDisplayCoordinateFormatDDDDDDDD
        };
    enum CTTGPSLoggerSettingsDiaplayItemsIndex
        {
        ETTGPSLoggerSettingsDisplayItemsIndexLon = 0,
        ETTGPSLoggerSettingsDisplayItemsIndexLat,
        ETTGPSLoggerSettingsDisplayItemsIndexAlt,
        ETTGPSLoggerSettingsDisplayItemsIndexSeaAlt,
        ETTGPSLoggerSettingsDisplayItemsIndexHSpeedWithVSpeed,
        ETTGPSLoggerSettingsDisplayItemsIndexHSpeed,
        ETTGPSLoggerSettingsDisplayItemsIndexVSpeed,
        ETTGPSLoggerSettingsDisplayItemsIndexCourse,
        ETTGPSLoggerSettingsDisplayItemsIndexHAccWithVAcc,
        ETTGPSLoggerSettingsDisplayItemsIndexHAcc,
        ETTGPSLoggerSettingsDisplayItemsIndexVAcc,
        ETTGPSLoggerSettingsDisplayItemsIndexHDOPWithVDOP,
        ETTGPSLoggerSettingsDisplayItemsIndexHDOP,
        ETTGPSLoggerSettingsDisplayItemsIndexVDOP,
        ETTGPSLoggerSettingsDisplayItemsIndexPDOP,
        ETTGPSLoggerSettingsDisplayItemsIndexSatNum,
        ETTGPSLoggerSettingsDisplayItemsIndexSatTime,
        ETTGPSLoggerSettingsDisplayItemsIndexSysTime,
        ETTGPSLoggerSettingsDisplayItemsNum
        };
    enum CTTGPSLoggerSettingsDiaplayItemsInitial
        {
        ETTGPSLoggerSettingsDisplayItemsInitialLon = ETrue,
        ETTGPSLoggerSettingsDisplayItemsInitialLat = ETrue,
        ETTGPSLoggerSettingsDisplayItemsInitialAlt = ETrue,
        ETTGPSLoggerSettingsDisplayItemsInitialSeaAlt = EFalse,
        ETTGPSLoggerSettingsDisplayItemsInitialHSpeedWithVSpeed = ETrue,
        ETTGPSLoggerSettingsDisplayItemsInitialHSpeed = EFalse,
        ETTGPSLoggerSettingsDisplayItemsInitialVSpeed = EFalse,
        ETTGPSLoggerSettingsDisplayItemsInitialCourse = EFalse,
        ETTGPSLoggerSettingsDisplayItemsInitialHAccWithVAcc = ETrue,
        ETTGPSLoggerSettingsDisplayItemsInitialHAcc = EFalse,
        ETTGPSLoggerSettingsDisplayItemsInitialVAcc = EFalse,
        ETTGPSLoggerSettingsDisplayItemsInitialHDOPWithVDOP = ETrue,
        ETTGPSLoggerSettingsDisplayItemsInitialHDOP = EFalse,
        ETTGPSLoggerSettingsDisplayItemsInitialVDOP = EFalse,
        ETTGPSLoggerSettingsDisplayItemsInitialPDOP = EFalse,
        ETTGPSLoggerSettingsDisplayItemsInitialSatNum = ETrue,
        ETTGPSLoggerSettingsDisplayItemsInitialSatTime = ETrue,
        ETTGPSLoggerSettingsDisplayItemsInitialSysTime = ETrue
        };
    enum CTTGPSLoggerSettingsOutputAutostart
        {
        ETTGPSLoggerSettingsOutputAutostartDisabled = 0,
        ETTGPSLoggerSettingsOutputAutostartNMEA,
        ETTGPSLoggerSettingsOutputAutostartGPX,
        ETTGPSLoggerSettingsOutputAutostartKML,
        };
    enum CTTGPSLoggerSettingsOutputIntervals
        {
        ETTGPSLoggerSettingsOutputIntervalsNone = 0,
        ETTGPSLoggerSettingsOutputIntervals1Sec,
        ETTGPSLoggerSettingsOutputIntervals5Sec,
        ETTGPSLoggerSettingsOutputIntervals10Sec,
        ETTGPSLoggerSettingsOutputIntervals15Sec,
        ETTGPSLoggerSettingsOutputIntervals20Sec,
        ETTGPSLoggerSettingsOutputIntervals30Sec,
        ETTGPSLoggerSettingsOutputIntervals60Sec
        };
    enum CTTGPSLoggerSettingsOutputDistance
        {
        ETTGPSLoggerSettingsOutputDistanceNone = 0,
        ETTGPSLoggerSettingsOutputDistance10m,
        ETTGPSLoggerSettingsOutputDistance20m,
        ETTGPSLoggerSettingsOutputDistance25m,
        ETTGPSLoggerSettingsOutputDistance50m,
        ETTGPSLoggerSettingsOutputDistance100m,
        ETTGPSLoggerSettingsOutputDistance200m,
        ETTGPSLoggerSettingsOutputDistance250m,
        ETTGPSLoggerSettingsOutputDistance500m,
        ETTGPSLoggerSettingsOutputDistance1000m
        };
    enum CTTGPSLoggerSettingsOutputAutoPauseSpeed
        {
        ETTGPSLoggerSettingsOutputAutoPauseSpeedNone = 0,
        ETTGPSLoggerSettingsOutputAutoPauseSpeed0,
        ETTGPSLoggerSettingsOutputAutoPauseSpeed1,
        ETTGPSLoggerSettingsOutputAutoPauseSpeed2,
        ETTGPSLoggerSettingsOutputAutoPauseSpeed3,
        ETTGPSLoggerSettingsOutputAutoPauseSpeed4,
        ETTGPSLoggerSettingsOutputAutoPauseSpeed5,
        ETTGPSLoggerSettingsOutputAutoPauseSpeed10,
        ETTGPSLoggerSettingsOutputAutoPauseSpeed20,
        };
    enum CTTGPSLoggerSettingsOutputAutoPauseAcc
    	{
        ETTGPSLoggerSettingsOutputAutoPauseAccNone = 0,
        ETTGPSLoggerSettingsOutputAutoPauseAcc10,
        ETTGPSLoggerSettingsOutputAutoPauseAcc20,
        ETTGPSLoggerSettingsOutputAutoPauseAcc30,
        ETTGPSLoggerSettingsOutputAutoPauseAcc40,
        ETTGPSLoggerSettingsOutputAutoPauseAcc50,
        ETTGPSLoggerSettingsOutputAutoPauseAcc100
    	};
    enum CTTGPSLoggerSettingsOutputAutoPauseDOP
    	{
        ETTGPSLoggerSettingsOutputAutoPauseDOPNone = 0,
        ETTGPSLoggerSettingsOutputAutoPauseDOP2,
        ETTGPSLoggerSettingsOutputAutoPauseDOP3,
        ETTGPSLoggerSettingsOutputAutoPauseDOP4,
        ETTGPSLoggerSettingsOutputAutoPauseDOP5,
        ETTGPSLoggerSettingsOutputAutoPauseDOP6,
        ETTGPSLoggerSettingsOutputAutoPauseDOP7,
        ETTGPSLoggerSettingsOutputAutoPauseDOP8,
        ETTGPSLoggerSettingsOutputAutoPauseDOP9,
        ETTGPSLoggerSettingsOutputAutoPauseDOP10,
        ETTGPSLoggerSettingsOutputAutoPauseDOP20
    	};
    enum CTTGPSLoggerSettingsNMEASentencesIndex
        {
        ETTGPSLoggerSettingsNMEASentencesIndexGPGGA = 0,
        ETTGPSLoggerSettingsNMEASentencesIndexGPGLL,
        ETTGPSLoggerSettingsNMEASentencesIndexGPGSA,
        ETTGPSLoggerSettingsNMEASentencesIndexGPGST,
        ETTGPSLoggerSettingsNMEASentencesIndexGPGSV,
        ETTGPSLoggerSettingsNMEASentencesIndexGPRMC,
        ETTGPSLoggerSettingsNMEASentencesIndexGPVTG,
        ETTGPSLoggerSettingsNMEASentencesIndexGPZDA,
        ETTGPSLoggerSettingsNMEASentencesIndexOthers,
        ETTGPSLoggerSettingsNMEASentencesNum
        };
    enum CTTGPSLoggerSettingsNMEASentencesInitial
        {
        ETTGPSLoggerSettingsNMEASentencesInitialGPGGA = ETrue,
        ETTGPSLoggerSettingsNMEASentencesInitialGPGLL = ETrue,
        ETTGPSLoggerSettingsNMEASentencesInitialGPGSA = ETrue,
        ETTGPSLoggerSettingsNMEASentencesInitialGPGST = ETrue,
        ETTGPSLoggerSettingsNMEASentencesInitialGPGSV = ETrue,
        ETTGPSLoggerSettingsNMEASentencesInitialGPRMC = ETrue,
        ETTGPSLoggerSettingsNMEASentencesInitialGPVTG = ETrue,
        ETTGPSLoggerSettingsNMEASentencesInitialGPZDA = ETrue,
        ETTGPSLoggerSettingsNMEASentencesInitialOthers = EFalse
        };
    enum CTTGPSLoggerSettingsGPXTagsIndex
        {
        ETTGPSLoggerSettingsGPXTagsIndexTime = 0,
        ETTGPSLoggerSettingsGPXTagsIndexEle,
        ETTGPSLoggerSettingsGPXTagsIndexSeaLevel,
        ETTGPSLoggerSettingsGPXTagsIndexCourse,
        ETTGPSLoggerSettingsGPXTagsIndexSpeed,
        ETTGPSLoggerSettingsGPXTagsIndexUseVSpeed,
        ETTGPSLoggerSettingsGPXTagsIndexFix,
        ETTGPSLoggerSettingsGPXTagsIndexSat,
        ETTGPSLoggerSettingsGPXTagsIndexHdop,
        ETTGPSLoggerSettingsGPXTagsIndexVdop,
        ETTGPSLoggerSettingsGPXTagsIndexPdop,
        ETTGPSLoggerSettingsGPXTagsNum
        };
    enum CTTGPSLoggerSettingsGPXTagsInitial
        {
        ETTGPSLoggerSettingsGPXTagsInitialTime = ETrue,
        ETTGPSLoggerSettingsGPXTagsInitialEle = ETrue,
        ETTGPSLoggerSettingsGPXTagsInitialSeaAlt = ETrue,
        ETTGPSLoggerSettingsGPXTagsInitialCourse = ETrue,
        ETTGPSLoggerSettingsGPXTagsInitialSpeed = ETrue,
        ETTGPSLoggerSettingsGPXTagsInitialUseVSpeed = EFalse,
        ETTGPSLoggerSettingsGPXTagsInitialFix = ETrue,
        ETTGPSLoggerSettingsGPXTagsInitialSat = ETrue,
        ETTGPSLoggerSettingsGPXTagsInitialHdop = ETrue,
        ETTGPSLoggerSettingsGPXTagsInitialVdop = ETrue,
        ETTGPSLoggerSettingsGPXTagsInitialPdop = ETrue
        };
    enum CTTGPSLoggerSettingsKMLPmName
        {
        ETTGPSLoggerSettingsKMLPmNameNone = 0,
        ETTGPSLoggerSettingsKMLPmNamePoint,
        ETTGPSLoggerSettingsKMLPmNameTimeUTC,
        ETTGPSLoggerSettingsKMLPmNameTimeLocal
        };
    enum CTTGPSLoggerSettingsKMLPmDescriptionIndex
        {
        ETTGPSLoggerSettingsKMLPmDescriptionIndexPoint = 0,
        ETTGPSLoggerSettingsKMLPmDescriptionIndexLon,
        ETTGPSLoggerSettingsKMLPmDescriptionIndexLat,
        ETTGPSLoggerSettingsKMLPmDescriptionIndexAlt,
        ETTGPSLoggerSettingsKMLPmDescriptionIndexSeaAlt,
        ETTGPSLoggerSettingsKMLPmDescriptionIndexHSpeed,
        ETTGPSLoggerSettingsKMLPmDescriptionIndexVSpeed,
        ETTGPSLoggerSettingsKMLPmDescriptionIndexCourse,
        ETTGPSLoggerSettingsKMLPmDescriptionIndexHAcc,
        ETTGPSLoggerSettingsKMLPmDescriptionIndexVAcc,
        ETTGPSLoggerSettingsKMLPmDescriptionIndexHDOP,
        ETTGPSLoggerSettingsKMLPmDescriptionIndexVDOP,
        ETTGPSLoggerSettingsKMLPmDescriptionIndexPDOP,
        ETTGPSLoggerSettingsKMLPmDescriptionIndexSatNum,
        ETTGPSLoggerSettingsKMLPmDescriptionIndexSatTimeUTC,
        ETTGPSLoggerSettingsKMLPmDescriptionIndexSatTimeLocal,
        ETTGPSLoggerSettingsKMLPmDescriptionNum
        };
    enum CTTGPSLoggerSettingsKMLPmDescriptionInitial
        {
        ETTGPSLoggerSettingsKMLPmDescriptionInitialPoint = EFalse,
        ETTGPSLoggerSettingsKMLPmDescriptionInitialLon = ETrue,
        ETTGPSLoggerSettingsKMLPmDescriptionInitialLat = ETrue,
        ETTGPSLoggerSettingsKMLPmDescriptionInitialAlt = EFalse,
        ETTGPSLoggerSettingsKMLPmDescriptionInitialSeaAlt = EFalse,
        ETTGPSLoggerSettingsKMLPmDescriptionInitialHSpeed = EFalse,
        ETTGPSLoggerSettingsKMLPmDescriptionInitialVSpeed = EFalse,
        ETTGPSLoggerSettingsKMLPmDescriptionInitialCourse = EFalse,
        ETTGPSLoggerSettingsKMLPmDescriptionInitialHAcc = EFalse,
        ETTGPSLoggerSettingsKMLPmDescriptionInitialVAcc = EFalse,
        ETTGPSLoggerSettingsKMLPmDescriptionInitialHDOP = EFalse,
        ETTGPSLoggerSettingsKMLPmDescriptionInitialVDOP = EFalse,
        ETTGPSLoggerSettingsKMLPmDescriptionInitialPDOP = EFalse,
        ETTGPSLoggerSettingsKMLPmDescriptionInitialSatNum = EFalse,
        ETTGPSLoggerSettingsKMLPmDescriptionInitialSatTimeUTC = EFalse,
        ETTGPSLoggerSettingsKMLPmDescriptionInitialSatTimeLocal = ETrue
        };
    
public:
    static CTTGPSLoggerSettingsData* NewL();
    static CTTGPSLoggerSettingsData* NewLC();
	~CTTGPSLoggerSettingsData();

private:
	CTTGPSLoggerSettingsData();
	void ConstructL();

public:
	void CheckL();
	void LoadL();
	void SaveL();
    TInt& GetDisplayMeasurementSystem();
    TInt& GetDisplayCoordinateFormat();
    CArrayFix<TInt>& GetDisplayItems();
    TBool& GetDisplayShortcuts();
#ifdef DSYSTEM
    TBool& GetGeneralSystem();
#endif
    TInt& GetPositioningMethod();
    TBool& GetGpxSegment();
    TInt& GetOutputAutostart();
    //CAknMemorySelectionDialog::TMemory& GetOutputMemory();
    TInt& GetOutputMemory();
    TDes& GetOutputDirectory();
    TInt& GetOutputIntervals();
    TInt& GetOutputDistance();
    TInt& GetOutputAutoPauseHSpeed();
    TInt& GetOutputAutoPauseSpeed();
    TInt& GetOutputAutoPauseHAcc();
    TInt& GetOutputAutoPauseHDOP();
    TInt& GetOutputAutoPausePDOP();
    TDes& GetNMEAExt();
    CArrayFix<TInt>& GetNMEASentences();
    TDes& GetGPXExt();
    CArrayFix<TInt>& GetGPXTags();
    TDes& GetGPXAuthor();
    TDes& GetGPXEmail();
    TDes& GetGPXUrl();
    TDes& GetGPXUrlName();
    TDes& GetKMLExt();
    TInt& GetKMLPmName();
    CArrayFix<TInt>& GetKMLPmDescription();
    TInt DisplayMeasurementSystem() const;
    TInt DisplayCoordinateFormat() const;
    TInt DisplayItems(TInt aIndex) const;
    TBool DisplayShortcuts() const;
#ifdef DSYSTEM
    TBool GeneralSystem() const;
#endif
    TInt PositioningMethod() const;
    TBool GpxSegment() const;
    TInt OutputAutostart() const;
    TInt OutputIntervals() const;
    TInt OutputDistance() const;
    TInt OutputAutoPauseHSpeed() const;
    TInt OutputAutoPauseSpeed() const;
    TInt OutputAutoPauseHAcc() const;
    TInt OutputAutoPauseHDOP() const;
    TInt OutputAutoPausePDOP() const;
    void OutputDirectory(TDes& aDes) const;
    const TDesC& NMEAExt() const;
    TInt NMEASentences(TInt aIndex) const;
    const TDesC& GPXExt() const;
    const TDesC& GPXAuthor() const;
    const TDesC& GPXEmail() const;
    const TDesC& GPXUrl() const;
    const TDesC& GPXUrlName() const;
    TInt GPXTags(TInt aIndex) const;
    const TDesC& KMLExt() const;
    TInt KMLPmName() const;
    TInt KMLPmDescription(TInt aIndex) const;

private:
    TFileName iSettingsFileName;
    TInt iDisplayMeasurementSystem;
    TInt iDisplayCoordinateFormat;
	CArrayFixFlat<TInt> iDisplayItems;
    TBool iDisplayShortcuts;
#ifdef DSYSTEM
    TBool iGeneralSystem;
#endif
    TInt iPositioningMethod;
    TBool iGpxSegment;
    TInt iOutputAutostart;
//	CAknMemorySelectionDialog::TMemory iOutputMemory;
    TInt iOutputMemory;
    RBuf iOutputDirectory;
    TInt iOutputIntervals;
    TInt iOutputDistance;
    TInt iOutputAutoPauseHSpeed; // None,1024-,0-1 1024-,0-2 1024-,0-3 1024-,0-4 1024-,0-5 1024-,0-10 1024-,0-20 1024-,
    TInt iOutputAutoPauseSpeed;  // None,1024-,0-1 1024-,0-2 1024-,0-3 1024-,0-4 1024-,0-5 1024-,0-10 1024-,0-20 1024-, 
    TInt iOutputAutoPauseHAcc; // None,10-,20-,30-,40-,50-,100-
    TInt iOutputAutoPauseHDOP; // None,2-,3-,4-,5-,6-,7-,8-,9-,10-,20-
    TInt iOutputAutoPausePDOP; // None,2-,3-,4-,5-,6-,7-,8-,9-,10-,20-
    RBuf iNMEAExt;
	CArrayFixFlat<TInt> iNMEASentences;
    RBuf iGPXExt;
    RBuf iGPXAuthor;
    RBuf iGPXEmail;
    RBuf iGPXUrl;
    RBuf iGPXUrlName;
	CArrayFixFlat<TInt> iGPXTags;
    RBuf iKMLExt;
    TInt iKMLPmName;
	CArrayFixFlat<TInt> iKMLPmDescription;
	};

#endif // TTGPSLOGGERSETTINGSDATA_H
