.PHONY: all clean run run-cpu edit

all:
	cd .. ; scons -Q

clean:
	rm -rf ./build

run:
	rm -f test.out
	DYLD_LIBRARY_PATH=../Libraries/build/libcomputer/:../Libraries/build/libparticles/ ./build/simulator

run-cpu:
	rm -f test.out
	DYLD_LIBRARY_PATH=../Libraries/build/libcomputer/:../Libraries/build/libparticles/ ./build/simulator -c

edit:
	mate .
