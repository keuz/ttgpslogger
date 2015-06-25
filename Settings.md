### Display Tab ###
  * "Measurement system"
> Select "Measurement system" for display notation. (either "Metric" or "Imperial", default "Metric")
  * "Coordinate format"
> Select "Coordinate format" for display notation. (either "DDD°MM'SS.SS", "DDD°MM.MMMM" or "DDD.DDDDD°", default "DDD°MM'SS.SS")
  * "Items"
> Select the display items. ("Longitude", "Latitude", "Altitude", etc.)
  * "Shortcuts"
> Select whether to use Keyboard shortcuts. (default "Enabled")

### Output Tab ###
  * "Autostart"
> Select whether to begin the GPS logging automatically when this software is started.  (default "Disabled")
  * "Memory in use"
> Select the memory with which the GPS tracking file is generated. The file is generated to either of the following folders. (default C:(Built-in memory))
    * C:(Built-in memory)\Data\Others\TTGPSLogger
    * E:(Memory Card)\Data\Others\TTGPSLogger
  * "Intervals"
> Select logging intervals by time.
  * "Horizontal distance"
> Select logging intervals by horizontal distance.
  * "Auto pause(H.Speed)(Speed)(H.Acc)(HDOP)(PDOP)"
> "Auto pause" is the function that pause/resume automatically corresponding to the following GPS parameter obtained from "LBS"(Location Based Services).
    * H.Speed(Horizontal Speed)
    * Speed
    * H.Acc(Horizontal Accuracy)
    * HDOP
    * PDOP
### NMEA Tab ###
  * "File Name(Ext)"
> Input the extension name of the GPS tracking file(NMEA format).
  * "Sentences"
> Select which Sentences to output to the file among NMEA 0183 Sentences acquired from LBS(Location Based Services). ("GPGGA", "GPGLL", "GPGSA", etc.)
### GPX Tab ###
  * "File Name(Ext)"
> Input the extension name of the GPS tracking file(GPX format).
  * "Tags"
> Select child elements of the `<trkpt>` element. ("time", "ele", "course", etc.)
### KML Tab ###
  * "File Name(Ext)"
> Input the extension name of the GPS tracking file(KML format).
  * "Placemark(Name)"
> Select the element's content of the `<Name>` tag, which is child element of the`<Placemark>` tag. (either "None", "Point", "TimeStamp(UTC)", or "TimeStamp(Local)". default "Point")
  * "Placemark(Description)"
> Select the element's contents of the `<Description>` tag, which is child element of the `<Placemark>` tag.  ("Longitude", "Latitude", "Altitude", etc.)