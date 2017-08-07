bmconv %1_aif.mbm /c12..\res\aif\icon64.bmp /c12..\res\aif\icon64mask.bmp
devices -setdefault @UIQ3:com.symbian.UIQ
bldmake bldfiles
call abld build gcce urel
makesis %1.pkg
signsis helloworld.sis helloworld_signed.sis ..\res\edge_tmp.cer ..\res\edge_tmp.key 12345
