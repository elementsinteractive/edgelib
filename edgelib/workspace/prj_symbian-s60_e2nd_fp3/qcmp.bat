devices -setdefault @S60_2nd_FP3:com.nokia.series60

del \Symbian\8.1a\S60_2nd_FP3\epoc32\release\armi\urel\edge.lib
del ..\..\..\dist\lib\symbian-s60_e2nd_fp3\edge.lib

bldmake bldfiles
call abld build armi urel

copy \Symbian\8.1a\S60_2nd_FP3\epoc32\release\armi\urel\edge.lib ..\..\..\dist\lib\symbian-s60_e2nd_fp3\edge.lib
