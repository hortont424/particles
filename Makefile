.PHONY: all

all:
	xcodebuild

run:
	open ./build/Release/Interpolator.app

edit:
	mate *.m *.h