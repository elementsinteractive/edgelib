#!/bin/bash
#cd /SVN/edgelib/devlib/extlib/opengl_plugin/mac/release
lipo ./Release-iphoneos/libplugingl2-0.a ./Release-iphonesimulator/libplugingl2-0.a -create -output libplugingl2-0.a
