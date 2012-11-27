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
#ifndef TTGPSLOGGERRECORDERFORMATNMEA_H
#define TTGPSLOGGERRECORDERFORMATNMEA_H

class CBase;
class CTTGPSLoggerEngine;
class CTTGPSLoggerPositionData;
class MTTGPSLoggerRecorderFormat;

class CTTGPSLoggerRecorderFormatNMEA : public CBase, public MTTGPSLoggerRecorderFormat
	{
public:
    static CTTGPSLoggerRecorderFormatNMEA* NewL(CTTGPSLoggerEngine* aEngine);
    static CTTGPSLoggerRecorderFormatNMEA* NewLC(CTTGPSLoggerEngine* aEngine);
	~CTTGPSLoggerRecorderFormatNMEA();

private:
    CTTGPSLoggerRecorderFormatNMEA(CTTGPSLoggerEngine* aEngine);
	void ConstructL();

public:
    // From MTTGPSLoggerRecorderFormat
    void SetFileExtL(TDes& aFileExt);
    void SetHeaderL(TDes8& aHeader, const TDesC& aFileName);
    void SetBodyL(TDes8& aBody, const TDesC& aFileName, const TInt aFilePoint, CTTGPSLoggerPositionData* aPositionData);
    void SetSegmentL(TDes8& aBody);
    void SetFooterL(TDes8& aFooter);
    
private:
    CTTGPSLoggerEngine* iEngine;
	};

#endif // TTGPSLOGGERRECORDERFORMATNMEA_H
