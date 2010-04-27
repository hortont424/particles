.PHONY: all clean edit docs simulate interpolate preview

all:
	scons -Q

docs:
	doxygen particles.doxyfile
	open Libraries/build/doc/html/index.html

simulate:
	cd Simulator ; DYLD_LIBRARY_PATH=../Libraries/build/libcomputer/:../Libraries/build/libparticles/:../External/json-c/build/:../Libraries/build/libsimulator/:../Libraries/build/librenderer/:../Libraries/build/libpreviewer/ ./build/simulator -b

interpolate:
	./Interpolator/build/Interpolator.app/Contents/MacOS/Interpolator

preview:
	cd Simulator ; DYLD_LIBRARY_PATH=../Libraries/build/libcomputer/:../Libraries/build/libparticles/:../External/json-c/build/:../Libraries/build/libsimulator/:../Libraries/build/librenderer/:../Libraries/build/libpreviewer/ ./build/simulator -p

clean:
	rm -rf ./Libraries/build
	rm -rf ./Interpolator/build
	rm -rf ./Simulator/build
	rm -rf ./Previewer/build
	rm -rf ./External/json-c/build
	rm -rf ./Papers/build

edit:
	$(EDITOR) .
