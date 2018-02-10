# Microsoft Developer Studio Project File - Name="FreeScan" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FreeScan - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FreeScan.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FreeScan.mak" CFG="FreeScan - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FreeScan - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FreeScan - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FreeScan - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "FreeScan - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /incremental:no /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FreeScan - Win32 Release"
# Name "FreeScan - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "ActiveX"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\rbgauge.cpp
# End Source File
# Begin Source File

SOURCE=.\rbgauge.h
# End Source File
# End Group
# Begin Group "SetDesiredIdle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DesIdleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DesIdleDlg.h
# End Source File
# End Group
# Begin Group "ToolTips"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\OXToolTipCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\OXToolTipCtrl.h
# End Source File
# Begin Source File

SOURCE=.\TTPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\TTPropertyPage.h
# End Source File
# End Group
# Begin Group "SerialPort"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EnumSer.cpp
# End Source File
# Begin Source File

SOURCE=.\EnumSer.h
# End Source File
# Begin Source File

SOURCE=.\SerialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialPort.h
# End Source File
# End Group
# Begin Group "Status Dlg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\StatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusDlg.h
# End Source File
# End Group
# Begin Group "Protocols"

# PROP Default_Filter ""
# Begin Group "Lotus Esprit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Protocols\EspritParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\EspritParser.h
# End Source File
# Begin Source File

SOURCE=.\Protocols\EspritProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\EspritProtocol.h
# End Source File
# Begin Source File

SOURCE=".\Protocols\Lotus Esprit ALDL.doc"
# End Source File
# End Group
# Begin Group "Lotus Elan M100"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Protocols\ElanParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\ElanParser.h
# End Source File
# Begin Source File

SOURCE=.\Protocols\ElanProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\ElanProtocol.h
# End Source File
# Begin Source File

SOURCE=".\Protocols\Lotus Elan.txt"
# End Source File
# End Group
# Begin Group "GM 1994 Camaro Z28 (A274)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Protocols\GM Camaro Z28.doc"
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1994CamaroZ28Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1994CamaroZ28Parser.h
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1994CamaroZ28Protocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1994CamaroZ28Protocol.h
# End Source File
# End Group
# Begin Group "GM 1992 Pontiac (A186)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Protocols\GM Pontiac 3_4.doc"
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1992PontiacParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1992PontiacParser.h
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1992PontiacProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1992PontiacProtocol.h
# End Source File
# End Group
# Begin Group "GM 1989 Corvette"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Protocols\GM Chevrolet 89 Corvette.doc"
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1989CorvetteParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1989CorvetteParser.h
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1989CorvetteProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1989CorvetteProtocol.h
# End Source File
# End Group
# Begin Group "GM 1993 Camaro Z28 (A172)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Protocols\GM Camaro 92-93Y 93F.txt"
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1993CamaroZ28Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1993CamaroZ28Parser.h
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1993CamaroZ28Protocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\GM1993CamaroZ28Protocol.h
# End Source File
# End Group
# Begin Group "GM A27"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Protocols\A27.txt
# End Source File
# End Group
# Begin Group "GM A34"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Protocols\A34.txt
# End Source File
# End Group
# Begin Group "GM A143"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Protocols\A143.txt
# End Source File
# Begin Source File

SOURCE=.\Protocols\GMA143Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\GMA143Parser.h
# End Source File
# Begin Source File

SOURCE=.\Protocols\GMA143Protocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\GMA143Protocol.h
# End Source File
# End Group
# Begin Group "GM A55"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Protocols\A55.txt
# End Source File
# End Group
# Begin Group "GM A140"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Protocols\A140.txt
# End Source File
# Begin Source File

SOURCE=.\Protocols\GMA140Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\GMA140Parser.h
# End Source File
# Begin Source File

SOURCE=.\Protocols\GMA140Protocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\GMA140Protocol.h
# End Source File
# End Group
# Begin Group "Lotus Carlton"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Protocols\LotusCarltonParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\LotusCarltonParser.h
# End Source File
# Begin Source File

SOURCE=.\Protocols\LotusCarltonProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\LotusCarltonProtocol.h
# End Source File
# End Group
# Begin Group "A160"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Protocols\A160.txt
# End Source File
# Begin Source File

