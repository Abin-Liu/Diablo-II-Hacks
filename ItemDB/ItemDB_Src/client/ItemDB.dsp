# Microsoft Developer Studio Project File - Name="ItemDB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ItemDB - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ItemDB.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ItemDB.mak" CFG="ItemDB - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ItemDB - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ItemDB_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\Includes" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ItemDB_EXPORTS" /Fr /FD /c
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
# ADD LINK32 kernel32.lib user32.lib /nologo /dll /machine:I386 /out:"../Release/ItemDB.dll"
# Begin Target

# Name "ItemDB - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Client.cpp
# End Source File
# Begin Source File

SOURCE=.\DllMain.cpp
# End Source File
# Begin Source File

SOURCE=.\Inventory.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\me.cpp
# End Source File
# Begin Source File

SOURCE=.\offsets.cpp
# End Source File
# Begin Source File

SOURCE=.\Server20.cpp
# End Source File
# Begin Source File

SOURCE=.\StorageBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\StringEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TickThread.cpp
# End Source File
# Begin Source File

SOURCE=.\TranslateItemMod.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Client.h
# End Source File
# Begin Source File

SOURCE=.\d2functions.h
# End Source File
# Begin Source File

SOURCE=.\D2Ptrs.h
# End Source File
# Begin Source File

SOURCE=.\definitions.h
# End Source File
# Begin Source File

SOURCE=.\Inventory.h
# End Source File
# Begin Source File

SOURCE=.\ItemCheck.h
# End Source File
# Begin Source File

SOURCE=.\Offsets.h
# End Source File
# Begin Source File

SOURCE=.\Server20.h
# End Source File
# Begin Source File

SOURCE=.\StorageBuffer.h
# End Source File
# End Group
# End Target
# End Project
