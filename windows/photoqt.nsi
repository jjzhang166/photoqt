; NSIS Modern User Interface
; PhotoQT Setup

;--------------------------------
; INCLUDES

	; Include Modern UI
	!include MUI2.nsh
  
	; Include stuff for nsdialog
	!include LogicLib.nsh
	!include nsDialogs.nsh
	
	; Register app for filetypes
	!include "FileAssociation.nsh"
	
	; For 32/64-Bit detection
	!include x64.nsh

;--------------------------------
; GENERAL

	; Name and file
	Name "PhotoQt"
	OutFile "photoqt-1.1.exe"

	; Default installation folder
	InstallDir "$PROGRAMFILES\PhotoQt"
	
	; Get installation folder from registry if available
	InstallDirRegKey HKCU "Software\PhotoQt" ""

	; Request application privileges for Windows Vista
	RequestExecutionLevel admin

;--------------------------------
; INTERFACE SETTINGS

	!define MUI_ABORTWARNING

;--------------------------------
; PAGES


	; Welcome text
	!define MUI_WELCOMEPAGE_TITLE "Welcome to PhotoQt Setup"
	!define MUI_WELCOMEPAGE_TEXT "This installer will guide you through the installation of the PhotoQt.$\r$\n$\r$\nPhotoQt is a simple image viewer, designed to be good looking, highly configurable, yet easy to use and fast.$\r$\n$\r$\n$\r$\nPLEASE NOTE, that the windows version has only been confirmed to be running. No features have been tested!"

	; Installer pages
	!insertmacro MUI_PAGE_WELCOME
	!insertmacro MUI_PAGE_LICENSE "app32\license.txt"
	!insertmacro MUI_PAGE_DIRECTORY
	!insertmacro MUI_PAGE_INSTFILES
	Page custom FinalStepsInit FinalStepsLeave
	!insertmacro MUI_PAGE_FINISH
	
	; UNinstaller pages
	!insertmacro MUI_UNPAGE_CONFIRM
	!insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
; LANGUAGES
 
	!insertmacro MUI_LANGUAGE "English"

;--------------------------------
; INSTALLER SECTIONS

Section "PhotoQt" SecDummy
	
	; Install files

	SetOutPath "$INSTDIR"
	${If} ${RunningX64}
		
		MessageBox MB_OK "64BIT"

		File "app64\libbz2-1.dll"
		File "app64\libEGL.dll"
		File "app64\libexiv2.dll"
		File "app64\libexpat-1.dll"
		File "app64\libfreetype-6.dll"
		File "app64\libgcc_s_seh-1.dll"
		File "app64\libgif-7.dll"
		File "app64\libGLESv2.dll"
		File "app64\libgomp-1.dll"
		File "app64\libGraphicsMagick-3.dll"
		File "app64\libGraphicsMagick++-3.dll"
		File "app64\libiconv-2.dll"
		File "app64\libjpeg-8.dll"
		File "app64\libpcre16-0.dll"
		File "app64\libpng16-16.dll"
		File "app64\libsqlite3-0.dll"
		File "app64\libstdc++-6.dll"
		File "app64\libtiff-5.dll"
		File "app64\libwebp-5.dll"
		File "app64\libwinpthread-1.dll"
		File "app64\libxml2-2.dll"
		File "app64\Qt5Core.dll"
		File "app64\Qt5Gui.dll"
		File "app64\Qt5Multimedia.dll"
		File "app64\Qt5Network.dll"
		;File "app64\Qt5Svg.dll"
		File "app64\Qt5Sql.dll"
		File "app64\Qt5Widgets.dll"
		File "app64\zlib1.dll"
	${Else}
		MessageBox MB_OK "32BIT"
		File "app32\libbz2-1.dll"
		File "app32\libEGL.dll"
		File "app32\libexiv2.dll"
		File "app32\libexpat-1.dll"
		File "app32\libfreetype-6.dll"
		File "app32\libgcc_s_sjlj-1.dll"
		File "app32\libgif-7.dll"
		File "app32\libGLESv2.dll"
		File "app32\libgomp-1.dll"
		File "app32\libGraphicsMagick-3.dll"
		File "app32\libGraphicsMagick++-3.dll"
		File "app32\libiconv-2.dll"
		File "app32\libjpeg-8.dll"
		File "app32\libpcre16-0.dll"
		File "app32\libpng16-16.dll"
		File "app32\libsqlite3-0.dll"
		File "app32\libstdc++-6.dll"
		File "app32\libtiff-5.dll"
		File "app32\libwebp-5.dll"
		File "app32\libwinpthread-1.dll"
		File "app32\libxml2-2.dll"
		File "app32\Qt5Core.dll"
		File "app32\Qt5Gui.dll"
		File "app32\Qt5Multimedia.dll"
		File "app32\Qt5Network.dll"
		;File "app32\Qt5Svg.dll"
		File "app32\Qt5Sql.dll"
		File "app32\Qt5Widgets.dll"
		File "app32\zlib1.dll"
	${EndIf}
	
	File "app32\license.txt"
	File "app32\photoqt.exe"
	File "app32\logo.ico"

	SetOutPath "$INSTDIR\sqldrivers"
	${If} ${RunningX64}
		File "app64\sqldrivers\qsqlite.dll"
	${Else}
		File "app32\sqldrivers\qsqlite.dll"
	${EndIf}
	
	SetOutPath "$INSTDIR\platforms"
	${If} ${RunningX64}
		File "app64\platforms\qwindows.dll"
	${Else}
		File "app32\platforms\qwindows.dll"
	${EndIf}
	
	SetOutPath "$INSTDIR\imageformats"
	${If} ${RunningX64}
		File "app64\imageformats\qdds.dll"
		File "app64\imageformats\qgif.dll"
		File "app64\imageformats\qicns.dll"
		File "app64\imageformats\qico.dll"
		File "app64\imageformats\qjp2.dll"
		File "app64\imageformats\qjpeg.dll"
		File "app64\imageformats\qmng.dll"
		File "app64\imageformats\qtga.dll"
		File "app64\imageformats\qtiff.dll"
		File "app64\imageformats\qwbmp.dll"
		File "app64\imageformats\qwebp.dll"
	${Else}
		File "app32\imageformats\qdds.dll"
		File "app32\imageformats\qgif.dll"
		File "app32\imageformats\qicns.dll"
		File "app32\imageformats\qico.dll"
		File "app32\imageformats\qjp2.dll"
		File "app32\imageformats\qjpeg.dll"
		File "app32\imageformats\qmng.dll"
		File "app32\imageformats\qtga.dll"
		File "app32\imageformats\qtiff.dll"
		File "app32\imageformats\qwbmp.dll"
		File "app32\imageformats\qwebp.dll"
	${EndIf}	

	; Store installation folder
	WriteRegStr HKCU "Software\PhotoQt" "" $INSTDIR
	
	; Create uninstaller
	WriteUninstaller "$INSTDIR\uninstall.exe"
  
