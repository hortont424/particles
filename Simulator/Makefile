.PHONY: all clean run run-cpu edit quick docs quickdocs ctypes

all: quick quickdocs

quick:
	cd .. ; scons -Q

quickdocs:
	doxygen libsimulator/libsimulator.doxyfile

docs: quickdocs
	open build/doc/html/modules.html

# TODO: need to find frameworks and conditionally include the headers from
# the toplevel instead of OpenCL/ if being called from here... or not, because
# that won't cover things like OpenGL/gl.h which are included indirectly...
# Basically, the fact that h2xml doesn't support -framework is the problem.
# Or does it? I haven't tried it - it's just a gccxml frontend so that might
# be easy to fix...
# Right now, I have OpenGL and OpenCL folders with the headers in them sitting
# on my desktop, so the following works as expected
ctypes: quick
	h2xml.py -I . -I libsimulator/ -I /Users/hortont/Desktop/ -o libsimulator.xml libsimulator/*.h
	xml2py.py libsimulator.xml -r "SM.*" -o libsimulator.py -l ./build/libsimulator/libsimulator.dylib

clean:
	rm -rf ./build
	rm -f libsimulator.py libsimulator.pyc libsimulator.xml

run:
	rm -f test.out
	DYLD_LIBRARY_PATH="build/libsimulator/" ./build/simulator

run-cpu:
	rm -f test.out
	DYLD_LIBRARY_PATH="build/libsimulator/" ./build/simulator -c

edit:
	mate .
