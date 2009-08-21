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
#include <e32debug.h>
#include <e32base.h>
#include <e32math.h>
#include <e32std.h>
#include <aknnotewrappers.h> 
#include <coemain.h>
#include <f32file.h>
#include <lbs.h>
#include <s32file.h>
#include <TTGPSLogger.rsg>
#include "TTGPSLoggerCommon.h"
#include "TTGPSLoggerAppUi.h"
#include "TTGPSLoggerEngine.h"
#include "TTGPSLoggerRecorderFormat.h"
#include "TTGPSLoggerPositionData.h"
#include "TTGPSLoggerPositionObserver.h"
#include "TTGPSLoggerRecorderFormat.h"
#include "TTGPSLoggerRecorderObserver.h"
#include "TTGPSLoggerSettingsData.h"
#include "TTGPSLoggerPositionRecorder.h"

CTTGPSLoggerPositionRecorder::CTTGPSLoggerPositionRecorder(CTTGPSLoggerEngine* aEngine) :
    iEngine(aEngine),
    iRecorderFormat(NULL),
    iStatus(EStatusNone),
    iPositionData(NULL),
    iFilePoint(0),
    iFileSize(0),
    iStartRecordTime(0)
    {
    iLastRecordTime.UniversalTime();
    }

CTTGPSLoggerPositionRecorder::~CTTGPSLoggerPositionRecorder()
    {
    iRecorderObserver.Close();
    iFileDataBuf.Close();
    delete iPositionData;
    delete iMsgNaviStateNone;
    delete iMsgNaviStateWaiting;
    delete iMsgNaviStateRecording;
    delete iMsgNaviStatePausing;
    delete iMsgNaviStateAutoPausing;
    delete iMsgRecordSuccess;
    delete iMsgRecordFail1;
    delete iMsgRecordFail2;
    }

CTTGPSLoggerPositionRecorder* CTTGPSLoggerPositionRecorder::NewLC(CTTGPSLoggerEngine* aEngine)
    {
    CTTGPSLoggerPositionRecorder* self = new (ELeave) CTTGPSLoggerPositionRecorder(aEngine);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CTTGPSLoggerPositionRecorder* CTTGPSLoggerPositionRecorder::NewL(CTTGPSLoggerEngine* aEngine)
    {
    CTTGPSLoggerPositionRecorder* self = CTTGPSLoggerPositionRecorder::NewLC(aEngine);
    CleanupStack::Pop(); // self;
    return self;
    }

void CTTGPSLoggerPositionRecorder::ConstructL()
    {
    iMsgNaviStateNone = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_NAVISTATE_NONE);
    iMsgNaviStateWaiting = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_NAVISTATE_WAITING);
    iMsgNaviStateRecording = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_NAVISTATE_RECORDING);
    iMsgNaviStatePausing = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_NAVISTATE_PAUSING);
    iMsgNaviStateAutoPausing = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF32_MAINVIEW_NAVISTATE_AUTOPAUSING);
    
    iMsgRecordSuccess = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF_MSG_RECORD_SUCCESS);
    iMsgRecordFail1 = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF_MSG_RECORD_FAIL1);
    iMsgRecordFail2 = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF_MSG_RECORD_FAIL2);
    
    iPositionData = CTTGPSLoggerPositionData::NewL();
    iFileDataBuf.CreateL(8192);
    }

void CTTGPSLoggerPositionRecorder::Init()
    {
    iRecorderFormat = NULL;
    iPositionData->Init();
    iFileName.Zero();
    iFilePath.Zero();
    iFilePoint = 0;
    iFileSize = 0;
    iFileDataBuf.Zero();
    iLastRecordTime.UniversalTime();
    iStartRecordTime = 0;
    iLastPosition = TPosition();
    }

void CTTGPSLoggerPositionRecorder::AddRecorderObserver(MTTGPSLoggerRecorderObserver* aObserver)
    {
    if (iRecorderObserver.Find(aObserver) == KErrNotFound)
        {
        iRecorderObserver.Append(aObserver);
        switch(iStatus)
        	{
            case EStatusWaiting:
            case EStatusRecording:
            case EStatusAutoPausing:
            case EStatusPausing:
            	{
            	aObserver->RecorderUpdatedL(MsgNaviState(), iFileName, iFilePoint, iFileSize);
                break;
            	}
            case EStatusNone:
            default:
                break;
        	}
        }
    }
