devices -setdefault @UIQ_21:com.symbian.UIQ
bldmake bldfiles
call abld build armi urel
copy \Symbian\UIQ_21\epoc32\release\armi\urel\%1.app
makesis %1.pkg

