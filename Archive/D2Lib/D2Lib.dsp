# Microsoft Developer Studio Project File - Name="D2Lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=D2Lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "D2Lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "D2Lib.mak" CFG="D2Lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "D2Lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "D2Lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "D2Lib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"D2Lib.lib"

!ELSEIF  "$(CFG)" == "D2Lib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "D2Lib - Win32 Release"
# Name "D2Lib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Classes\Definition\AffectionDef.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Abstract.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Merc.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Monster.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Object.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Player.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\D2PlayerStats.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\D2StaticObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\D2Stdio.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\D2TextDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Unit.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\Ini.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\item.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\KeyBind.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\KeyDef.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\MapDef.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\MsgSender.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\PlayerDef.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\properties.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\SharedMem.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\SpellDef.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\StringParse.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\Ticker.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\WaypointDef.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Classes\Definition\AffectionDef.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Template\ArrayEx.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\BitFields.h
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Hackit\CommonStructs.h
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Abstract.h
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Hackit\D2Client.h
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Hackit\D2HackIt.h
# End Source File
# Begin Source File

SOURCE=.\D2Lib.h
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Merc.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\D2MessageDef.h
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Monster.h
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Object.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\D2ObjFingerPrinter.h
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Player.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\D2PlayerStats.h
# End Source File
# Begin Source File

SOURCE=.\Classes\D2StaticObject.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\D2Stdio.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\D2TextDraw.h
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Unit.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\Ini.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\item.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\itemsize.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\KeyBind.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\KeyDef.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Template\LinkedList.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\MapDef.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\MsgSender.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\PlayerDef.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\properties.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\SharedMem.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\SpellDef.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\StringParse.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\Ticker.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\WaypointDef.h
# End Source File
# End Group
# End Target
# End Project
