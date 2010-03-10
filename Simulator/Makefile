.PHONY: all clean run run-cpu edit quick docs quickdocs

all: quick quickdocs

quick:
	cd .. ; scons -Q

quickdocs:
	doxygen libsimulator/libsimulator.doxyfile

docs: quickdocs
	open build/doc/html/modules.html

clean:
	rm -rf ./build

run:
	rm -f test.out
	DYLD_LIBRARY_PATH="build/libsimulator/" ./build/simulator

run-cpu:
	rm -f test.out
	DYLD_LIBRARY_PATH="build/libsimulator/" ./build/simulator -c

edit:
	mate .
