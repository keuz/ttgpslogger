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
#ifndef TTGPSLOGGERENGINE_H
#define TTGPSLOGGERENGINE_H

#include <coedef.h>

class CAknToolbar;
class CBase;
class CEikMenuPane;
class CTTGPSLoggerAppUi;
class CTTGPSLoggerSettingsData;
class CTTGPSLoggerPositionRecorder;
class CTTGPSLoggerPositionRequester;
class CTTGPSLoggerRecorderFormatNMEA;
class CTTGPSLoggerRecorderFormatGPX;
class CTTGPSLoggerRecorderFormatKML;
class MTTGPSLoggerPositionObserver;
class MTTGPSLoggerRecorderObserver;

class CTTGPSLoggerEngine : public CBase
	{
public:
	static CTTGPSLoggerEngine* NewL(CTTGPSLoggerAppUi* aAppUi);
	static CTTGPSLoggerEngine* NewLC(CTTGPSLoggerAppUi* aAppUi);
    ~CTTGPSLoggerEngine();

private:
	CTTGPSLoggerEngine(CTTGPSLoggerAppUi* aAppUi);
	void ConstructL();

public:
    inline CTTGPSLoggerAppUi* AppUi() const
        {
        return iAppUi;
        };
    inline CTTGPSLoggerSettingsData* Settings() const
        {
        return iSettingsData;
        };
	void AddPositionObserver(MTTGPSLoggerPositionObserver* aObserver);
    void DeletePositionObserver(MTTGPSLoggerPositionObserver* aObserver);
    void AddRecorderObserver(MTTGPSLoggerRecorderObserver* aObserver);
    void DeleteRecorderObserver(MTTGPSLoggerRecorderObserver* aObserver);
    void MenuHide();
    void MenuStartNmeaL(TTime aTime);
    void MenuStartGpxL(TTime aTime);
	void MenuStartKmlL(TTime aTime);
    void MenuStartNowL();
    void MenuStartNowNmeaL();
    void MenuStartNowGpxL();
    void MenuStartNowKmlL();
    void MenuStartAfterL();
    void MenuStartAfterNmeaL();
    void MenuStartAfterGpxL();
    void MenuStartAfterKmlL();
    void MenuRecordL();
    void MenuPauseL();
    void MenuResumeL();
    void MenuStopL();
    void MenuSatelliteL();
    void MenuGPSStatusL();
    void MenuNMEAViewerL();
    void MenuSettingsL();
    void MenuAboutL();
    void MenuShortcutsL();
    void MenuLockL();
    TKeyResponse HandleShortcutKeyL(TUint aCode);
    void SetSoftKeyL();
    void SetToolbarL();
    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);
#ifdef __S60_50__
    void DynInitToolbarL(TInt aResourceId, CAknToolbar* aToolbar);
#endif // __S60_50__
    TTime InputStartTime();
	
private:
    CTTGPSLoggerAppUi* iAppUi;
    CTTGPSLoggerSettingsData *iSettingsData;
	CTTGPSLoggerPositionRequester* iPositionRequester;
    CTTGPSLoggerPositionRecorder *iPositionRecorder;
    CTTGPSLoggerRecorderFormatNMEA *iRecorderFormatNMEA;
    CTTGPSLoggerRecorderFormatGPX *iRecorderFormatGPX;
    CTTGPSLoggerRecorderFormatKML *iRecorderFormatKML;
    TBuf<64> iMsgStartError1;
	};

#endif // TTGPSLOGGERENGINE_H
