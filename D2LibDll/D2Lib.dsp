# Microsoft Developer Studio Project File - Name="D2Lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

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
!MESSAGE "D2Lib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "D2Lib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "D2LIB_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "D2LIB_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "D2LIB_EXPORTS" /YX /FD /GZ  /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "D2LIB_EXPORTS" /YX /FD /GZ  /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "D2Lib - Win32 Release"
# Name "D2Lib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Classes\Definition\AuraDef.cpp
# End Source File
# Begin Source File

SOURCE=.\D2LibDll.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\Ini.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\KeyDef.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\MapDef.cpp
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\PlayerDef.cpp
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
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Classes\Template\ArrayEx.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Classes\Definition\AuraDef.h
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

SOURCE=.\Classes\Definition\D2LibDef.h
# End Source File
# Begin Source File

SOURCE=.\D2LibDll.h
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

SOURCE=.\Classes\D2StaticObject.h
# End Source File
# Begin Source File

SOURCE=.\Classes\D2Unit.h
# End Source File
# Begin Source File

SOURCE=.\Classes\Utils\Ini.h
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

SOURCE=.\Classes\Definition\PlayerDef.h
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
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
