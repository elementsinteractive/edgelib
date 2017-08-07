devices -setdefault @Series80_DP2_0_SDK:com.nokia.Series80
bldmake bldfiles
call abld build armi urel
copy \Symbian\7.0s\S80_DP2_0_SDK\epoc32\release\armi\urel\%1.app
makesis %1.pkg
