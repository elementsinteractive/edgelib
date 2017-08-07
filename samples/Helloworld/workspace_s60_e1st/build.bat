set OLDPATH=%PATH%
set OLDEPOC=%EPOCROOT%
set PATH=%symbianroot%\6.1\Shared\EPOC32\gcc\bin\;%symbianroot%\6.1\Shared\Epoc32\Tools\;%symbianroot%\6.1\Series60\Epoc32\Tools\;%path%
set EPOCROOT=\Symbian\6.1\Series60\
devices -setdefault @Series60_1_2_CW:com.Nokia.Series60_1_2_CW
call bldmake bldfiles
call abld build armi urel
copy \Symbian\6.1\Series60\epoc32\release\armi\urel\%1.app
makesis %1.pkg
set PATH=%OLDPATH%
set EPOCROOT=%OLDEPOC%