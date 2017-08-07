set OLDPATH=%PATH%
set OLDEPOC=%EPOCROOT%
set PATH=%symbianroot%\6.0\Shared\EPOC32\gcc\bin\;%symbianroot%\6.0\Shared\Epoc32\Tools\;%symbianroot%\6.0\NokiaCPP\Epoc32\Tools\;%path%;C:\Program Files\Microsoft Visual Studio\VC98\Bin;C:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin
set EPOCROOT=\Symbian\6.0\NokiaCPP\
bldmake bldfiles
call abld build armi urel
copy \Symbian\6.0\NokiaCPP\epoc32\release\armi\urel\%1.app
makesis %1.pkg
set PATH=%OLDPATH%
set EPOCROOT=%OLDEPOC%
