devices -setdefault @UIQ_21:com.symbian.UIQ

del \Symbian\UIQ_21\epoc32\release\armi\urel\edge.lib
del ..\..\..\dist\lib\symbian-uiq_2-1\edge.lib

bldmake bldfiles
call abld build armi urel

copy \Symbian\UIQ_21\epoc32\release\armi\urel\edge.lib ..\..\..\dist\lib\symbian-uiq_2-1\edge.lib