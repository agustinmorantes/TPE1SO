all:
	cd application; make all
	cd view; make all
	cd worker; make all

clean:
	cd application; make clean
	cd view; make clean
	cd worker; make clean

test:
	cd application; make test
	cd view; make test
	cd worker; make test

.PHONY: all clean test
