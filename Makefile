.PHONY: all clean edit

all:
	xcodebuild

clean:
	rm -rf ./Interpolator/build
	rm -rf ./Simulator/build
	rm -rf ./build

edit:
	mate .
