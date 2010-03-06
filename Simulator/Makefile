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
	rm -f test.out
	./build/simulator

run-cpu:
	rm -f test.out
	./build/simulator -c

edit:
	mate .