void CTTGPSLoggerPositionRecorder::DeleteRecorderObserver(MTTGPSLoggerRecorderObserver* aObserver)
    {
    TInt i1 = iRecorderObserver.Find(aObserver);
    if (i1 != KErrNotFound)
        {
        iRecorderObserver.Remove(i1);
        }
    }

void CTTGPSLoggerPositionRecorder::StartL(MTTGPSLoggerRecorderFormat *aRecorderFormat, TTime aStartTime)
    {
    if (iStatus == EStatusNone && aRecorderFormat)
        {
        TTime time;
        time.UniversalTime();
        iStartRecordTime = aStartTime;
        iRecorderFormat = aRecorderFormat;
        SetFilePathL();
        if (time >= iStartRecordTime)
            {
            iStatus = EStatusRecording;
            }
        else
            {
            iStatus = EStatusWaiting;
            }
        for (int i1 = 0; i1 < iRecorderObserver.Count(); i1++)
            {
            iRecorderObserver[i1]->RecorderUpdatedL(MsgNaviState(), iFileName, iFilePoint, iFileSize);
            }
        }
    }

void CTTGPSLoggerPositionRecorder::RecordL()
    {
    if (!iPositionData->IsValid() ||
        iPositionData->SatelliteTime() == 0 ||
        Math::IsNaN(iPositionData->Position().Longitude()) ||
        Math::IsNaN(iPositionData->Position().Latitude()))
        {
        CAknWarningNote *note = new (ELeave) CAknWarningNote(EFalse);
        note->ExecuteLD(*iMsgRecordFail1);
        }
    else if (iLastRecordTime == iPositionData->SatelliteTime())
        {
        RBuf msg;
        msg.CleanupClosePushL();
        msg.CreateL(128);
        msg.Append(*iMsgRecordFail2);
        msg.Append(_L("("));
        TTGPSLoggerCommon::AppendFormatDateTimeL(msg, iLastRecordTime, TRUE);
        msg.Append(_L(")"));
        CAknInformationNote *note = new (ELeave) CAknInformationNote(EFalse);
        note->ExecuteLD(msg);
        CleanupStack::PopAndDestroy(); // msg
        }
    else
        {
        if (iFilePoint == 0)
        	{
            iRecorderFormat->SetHeaderL(iFileDataBuf, iFileName);
            SetDataToFileL(iFileDataBuf);
        	}
        iRecorderFormat->SetBodyL(iFileDataBuf, iFileName, iFilePoint + 1, iPositionData);
        if (iFileSize > 0)
        	{
            AppendDataToFileL(iFileDataBuf);
        	}
        else
        	{
            SetDataToFileL(iFileDataBuf);
        	}
        iFilePoint += 1;
        iLastRecordTime = iPositionData->SatelliteTime();
        iLastPosition = iPositionData->Position();
        for (int i1 = 0; i1 < iRecorderObserver.Count(); i1++)
            {
            iRecorderObserver[i1]->RecorderUpdatedL(MsgNaviState(), iFileName, iFilePoint, iFileSize);
            }
        
        RBuf msg;
        msg.CleanupClosePushL();
        msg.CreateL(128);
        msg.Append(*iMsgRecordSuccess);
        msg.Append(_L("("));
        TTGPSLoggerCommon::AppendFormatDateTimeL(msg, iLastRecordTime, TRUE);
        msg.Append(_L(")"));
        CAknConfirmationNote *note = new (ELeave) CAknConfirmationNote(EFalse);
        note->ExecuteLD(msg);
        CleanupStack::PopAndDestroy(); // msg
        }
    }

void CTTGPSLoggerPositionRecorder::PauseL()
    {
    if (iStatus == EStatusRecording || iStatus == EStatusAutoPausing)
        {
        iStatus = EStatusPausing;
        for (int i1 = 0; i1 < iRecorderObserver.Count(); i1++)
            {
            iRecorderObserver[i1]->RecorderUpdatedL(MsgNaviState(), iFileName, iFilePoint, iFileSize);
            }
        }
    }

