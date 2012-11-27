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
#include <e32cmn.h>
#include <e32debug.h>
#include <e32std.h>
#include <aknappui.h>
#include <aknquerydialog.h>
#include <aknnotewrappers.h> 
#include <lbs.h>
#include <TTGPSLogger.rsg>
#include "TTGPSLoggerPositionData.h"
#include "TTGPSLoggerPositionObserver.h"
#include "TTGPSLoggerPositionRequester.h"

//RDebug::Print(_L("%d\n"), __LINE__);

_LIT(KRequestor, "TTGPSLogger");

CTTGPSLoggerPositionRequester::CTTGPSLoggerPositionRequester() :
	CActive(EPriorityStandard), // Standard priority
    iPositionInfo(NULL),
    iPositionData(NULL),
    iPositionModuleId(TUid::Null())
	{
	}

CTTGPSLoggerPositionRequester* CTTGPSLoggerPositionRequester::NewLC(TInt PositioningMethod)
	{
	CTTGPSLoggerPositionRequester* self =
			new (ELeave) CTTGPSLoggerPositionRequester();
	CleanupStack::PushL(self);
	self->ConstructL(PositioningMethod);
	return self;
	}

CTTGPSLoggerPositionRequester* CTTGPSLoggerPositionRequester::NewL(TInt PositioningMethod)
	{
	CTTGPSLoggerPositionRequester* self =
			CTTGPSLoggerPositionRequester::NewLC(PositioningMethod);
	CleanupStack::Pop(); // self;
	return self;
	}

void CTTGPSLoggerPositionRequester::ConstructL(TInt PositioningMethod)
	{
    iPositioningMethod=PositioningMethod;
	//RDebug::Print(_L("%d\n"), __LINE__);
	User::LeaveIfError(iPositionServer.Connect());
	//RDebug::Print(_L("%d\n"), __LINE__);
    
    CActiveScheduler::Add(this); // Add to scheduler
	
    TPositionFieldIdList list = {EPositionFieldHorizontalSpeed,
                                 EPositionFieldVerticalSpeed,
                                 EPositionFieldTrueCourse,
                                 EPositionFieldMagneticCourse,
                                 EPositionFieldHeading,
                                 EPositionFieldMagneticHeading,
                                 EPositionFieldNMEASentences,
                                 EPositionFieldSatelliteNumInView,
                                 EPositionFieldSatelliteNumUsed,
                                 EPositionFieldSatelliteTime,
                                 EPositionFieldSatelliteHorizontalDoP,
                                 EPositionFieldSatelliteVerticalDoP,
                                 EPositionFieldSatellitePositionDoP,
                                 EPositionFieldSatelliteSeaLevelAltitude,
                                 EPositionFieldSatelliteTimeDoP,
                                 EPositionFieldSatelliteGeoidalSeparation,
                                 EPositionFieldNone};
    iPositionInfo = HPositionGenericInfo::NewL(8192, 64);
    iPositionInfo->SetRequestedFields(list);
    iPositionData = CTTGPSLoggerPositionData::NewL();
    
    iMsgPositioningMethod1 = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF_MSG_POSITIONING_METHOD1);
    iMsgPositioningMethod2 = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF_MSG_POSITIONING_METHOD2);
    iMsgLBSError1 = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF_MSG_LBS_ERROR1);
    iMsgLBSError2 = CEikonEnv::Static()->AllocReadResourceL(R_TTGP_TBUF_MSG_LBS_ERROR2);
	}

CTTGPSLoggerPositionRequester::~CTTGPSLoggerPositionRequester()
	{
	Cancel();
    iPositionObserver.Close();
	iPositioner.Close();
	iPositionServer.Close();
	delete iPositionInfo;
    delete iPositionData;
    delete iMsgPositioningMethod1;
    delete iMsgPositioningMethod2;
    delete iMsgLBSError1;
    delete iMsgLBSError2;
	}

void CTTGPSLoggerPositionRequester::DoCancel()
	{
	iPositioner.CancelRequest(EPositionerNotifyPositionUpdate);
	}

void CTTGPSLoggerPositionRequester::StartL()
	{
	Stop();
	//RDebug::Print(_L("%d\n"), __LINE__);

#ifdef POSMETH
	TPositionModuleInfo info;
	if ( (iPositioningMethod>0) &&
				((iPositionServer.GetModuleInfoByIndex(iPositioningMethod-1,info) == KErrNone) && (info.IsAvailable())) )
			{
			User::LeaveIfError(iPositioner.Open(iPositionServer, info.ModuleId()));
			}
	else
#endif
		User::LeaveIfError(iPositioner.Open(iPositionServer));
	
	//RDebug::Print(_L("%d\n"), __LINE__);
	User::LeaveIfError(iPositioner.SetRequestor(CRequestor::ERequestorService, CRequestor::EFormatApplication, KRequestor));
	//RDebug::Print(_L("%d\n"), __LINE__);
	const TTimeIntervalMicroSeconds KUpdateInterval(1000000);
	const TTimeIntervalMicroSeconds KTimeOut(2000000);
	const TTimeIntervalMicroSeconds KMaxUpdateAge(0);
	TPositionUpdateOptions options;
	options.SetUpdateInterval(KUpdateInterval);
	options.SetUpdateTimeOut(KTimeOut);
	options.SetMaxUpdateAge(KMaxUpdateAge);
	options.SetAcceptPartialUpdates(ETrue);
	//RDebug::Print(_L("%d\n"), __LINE__);
	User::LeaveIfError(iPositioner.SetUpdateOptions(options));
	//RDebug::Print(_L("%d\n"), __LINE__);
	iPositioner.NotifyPositionUpdate(*iPositionInfo, iStatus);
	//RDebug::Print(_L("%d\n"), __LINE__);
	SetActive();
	}

