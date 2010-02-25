.PHONY: all clean run run-cpu edit

all:
	xcodebuild

clean:
	rm -rf ./build

run:
	./build/Release/Simulator

run-cpu:
	./build/Release/Simulator -c

edit:
	mate *.c *.h
