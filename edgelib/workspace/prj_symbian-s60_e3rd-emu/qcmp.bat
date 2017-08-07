devices -setdefault @S60_3rd:com.nokia.s60

del \Symbian\9.1\S60_3rd\epoc32\release\winscw\udeb\edge.lib
del ..\..\..\dist\lib\symbian-s60_e3rd-emu\edge.lib

bldmake bldfiles
call abld build winscw udeb

copy \Symbian\9.1\S60_3rd\epoc32\release\winscw\udeb\edge.lib ..\..\..\dist\lib\symbian-s60_e3rd-emu\edge.lib
