; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "MapWinGIS"
#define MyAppVersion "4.9.3.5"
#define MyAppPublisher "MapWindow Open Source GIS Community"
#define MyAppURL "http://www.mapwindow.org"
#define SetupLocation "C:\dev\mapwingis\src\InnoSetup"
#define Bin32Location "C:\dev\mapwingis\src\bin"
;; #define x64BitVersion

#ifdef x64BitVersion
  #define CPU "x64"
  #define vcredist "vcredist_x64_2010_sp1.exe"
  #define MySourceDir "C:\dev\mapwingis\src\bin\x64\"
  #define SystemFlag "64bit"
  #define DemoLocation "C:\dev\mapwingis\demo\MWLite.GUI\bin\x64\Release"
#else
  #define CPU "Win32"
  #define vcredist "vcredist_x86_2010_sp1.exe"
  #define MySourceDir "C:\dev\mapwingis\src\bin\Win32\"
  #define SystemFlag "32bit"
  #define DemoLocation "C:\dev\mapwingis\demo\MWLite.GUI\bin\x86\Release"
#endif

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
#ifdef x64BitVersion
  AppId={{6BEBD4FD-49F2-4D2C-8AA3-5DBADFCC9F2E}
  ArchitecturesAllowed=x64
#else
  AppId={{C31CF8C2-B385-460A-9533-5C92B457D4C2}
#endif
AppName={#MyAppName}-{#CPU}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
UsePreviousAppDir=yes
DefaultDirName=C:\dev\{#MyAppName}
DisableProgramGroupPage=yes
DefaultGroupName={#MyAppName}
LicenseFile={#Bin32Location}\Licenses\MapWinGISLicense.rtf
InfoBeforeFile={#Bin32Location}\ReleaseNotes.rtf
OutputDir={#SetupLocation}
OutputBaseFilename=MapWinGIS-only-v{#MyAppVersion}-{#CPU}
SetupIconFile={#Bin32Location}\MapWindow.ico
Compression=lzma
SolidCompression=yes
WizardImageFile={#SetupLocation}\WizImage-MW.bmp
WizardSmallImageFile={#SetupLocation}\WizSmallImage-MW.bmp
AppCopyright={#MyAppPublisher}
PrivilegesRequired=poweruser
MinVersion=0,5.01sp3   
ChangesEnvironment=yes
AlwaysShowDirOnReadyPage=True
EnableDirDoesntExistWarning=True
UninstallDisplayName=MapWinGIS uninstall
CompressionThreads=2
LZMANumBlockThreads=2
UninstallDisplayIcon={uninstallexe}
AppComments=This package will install {#MyAppName} {#MyAppVersion}
AppContact=Paul Meems (info@mapwindow.nl)

[Files]
Source: "{#MySourceDir}\cfitsio.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\gdal200.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\geos.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\geos_c.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\hdf5.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\hdf5_hl.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\iconv.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\libcurl.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\libexpat.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\libmysql.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\libpq.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\libtiff.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\lti_dsdk_9.0.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\lti_lidar_dsdk_1.1.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\MapWinGIS.ocx"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
;Source: "{#MySourceDir}\mfc90.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\NCSEcw.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\netcdf.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\openjp2.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\proj.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\spatialite.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\szip.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\tbb.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\xdrdll.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\xerces-c_2_8.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
Source: "{#MySourceDir}\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion {#SystemFlag}; Components: MapWinGIS_Core
;; IntelliSense:
Source: "{#Bin32Location}\AxInterop.MapWinGIS.xml"; DestDir: "{app}"; Flags: ignoreversion; Components: MapWinGIS_Core
Source: "{#Bin32Location}\Interop.MapWinGIS.xml"; DestDir: "{app}"; Flags: ignoreversion; Components: MapWinGIS_Core
;; Licenses
Source: "{#Bin32Location}\Licenses\GDALLicense.rtf"; DestDir: "{app}\Licenses\"; Flags: ignoreversion; Components: MapWinGIS_Core
Source: "{#Bin32Location}\Licenses\GISInternalsLicense.rtf"; DestDir: "{app}\Licenses\"; Flags: ignoreversion; Components: MapWinGIS_Core
Source: "{#Bin32Location}\Licenses\MapWinGISLicense.rtf"; DestDir: "{app}\Licenses\"; Flags: ignoreversion; Components: MapWinGIS_Core
;Source: "Licenses\FileGDBLicense.rtf"; DestDir: "{app}\Licenses\"; Flags: ignoreversion; Components: MapWinGIS_Core
;Source: "Licenses\FITSLicense.rtf"; DestDir: "{app}\Licenses\"; Flags: ignoreversion; Components: MapWinGIS_Core
;Source: "Licenses\HDF4License.rtf"; DestDir: "{app}\Licenses\"; Flags: ignoreversion; Components: MapWinGIS_Core
;Source: "Licenses\HDF5License.rtf"; DestDir: "{app}\Licenses\"; Flags: ignoreversion; Components: MapWinGIS_Core
;Source: "Licenses\OCILicense.rtf"; DestDir: "{app}\Licenses\"; Flags: ignoreversion; Components: MapWinGIS_Core
;Source: "Licenses\SZIPLicense.rtf"; DestDir: "{app}\Licenses\"; Flags: ignoreversion; Components: MapWinGIS_Core
;; GDAL Plugins:
Source: "{#MySourceDir}\gdalplugins\gdal_ECW_JP2ECW.dll"; DestDir: "{app}\gdalplugins\"; Flags: ignoreversion recursesubdirs createallsubdirs {#SystemFlag}; Components: ECW
Source: "{#Bin32Location}\Licenses\ECW5License.rtf"; DestDir: "{app}\Licenses\"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: ECW
Source: "{#MySourceDir}\gdalplugins\gdal_MrSID.dll"; DestDir: "{app}\gdalplugins\"; Flags: ignoreversion {#SystemFlag}; Components: MrSID
Source: "{#Bin32Location}\Licenses\MRSIDLicense.rtf"; DestDir: "{app}\Licenses\"; Flags: ignoreversion; Components: MrSID
Source: "{#MySourceDir}\gdalplugins\gdal_netCDF.dll"; DestDir: "{app}\gdalplugins\"; Flags: ignoreversion {#SystemFlag}; Components: NetCDF
Source: "{#Bin32Location}\Licenses\NetCDFLicense.rtf"; DestDir: "{app}\Licenses\"; Flags: ignoreversion; Components: NetCDF
;; GDAL data
Source: "{#MySourceDir}\gdal-data\*"; DestDir: "{app}\gdal-data"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: MapWinGIS_Core
Source: "{#Bin32Location}\PROJ_NAD\*"; DestDir: "{app}\PROJ_NAD"; Flags: ignoreversion recursesubdirs createallsubdirs; Components: MapWinGIS_Core
;; Demo application:
Source: "{#DemoLocation}\*.dll"; DestDir: "{app}\MapWindowLite\"; Flags: ignoreversion {#SystemFlag}; Components: DemoApp
Source: "{#DemoLocation}\*.exe"; DestDir: "{app}\MapWindowLite\"; Flags: ignoreversion {#SystemFlag}; Components: DemoApp
Source: "{#DemoLocation}\*.exe.config"; DestDir: "{app}\MapWindowLite\"; Flags: ignoreversion {#SystemFlag}; Components: DemoApp
Source: "{#DemoLocation}\*.xml"; DestDir: "{app}\MapWindowLite\"; Flags: ignoreversion {#SystemFlag}; Components: DemoApp

;; To register the ocx:
Source: "{#SetupLocation}\regMapWinGIS.cmd"; DestDir: "{app}"; Flags: ignoreversion; Components: MapWinGIS_Core
Source: "{#SetupLocation}\unregMapWinGIS.cmd"; DestDir: "{app}"; Flags: ignoreversion; Components: MapWinGIS_Core
;; Source: "{#SetupLocation}\SetEnv.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: MapWinGIS_Core
;; VC++ files
Source: "{#SetupLocation}\{#vcredist}"; DestDir: "{tmp}"; Flags: deleteafterinstall ignoreversion {#SystemFlag}
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Messages]
BeveledLabel=MapWinGIS Open Source ActiveX Map Control

[Components]
Name: "MapWinGIS_Core"; Description: "Core files for MapWinGIS"; Types: full custom compact; Flags: fixed
Name: "NetCDF"; Description: "Add NetCDF support"; Types: full custom
Name: "ECW"; Description: "Add ECW & JPEG2000 support"; Types: full custom
Name: "MrSID"; Description: "Add MrSID support"; Types: full custom
Name: "DemoApp"; Description: "Small demo application to showcase the new features"; Types: full custom

[Run]
; Install VC++ 2010 if needed:
#ifdef x64BitVersion
Filename: "{tmp}\{#vcredist}"; Parameters: "/qb"; Flags: waituntilterminated; Check: VCRedistNeedsInstall_x64()
#else
Filename: "{tmp}\{#vcredist}"; Parameters: "/qb"; Flags: waituntilterminated; Check: VCRedistNeedsInstall_x86()
#endif
;Run some command files:
Filename: "{app}\regMapWinGIS.cmd"; WorkingDir: "{app}"; Flags: runhidden
Filename: "{app}\MapWindowLite\MapWindowLite.exe"; Flags: shellexec runasoriginaluser postinstall nowait skipifsilent; Description: "Start MapWindowLite, the demo application"
Filename: "http://www.mapwindow.org/documentation/mapwingis4.9/getting_started.html?utm_source=MWv49&utm_medium=cpc&utm_campaign=MWGv493Installer"; Flags: shellexec runasoriginaluser postinstall nowait skipifsilent; Description: "Go to the online documentation"
Filename: "http://www.mapwindow.org/documentation/mapwingis4.9/MapWindow49.html?utm_source=MWv49&utm_medium=cpc&utm_campaign=MWGv493Installer"; Flags: shellexec runasoriginaluser postinstall nowait skipifsilent; Description: "Read about the future of MapWinGIS/MapWindow v5"
Filename: "http://www.mapwindow.org/documentation/mapwingis4.9/version_history.html?utm_source=MWv49&utm_medium=cpc&utm_campaign=MWGv493Installer"; Flags: shellexec runasoriginaluser postinstall nowait skipifsilent; Description: "Read about the changes in this version"

[UninstallRun]
Filename: "{app}\unregMapWinGIS.cmd"; WorkingDir: "{app}"; Flags: runhidden

[Registry]

[Code]
#IFDEF UNICODE
  #DEFINE AW "W"
#ELSE
  #DEFINE AW "A"
#ENDIF
type
  INSTALLSTATE = Longint;
const
  INSTALLSTATE_INVALIDARG = -2;  // An invalid parameter was passed to the function.
  INSTALLSTATE_UNKNOWN = -1;     // The product is neither advertised or installed.
  INSTALLSTATE_ADVERTISED = 1;   // The product is advertised but not installed.
  INSTALLSTATE_ABSENT = 2;       // The product is installed for a different user.
  INSTALLSTATE_DEFAULT = 5;      // The product is installed for the current user.

  VC_2008_REDIST_X86 = '{FF66E9F6-83E7-3A3E-AF14-8DE9A809A6A4}';
  VC_2008_REDIST_X64 = '{350AA351-21FA-3270-8B7A-835434E766AD}';
  VC_2008_REDIST_IA64 = '{2B547B43-DB50-3139-9EBE-37D419E0F5FA}';
  VC_2008_SP1_REDIST_X86 = '{9A25302D-30C0-39D9-BD6F-21E6EC160475}';
  VC_2008_SP1_REDIST_X64 = '{8220EEFE-38CD-377E-8595-13398D740ACE}';
  VC_2008_SP1_REDIST_IA64 = '{5827ECE1-AEB0-328E-B813-6FC68622C1F9}';
  VC_2008_SP1_ATL_SEC_UPD_REDIST_X86 = '{1F1C2DFC-2D24-3E06-BCB8-725134ADF989}';
  VC_2008_SP1_ATL_SEC_UPD_REDIST_X64 = '{4B6C7001-C7D6-3710-913E-5BC23FCE91E6}';
  VC_2008_SP1_ATL_SEC_UPD_REDIST_IA64 = '{977AD349-C2A8-39DD-9273-285C08987C7B}';
  VC_2008_SP1_MFC_SEC_UPD_REDIST_X86 = '{9BE518E6-ECC6-35A9-88E4-87755C07200F}';
  VC_2008_SP1_MFC_SEC_UPD_REDIST_X64 = '{5FCE6D76-F5DC-37AB-B2B8-22AB8CEDB1D4}';
  VC_2008_SP1_MFC_SEC_UPD_REDIST_IA64 = '{515643D1-4E9E-342F-A75A-D1F16448DC04}';

  VC_2010_REDIST_X86 = '{196BB40D-1578-3D01-B289-BEFC77A11A1E}';
  VC_2010_REDIST_X64 = '{DA5E371C-6333-3D8A-93A4-6FD5B20BCC6E}';
  VC_2010_REDIST_IA64 = '{C1A35166-4301-38E9-BA67-02823AD72A1B}';
  VC_2010_SP1_REDIST_X86 = '{F0C3E5D1-1ADE-321E-8167-68EF0DE699A5}';
  VC_2010_SP1_REDIST_X64 = '{1D8E6291-B0D5-35EC-8441-6616F567A0F7}';
  VC_2010_SP1_REDIST_IA64 = '{88C73C1C-2DE5-3B01-AFB8-B46EF4AB41CD}';

function MsiQueryProductState(szProduct: string): INSTALLSTATE; 
  external 'MsiQueryProductState{#AW}@msi.dll stdcall';

function VCVersionInstalled(const ProductID: string): Boolean;
begin
  Result := MsiQueryProductState(ProductID) = INSTALLSTATE_DEFAULT;
end;

function VCRedistNeedsInstall_x86(): Boolean;
begin
  // here the Result must be True when you need to install your VCRedist
  // or False when you don't need to, so now it's upon you how you build
  // this statement, the following won't install your VC redist only when
  // the Visual C++ 2008 Redist (x86) and Visual C++ 2008 SP1 Redist(x86)
  // are installed for the current user
  Result := not (VCVersionInstalled(VC_2010_SP1_REDIST_X86));
end;

function VCRedistNeedsInstall_x64(): Boolean;
begin
  // here the Result must be True when you need to install your VCRedist
  // or False when you don't need to, so now it's upon you how you build
  // this statement, the following won't install your VC redist only when
  // the Visual C++ 2008 Redist (x86) and Visual C++ 2008 SP1 Redist(x86)
  // are installed for the current user
  Result := not (VCVersionInstalled(VC_2010_SP1_REDIST_X64));
end;

function NeedsAddPath(Param: string): boolean;
var
  OrigPath: string;
begin
  if not RegQueryStringValue(HKEY_LOCAL_MACHINE,
    'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
    'Path', OrigPath)
  then begin
    Result := True;
    exit;
  end;
  // look for the path with leading and trailing semicolon
  // Pos() returns 0 if not found
  Result := Pos(';' + Param + ';', ';' + OrigPath + ';') = 0;
end;


[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "afrikaans"; MessagesFile: "compiler:Languages\Afrikaans.isl"
Name: "albanian"; MessagesFile: "compiler:Languages\Albanian.isl"
Name: "arabic"; MessagesFile: "compiler:Languages\Arabic.isl"
Name: "basque"; MessagesFile: "compiler:Languages\Basque.isl"
Name: "belarusian"; MessagesFile: "compiler:Languages\Belarusian.isl"
Name: "bosnian"; MessagesFile: "compiler:Languages\Bosnian.isl"
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "bulgarian"; MessagesFile: "compiler:Languages\Bulgarian.isl"
Name: "catalan"; MessagesFile: "compiler:Languages\Catalan.isl"
Name: "chinesesimp"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"
Name: "chinesetrad"; MessagesFile: "compiler:Languages\ChineseTraditional.isl"
Name: "corsican"; MessagesFile: "compiler:Languages\Corsican.isl"
Name: "czech"; MessagesFile: "compiler:Languages\Czech.isl"
Name: "danish"; MessagesFile: "compiler:Languages\Danish.isl"
Name: "dutch"; MessagesFile: "compiler:Languages\Dutch.isl"
Name: "estonian"; MessagesFile: "compiler:Languages\Estonian.isl"
Name: "finnish"; MessagesFile: "compiler:Languages\Finnish.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "galician"; MessagesFile: "compiler:Languages\Galician.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "greek"; MessagesFile: "compiler:Languages\Greek.isl"
Name: "hebrew"; MessagesFile: "compiler:Languages\Hebrew.isl"
Name: "hungarian"; MessagesFile: "compiler:Languages\Hungarian.isl"
Name: "icelandic"; MessagesFile: "compiler:Languages\Icelandic.isl"
Name: "indonesian"; MessagesFile: "compiler:Languages\Indonesian.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "korean"; MessagesFile: "compiler:Languages\Korean.isl"
Name: "lithuanian"; MessagesFile: "compiler:Languages\Lithuanian.isl"
Name: "luxemburgish"; MessagesFile: "compiler:Languages\Luxemburgish.isl"
Name: "macedonian"; MessagesFile: "compiler:Languages\Macedonian.isl"
Name: "malaysian"; MessagesFile: "compiler:Languages\Malaysian.isl"
Name: "nepali"; MessagesFile: "compiler:Languages\Nepali.islu"
Name: "norwegian"; MessagesFile: "compiler:Languages\Norwegian.isl"
Name: "polish"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "portuguese"; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: "romanian"; MessagesFile: "compiler:Languages\Romanian.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "serbiancyrillic"; MessagesFile: "compiler:Languages\SerbianCyrillic.isl"
Name: "serbianlatin"; MessagesFile: "compiler:Languages\SerbianLatin.isl"
Name: "slovak"; MessagesFile: "compiler:Languages\Slovak.isl"
Name: "slovenian"; MessagesFile: "compiler:Languages\Slovenian.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "swedish"; MessagesFile: "compiler:Languages\Swedish.isl"
Name: "tatarish"; MessagesFile: "compiler:Languages\Tatar.isl"
Name: "turkish"; MessagesFile: "compiler:Languages\Turkish.isl"
Name: "ukrainian"; MessagesFile: "compiler:Languages\Ukrainian.isl"

