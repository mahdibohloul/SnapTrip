CC_EXEC = g++ --std=c++11

BUILD_DIR = ./build
DB_DIR = ./Models

all: utrip.out

utrip.out: ${BUILD_DIR}/main.o ${BUILD_DIR}/Database.o ${BUILD_DIR}/Hotel.o ${BUILD_DIR}/Room.o ${BUILD_DIR}/StandardRoom.o | build_dir
	${CC_EXEC} ${BUILD_DIR}/main.o ${BUILD_DIR}/Database.o ${BUILD_DIR}/Hotel.o ${BUILD_DIR}/Room.o ${BUILD_DIR}/StandardRoom.o -o utrip.out

${BUILD_DIR}/main.o: main.cpp | build_dir
	${CC_EXEC} -c main.cpp -o ${BUILD_DIR}/main.o

${BUILD_DIR}/Database.o: ${DB_DIR}/Database.cpp ${DB_DIR}/Database.hpp | build_dir
	${CC_EXEC} -c ${DB_DIR}/Database.cpp -o ${BUILD_DIR}/Database.o

${BUILD_DIR}/Hotel.o: ${DB_DIR}/Hotel.cpp ${DB_DIR}/Hotel.hpp ${DB_DIR}/Database.hpp | build_dir
	${CC_EXEC} -c ${DB_DIR}/Hotel.cpp -o ${BUILD_DIR}/Hotel.o

${BUILD_DIR}/Room.o: ${DB_DIR}/Room.cpp ${DB_DIR}/Room.hpp ${DB_DIR}/Hotel.hpp ConstNames.hpp | build_dir
	${CC_EXEC} -c ${DB_DIR}/Room.cpp -o ${BUILD_DIR}/Room.o

${BUILD_DIR}/StandardRoom.o: ${DB_DIR}/StandardRoom.cpp ${DB_DIR}/StandardRoom.hpp ${DB_DIR}/Room.hpp ConstNames.hpp | build_dir
	${CC_EXEC} -c ${DB_DIR}/StandardRoom.cpp -o ${BUILD_DIR}/StandardRoom.o

build_dir:
	mkdir -p ${BUILD_DIR}

clean:
	rm -rf build *.out
