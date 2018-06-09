# Microsoft Developer Studio Project File - Name="ItemDB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ItemDB - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ItemDB.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ItemDB.mak" CFG="ItemDB - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ItemDB - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ItemDB - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ItemDB - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../Release/ItemDB.exe"

!ELSEIF  "$(CFG)" == "ItemDB - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../release/ItemDB.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ItemDB - Win32 Release"
# Name "ItemDB - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\External\ado.cpp
# End Source File
# Begin Source File

SOURCE=.\RegExp\BuiltinVar.cpp
# End Source File
# Begin Source File

SOURCE=.\CharList.cpp
# End Source File
# Begin Source File

SOURCE=.\CharView.cpp
# End Source File
# Begin Source File

SOURCE=.\Diablo2\D2ItemDesc.cpp
# End Source File
# Begin Source File

SOURCE=.\External\D2Monitor.cpp
# End Source File
# Begin Source File

SOURCE=.\Diablo2\D2Storage.cpp
# End Source File
# Begin Source File

SOURCE=.\Diablo2\D2View.cpp
# End Source File
# Begin Source File

SOURCE=.\External\EditEx.cpp
# End Source File
# Begin Source File

SOURCE=.\RegExp\ExamItemProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\Diablo2\HoverWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\External\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\External\Ini.cpp
# End Source File
# Begin Source File

SOURCE=.\Diablo2\ItemDatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemDB.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemDB.rc
# End Source File
# Begin Source File

SOURCE=.\ItemDBDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemDBToolbar.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemDBView.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemListView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\RegExp\ModDef.cpp
# End Source File
# Begin Source File

SOURCE=.\External\MsgWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\External\Picture.cpp
# End Source File
# Begin Source File

SOURCE=.\External\PopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\External\ProgressWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\RealmView.cpp
# End Source File
# Begin Source File

SOURCE=.\External\ReportCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\External\ResPack.cpp
# End Source File
# Begin Source File

SOURCE=.\External\Singleton.cpp
# End Source File
# Begin Source File

SOURCE=.\Diablo2\SpellDef.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\External\StringEx.cpp
# End Source File
# Begin Source File

SOURCE=.\RegExp\syntax.cpp
# End Source File
# Begin Source File

SOURCE=.\External\SystemTray.cpp
# End Source File
# Begin Source File

SOURCE=.\External\TickThread.cpp
# End Source File
# Begin Source File

SOURCE=.\External\WinAppEx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\External\ado.h
# End Source File
# Begin Source File

SOURCE=.\CharList.h
# End Source File
# Begin Source File

SOURCE=.\CharView.h
# End Source File
# Begin Source File

SOURCE=.\Diablo2\D2ItemDesc.h
# End Source File
# Begin Source File

SOURCE=.\External\D2Monitor.h
# End Source File
# Begin Source File

SOURCE=.\Diablo2\D2Storage.h
# End Source File
# Begin Source File

SOURCE=.\Diablo2\D2View.h
# End Source File
# Begin Source File

SOURCE=.\External\EditEx.h
# End Source File
# Begin Source File

SOURCE=.\Diablo2\HoverWnd.h
# End Source File
# Begin Source File

SOURCE=.\External\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\External\Ini.h
# End Source File
# Begin Source File

SOURCE=.\InternalMsgs.h
# End Source File
# Begin Source File

SOURCE=.\Diablo2\ItemDatabase.h
# End Source File
# Begin Source File

SOURCE=.\ItemDB.h
# End Source File
# Begin Source File

SOURCE=.\Diablo2\ItemDBDef.h
# End Source File
# Begin Source File

SOURCE=.\ItemDBDoc.h
# End Source File
# Begin Source File

SOURCE=.\ItemDBToolbar.h
# End Source File
# Begin Source File

SOURCE=.\ItemDBView.h
# End Source File
# Begin Source File

SOURCE=.\ItemListView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\External\memdc.h
# End Source File
# Begin Source File

SOURCE=.\External\MsgWnd.h
# End Source File
# Begin Source File

SOURCE=.\External\Picture.h
# End Source File
# Begin Source File

SOURCE=.\External\PopupMenu.h
# End Source File
# Begin Source File

SOURCE=.\External\ProgressWnd.h
# End Source File
# Begin Source File

SOURCE=.\RealmView.h
# End Source File
# Begin Source File

SOURCE=.\External\RemoteLib.h
# End Source File
# Begin Source File

SOURCE=.\External\ReportCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\External\ResPack.h
# End Source File
# Begin Source File

SOURCE=.\External\Singleton.h
# End Source File
# Begin Source File

SOURCE=.\Diablo2\SpellDef.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\External\StringEx.h
# End Source File
# Begin Source File

SOURCE=.\External\SystemTray.h
# End Source File
# Begin Source File

SOURCE=.\External\TickThread.h
# End Source File
# Begin Source File

SOURCE=.\External\WinAppEx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\blank.ico
# End Source File
# Begin Source File

SOURCE=.\res\chars.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\ItemDB.ico
# End Source File
# Begin Source File

SOURCE=.\res\ItemDB.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ItemDBDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\External\RemoteLib.lib
# End Source File
# End Target
# End Project
