@echo off

@p:\prj\games\tools\edgeide\edgebuilder pc-x86/build_gl.epj /autobuild /p:select
@p:\prj\games\tools\edgeide\edgebuilder pc-x86/build_gles_pvr.epj /autobuild /p:select
@p:\prj\games\tools\edgeide\edgebuilder pc-x86/build_gles_rst.epj /autobuild /p:select
@p:\prj\games\tools\edgeide\edgebuilder android/build_gles_1-0.epj /autobuild /p:select
@p:\prj\games\tools\edgeide\edgebuilder sym/build_gles_1-0.epj /autobuild /p:select
@p:\prj\games\tools\edgeide\edgebuilder sym-pre9/build_gles_rst.epj /autobuild /p:select
@p:\prj\games\tools\edgeide\edgebuilder antix/build_gles_1-0.epj /autobuild /p:select
@p:\prj\games\tools\edgeide\edgebuilder wce-arm/build_gles_1-0.epj /autobuild /p:select
@p:\prj\games\tools\edgeide\edgebuilder gizmondo/build_gles_1-0.epj /autobuild /p:select

@echo Please update maemo-sim, maemo-dev, iphone and macos manually