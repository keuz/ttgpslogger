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
#ifndef TTGPSLOGGERPOSITIONRECORDER_H
#define TTGPSLOGGERPOSITIONRECORDER_H

#ifdef USELIGHTS
#include <HWRMLight.h> // Link against HWRMLightClient.lib.
#endif

class CTTGPSLoggerEngine;
class CTTGPSLoggerPositionData;
class MTTGPSLoggerRecorderFormat;
class MTTGPSLoggerPositionObserver;
class MTTGPSLoggerRecorderObserver;

class CTTGPSLoggerPositionRecorder : public CBase, public MTTGPSLoggerPositionObserver
	{
public:
	enum TTTGPSLoggerPositionRecorderStatus
	    {
        EStatusNone = 0,
	    EStatusWaiting,
	    EStatusRecording,
	    EStatusPausing,
	    EStatusAutoPausing,
	    EStatusNum
	    };
	
public:
    static CTTGPSLoggerPositionRecorder* NewL(CTTGPSLoggerEngine* aEngine);
    static CTTGPSLoggerPositionRecorder* NewLC(CTTGPSLoggerEngine* aEngine);
	~CTTGPSLoggerPositionRecorder();

private:
	CTTGPSLoggerPositionRecorder(CTTGPSLoggerEngine* aEngine);
	void ConstructL();

public:
    inline TInt Status() const
        {
        return iStatus;
        };
    // From MTTGPSLoggerPositionObserver
    void PositionUpdatedL(CTTGPSLoggerPositionData* aPositionData);
    void PositionUpdatedPartialL(CTTGPSLoggerPositionData* aPositionData);
    void PositionQualityLossL();
    void PositionTimedOutL();
    void PositionErrorL(TInt aErrorCode);
    
    void AddRecorderObserver(MTTGPSLoggerRecorderObserver* aObserver);
    void DeleteRecorderObserver(MTTGPSLoggerRecorderObserver* aObserver);
    void StartL(MTTGPSLoggerRecorderFormat *aRecorderFormat, TTime aStartTime);
    void RecordL();
    void PauseL();
    void ResumeL();
    void SyncTimeL();
    void StopL();
    
private:
    void Init();
    void AppendDataToFileL(const TDesC8& aData);
    void SetDataToFileL(const TDesC8& aData);
    void SetFilePathL();
    TInt NewStatus();
    TBool AutoRecord();
    TTime GetPositionTime(HPositionGenericInfo* aPositionInfo);
    const TDesC& MsgNaviState();
    
private:
    CTTGPSLoggerEngine* iEngine;
    MTTGPSLoggerRecorderFormat *iRecorderFormat;
    TInt iStatus;
    RPointerArray<MTTGPSLoggerRecorderObserver> iRecorderObserver;
    TFileName iFileName;
    TFileName iFilePath;
    TInt iFilePoint;
    TInt iFileSize;
    RBuf8 iFileDataBuf;
    TTime iLastRecordTime;
    TPosition iLastPosition;
    CTTGPSLoggerPositionData* iPositionData;
    TTime iStartRecordTime;
    HBufC* iMsgNaviStateNone;
    HBufC* iMsgNaviStateWaiting;
    HBufC* iMsgNaviStateRecording;
    HBufC* iMsgNaviStatePausing;
    HBufC* iMsgNaviStateAutoPausing;
    HBufC* iMsgRecordSuccess;
    HBufC* iMsgRecordFail1;
    HBufC* iMsgRecordFail2;
    HBufC* iMsgSatTime;
#ifdef USELIGHTS
    CHWRMLight* iLight;
#endif
};

#endif // TTGPSLOGGERPOSITIONRECORDER_H