SectionEnd
  

;--------------------------------
; INSTALLER FUNTIONS


; Custom page (nsdialog)
Var Dialog

; Description text
Var LabelFiletypeDesc

; Variables for checkboxes and their states
Var CheckboxBasic
Var CheckboxBasic_State
Var CheckboxAdvanced
Var CheckboxAdvanced_State
Var CheckboxStartMenu
Var CheckboxStartMenu_State
Var CheckboxDesktop
Var CheckboxDesktop_State

Function FinalStepsInit
  
	; Set header and subtitle
	!insertmacro MUI_HEADER_TEXT "Finishing up" "Just a few final steps"
  
	; Create dialog
	nsDialogs::Create 1018
	Pop $Dialog
	${If} $Dialog == error
		Abort
	${EndIf}
	
	; Create description label
	${NSD_CreateLabel} 0 0 100% 24u "We're almost done! If you don't want PhotoQt to be set as default application for image formats, uncheck the two boxes below:"
	Pop $LabelFiletypeDesc
	
	
	; Create all the checkboxes
	
	${NSD_CreateCheckbox} 0 25u 100% 12u "Register for the most common image formats"
	Pop $CheckboxBasic
	${NSD_Check} $CheckboxBasic
	
	${NSD_CreateCheckbox} 0 38u 100% 12u "Register for all supported image formats (including slightly more exotic ones)"
	Pop $CheckboxAdvanced
	${NSD_Check} $CheckboxAdvanced
	
	${NSD_CreateHLine} 0 60u 100% 1u HLineBeforeDesktop
	
	${NSD_CreateCheckbox} 0 70u 100% 12u "Create Desktop Icon"
	Pop $CheckboxDesktop
	${NSD_Check} $CheckboxDesktop
	
	${NSD_CreateCheckbox} 0 83u 100% 12u "Create Start menu entry"
	Pop $CheckboxStartMenu
	${NSD_Check} $CheckboxStartMenu
	
	
	; Finally, show dialog
	nsDialogs::Show
  
FunctionEnd

