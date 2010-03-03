.PHONY: all clean run run-cpu edit

all:
	scons -Q

clean:
	rm -rf ./build

run:
	rm test.out
	./build/simulator

run-cpu:
	rm test.out
	./build/simulator -c

edit:
	mate *.c *.h libsimulator/*.c libsimulator/*.h
