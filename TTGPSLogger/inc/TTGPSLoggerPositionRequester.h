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
#ifndef TTGPSLOGGERPOSITIONREQUESTER_H
#define TTGPSLOGGERPOSITIONREQUESTER_H

class CActive;
class RPositioner;
class RPositionServer;
class HPositionGenericInfo;
class MTTGPSLoggerPositionObserver;

class CTTGPSLoggerPositionRequester : public CActive
	{
public:
	static CTTGPSLoggerPositionRequester* NewL();
	static CTTGPSLoggerPositionRequester* NewLC();
    ~CTTGPSLoggerPositionRequester();

private:
    CTTGPSLoggerPositionRequester();
    void ConstructL();

private:
	// From CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

public:
    void StartL();
    void Stop();
    void AddPositionObserver(MTTGPSLoggerPositionObserver* aObserver);
    void DeletePositionObserver(MTTGPSLoggerPositionObserver* aObserver);
	void SetPositionModuleInfo();
	
private:
	RPositionServer iPositionServer;
	RPositioner iPositioner;
    HPositionGenericInfo* iPositionInfo;
    CTTGPSLoggerPositionData* iPositionData;
    TPositionModuleId iPositionModuleId;
    TPositionModuleInfo iPositionModuleInfo;
    RPointerArray<MTTGPSLoggerPositionObserver> iPositionObserver;
    HBufC* iMsgPositioningMethod1;
    HBufC* iMsgPositioningMethod2;
    HBufC* iMsgLBSError1;
    HBufC* iMsgLBSError2;
	};

#endif // TTGPSLOGGERPOSITIONREQUESTER_H
