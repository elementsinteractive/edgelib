@echo Copy opengl plugin libs...
@copy /Y "pc-x86\release\Windows desktop (vc 6.0)\*.lib" "..\..\..\dist\lib\pc-x86\opengl"
@copy /Y "pc-x86\release\Windows desktop (vc 2005)\*.lib" "..\..\..\dist\lib\pc-x86_vc2005\opengl"

@copy /Y "gizmondo\release\Gizmondo\*.lib" "..\..\..\dist\lib\gizmondo\opengl"

@copy /Y "wce-arm\release\WM Pocket PC (2003)\*.lib" "..\..\..\dist\lib\ppc2003-arm\opengl"
@copy /Y "wce-arm\release\WM Pocket PC (2003)\*.lib" "..\..\..\dist\lib\ppc2003-arm_vc2005\opengl"
@copy /Y "wce-arm\release\WM Pocket PC (2003)\*.lib" "..\..\..\dist\lib\smartphone2003-arm\opengl"
@copy /Y "wce-arm\release\WM Pocket PC (2003)\*.lib" "..\..\..\dist\lib\smartphone2003-arm_vc2005\opengl"

@copy /Y "wce-arm\release\WM Pocket PC (2005)\*.lib" "..\..\..\dist\lib\ppc2005-arm\opengl"
@copy /Y "wce-arm\release\WM Pocket PC (2005)\*.lib" "..\..\..\dist\lib\smartphone2005-arm\opengl"
@copy /Y "wce-arm\release\WM Pocket PC (2005)\*.lib" "..\..\..\dist\lib\ppc2006-arm\opengl"
@copy /Y "wce-arm\release\WM Pocket PC (2005)\*.lib" "..\..\..\dist\lib\smartphone2006-arm\opengl"

@copy /Y "sym-pre9\release\Series 60 (1st edition)\*.lib" "..\..\..\dist\lib\symbian-s60_e1st\opengl"
@copy /Y "sym-pre9\release\Series 60 (1st edition)\*.lib" "..\..\..\dist\lib\symbian-s60_e2nd_fp2\opengl"
@copy /Y "sym-pre9\release\Series 60 (1st edition)\*.lib" "..\..\..\dist\lib\symbian-s60_e2nd_fp3\opengl"
@copy /Y "sym-pre9\release\Series 60 (1st edition)\*.lib" "..\..\..\dist\lib\symbian-s80_e1st\opengl"
@copy /Y "sym-pre9\release\Series 60 (1st edition)\*.lib" "..\..\..\dist\lib\symbian-s80_e2nd\opengl"
@copy /Y "sym-pre9\release\Series 60 (1st edition)\*.lib" "..\..\..\dist\lib\symbian-s90\opengl"
@copy /Y "sym-pre9\release\Series 60 (1st edition)\*.lib" "..\..\..\dist\lib\symbian-uiq_2-1\opengl"

@copy /Y "sym\release\Series 60 (3rd edition)\*.lib" "..\..\..\dist\lib\symbian-s60_e3rd\opengl"

@copy /Y "sym\release\UIQ (3.0)\*.lib" "..\..\..\dist\lib\symbian-uiq_3-0\opengl"

@copy /Y "mac\release\*.a" "..\..\..\dist\lib\iphone\opengl"
@copy /Y "mac-es2\release\*.a" "..\..\..\dist\lib\iphone\opengl"

@copy /Y "antix\release\Antix Gameplayer\*.lib" "..\..\..\dist\lib\antix\opengl"

@copy /Y "android\release\Android\plugingl1-0.lib" "..\..\..\dist\lib\android\opengl\libplugingl1-0.a"