devices -setdefault @UIQ3:com.symbian.UIQ

del \Symbian\UIQ3SDK\epoc32\release\winscw\udeb\edge.lib
del ..\..\..\dist\lib\symbian-uiq_3-0-emu\edge.lib

bldmake bldfiles
call abld build winscw udeb

copy \Symbian\UIQ3SDK\epoc32\release\winscw\udeb\edge.lib ..\..\..\dist\lib\symbian-uiq_3-0-emu\edge.lib