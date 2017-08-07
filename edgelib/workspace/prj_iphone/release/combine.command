#!/bin/bash
#cd /SVN/edgelib/devlib/workspace/prj_iphone/release
lipo ./Release-iphoneos/libedge.a ./Release-iphonesimulator/libedge.a -create -output libedge.a
