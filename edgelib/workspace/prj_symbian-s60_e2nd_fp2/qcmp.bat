devices -setdefault @S60_2nd_FP2:com.nokia.Series60

del \Symbian\8.0a\S60_2nd_FP2\epoc32\release\armi\urel\edge.lib
del ..\..\..\dist\lib\symbian-s60_e2nd_fp2\edge.lib

bldmake bldfiles
call abld build armi urel

copy \Symbian\8.0a\S60_2nd_FP2\epoc32\release\armi\urel\edge.lib ..\..\..\dist\lib\symbian-s60_e2nd_fp2\edge.lib
