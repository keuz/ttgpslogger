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
#ifndef TTGPSLOGGERRECORDEROBSERVER_H_
#define TTGPSLOGGERRECORDEROBSERVER_H_

class TPositionInfoBase;

class MTTGPSLoggerRecorderObserver
	{
public:
    virtual void RecorderUpdatedL(const TDesC& aStatus) = 0;
    virtual void RecorderUpdatedL(const TDesC& aStatus, const TDesC& aFileName, TInt aFilePoint, TInt aFileSize) = 0;
	};

#endif /* TTGPSLOGGERRECORDEROBSERVER_H_ */