Function FinalStepsLeave

	; Get checkbox states
	${NSD_GetState} $CheckboxBasic $CheckboxBasic_State
	${NSD_GetState} $CheckboxAdvanced $CheckboxAdvanced_State
	${NSD_GetState} $CheckboxDesktop $CheckboxDesktop_State
	${NSD_GetState} $CheckboxStartMenu $CheckboxStartMenu_State
	
	; Register basic file types
	${If} $CheckboxBasic_State == ${BST_CHECKED}
	${OrIf} $CheckboxAdvanced_State == ${BST_CHECKED}
	
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".bmp" "BMP_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".bitmap" "BMP_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".gif" "JPEG2000_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".tif" "JPEG2000_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".tiff" "JPEG2000_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".jpeg2000" "JPEG2000_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".jp2" "JPEG2000_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".jpc" "JPEG2000_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".j2k" "JPEG2000_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".jpf" "JPEG2000_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".jpx" "JPEG2000_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".jpm" "JPEG2000_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".mj2" "JPEG2000_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".ico" "ICON_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".picon" "PICON_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".jpeg" "JPEG_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".jpg" "JPEG_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".png" "PNG_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".pbm" "PBM_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".pgm" "PGM_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".ppm" "PPM_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".svg" "SVG_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".xbm" "XBM_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".xpm" "XPM_FILE"
		
	${EndIf}
	
	; Register advanced file types
	${If} $CheckboxAdvanced_State == ${BST_CHECKED}

		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".avs" "AVS_X_IMAGE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".x" "AVS_X_IMAGE"
		
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".cals" "CALS_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".cal" "CALS_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".dcl" "CALS_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".ras" "CALS_FILE"
		
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".cin" "CIN_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".cut" "CUT_FILE"
		
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".acr" "DICOM_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".dcm" "DICOM_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".dicom" "DICOM_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".dic" "DICOM_FILE"
		
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".dcx" "DCX_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".dib" "DIB_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".dpx" "DPX_FILE"
		
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".fax" "FAX_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".fits" "FITS_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".fts" "FITS_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".fit" "FITS_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".fpx" "FPX_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".jng" "JNG_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".mat" "MAT_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".miff" "MIFF_FILE"
		
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".mng" "MNG_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".mono" "MONO_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".mtv" "MTV_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".otb" "OTB_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".p7" "P7_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".palm" "PALM_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".pam" "PAM_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".pcx" "PCX_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".pdb" "PDB_FILE"
		
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".pict" "PICT_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".pct" "PICT_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".pic" "PICT_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".pix" "PIX_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".pal" "PIX_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".pnm" "PNM_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".ptif" "PTIF_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".ptiff" "PTIFF_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".sfw" "SFW_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".sgi" "SGI_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".sun" "SUN_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".tga" "TGA_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".vicar" "VICAR_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".viff" "VIFF_FILE"
		
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".wbmp" "WBMP_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".wbm" "WBMP_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".wpg" "WPG_FILE"
		!insertmacro RegisterExtensionCall "$INSTDIR\photoqt.exe" ".xwd" "XWD_FILE"
		
	${EndIf}
	
	; Create desktop icon
	${If} $CheckboxDesktop_State == ${BST_CHECKED}
	
		CreateShortcut "$desktop\PhotoQt.lnk" "$instdir\photoqt.exe" "" "$INSTDIR\logo.ico" 0
		
	${EndIf}
	
	; Create startmenu entry
	${If} $CheckboxStartMenu_State == ${BST_CHECKED}
	
		CreateDirectory "$SMPROGRAMS\PhotoQt"
		CreateShortCut "$SMPROGRAMS\PhotoQt\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
		CreateShortCut "$SMPROGRAMS\PhotoQt\PhotoQt.lnk" "$INSTDIR\photoqt.exe" "" "$INSTDIR\logo.ico" 0
		
	${EndIf}
	
	; Update icons
	System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) v (0x08000000, 0, 0, 0)'

FunctionEnd

;--------------------------------
; UNINSTALLER SECTION

