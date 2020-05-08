CC=gcc
CXX=g++OMP=
all: imagetools flashphoto mia tests

check-style:
	cd src/flashphoto; make check-style

check-style-cse:
	cd src/flashphoto; make check-style-cse

imagetools:
	cd src/imagetools; make -j

mia:
	cd src/mia; make -j

flashphoto:
	cd src/flashphoto; make -j

tests:
	cd src/tests; make -j

clean:
	rm -rf build

clobber: clean
	cd src/external/MinGfx; make clobber
	cd src/external/googletest; make clean
