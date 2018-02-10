/////////////////////////////////////////////////////////////////////////////

FreeScan was written by Andy Whittaker - mail@andywhittaker.com

Macclesfield, United Kingdom.

See https://www.andywhittaker.com/ecu/ for more details.

You need to register the RBGauge ActiveX control to enable the dashboard to
function. This can be done with RegSrv32.exe. Please note that I have tried
to locate the owner of RBGauge but cannot.

If you make any modifications with or have any suggestions about this code
then please let me know about them.

/////////////////////////////////////////////////////////////////////////////

The code is supplied without warrantee nor guarantee.

You may not sell this code in any form whatsoever. If you wish to sell it, you
must seek, by all means possible, Andy Whittaker's permission to do this. A
royalty may be asked for by Andy Whittaker (probably a large, brown envelope
stuffed full of money, US Dollars, Euros or Pounds Sterling).

/////////////////////////////////////////////////////////////////////////////
This file contains a summary of what you will find in each of the files that
make up the FreeScan application.

FreeScan.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

FreeScan.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CEspritApp application class.

FreeScan.cpp
    This is the main application source file that contains the application
    class CEspritApp.

FreeScan.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

FreeScan.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

res\FreeScan.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file FreeScan.rc.

res\FreeScan.rc2
    This file contains resources that are not edited by Microsoft 
	Visual C++.  You should place all resources not editable by
	the resource editor in this file.

Protocols Folder

Supervisor.cpp
	Handles the interface between the main application and the currently
	selected protocol. It does this by sending messages to the hidden
	protocol's window.

GMBaseFunctions.cpp
	The base class for the espritparser, contains standard functions for the
	GM ECUs.

EspritParser.cpp
	The base class for the espritprotocol, parses the incoming data-stream from
	the ECU to meaningful values. This is where all of the parsed data values
	are stored in the program. When new variables are deciphered from the ECU
	data-stream, they are added within this file.

EspritProtocol.cpp
	Derived from the parser class. Handles all messages from the serial port.
	Synchronises and splits the incoming data-stream into different messages
	for eventual processing by the parser. Also contains the command strings
	to send to the ECU.

ActiveX Folder
	DCOM interface files for RBGauge.

SetDesiredIdle Folder
	Dialog files for the set desired idle speed function in the Advanced.cpp
	file.

ToolTips Folder
	Source code for the ToolTip Class, Dundas Software: http://www.dundas.com/
	These people kindly donated this class for public use.

SerialPort Folder
	SerialPort.cpp
	Contains the multi-threaded code for communicating with the serial port.
	Thanks to Remon Spekreijse, et al for pointers on this. This code was
	found at http://www.codeguru.com/ in the networks section.

	EnumSer.cpp
	Taken from PJNaughter's web site. This code checks to see what serial
	ports are installed in the PC.

Status Dlg Folder
	This is the class for displaying a non-model debug window that can run
	next to your application's window. Provides a convenient peep inside
	the progam.

/////////////////////////////////////////////////////////////////////////////

MainDlg.h, MainDlg.cpp - the main PropertySheet dialog
    These files contain the CEspritDlg class.  This class defines the behaviour
	of FreeScan's main PropertySheet dialog.  The PropertyPages	are added and
	called from here and their dialog templates are in FreeScan.rc, which can
	be edited in Microsoft Visual C++.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named FreeScan.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

If this application uses MFC in a shared DLL, and this application is 
in a language other than the operating system's current language, you
will need to copy the corresponding localized resources MFC42XXX.DLL
from the Microsoft Visual C++ CD-ROM onto the system or system32 directory,
and rename it to be MFCLOC.DLL.  ("XXX" stands for the language abbreviation.
For example, MFC42DEU.DLL contains resources translated to German.)  If you
don't do this, some of the UI elements of this application will remain in the
language of the operating system.

/////////////////////////////////////////////////////////////////////////////
