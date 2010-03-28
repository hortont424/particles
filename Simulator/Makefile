.PHONY: all clean run run-cpu edit docs

all:
	cd .. ; scons -Q

docs:
	doxygen libsimulator/libsimulator.doxyfile
	open build/doc/html/modules.html

clean:
	rm -rf ./build

run:
	rm -f test.out
	DYLD_LIBRARY_PATH=../Libraries/build/libsimulator/:../Libraries/build/libparticles/ ./build/simulator

run-cpu:
	rm -f test.out
	DYLD_LIBRARY_PATH=../Libraries/build/libsimulator/:../Libraries/build/libparticles/ ./build/simulator -c

edit:
	mate .
