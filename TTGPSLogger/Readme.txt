TTGPSLogger Version 0.5.3 (2012-11-11)

New feature provided by drop 0.5.3 (2012-11-11): this version allows changing the default positioning method set in the phone configuration.

To use a positioning method different from the default one, select "Settings", check new tab "General", "Positioning Method" option.

_____________________________________________________________________________
New features and fixes provided by drop 0.5.2 (2012-02-12):
- Support of C5-03 phone with single tap UI (issue 16 fixed)
- Workaround to manage multi-selection menus
- Management of Mass memory selection, for mobile phones supporting it
- RVCT compiler

_____________________________________________________________________________
New features and fixes provided by drop 0.5.1 (2011-09-03):

- Now the Display tab allows setting TTGPSLogger as System Application; this allow to:
   o Keep the application running also in case of lack of resources
   o Avoid terminating the application when pressing the red phone key
System Application setting is suggested. After modifying this setting, restart the application to activate changes
- Now the GPX header includes as fixed XSL stylesheet (can be useful if a file named gpx.xsl is present).
- The GPX header allows adding optional Author, Email, Url and UrlName tags through appropriate GPX settings; unvalued fields do not generate the related header tags.
- Now the Display tab includes the Segments option, which allows segmenting the GPX track (trkseg chunks) by manually pausing and resuming a recording
- The GPX tags include two new options:
   o Use Sea Level Altitude, to select Sea Level Altitude for <ele> tags (when both ele and sea level are checked, in case sea level GPS data is missing, the application uses ele; if only a single checkbox is selected, the respective active feature is used).
   o Use Vertical Speed, to disable calculating the 3D absolute speed and to set <speed> tags with the horizontal speed over ground: disabling absolute speed eliminates errors related to the GPS vertical speed computation (e.g., erroneous speeds or wrong big speed numbers that might be present in GPX data). See issue 14.
- Now the application allows manually syncing the phone system time with the GPS time

Bug fixed:

- No loss of data if the application is killed (now the application always flushes the output buffer)
- Patches from andre.tippel for decimal separator (issues 2, 4 and 13), number of decimals (issue 2) and GPX field order (issue 3)
- Thousand separator removed from GPX fields
- Fixed missing GPX <speed> tags in certain cases (e.g., with language different from English)

Note: all modifications have been done in harry, with Carbide C++ 3.3 and Symbian^3 SDK. This code drop is not clean and might include errors; an experienced Symbian programmer might help in improving code, revising my modifications one by one and recompiling the code
