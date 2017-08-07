@echo off

@set path=%path%;c:\devkitGP2X\bin

@echo Compiling...
@echo helloworld.cpp
@arm-linux-g++ -O3 -I c:\projects\edge\include -D__LINUX__ -D__GP2X__ -c ..\code\helloworld.cpp -o helloworld.obj

@echo.

@echo Linking...
@arm-linux-g++ helloworld.obj --whole-archive -static -ledge -ledgerender -lz --no-whole-archive -L c:\projects\edge\lib\gp2x_devkit -o helloworld.gpe

@echo.

@echo helloworld.gpe
