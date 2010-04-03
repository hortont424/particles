.PHONY: all clean edit docs

all:
	scons -Q

docs:
	doxygen particles.doxyfile
	open Libraries/build/doc/html/index.html

clean:
	rm -rf ./Libraries/build
	rm -rf ./Interpolator/build
	rm -rf ./Simulator/build
	rm -rf ./Previewer/build
	rm -rf ./External/json-c/build

edit:
	mate .
