.PHONY: all clean run run-cpu edit

all:
	xcodebuild

clean:
	rm -rf ./build

run:
	./build/Debug/Simulator

run-cpu:
	./build/Debug/Simulator -c

edit:
	mate *.c *.h libsimulator/*.c libsimulator/*.h
