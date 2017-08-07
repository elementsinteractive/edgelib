devices -setdefault @S60_3rd:com.nokia.s60

del \Symbian\9.1\S60_3rd\epoc32\release\armv5\urel\edge.lib
del ..\..\..\dist\lib\symbian-s60_e3rd\edge.lib

bldmake bldfiles
call abld build gcce urel

copy \Symbian\9.1\S60_3rd\epoc32\release\armv5\urel\edge.lib ..\..\..\dist\lib\symbian-s60_e3rd\edge.lib
