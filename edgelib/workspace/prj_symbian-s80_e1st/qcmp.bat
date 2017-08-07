set OLDPATH=%PATH%
set OLDEPOC=%EPOCROOT%
set PATH=%symbianroot%\6.0\Shared\EPOC32\gcc\bin\;%symbianroot%\6.0\Shared\Epoc32\Tools\;%symbianroot%\6.0\NokiaCPP\Epoc32\Tools\;%path%;C:\Program Files\Microsoft Visual Studio\VC98\Bin;C:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin
set EPOCROOT=\Symbian\6.0\NokiaCPP\
devices -setdefault @Series60_1_2_CW:com.Nokia.Series60_1_2_CW

del %EPOCROOT%\epoc32\release\armi\urel\edge.lib
del ..\..\..\dist\lib\symbian-s80_e1st\edge.lib

call bldmake bldfiles
call abld build armi urel

copy %EPOCROOT%\epoc32\release\armi\urel\edge.lib ..\..\..\dist\lib\symbian-s80_e1st\edge.lib

set PATH=%OLDPATH%
set EPOCROOT=%OLDEPOC%