Section "Uninstall"

	; Delete all files and directories

	Delete "$INSTDIR\uninstall.exe"
	
	Delete "$INSTDIR\libbz2-1.dll"
	Delete "$INSTDIR\libEGL.dll"
	Delete "$INSTDIR\libexiv2.dll"
	Delete "$INSTDIR\libexpat-1.dll"
	Delete "$INSTDIR\libfreetype-6.dll"
	Delete "$INSTDIR\libgcc_s_seh-1.dll"	; 64 Bit
	Delete "$INSTDIR\libgcc_s_sjlj-1.dll"	; 32 Bit
	Delete "$INSTDIR\libgif-7.dll"
	Delete "$INSTDIR\libGLESv2.dll"
	Delete "$INSTDIR\libgomp-1.dll"
	Delete "$INSTDIR\libGraphicsMagick-3.dll"
	Delete "$INSTDIR\libGraphicsMagick++-3.dll"
	Delete "$INSTDIR\libiconv-2.dll"
	Delete "$INSTDIR\libjpeg-8.dll"
	Delete "$INSTDIR\libpcre16-0.dll"
	Delete "$INSTDIR\libpng16-16.dll"
	Delete "$INSTDIR\libsqlite3-0.dll"
	Delete "$INSTDIR\libstdc++-6.dll"
	Delete "$INSTDIR\libtiff-5.dll"
	Delete "$INSTDIR\libwebp-5.dll"
	Delete "$INSTDIR\libwinpthread-1.dll"
	Delete "$INSTDIR\libxml2-2.dll"
	Delete "$INSTDIR\Qt5Core.dll"
	Delete "$INSTDIR\Qt5Gui.dll"
	Delete "$INSTDIR\Qt5Multimedia.dll"
	Delete "$INSTDIR\Qt5Network.dll"
	Delete "$INSTDIR\Qt5Sql.dll"
	Delete "$INSTDIR\Qt5Widgets.dll"
	Delete "$INSTDIR\zlib1.dll"
	
	Delete "$INSTDIR\license.txt"
	Delete "$INSTDIR\photoqt.exe"
	Delete "$INSTDIR\logo.ico"

	Delete "$INSTDIR\sqldrivers\qsqlite.dll"
	Delete "$INSTDIR\platforms\qwindows.dll"
	
	Delete "$INSTDIR\imageformats\qdds.dll"
	Delete "$INSTDIR\imageformats\qgif.dll"
	Delete "$INSTDIR\imageformats\qicns.dll"
	Delete "$INSTDIR\imageformats\qico.dll"
	Delete "$INSTDIR\imageformats\qjp2.dll"
	Delete "$INSTDIR\imageformats\qjpeg.dll"
	Delete "$INSTDIR\imageformats\qmng.dll"
	Delete "$INSTDIR\imageformats\qtga.dll"
	Delete "$INSTDIR\imageformats\qtiff.dll"
	Delete "$INSTDIR\imageformats\qwbmp.dll"
	Delete "$INSTDIR\imageformats\qwebp.dll"

	Delete "$desktop\PhotoQt.lnk"

	RMDir "$INSTDIR\platforms"
	RMDir "$INSTDIR\sqldrivers"
	RMDir "$INSTDIR\imageformats"
	RMDir "$INSTDIR"
  
	; De-register file types
	!insertmacro UnRegisterExtensionCall ".bmp" "BMP_FILE"
	!insertmacro UnRegisterExtensionCall ".bitmap" "BMP_FILE"
	!insertmacro UnRegisterExtensionCall ".gif" "JPEG2000_FILE"
	!insertmacro UnRegisterExtensionCall ".tif" "JPEG2000_FILE"
	!insertmacro UnRegisterExtensionCall ".tiff" "JPEG2000_FILE"
	!insertmacro UnRegisterExtensionCall ".jpeg2000" "JPEG2000_FILE"
	!insertmacro UnRegisterExtensionCall ".jp2" "JPEG2000_FILE"
	!insertmacro UnRegisterExtensionCall ".jpc" "JPEG2000_FILE"
	!insertmacro UnRegisterExtensionCall ".j2k" "JPEG2000_FILE"
	!insertmacro UnRegisterExtensionCall ".jpf" "JPEG2000_FILE"
	!insertmacro UnRegisterExtensionCall ".jpx" "JPEG2000_FILE"
	!insertmacro UnRegisterExtensionCall ".jpm" "JPEG2000_FILE"
	!insertmacro UnRegisterExtensionCall ".mj2" "JPEG2000_FILE"
	!insertmacro UnRegisterExtensionCall ".ico" "ICON_FILE"
	!insertmacro UnRegisterExtensionCall ".picon" "PICON_FILE"
	!insertmacro UnRegisterExtensionCall ".jpeg" "JPEG_FILE"
	!insertmacro UnRegisterExtensionCall ".jpg" "JPEG_FILE"
	!insertmacro UnRegisterExtensionCall ".png" "PNG_FILE"
	!insertmacro UnRegisterExtensionCall ".pbm" "PBM_FILE"
	!insertmacro UnRegisterExtensionCall ".pgm" "PGM_FILE"
	!insertmacro UnRegisterExtensionCall ".ppm" "PPM_FILE"
	!insertmacro UnRegisterExtensionCall ".svg" "SVG_FILE"
	!insertmacro UnRegisterExtensionCall ".xbm" "XBM_FILE"
	!insertmacro UnRegisterExtensionCall ".xpm" "XPM_FILE"
	
	!insertmacro UnRegisterExtensionCall ".avs" "AVS_X_IMAGE"
	!insertmacro UnRegisterExtensionCall ".x" "AVS_X_IMAGE"
	
	!insertmacro UnRegisterExtensionCall ".cals" "CALS_FILE"
	!insertmacro UnRegisterExtensionCall ".cal" "CALS_FILE"
	!insertmacro UnRegisterExtensionCall ".dcl" "CALS_FILE"
	!insertmacro UnRegisterExtensionCall ".ras" "CALS_FILE"
	
	!insertmacro UnRegisterExtensionCall ".cin" "CIN_FILE"
	!insertmacro UnRegisterExtensionCall ".cut" "CUT_FILE"
	
	!insertmacro UnRegisterExtensionCall ".acr" "DICOM_FILE"
	!insertmacro UnRegisterExtensionCall ".dcm" "DICOM_FILE"
	!insertmacro UnRegisterExtensionCall ".dicom" "DICOM_FILE"
	!insertmacro UnRegisterExtensionCall ".dic" "DICOM_FILE"
	
	!insertmacro UnRegisterExtensionCall ".dcx" "DCX_FILE"
	!insertmacro UnRegisterExtensionCall ".dib" "DIB_FILE"
	!insertmacro UnRegisterExtensionCall ".dpx" "DPX_FILE"
	
	!insertmacro UnRegisterExtensionCall ".fax" "FAX_FILE"
	!insertmacro UnRegisterExtensionCall ".fits" "FITS_FILE"
	!insertmacro UnRegisterExtensionCall ".fts" "FITS_FILE"
	!insertmacro UnRegisterExtensionCall ".fit" "FITS_FILE"
	!insertmacro UnRegisterExtensionCall ".fpx" "FPX_FILE"
	!insertmacro UnRegisterExtensionCall ".jng" "JNG_FILE"
	!insertmacro UnRegisterExtensionCall ".mat" "MAT_FILE"
	!insertmacro UnRegisterExtensionCall ".miff" "MIFF_FILE"
	
	!insertmacro UnRegisterExtensionCall ".mng" "MNG_FILE"
	!insertmacro UnRegisterExtensionCall ".mono" "MONO_FILE"
	!insertmacro UnRegisterExtensionCall ".mtv" "MTV_FILE"
	!insertmacro UnRegisterExtensionCall ".otb" "OTB_FILE"
	!insertmacro UnRegisterExtensionCall ".p7" "P7_FILE"
	!insertmacro UnRegisterExtensionCall ".palm" "PALM_FILE"
	!insertmacro UnRegisterExtensionCall ".pam" "PAM_FILE"
	!insertmacro UnRegisterExtensionCall ".pcx" "PCX_FILE"
	!insertmacro UnRegisterExtensionCall ".pdb" "PDB_FILE"
	
	!insertmacro UnRegisterExtensionCall ".pict" "PICT_FILE"
	!insertmacro UnRegisterExtensionCall ".pct" "PICT_FILE"
	!insertmacro UnRegisterExtensionCall ".pic" "PICT_FILE"
	!insertmacro UnRegisterExtensionCall ".pix" "PIX_FILE"
	!insertmacro UnRegisterExtensionCall ".pal" "PIX_FILE"
	!insertmacro UnRegisterExtensionCall ".pnm" "PNM_FILE"
	!insertmacro UnRegisterExtensionCall ".ptif" "PTIF_FILE"
	!insertmacro UnRegisterExtensionCall ".ptiff" "PTIFF_FILE"
	!insertmacro UnRegisterExtensionCall ".sfw" "SFW_FILE"
	!insertmacro UnRegisterExtensionCall ".sgi" "SGI_FILE"
	!insertmacro UnRegisterExtensionCall ".sun" "SUN_FILE"
	!insertmacro UnRegisterExtensionCall ".tga" "TGA_FILE"
	!insertmacro UnRegisterExtensionCall ".vicar" "VICAR_FILE"
	!insertmacro UnRegisterExtensionCall ".viff" "VIFF_FILE"
	
	!insertmacro UnRegisterExtensionCall ".wbmp" "WBMP_FILE"
	!insertmacro UnRegisterExtensionCall ".wbm" "WBMP_FILE"
	!insertmacro UnRegisterExtensionCall ".wpg" "WPG_FILE"
	!insertmacro UnRegisterExtensionCall ".xwd" "XWD_FILE"
  
  
	; Update icons
	System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) v (0x08000000, 0, 0, 0)'

	; And delete registry key
	DeleteRegKey /ifempty HKCU "Software\PhotoQt"

SectionEnd
