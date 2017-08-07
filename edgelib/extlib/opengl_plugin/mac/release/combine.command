#!/bin/bash
#cd /SVN/edgelib/devlib/extlib/opengl_plugin/mac/release
lipo ./Release-iphoneos/libplugingl1-1.a ./Release-iphonesimulator/libplugingl1-1.a -create -output libplugingl1-1.a
