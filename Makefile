.PHONY: all clean edit

all:
	scons -Q

clean:
	rm -rf ./Interpolator/build
	rm -rf ./Simulator/build
	rm -rf ./Previewer/build

edit:
	mate .
