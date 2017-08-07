
all: libplugingl-2.a

CFLAGS = -O2 -pipe -fomit-frame-pointer -I ../../../include -D__MAEMO__

libplugingl-1.a: wrap_gles.o Makefile
	ar rc $@ $<

libplugingl-2.a: wrap_gles2.o Makefile
	ar rc $@ $<

wrap_gles.o:  ../wrap_gles.cpp ../wrap_gles.h Makefile
	gcc -c -o $@ ../wrap_gles.cpp $(CFLAGS)

wrap_gles2.o: ../wrap_gles2.cpp ../wrap_gles2.h Makefile
	gcc -c -o $@ ../wrap_gles2.cpp $(CFLAGS)

install: libplugingl-2.a
	cp libplugingl-2.a ../../../../dist/lib/maemo-5_dev

clean:
	-rm *.o
