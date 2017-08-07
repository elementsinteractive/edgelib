devices -setdefault @NOKIA_7710_CW:com.nokia.Nokia7710

del \Symbian\7.0s\Nokia_7710_SDK_CW\epoc32\release\armi\urel\edge.lib
del ..\..\..\dist\lib\symbian-s90\edge.lib

bldmake bldfiles
call abld build armi urel

copy \Symbian\7.0s\Nokia_7710_SDK_CW\epoc32\release\armi\urel\edge.lib ..\..\..\dist\lib\symbian-s90\edge.lib