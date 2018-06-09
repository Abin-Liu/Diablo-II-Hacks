# Microsoft Developer Studio Project File - Name="D2HackIt" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=D2HackIt - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "D2HackIt.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "D2HackIt.mak" CFG="D2HackIt - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "D2HackIt - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "D2HackIt - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "D2HackIt - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "D2HACKIT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\Includes" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "D2HACKIT_EXPORTS" /Fr /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /pdb:none /machine:I386 /out:"..\Release\D2HackIt.dll"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "D2HackIt - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "D2HACKIT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /Od /I "..\Includes" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "D2HACKIT_EXPORTS" /D "D2HACKIT_DEBUG" /Fr /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /pdb:none /debug /machine:I386
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "D2HackIt - Win32 Release"
# Name "D2HackIt - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AffectionDef.cpp
# End Source File
# Begin Source File

SOURCE=.\AntiDetection.cpp
# End Source File
# Begin Source File

SOURCE=.\BackToTown.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\PathAlgorithms\CollisionMap.cpp
# End Source File
# Begin Source File

SOURCE=.\PathAlgorithms\Common.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Stdio.cpp
# End Source File
# Begin Source File

SOURCE=.\DllMain.cpp
# End Source File
# Begin Source File

SOURCE=.\EnterWayPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\EnumPresetUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\GameCommandLine.cpp
# End Source File
# Begin Source File

SOURCE=.\GameKeyDown.cpp
# End Source File
# Begin Source File

SOURCE=.\GameMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\GameOnDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\GamePacketReceived.cpp
# End Source File
# Begin Source File

SOURCE=.\GamePacketSent.cpp
# End Source File
# Begin Source File

SOURCE=.\GamePlayerInfoIntercept.cpp
# End Source File
# Begin Source File

SOURCE=.\GamePrintFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\GameSendPacketToGame.cpp
# End Source File
# Begin Source File

SOURCE=.\GameSendPacketToServer.cpp
# End Source File
# Begin Source File

SOURCE=.\HelperFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\IniFileHandlers.cpp
# End Source File
# Begin Source File

SOURCE=.\Inventory.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyDef.cpp
# End Source File
# Begin Source File

SOURCE=..\Includes\LinkedList\LinkedList.cpp
# End Source File
# Begin Source File

SOURCE=.\MapDef.cpp
# End Source File
# Begin Source File

SOURCE=.\MapDll.cpp
# End Source File
# Begin Source File

SOURCE=.\me.cpp
# End Source File
# Begin Source File

SOURCE=.\MemorySearchFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleWalker.cpp
# End Source File
# Begin Source File

SOURCE=.\MotionJob.cpp
# End Source File
# Begin Source File

SOURCE=.\NpcSession.cpp
# End Source File
# Begin Source File

SOURCE=.\offsets.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherExportedFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet.cpp
# End Source File
# Begin Source File

SOURCE=.\psapi.cpp
# End Source File
# Begin Source File

SOURCE=.\Server20.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerStartStop.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellDef.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities\StateTicker.cpp
# End Source File
# Begin Source File

SOURCE=.\PathAlgorithms\TeleportPath.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities\TickThread.cpp
# End Source File
# Begin Source File

SOURCE=.\toolhelp.cpp
# End Source File
# Begin Source File

SOURCE=.\PathAlgorithms\WalkPath.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AffectionDef.h
# End Source File
# Begin Source File

SOURCE=.\BackToTown.h
# End Source File
# Begin Source File

SOURCE=.\PathAlgorithms\CollisionMap.h
# End Source File
# Begin Source File

SOURCE=.\PathAlgorithms\Common.h
# End Source File
# Begin Source File

SOURCE=..\Includes\CommonStructs.h
# End Source File
# Begin Source File

SOURCE=..\Includes\Constants.h
# End Source File
# Begin Source File

SOURCE=.\d2functions.h
# End Source File
# Begin Source File

SOURCE=..\Includes\D2HackIt.h
# End Source File
# Begin Source File

SOURCE=.\D2HackItServer.h
# End Source File
# Begin Source File

SOURCE=.\D2Ptrs.h
# End Source File
# Begin Source File

SOURCE=.\definitions.h
# End Source File
# Begin Source File

SOURCE=.\EnterWayPoint.h
# End Source File
# Begin Source File

SOURCE=.\GameMessage.h
# End Source File
# Begin Source File

SOURCE=.\Inventory.h
# End Source File
# Begin Source File

SOURCE=.\ItemCheck.h
# End Source File
# Begin Source File

SOURCE=.\KeyDef.h
# End Source File
# Begin Source File

SOURCE=.\MapDef.h
# End Source File
# Begin Source File

SOURCE=.\MapDll.h
# End Source File
# Begin Source File

SOURCE=.\me.h
# End Source File
# Begin Source File

SOURCE=.\ModuleWalker.h
# End Source File
# Begin Source File

SOURCE=.\MotionJob.h
# End Source File
# Begin Source File

SOURCE=.\NpcSession.h
# End Source File
# Begin Source File

SOURCE=.\Offsets.h
# End Source File
# Begin Source File

SOURCE=.\Packet.h
# End Source File
# Begin Source File

SOURCE=.\Server20.h
# End Source File
# Begin Source File

SOURCE=.\ServerDraw.h
# End Source File
# Begin Source File

SOURCE=.\ServerStructs.h
# End Source File
# Begin Source File

SOURCE=.\SpellDef.h
# End Source File
# Begin Source File

SOURCE=.\Utilities\StateTicker.h
# End Source File
# Begin Source File

SOURCE=.\PathAlgorithms\TeleportPath.h
# End Source File
# Begin Source File

SOURCE=.\Utilities\TickThread.h
# End Source File
# Begin Source File

SOURCE=.\PathAlgorithms\WalkPath.h
# End Source File
# End Group
# Begin Group "Client Files (Ref)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Includes\ClientCore.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\ClientImplementation.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\Includes\D2Client.h
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Source File

SOURCE=..\D2HackIt.ini
# End Source File
# End Target
# End Project
