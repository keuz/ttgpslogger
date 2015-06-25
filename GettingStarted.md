### System Requirement ###

It is assumed to this software to operate with the following mobile device.
  * Symbian S60 3rd Edition FP1(Nokia N82,N95,E71,E90 etc.)
  * Symbian S60 3rd Edition FP2(Nokia N78,N79,N86,E52,E75,Samsung i8510, etc.)
  * Symbian S60 5th Edition(Nokia 5800XpressMusic,N97,Samsung i8910, etc.)

It confirms the operation with the following mobile devices like this software now.
  * Nokia N82(Symbian S60 3rd Edition FP1) and Built-in GPS
  * Nokia N82(Symbian S60 3rd Edition FP1) and Bluetooth GPS(Nokia LD-3W)
  * Nokia N79(Symbian S60 3rd Edition FP2) and Built-in GPS
  * Nokia N79(Symbian S60 3rd Edition FP2) and Bluetooth GPS(Nokia LD-3W)

(In Symbian S60 5th Edition, it confirms the operation only on the emulator.)

### How to install ###

  1. Please download sis file of the latest version from [Downloads](http://code.google.com/p/ttgpslogger/downloads/list).
  1. Please sign sis file with [Open Signed Online](https://www.symbiansigned.com/app/page/public/openSignedOnline.do).
  1. Please uninstall it absolutely if the previous version is installed in a mobile device.
  1. Please install signed sis file in a mobile device.

### How to use ###

  1. Before executing this software, please execute "Settings". In the setting of "Positioning Method", please select only either "Bluetooth GPS", "Assisted GPS" or "Integrated GPS". <br>The tracking log cannot be output in "Network Based".<p><img src='http://ttgpslogger.googlecode.com/svn/images/Scr000004.jpg' /></p>
<ol><li>When it starts first time of this software, the pairing process might have to be done if "Bluetooth GPS" is selected as "Positioning Method". <p><img src='http://ttgpslogger.googlecode.com/svn/images/Scr000007.jpg' /> <img src='http://ttgpslogger.googlecode.com/svn/images/Scr000008.jpg' /> <img src='http://ttgpslogger.googlecode.com/svn/images/Scr000009.jpg' /> <img src='http://ttgpslogger.googlecode.com/svn/images/Scr000010.jpg' /> </p>
</li><li>By "Start" or "Start Now" commands, the GPS logging can be begun. Either <a href='http://en.wikipedia.org/wiki/NMEA_0183'>NMEA 0183</a>, <a href='http://en.wikipedia.org/wiki/GPS_eXchange_Format'>GPX 1.0</a> or <a href='http://en.wikipedia.org/wiki/Keyhole_Markup_Language'>KML 2.2</a> can be selected as a logging format.<br>In "Start" command, when GPS timestamp(not Phone timestamp) passes the input "Start Date/Time", the GPS logging is begun.  <p><img src='http://ttgpslogger.googlecode.com/svn/images/Scr000013.jpg' /> <img src='http://ttgpslogger.googlecode.com/svn/images/Scr000014.jpg' /> <img src='http://ttgpslogger.googlecode.com/svn/images/Scr000015.jpg' /> <img src='http://ttgpslogger.googlecode.com/svn/images/Scr000016.jpg' /></p>
</li><li>"Pause" command can pause the GPS logging, and "Resume" command can resume it.<p><img src='http://ttgpslogger.googlecode.com/svn/images/Scr000017.jpg' /> <img src='http://ttgpslogger.googlecode.com/svn/images/Scr000018.jpg' /> <img src='http://ttgpslogger.googlecode.com/svn/images/Scr000019.jpg' /> <img src='http://ttgpslogger.googlecode.com/svn/images/Scr000020.jpg' /></p>
</li><li>"Stop" command can stop the GPS logging.<p><img src='http://ttgpslogger.googlecode.com/svn/images/Scr000021.jpg' /> <img src='http://ttgpslogger.googlecode.com/svn/images/Scr000022.jpg' /></p>The GPS tracking log file is generated to either of the following folders. By "Settings" command, it is possible to switch the folder.<br>
<ul><li>C:(Built-in memory)\Data\Others\TTGPSLogger<br>
</li><li>E:(Memory Card)\Data\Others\TTGPSLogger