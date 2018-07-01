"C:\Program Files\MRE SDK V3.0.00\tools\DllPackage.exe" "C:\Users\samsu\Documents\WatchDue\WatchDue.vcproj"
if %errorlevel% == 0 (
 echo postbuild OK.
  copy WatchDue.vpp ..\..\..\MoDIS_VC9\WIN32FS\DRIVE_E\WatchDue.vpp /y
exit 0
)else (
echo postbuild error
  echo error code: %errorlevel%
  exit 1
)