void CTTGPSLoggerPositionRecorder::ResumeL()
    {
    if (iStatus == EStatusPausing)
        {
        iStatus = EStatusRecording;
        iStatus = NewStatus();
        for (int i1 = 0; i1 < iRecorderObserver.Count(); i1++)
            {
            iRecorderObserver[i1]->RecorderUpdatedL(MsgNaviState(), iFileName, iFilePoint, iFileSize);
            }
        }
    }

void CTTGPSLoggerPositionRecorder::StopL()
    {
    if (iStatus == EStatusRecording || iStatus == EStatusPausing || iStatus == EStatusAutoPausing)
        {
        iStatus = EStatusNone;
        iRecorderFormat->SetFooterL(iFileDataBuf);
        AppendDataToFileL(iFileDataBuf);
        Init();
        for (int i1 = 0; i1 < iRecorderObserver.Count(); i1++)
            {
            iRecorderObserver[i1]->RecorderUpdatedL(MsgNaviState());
            }
        }
    else if (iStatus == EStatusWaiting)
        {
        iStatus = EStatusNone;
        Init();
        for (int i1 = 0; i1 < iRecorderObserver.Count(); i1++)
            {
            iRecorderObserver[i1]->RecorderUpdatedL(MsgNaviState());
            }
        }
    }

void CTTGPSLoggerPositionRecorder::PositionUpdatedL(CTTGPSLoggerPositionData* aPositionData)
    {
    iPositionData->SetPositionInfoL(aPositionData);
    TInt oldStatus = iStatus;
    TInt newStatus = NewStatus();
    if (newStatus == EStatusRecording && AutoRecord())
    	{
        iStatus = EStatusRecording;
        if (iFilePoint == 0)
        	{
            iRecorderFormat->SetHeaderL(iFileDataBuf, iFileName);
            SetDataToFileL(iFileDataBuf);
        	}
        iRecorderFormat->SetBodyL(iFileDataBuf, iFileName, iFilePoint + 1, iPositionData);
        if (iFileSize > 0)
        	{
            AppendDataToFileL(iFileDataBuf);
        	}
        else
        	{
            SetDataToFileL(iFileDataBuf);
        	}
        iFilePoint += 1;
        iLastRecordTime = iPositionData->SatelliteTime();
        iLastPosition = iPositionData->Position();
        for (int i1 = 0; i1 < iRecorderObserver.Count(); i1++)
            {
            iRecorderObserver[i1]->RecorderUpdatedL(MsgNaviState(), iFileName, iFilePoint, iFileSize);
            }
    	}
    else
    	{
    	iStatus = newStatus;
        if (oldStatus != newStatus)
        	{
            for (int i1 = 0; i1 < iRecorderObserver.Count(); i1++)
                {
                iRecorderObserver[i1]->RecorderUpdatedL(MsgNaviState(), iFileName, iFilePoint, iFileSize);
                }
        	}
    	}
    if (oldStatus != newStatus)
    	{
    	if ((oldStatus == EStatusRecording && newStatus == EStatusAutoPausing) ||
    		(oldStatus == EStatusAutoPausing && newStatus == EStatusRecording))
    		{
    		}
    	else
    		{
            iEngine->SetSoftKeyL();
            iEngine->SetToolbarL();
    		}
    	}
    }

void CTTGPSLoggerPositionRecorder::PositionUpdatedPartialL(CTTGPSLoggerPositionData* aPositionData)
    {
    PositionUpdatedL(aPositionData);
    }

void CTTGPSLoggerPositionRecorder::PositionQualityLossL()
    {
    iPositionData->Init();
    }

void CTTGPSLoggerPositionRecorder::PositionTimedOutL()
    {
    iPositionData->Init();
    }

void CTTGPSLoggerPositionRecorder::PositionErrorL(TInt aErrorCode)
    {
    iPositionData->Init();
    }

