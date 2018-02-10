@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by FreeScan.HPJ. >"hlp\FreeScan.hm"
echo. >>"hlp\FreeScan.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\FreeScan.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\FreeScan.hm"
echo. >>"hlp\FreeScan.hm"
echo // Prompts (IDP_*) >>"hlp\FreeScan.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\FreeScan.hm"
echo. >>"hlp\FreeScan.hm"
echo // Resources (IDR_*) >>"hlp\FreeScan.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\FreeScan.hm"
echo. >>"hlp\FreeScan.hm"
echo // Dialogs (IDD_*) >>"hlp\FreeScan.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\FreeScan.hm"
echo. >>"hlp\FreeScan.hm"
echo // Frame Controls (IDW_*) >>"hlp\FreeScan.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\FreeScan.hm"
REM -- Make Help for Project FreeScan

exit

echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\FreeScan.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\FreeScan.hlp" goto :Error
if not exist "hlp\FreeScan.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\FreeScan.hlp" Debug
if exist Debug\nul copy "hlp\FreeScan.cnt" Debug
if exist Release\nul copy "hlp\FreeScan.hlp" Release
if exist Release\nul copy "hlp\FreeScan.cnt" Release
echo.
goto :done

:Error
echo hlp\FreeScan.hpj(1) : error: Problem encountered creating help file

:done
echo.
