#inputs
NAME := eventcore
JNAME := Eventcore
PRJPATH := $(shell echo `pwd`)
SDKPATH := /android-sdk-linux_x86
NDKPATH := /android-ndk-r5b
EDGEINSTALL := /home/marcel/dev/edgelib_android
C_TARGET  := android-4
J_TARGET  := android-3
C_SOURCES := $(shell find -L $(PRJPATH)/jni -maxdepth 1 -name "*.cpp") $(shell find ~/dev/edgelib_android/devlib/source -name "*.cpp")
J_SOURCES := $(shell find $(PRJPATH)/src -name "*.java")

ADDINCLUDES := -I ../../../devlib/include -I ../../../devlib/source -I ../../../extsdk/zlib/include -I ../../../devlib/source/core/extlib

# android stuff
#   makestuff := -MMD -MP -MF /home/marcel/dev/android-tests/basic-test/obj/local/armeabi/objs/buildtest/test.o.d.org
PLATFORMPATH := $(NDKPATH)/platforms/$(C_TARGET)

JAVAHEADER    := javah -jni
CXX_COMPILER  := $(NDKPATH)/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/arm-linux-androideabi-g++
ADB           := $(SDKPATH)/platform-tools/adb
JAVAC	      := javac

CXX_ARCHFLAGS := -fpic -ffunction-sections -funwind-tables -fstack-protector -march=armv5te -msoft-float -fno-exceptions -fno-rtti -mthumb -fno-strict-aliasing -finline-limit=64  -Wa,--noexecstack
CXX_OPTIMISE  := -mtune=xscale -Os -fomit-frame-pointer -O2 
CXX_WARNINGS  := -Wno-psabi
CXX_INCLUDE   := -I$(NDKPATH)/sources/cxx-stl/system/include -I$(PRJPATH)/jni -I$(PRJPATH)/src -I$(PLATFORMPATH)/arch-arm/usr/include $(ADDINCLUDES)
CXX_DEFINES   := -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__ -DANDROID  -DNDEBUG
CXX_CUSTOM    := -pipe -Wall -Wextra -Wno-long-long -Wno-unused -Wno-psabi

CL_SYSROOT    :=  --sysroot=$(PLATFORMPATH)/arch-arm
CL_SDKINPUTS  := $(PLATFORMPATH)/arch-arm/usr/lib/libc.so $(PLATFORMPATH)/arch-arm/usr/lib/libstdc++.so $(PLATFORMPATH)/arch-arm/usr/lib/libm.so
CL_FLAGS      := -Wl,--no-undefined -Wl,-z,noexecstack -L$(PLATFORMPATH)/arch-arm/usr/lib -lstdc++ -Wl,-rpath-link=$(PLATFORMPATH)/arch-arm/usr/lib -lsupc++ 
CL_LIBS       := -lz -lGLESv1_CM
CL_CUSTOM     := -L $(EDGEINSTALL)/dist/lib/android -ledgerender

J_CLASSPATH   := -classpath $(PRJPATH):$(PRJPATH)/classes/bin -bootclasspath $(SDKPATH)/platforms/$(J_TARGET)/android.jar
J_FLAGS	      := -target 1.5 -encoding UTF-8 -source 1.5 -g
J_SOURCEPATH  := -sourcepath $(PRJPATH)/src:$(PRJPATH)/gen
J_DESTPATH1   := -d $(PRJPATH)/src
J_DESTPATH2   := -d $(PRJPATH)/bin/classes

SONAME := lib$(NAME).so
C_OBJECTS  := $(patsubst %.cpp,%.o,$(C_SOURCES))
J_LOBJECTS := $(patsubst %.java,%.class,$(J_SOURCES))
J_OBJECTS  := $(shell echo $(J_LOBJECTS) | sed "s/src/bin\/classes/g")
J_HEADERS  := $(patsubst %.java,%.h,$(J_SOURCES))
CXXFLAGS := $(CXX_ARCHFLAGS) $(CXX_OPTIMISE) $(CXX_INCLUDE) $(CXX_DEFINES) $(CXX_WARNINGS) $(CXX_CUSTOM)
CL_INPUTS := $(C_OBJECTS) $(PRJPATH)/obj/local/armeabi/libstdc++.a $(CL_SDKINPUTS) 
CL_OUTCONF :=  -Wl,-soname,$(SONAME) -shared
#SED_PRJPATH :=  $(shell echo -n s/ && echo -n $(PRJPATH) | sed "s/\//\\\?/g" | sed "s/\?/\//g" && echo -n // )
SED_PRJPATH := $(shell echo -n s/ && echo -n $(PRJPATH) | sed "s/\//./g" && echo -n // )

# phony rules
all: default

default: $(PRJPATH)/libs/armeabi/$(SONAME)

install: $(PRJPATH)/libs/armeabi/$(SONAME)
	$(ADB) install -r $(PRJPATH)/bin/$(JNAME)-debug.apk

test:
	@echo local objects: $(J_LOBJECTS)
	@echo destination objects: $(J_OBJECTS)
	@echo c files: $(C_OBJECTS)

.PHONY: default install all

# build rules
$(PRJPATH)/libs/armeabi/$(SONAME): $(PRJPATH)/obj/local/armeabi/$(SONAME) Makefile
	cp $< $@

#$(C_OBJECTS): $(J_HEADERS)

%.o:%.cpp Makefile 
	@echo [compile] $@
	@$(CXX_COMPILER) $(CXXFLAGS) -c $< -o $@

%.class:%.java Makefile
	@echo [javac] $@
	@javac $(J_CLASSPATH) $(J_FLAGS) $(J_SOURCEPATH) $(J_DESTPATH1) $<
	@javac $(J_CLASSPATH) $(J_FLAGS) $(J_SOURCEPATH) $(J_DESTPATH2) $<

%.h:%.class Makefile	
	@echo [jni] $@
	@$(JAVAHEADER) -o $@ -classpath $(PRJPATH)/bin/classes $(shell echo $< | sed "s/\//./g" | sed $(SED_PRJPATH) | sed "s/.src.//" | sed "s/src.//" | sed "s/.java//" | sed "s/.class//")
	@touch $@

$(PRJPATH)/obj/local/armeabi/$(SONAME): $(C_OBJECTS) $(J_HEADERS) Makefile
	@echo [link] $@
	$(CXX_COMPILER) $(CL_OUTCONF) $(CL_INPUTS) $(CL_LIBS) $(CL_FLAGS) -o $@

EXTAPIPATH :=~/dev/edgelib_android/devlib/source/core/extapi
$(EXTAPIPATH)/androidnatives.o: $(EXTAPIPATH)/androidview.h $(EXTAPIPATH)/android3dview.h $(EXTAPIPATH)/androidapp.h $(EXTAPIPATH)/androidutil.h $(EXTAPIPATH)/androidbuffer.h $(EXTAPIPATH)/androidfile.h