void CTTGPSLoggerPositionRecorder::AppendDataToFileL(const TDesC8& aData)
    {
    TInt err;
    
    if (aData.Size() == 0)
    	return;
    
    RFile file;
    err = file.Open(CCoeEnv::Static()->FsSession(), iFilePath, EFileStreamText | EFileWrite);
    CleanupClosePushL(file);
    if (err != KErrNone)
        {
        CleanupStack::PopAndDestroy(); // file
        return;
        }
    TInt pos = 0;
    err = file.Seek(ESeekEnd, pos);
    if (err != KErrNone)
        {
        CleanupStack::PopAndDestroy(); // file
        return;
        }
    RFileWriteStream outputFileStream(file, pos);
    CleanupClosePushL(outputFileStream);
    outputFileStream.WriteL(aData);
    iFileSize += aData.Size();
    CleanupStack::PopAndDestroy(); // outputFileStream
    CleanupStack::PopAndDestroy(); // file
    }

void CTTGPSLoggerPositionRecorder::SetDataToFileL(const TDesC8& aData)
    {
    TInt err;
    
    if (aData.Size() == 0)
    	return;
    
    err = CCoeEnv::Static()->FsSession().MkDirAll(iFilePath);
    if ((err != KErrNone) && (err != KErrAlreadyExists))
        {
        return;
        }
    
    RFile file;
    err = file.Replace(CCoeEnv::Static()->FsSession(), iFilePath, EFileStreamText | EFileWrite);
    CleanupClosePushL(file);
    if (err != KErrNone)
        {
        CleanupStack::PopAndDestroy(); // file
        return;
        }
    RFileWriteStream outputFileStream(file);
    CleanupClosePushL(outputFileStream);
    outputFileStream.WriteL(aData);
    iFileSize = aData.Size();
    CleanupStack::PopAndDestroy(); // outputFileStream
    CleanupStack::PopAndDestroy(); // file
    }

void CTTGPSLoggerPositionRecorder::SetFilePathL()
    {
    RBuf fileExt;
    fileExt.CleanupClosePushL();
    fileExt.CreateL(KMaxPath);
    iRecorderFormat->SetFileExtL(fileExt);
    
    iFilePath.Zero();
    iFileName.Zero();
    
    TTime time = iStartRecordTime;
    TLocale locale;
    TTimeIntervalSeconds universalTimeOffset(locale.UniversalTimeOffset());
    time += universalTimeOffset;
    time.FormatL(iFileName, _L("%F%Y%M%D%H%T%S"));
    iFileName.Append(KExtDelimiter);
    iFileName.Append(fileExt);
    iEngine->Settings()->OutputDirectory(iFilePath);
    iFilePath.Append(iFileName);
    
    CleanupStack::PopAndDestroy(); // fileExt
    }

