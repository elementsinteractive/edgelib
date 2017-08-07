devices -setdefault @Series80_DP2_0_SDK:com.nokia.Series80

del \Symbian\7.0s\S80_DP2_0_SDK\epoc32\release\armi\urel\edge.lib
del ..\..\..\dist\lib\symbian-s80_e2nd\edge.lib

bldmake bldfiles
call abld build armi urel

copy \Symbian\7.0s\S80_DP2_0_SDK\epoc32\release\armi\urel\edge.lib ..\..\..\dist\lib\symbian-s80_e2nd\edge.lib