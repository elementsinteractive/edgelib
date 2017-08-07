devices -setdefault @S60_5th_Edition_SDK_v0.9:com.nokia.s60

@rem del \S60\devices\S60_5th_Edition_SDK_v0.9\epoc32\release\armv5\urel\edge.lib
@rem del ..\..\..\dist\lib\symbian-s60_e5th\edge.lib

bldmake bldfiles
call abld build gcce urel

@rem copy \S60\devices\S60_5th_Edition_SDK_v0.9\epoc32\release\armv5\urel\edge.lib ..\..\..\dist\lib\symbian-s60_e5th\edge.lib
