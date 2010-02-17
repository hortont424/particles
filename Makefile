.PHONY: all

all:
	xcodebuild

run:
	./build/Release/Interpolator.app/Contents/MacOS/Interpolator

edit:
	mate *.m *.h