SOURCE=.\Protocols\GMA160Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\GMA160Parser.h
# End Source File
# Begin Source File

SOURCE=.\Protocols\GMA160Protocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\GMA160Protocol.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Protocols\GMBaseFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocols\GMBaseFunctions.h
# End Source File
# Begin Source File

SOURCE=.\Supervisor.cpp
# End Source File
# Begin Source File

SOURCE=.\Supervisor.h
# End Source File
# End Group
# Begin Group "AboutBox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\About.cpp
# End Source File
# Begin Source File

SOURCE=.\About.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# End Group
# Begin Group "Led"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Led.cpp
# End Source File
# Begin Source File

SOURCE=.\Led.h
# End Source File
# End Group
# Begin Group "StringEx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MABString.cpp
# End Source File
# Begin Source File

SOURCE=.\MABString.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AdvancedDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DashBoardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DetailDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EngineDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EngineViewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FreeScan.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\FreeScan.hpj
# End Source File
# Begin Source File

SOURCE=.\FreeScan.rc
# End Source File
# Begin Source File

SOURCE=.\MainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RawMode00.cpp
# End Source File
# Begin Source File

SOURCE=.\RawMode01.cpp
# End Source File
# Begin Source File

SOURCE=.\SensorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusBitsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TCodesDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AdvancedDlg.h
# End Source File
# Begin Source File

SOURCE=.\DashBoardDlg.h
# End Source File
# Begin Source File

SOURCE=.\DetailDlg.h
# End Source File
# Begin Source File

SOURCE=.\EngineDlg.h
# End Source File
# Begin Source File

SOURCE=.\EngineViewDlg.h
# End Source File
# Begin Source File

SOURCE=.\FreeScan.h
# End Source File
# Begin Source File

SOURCE=.\MainDlg.h
# End Source File
# Begin Source File

SOURCE=.\RawMode00.h
# End Source File
# Begin Source File

SOURCE=.\RawMode01.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h

!IF  "$(CFG)" == "FreeScan - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=FreeScan
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "FreeScan - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Making help include file...
TargetName=FreeScan
InputPath=.\Resource.h

"hlp\$(TargetName).hm" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo. >"hlp\$(TargetName).hm" 
	echo // Commands (ID_* and IDM_*) >>"hlp\$(TargetName).hm" 
	makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Prompts (IDP_*) >>"hlp\$(TargetName).hm" 
	makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Resources (IDR_*) >>"hlp\$(TargetName).hm" 
	makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Dialogs (IDD_*) >>"hlp\$(TargetName).hm" 
	makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\$(TargetName).hm" 
	echo. >>"hlp\$(TargetName).hm" 
	echo // Frame Controls (IDW_*) >>"hlp\$(TargetName).hm" 
	makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\$(TargetName).hm" 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SensorDlg.h
# End Source File
# Begin Source File

SOURCE=.\StatusBitsDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TCodesDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FreeScan.ico
# End Source File
# Begin Source File

SOURCE=.\res\FreeScan.rc2
# End Source File
# Begin Source File

SOURCE=.\res\leds.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo.bmp
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter "cnt;rtf"
# Begin Source File

SOURCE=.\hlp\AfxDlg.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\FreeScan.cnt

!IF  "$(CFG)" == "FreeScan - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Release
InputPath=.\hlp\FreeScan.cnt
InputName=FreeScan

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ELSEIF  "$(CFG)" == "FreeScan - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build - Copying contents file...
OutDir=.\Debug
InputPath=.\hlp\FreeScan.cnt
InputName=FreeScan

"$(OutDir)\$(InputName).cnt" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy "hlp\$(InputName).cnt" $(OutDir)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hlp\FreeScan.hm
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section FreeScan : {32C38C41-6A6B-11D0-A74C-444553540000}
# 	2:5:Class:CRBGauge
# 	2:10:HeaderFile:rbgauge.h
# 	2:8:ImplFile:rbgauge.cpp
# End Section
# Section FreeScan : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section FreeScan : {32C38C43-6A6B-11D0-A74C-444553540000}
# 	2:21:DefaultSinkHeaderFile:rbgauge.h
# 	2:16:DefaultSinkClass:CRBGauge
# End Section
# Section FreeScan : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
