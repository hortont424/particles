.PHONY: all clean run run-cpu edit quick docs quickdocs

all: quick quickdocs

quick:
	scons -Q

quickdocs:
	doxygen libsimulator/libsimulator.doxyfile

docs: quickdocs
	open build/doc/html/modules.html

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
