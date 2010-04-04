.PHONY: all clean edit docs simulate interpolate preview

all:
	scons -Q

docs:
	doxygen particles.doxyfile
	open Libraries/build/doc/html/index.html

simulate:
	rm -f ./Simulator/test.out
	cd Simulator ; DYLD_LIBRARY_PATH=../Libraries/build/libcomputer/:../Libraries/build/libparticles/:../External/json-c/build/ ./build/simulator

interpolate:
	./Interpolator/build/Interpolator.app/Contents/MacOS/Interpolator

preview:
	cd Previewer ; ./build/previewer

clean:
	rm -rf ./Libraries/build
	rm -rf ./Interpolator/build
	rm -rf ./Simulator/build
	rm -rf ./Previewer/build
	rm -rf ./External/json-c/build

edit:
	mate .
