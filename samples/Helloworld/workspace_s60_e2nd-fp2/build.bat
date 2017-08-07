devices -setdefault @S60_2nd_FP2:com.nokia.Series60
bldmake bldfiles
call abld build armi urel
copy \Symbian\8.0a\S60_2nd_FP2\epoc32\release\armi\urel\%1.app
makesis %1.pkg

