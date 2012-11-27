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
#include <e32math.h>
#include <e32std.h>
#include <coemain.h>
#include <lbs.h>
#include "TTGPSLoggerCommon.h"
#include "TTGPSLoggerEngine.h"
#include "TTGPSLoggerPositionData.h"
#include "TTGPSLoggerSettingsData.h"
#include "TTGPSLoggerRecorderFormat.h"
#include "TTGPSLoggerRecorderFormatNMEA.h"

CTTGPSLoggerRecorderFormatNMEA::CTTGPSLoggerRecorderFormatNMEA(CTTGPSLoggerEngine* aEngine) :
    iEngine(aEngine)
    {
    }

CTTGPSLoggerRecorderFormatNMEA::~CTTGPSLoggerRecorderFormatNMEA()
    {
    }

CTTGPSLoggerRecorderFormatNMEA* CTTGPSLoggerRecorderFormatNMEA::NewLC(CTTGPSLoggerEngine* aEngine)
    {
    CTTGPSLoggerRecorderFormatNMEA* self =
            new (ELeave) CTTGPSLoggerRecorderFormatNMEA(aEngine);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CTTGPSLoggerRecorderFormatNMEA* CTTGPSLoggerRecorderFormatNMEA::NewL(CTTGPSLoggerEngine* aEngine)
    {
    CTTGPSLoggerRecorderFormatNMEA* self =
    CTTGPSLoggerRecorderFormatNMEA::NewLC(aEngine);
    CleanupStack::Pop(); // self;
    return self;
    }

void CTTGPSLoggerRecorderFormatNMEA::ConstructL()
    {
    }

void CTTGPSLoggerRecorderFormatNMEA::SetFileExtL(TDes& aFileExt)
    {
    aFileExt.Copy(iEngine->Settings()->NMEAExt());
    }

void CTTGPSLoggerRecorderFormatNMEA::SetHeaderL(TDes8& aHeader, const TDesC& aFileName)
    {
    aHeader.Zero();
    }

void CTTGPSLoggerRecorderFormatNMEA::SetBodyL(TDes8& aBody, const TDesC& aFileName, const TInt aFilePoint, CTTGPSLoggerPositionData* aPositionData)
    {
    const TInt KNMEAHeaderLength = 6;
    _LIT8(KNMEAHeaderGPGGA, "$GPGGA");
    _LIT8(KNMEAHeaderGPGLL, "$GPGLL");
    _LIT8(KNMEAHeaderGPGSA, "$GPGSA");
    _LIT8(KNMEAHeaderGPGST, "$GPGST");
    _LIT8(KNMEAHeaderGPGSV, "$GPGSV");
    _LIT8(KNMEAHeaderGPRMC, "$GPRMC");
    _LIT8(KNMEAHeaderGPVTG, "$GPVTG");
    _LIT8(KNMEAHeaderGPZDA, "$GPZDA");
    
    aBody.Zero();
    
    TInt retPos(0);
    TPtrC8 ptr(aPositionData->GetNMEASentences());
    while (ETrue)
    	{
        retPos = ptr.Find(_L8("\n"));
        if (retPos == KErrNotFound)
        	{
        	break;
        	}
        if (retPos >= KNMEAHeaderLength)
        	{
            TPtrC8 header = ptr.Left(KNMEAHeaderLength);
            if (header.Compare(KNMEAHeaderGPGGA) == 0)
            	{
            	if (iEngine->Settings()->NMEASentences(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsNMEASentencesIndexGPGGA))
            		{
            	    aBody.Append(ptr.Left(retPos + 1));
            		}
            	}
            else if (header.Compare(KNMEAHeaderGPGLL) == 0)
            	{
            	if (iEngine->Settings()->NMEASentences(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsNMEASentencesIndexGPGLL))
            		{
            	    aBody.Append(ptr.Left(retPos + 1));
            		}
            	}
            else if (header.Compare(KNMEAHeaderGPGSA) == 0)
            	{
            	if (iEngine->Settings()->NMEASentences(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsNMEASentencesIndexGPGSA))
            		{
            	    aBody.Append(ptr.Left(retPos + 1));
            		}
            	}
            else if (header.Compare(KNMEAHeaderGPGST) == 0)
            	{
            	if (iEngine->Settings()->NMEASentences(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsNMEASentencesIndexGPGST))
            		{
            	    aBody.Append(ptr.Left(retPos + 1));
            		}
            	}
            else if (header.Compare(KNMEAHeaderGPGSV) == 0)
            	{
            	if (iEngine->Settings()->NMEASentences(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsNMEASentencesIndexGPGSV))
            		{
            	    aBody.Append(ptr.Left(retPos + 1));
            		}
            	}
            else if (header.Compare(KNMEAHeaderGPRMC) == 0)
            	{
            	if (iEngine->Settings()->NMEASentences(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsNMEASentencesIndexGPRMC))
            		{
            	    aBody.Append(ptr.Left(retPos + 1));
            		}
            	}
            else if (header.Compare(KNMEAHeaderGPVTG) == 0)
            	{
            	if (iEngine->Settings()->NMEASentences(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsNMEASentencesIndexGPVTG))
            		{
            	    aBody.Append(ptr.Left(retPos + 1));
            		}
            	}
            else if (header.Compare(KNMEAHeaderGPZDA) == 0)
            	{
            	if (iEngine->Settings()->NMEASentences(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsNMEASentencesIndexGPZDA))
            		{
            	    aBody.Append(ptr.Left(retPos + 1));
            		}
            	}
            else
            	{
            	if (iEngine->Settings()->NMEASentences(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsNMEASentencesIndexOthers))
            		{
            	    aBody.Append(ptr.Left(retPos + 1));
            		}
            	}
        	}
        else
        	{
        	if (iEngine->Settings()->NMEASentences(CTTGPSLoggerSettingsData::ETTGPSLoggerSettingsNMEASentencesIndexOthers))
        		{
        	    aBody.Append(ptr.Left(retPos + 1));
        		}
        	}
        
    	if ((retPos + 1) >= ptr.Length())
    		{
    		break;
    		}
    	ptr.Set(ptr.Mid(retPos + 1));
    	
    	}
    }

void CTTGPSLoggerRecorderFormatNMEA::SetSegmentL(TDes8& aBody)
    {
    }

void CTTGPSLoggerRecorderFormatNMEA::SetFooterL(TDes8& aFooter)
    {
    aFooter.Zero();
    }
