all:
	cd shmLibrary; make all
	cd application; make all
	cd view; make all
	cd worker; make all
	@cp application/*.out .
	@cp view/*.out .
	@cp worker/*.out .

clean:
	cd application; make clean
	cd view; make clean
	cd worker; make clean
	cd shmLibrary; make clean
	@rm -f *.out

.PHONY: all clean test
