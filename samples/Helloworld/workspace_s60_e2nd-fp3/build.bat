devices -setdefault @S60_2nd_FP3:com.nokia.series60
bldmake bldfiles
call abld build armi urel
copy \Symbian\8.1a\S60_2nd_FP3\epoc32\release\armi\urel\%1.app
makesis %1.pkg

