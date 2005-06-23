# Microsoft Developer Studio Project File - Name="mesgui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=mesgui - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mesgui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mesgui.mak" CFG="mesgui - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mesgui - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "mesgui - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mesgui - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "mesgui - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/mesgui.lib"

!ENDIF 

# Begin Target

# Name "mesgui - Win32 Release"
# Name "mesgui - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\font.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ggButton.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ggCheckBox.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ggControl.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ggDDMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ggFrame.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ggMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ggProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ggRadioBox.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ggSpinner.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ggTable.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ggText.cpp
# End Source File
# Begin Source File

SOURCE=..\src\mesgui.cpp
# End Source File
# Begin Source File

SOURCE=..\src\misc.cpp
# End Source File
# Begin Source File

SOURCE=..\src\texture.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\control_common.h
# End Source File
# Begin Source File

SOURCE=..\src\font.h
# End Source File
# Begin Source File

SOURCE=..\include\mesgui.h
# End Source File
# Begin Source File

SOURCE=..\src\misc.h
# End Source File
# Begin Source File

SOURCE=..\src\scancodes.h
# End Source File
# Begin Source File

SOURCE=..\src\texture.h
# End Source File
# End Group
# End Target
# End Project