TInt CTTGPSLoggerPositionRecorder::NewStatus()
	{
    switch(iStatus)
    	{
        case EStatusWaiting:
        	{
            if (!iPositionData->IsValid() || iPositionData->SatelliteTime() < iStartRecordTime)
            	{
            	return iStatus;
            	}
            break;
        	}
        case EStatusRecording:
        case EStatusAutoPausing:
        	{
            if (!iPositionData->IsValid() || iPositionData->SatelliteTime() == 0)
            	{
        	    return iStatus;
            	}
            break;
        	}
        case EStatusNone:
        case EStatusPausing:
        default:
        	return iStatus;
    	}

    TInt statusNew = EStatusRecording;
    if (iEngine->Settings()->OutputAutoPauseHSpeed() >= 0)
		{
		if (Math::IsNaN(iPositionData->HorizontalSpeed()))
			{
	        statusNew = EStatusAutoPausing;
			}
		else
			{
			if (iPositionData->HorizontalSpeed() * 3.6 < iEngine->Settings()->OutputAutoPauseHSpeed() ||
			    iPositionData->HorizontalSpeed() * 3.6 >= 1024.0)
				{
		        statusNew = EStatusAutoPausing;
				}
			}
		}
	if (iEngine->Settings()->OutputAutoPauseSpeed() >= 0)
		{
	    TRealX realNaN;
	    realNaN.SetNaN();
	    TReal64 speed;
	    realNaN.GetTReal(speed);
	    if (!Math::IsNaN(iPositionData->HorizontalSpeed()) &&
	        !Math::IsNaN(iPositionData->VerticalSpeed()))
	        {
	        TReal64 tmp = iPositionData->HorizontalSpeed() * iPositionData->HorizontalSpeed() +
	                      iPositionData->VerticalSpeed() * iPositionData->VerticalSpeed();
	        Math::Sqrt(speed, tmp);
	        }
	    else if (!Math::IsNaN(iPositionData->VerticalSpeed()))
	        {
	        speed = iPositionData->VerticalSpeed();
	        }
	    else if (!Math::IsNaN(iPositionData->HorizontalSpeed()))
	        {
	        speed = iPositionData->HorizontalSpeed();
	        }
		
		if (Math::IsNaN(speed))
			{
	        statusNew = EStatusAutoPausing;
			}
		else
			{
			if (speed * 3.6 < iEngine->Settings()->OutputAutoPauseHSpeed() ||
			    speed * 3.6 >= 1024.0)
				{
		        statusNew = EStatusAutoPausing;
				}
			}
		}
	if (iEngine->Settings()->OutputAutoPauseHAcc() > 0)
		{
		if (Math::IsNaN(iPositionData->Position().HorizontalAccuracy()))
			{
	        statusNew = EStatusAutoPausing;
			}
		else
			{
			if (iPositionData->Position().HorizontalAccuracy() >= iEngine->Settings()->OutputAutoPauseHAcc())
				{
		        statusNew = EStatusAutoPausing;
				}
			}
		}
	if (iEngine->Settings()->OutputAutoPauseHDOP() > 0)
		{
		if (Math::IsNaN(iPositionData->SatelliteHorizontalDoP()))
			{
	        statusNew = EStatusAutoPausing;
			}
		else
			{
			if (iPositionData->SatelliteHorizontalDoP() >= iEngine->Settings()->OutputAutoPauseHDOP())
				{
		        statusNew = EStatusAutoPausing;
				}
			}
		}
	if (iEngine->Settings()->OutputAutoPausePDOP() > 0)
		{
		if (Math::IsNaN(iPositionData->SatellitePositionDoP()))
			{
	        statusNew = EStatusAutoPausing;
			}
		else
			{
			if (iPositionData->SatellitePositionDoP() >= iEngine->Settings()->OutputAutoPausePDOP())
				{
		        statusNew = EStatusAutoPausing;
				}
			}
		}
    
	return statusNew;
	}

TBool CTTGPSLoggerPositionRecorder::AutoRecord()
    {
    if (!iPositionData->IsValid() ||
        iPositionData->SatelliteTime() == 0 ||
        Math::IsNaN(iPositionData->Position().Longitude()) ||
        Math::IsNaN(iPositionData->Position().Latitude()))
        {
        return EFalse;
        }
    
    if (iEngine->Settings()->OutputIntervals() > 0)
    	{
        TTime tm = iPositionData->SatelliteTime();
        if ((tm.DateTime().Second() % iEngine->Settings()->OutputIntervals()) == 0)
        	{
            return ETrue;
        	}
    	}
    
    if (iEngine->Settings()->OutputDistance() > 0)
    	{
    	if (Math::IsNaN(iLastPosition.Longitude()) || Math::IsNaN(iLastPosition.Latitude()))
    		{
            return ETrue;
    		}
    	TCoordinate lastCoodinate = TCoordinate(iLastPosition.Latitude(), iLastPosition.Longitude());
    	TCoordinate nowCoodinate = TCoordinate(iPositionData->Position().Latitude(), iPositionData->Position().Longitude());
    	TReal32 distance = 0;
    	nowCoodinate.Distance(lastCoodinate, distance);
    	if (distance >= iEngine->Settings()->OutputDistance())
        	{
            return ETrue;
        	}
        }
    
    return EFalse;
    }

const TDesC& CTTGPSLoggerPositionRecorder::MsgNaviState()
	{
    switch(iStatus)
    	{
        case EStatusNone:
        	return *iMsgNaviStateNone;
        case EStatusWaiting:
        	return *iMsgNaviStateWaiting;
        case EStatusRecording:
        	return *iMsgNaviStateRecording;
        case EStatusPausing:
        	return *iMsgNaviStatePausing;
        case EStatusAutoPausing:
        	return *iMsgNaviStateAutoPausing;
        default:
        	return *iMsgNaviStateNone;
    	}
	}
