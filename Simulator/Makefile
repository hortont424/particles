.PHONY: all clean run run-cpu edit

all:
	scons -Q

clean:
	rm -rf ./build

run:
	./build/simulator

run-cpu:
	./build/simulator -c

edit:
	mate *.c *.h libsimulator/*.c libsimulator/*.h
