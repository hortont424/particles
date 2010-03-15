.PHONY: all clean run edit

all:
	cd .. ; scons -Q

clean:
	rm -rf ./build

run:
	DYLD_LIBRARY_PATH="../External/BWToolkit/BWToolkitFramework.Framework" ./build/Interpolator.app/Contents/MacOS/Interpolator

edit:
	mate .