void CTTGPSLoggerPositionRequester::Stop()
	{
	Cancel();
	iPositioner.Close();
    iPositionModuleId = TUid::Null();
	}

void CTTGPSLoggerPositionRequester::RunL()
	{
	switch (iStatus.Int())
		{
		case KErrNone:
        case KPositionPartialUpdate:
		    {
            SetPositionModuleInfo();
		    iPositionData->SetPositionInfoL(iPositionModuleInfo, iPositionInfo);
            for (int i1 = 0; i1 < iPositionObserver.Count(); i1++)
                {
                if (iStatus.Int() == KErrNone)
                    {
                    iPositionObserver[i1]->PositionUpdatedL(iPositionData);
                    }
                else
                    {
                    iPositionObserver[i1]->PositionUpdatedPartialL(iPositionData);
                    }
                }
            iPositionInfo->ClearPositionData();
            iPositioner.NotifyPositionUpdate(*iPositionInfo, iStatus);
            SetActive();
            break;
		    }
		case KPositionQualityLoss:
            {
            SetPositionModuleInfo();
            iPositionInfo->ClearPositionData();
            for (int i1 = 0; i1 < iPositionObserver.Count(); i1++)
                {
                iPositionObserver[i1]->PositionQualityLossL();
                }
            iPositioner.NotifyPositionUpdate(*iPositionInfo, iStatus);
            SetActive();
            break;
            }
		case KErrTimedOut:
            {
            SetPositionModuleInfo();
            iPositionInfo->ClearPositionData();
            for (int i1 = 0; i1 < iPositionObserver.Count(); i1++)
                {
                iPositionObserver[i1]->PositionTimedOutL();
                }
            iPositioner.NotifyPositionUpdate(*iPositionInfo, iStatus);
            SetActive();
            break;
            }
        //case KErrArgument:
            //{
            //iPositionInfoBase = &iPositionInfo[0];
            //iPositioner.NotifyPositionUpdate(*iPositionInfoBase, iStatus);
            //SetActive();
            //break;
            //}
        case KErrCancel:
            {
            break;
            }
        case KErrArgument:
        case KErrAccessDenied:
        default:
            iPositionInfo->ClearPositionData();
            for (int i1 = 0; i1 < iPositionObserver.Count(); i1++)
                {
                iPositionObserver[i1]->PositionErrorL(iStatus.Int());
                }
            CAknQueryDialog* dialog1 = CAknQueryDialog::NewL();
            if (dialog1->ExecuteLD(R_TTGP_DIALOG_QUERY_NOTITLE_YESNO, *iMsgLBSError1))
                {
                StartL();
                }
            else
                {
                CAknConfirmationNote *note = new (ELeave) CAknConfirmationNote(EFalse);
                note->ExecuteLD(*iMsgLBSError2);
                }
			break;
		}
	}

TInt CTTGPSLoggerPositionRequester::RunError(TInt aError)
	{
	iPositioner.NotifyPositionUpdate(*iPositionInfo, iStatus);
	SetActive();
	return KErrNone;
	}

void CTTGPSLoggerPositionRequester::AddPositionObserver(MTTGPSLoggerPositionObserver* aObserver)
    {
    if (iPositionObserver.Find(aObserver) == KErrNotFound)
        {
        iPositionObserver.Append(aObserver);
        }
    }
void CTTGPSLoggerPositionRequester::DeletePositionObserver(MTTGPSLoggerPositionObserver* aObserver)
    {
    TInt i1 = iPositionObserver.Find(aObserver);
    if (i1 != KErrNotFound)
        {
        iPositionObserver.Remove(i1);
        }
    }

void CTTGPSLoggerPositionRequester::SetPositionModuleInfo()
    {
	TBuf<128> moduleName;
	TBuf<128> msg;

    if (iPositionModuleId == TUid::Null() && iPositionInfo->ModuleId() != TUid::Null())
        {
        iPositionModuleId = iPositionInfo->ModuleId();
        iPositionServer.GetModuleInfoById(iPositionModuleId, iPositionModuleInfo);
        iPositionModuleInfo.GetModuleName(moduleName);
        msg.Format(*iMsgPositioningMethod1, &moduleName);
        CAknInformationNote *note = new (ELeave) CAknInformationNote(EFalse);
        note->ExecuteLD(msg);
        }
    else if (iPositionModuleId != iPositionInfo->ModuleId() && iPositionInfo->ModuleId() != TUid::Null())
        {
        iPositionModuleId = iPositionInfo->ModuleId();
        iPositionServer.GetModuleInfoById(iPositionModuleId, iPositionModuleInfo);
        iPositionModuleInfo.GetModuleName(moduleName);
        msg.Format(*iMsgPositioningMethod2, &moduleName);
        CAknInformationNote *note = new (ELeave) CAknInformationNote(EFalse);
        note->ExecuteLD(msg);
        }
    else
        {
        return;
        }
    }
