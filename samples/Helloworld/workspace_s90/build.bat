devices -setdefault @NOKIA_7710_CW:com.nokia.Nokia7710
bldmake bldfiles
call abld build armi urel
copy \Symbian\7.0s\Nokia_7710_SDK_CW\epoc32\release\armi\urel\%1.app
makesis %1.pkg
