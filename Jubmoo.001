# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=jubmoo - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to jubmoo - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "jubmoo - Win32 Release" && "$(CFG)" != "jubmoo - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "jubmoo.mak" CFG="jubmoo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jubmoo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "jubmoo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "jubmoo - Win32 Debug"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "jubmoo - Win32 Release"

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
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\jubmoo.exe"

CLEAN : 
	-@erase "$(INTDIR)\Jubmoo.obj"
	-@erase "$(INTDIR)\jubmoo.res"
	-@erase "$(OUTDIR)\jubmoo.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/jubmoo.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x41e /d "NDEBUG"
# ADD RSC /l 0x41e /d "NDEBUG"
RSC_PROJ=/l 0x41e /fo"$(INTDIR)/jubmoo.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/jubmoo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/jubmoo.pdb" /machine:I386 /out:"$(OUTDIR)/jubmoo.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Jubmoo.obj" \
	"$(INTDIR)\jubmoo.res"

"$(OUTDIR)\jubmoo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "jubmoo - Win32 Debug"

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
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\jubmoo.exe"

CLEAN : 
	-@erase "$(INTDIR)\Jubmoo.obj"
	-@erase "$(INTDIR)\jubmoo.res"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\jubmoo.exe"
	-@erase "$(OUTDIR)\jubmoo.ilk"
	-@erase "$(OUTDIR)\jubmoo.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/jubmoo.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x41e /d "_DEBUG"
# ADD RSC /l 0x41e /d "_DEBUG"
RSC_PROJ=/l 0x41e /fo"$(INTDIR)/jubmoo.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/jubmoo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/jubmoo.pdb" /debug /machine:I386 /out:"$(OUTDIR)/jubmoo.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Jubmoo.obj" \
	"$(INTDIR)\jubmoo.res"

"$(OUTDIR)\jubmoo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "jubmoo - Win32 Release"
# Name "jubmoo - Win32 Debug"

!IF  "$(CFG)" == "jubmoo - Win32 Release"

!ELSEIF  "$(CFG)" == "jubmoo - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Jubmoo.c
DEP_CPP_JUBMO=\
	".\Jmalgr01.c"\
	".\Jmalgr02.c"\
	".\Jmalgr04.c"\
	".\resource.h"\
	

"$(INTDIR)\Jubmoo.obj" : $(SOURCE) $(DEP_CPP_JUBMO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\res\jubmoo.rc
DEP_RSC_JUBMOO=\
	".\res\anistat.bmp"\
	".\res\bmp1.bmp"\
	".\res\bmp10.bmp"\
	".\res\bmp11.bmp"\
	".\res\bmp1101.bmp"\
	".\res\bmp12.bmp"\
	".\res\bmp13.bmp"\
	".\res\bmp14.bmp"\
	".\res\bmp15.bmp"\
	".\res\bmp16.bmp"\
	".\res\bmp17.bmp"\
	".\res\bmp18.bmp"\
	".\res\bmp19.bmp"\
	".\res\bmp2.bmp"\
	".\res\bmp20.bmp"\
	".\res\bmp21.bmp"\
	".\res\bmp22.bmp"\
	".\res\bmp23.bmp"\
	".\res\bmp24.bmp"\
	".\res\bmp25.bmp"\
	".\res\bmp26.bmp"\
	".\res\bmp27.bmp"\
	".\res\bmp28.bmp"\
	".\res\bmp29.bmp"\
	".\res\bmp3.bmp"\
	".\res\bmp30.bmp"\
	".\res\bmp31.bmp"\
	".\res\bmp32.bmp"\
	".\res\bmp33.bmp"\
	".\res\bmp34.bmp"\
	".\res\bmp35.bmp"\
	".\res\bmp36.bmp"\
	".\res\bmp37.bmp"\
	".\res\bmp38.bmp"\
	".\res\bmp39.bmp"\
	".\res\bmp4.bmp"\
	".\res\bmp40.bmp"\
	".\res\bmp41.bmp"\
	".\res\bmp42.bmp"\
	".\res\bmp43.bmp"\
	".\res\bmp44.bmp"\
	".\res\bmp45.bmp"\
	".\res\bmp46.bmp"\
	".\res\bmp47.bmp"\
	".\res\bmp48.bmp"\
	".\res\bmp49.bmp"\
	".\res\bmp5.bmp"\
	".\res\bmp50.bmp"\
	".\res\bmp51.bmp"\
	".\res\bmp52.bmp"\
	".\res\bmp53.bmp"\
	".\res\bmp54.bmp"\
	".\res\bmp55.bmp"\
	".\res\bmp56.bmp"\
	".\res\bmp6.bmp"\
	".\res\bmp7.bmp"\
	".\res\bmp8.bmp"\
	".\res\bmp9.bmp"\
	".\res\ctable.bmp"\
	".\res\cur53.cur"\
	".\res\cur54.cur"\
	".\res\cur55.cur"\
	".\res\cur56.cur"\
	".\res\cur63.cur"\
	".\res\cur64.cur"\
	".\res\cur65.cur"\
	".\res\cur66.cur"\
	".\res\frmuser.bmp"\
	".\res\jubmoo.ico"\
	".\res\lfdw.bmp"\
	".\res\lfup.bmp"\
	".\res\mlfdw.bmp"\
	".\res\mlfup.bmp"\
	".\res\mrgdw.bmp"\
	".\res\mrgup.bmp"\
	".\res\rgdw.bmp"\
	".\res\rgup.bmp"\
	

!IF  "$(CFG)" == "jubmoo - Win32 Release"


"$(INTDIR)\jubmoo.res" : $(SOURCE) $(DEP_RSC_JUBMOO) "$(INTDIR)"
   $(RSC) /l 0x41e /fo"$(INTDIR)/jubmoo.res" /i "res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "jubmoo - Win32 Debug"


"$(INTDIR)\jubmoo.res" : $(SOURCE) $(DEP_RSC_JUBMOO) "$(INTDIR)"
   $(RSC) /l 0x41e /fo"$(INTDIR)/jubmoo.res" /i "res" /d "_DEBUG" $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
