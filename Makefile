CC_EXEC = g++ --std=c++11

BUILD_DIR = ./build
DB_DIR = ./Database

all: utrip.out

utrip.out: ${BUILD_DIR}/main.o | build_dir
	${CC_EXEC} ${BUILD_DIR}/main.o -o utrip.out

${BUILD_DIR}/main.o: main.cpp | build_dir
	${CC_EXEC} -c main.cpp -o ${BUILD_DIR}/main.o


build_dir:
	mkdir -p ${BUILD_DIR}

clean:
	rm -rf build *.out
