# Microsoft Developer Studio Project File - Name="jubmoo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=jubmoo - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Jubmoo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Jubmoo.mak" CFG="jubmoo - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jubmoo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "jubmoo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "jubmoo - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41e /d "NDEBUG"
# ADD RSC /l 0x41e /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "jubmoo - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41e /d "_DEBUG"
# ADD RSC /l 0x41e /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386

!ENDIF 

# Begin Target

# Name "jubmoo - Win32 Release"
# Name "jubmoo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Jubmoo.c
# End Source File
# Begin Source File

SOURCE=.\res\jubmoo.rc
# ADD BASE RSC /l 0x409 /i "res"
# ADD RSC /l 0x409 /i ".\res" /i "res"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\anistat.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp11.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp1101.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp12.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp13.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp14.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp15.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp17.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp18.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp19.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp20.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp21.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp22.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp23.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp25.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp26.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp27.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp28.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp29.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp30.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp31.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp32.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp33.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp34.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp35.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp36.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp37.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp38.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp39.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp40.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp41.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp42.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp43.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp44.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp45.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp46.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp47.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp48.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp49.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp50.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp51.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp52.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp53.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp54.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp55.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp56.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ctable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur53.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur54.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur55.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur56.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur63.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur64.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur65.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur66.cur
# End Source File
# Begin Source File

SOURCE=.\res\frmuser.bmp
# End Source File
# Begin Source File

SOURCE=.\res\jubmoo.ico
# End Source File
# Begin Source File

SOURCE=.\res\lfdw.bmp
# End Source File
# Begin Source File

SOURCE=.\res\lfup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mlfdw.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mlfup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mrgdw.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mrgup.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rgdw.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rgup.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Jmalgr01.c
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Jmalgr02.c
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Jmalgr04.c
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# End Target
# End Project
