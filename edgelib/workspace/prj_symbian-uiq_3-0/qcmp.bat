devices -setdefault @UIQ3:com.symbian.UIQ

del \Symbian\UIQ3SDK\epoc32\release\armv5\urel\edge.lib
del ..\..\..\dist\lib\symbian-uiq_3-0\edge.lib

bldmake bldfiles
call abld build gcce urel

copy \Symbian\UIQ3SDK\epoc32\release\armv5\urel\edge.lib ..\..\..\dist\lib\symbian-uiq_3-0\edge.lib