CC_EXEC = g++ --std=c++11

BUILD_DIR = ./build
DB_DIR = ./Models
ROOMS_DIR = ${DB_DIR}/Rooms

all: utrip.out

utrip.out: ${BUILD_DIR}/main.o ${BUILD_DIR}/Database.o ${BUILD_DIR}/Hotel.o ${BUILD_DIR}/Room.o ${BUILD_DIR}/StandardRoom.o ${BUILD_DIR}/DeluxeRoom.o ${BUILD_DIR}/LuxuryRoom.o ${BUILD_DIR}/PremiumRoom.o | build_dir
	${CC_EXEC} ${BUILD_DIR}/main.o ${BUILD_DIR}/Database.o ${BUILD_DIR}/Hotel.o ${BUILD_DIR}/Room.o ${BUILD_DIR}/StandardRoom.o ${BUILD_DIR}/DeluxeRoom.o ${BUILD_DIR}/LuxuryRoom.o ${BUILD_DIR}/PremiumRoom.o -o utrip.out

${BUILD_DIR}/main.o: main.cpp | build_dir
	${CC_EXEC} -c main.cpp -o ${BUILD_DIR}/main.o

${BUILD_DIR}/Database.o: ${DB_DIR}/Database.cpp ${DB_DIR}/Database.hpp | build_dir
	${CC_EXEC} -c ${DB_DIR}/Database.cpp -o ${BUILD_DIR}/Database.o

${BUILD_DIR}/Hotel.o: ${DB_DIR}/Hotel.cpp ${DB_DIR}/Hotel.hpp ${DB_DIR}/Database.hpp | build_dir
	${CC_EXEC} -c ${DB_DIR}/Hotel.cpp -o ${BUILD_DIR}/Hotel.o

${BUILD_DIR}/Room.o: ${ROOMS_DIR}/Room.cpp ${ROOMS_DIR}/Room.hpp ${DB_DIR}/Hotel.hpp ConstNames.hpp | build_dir
	${CC_EXEC} -c ${ROOMS_DIR}/Room.cpp -o ${BUILD_DIR}/Room.o

${BUILD_DIR}/StandardRoom.o: ${ROOMS_DIR}/StandardRoom.cpp ${ROOMS_DIR}/StandardRoom.hpp ${ROOMS_DIR}/Room.hpp ConstNames.hpp | build_dir
	${CC_EXEC} -c ${ROOMS_DIR}/StandardRoom.cpp -o ${BUILD_DIR}/StandardRoom.o

${BUILD_DIR}/DeluxeRoom.o: ${ROOMS_DIR}/DeluxeRoom.cpp ${ROOMS_DIR}/DeluxeRoom.hpp ${ROOMS_DIR}/Room.hpp ConstNames.hpp | build_dir
	${CC_EXEC} -c ${ROOMS_DIR}/DeluxeRoom.cpp -o ${BUILD_DIR}/DeluxeRoom.o

${BUILD_DIR}/PremiumRoom.o: ${ROOMS_DIR}/PremiumRoom.cpp ${ROOMS_DIR}/PremiumRoom.hpp ${ROOMS_DIR}/Room.hpp ConstNames.hpp | build_dir
	${CC_EXEC} -c ${ROOMS_DIR}/PremiumRoom.cpp -o ${BUILD_DIR}/PremiumRoom.o

${BUILD_DIR}/LuxuryRoom.o: ${ROOMS_DIR}/LuxuryRoom.cpp ${ROOMS_DIR}/LuxuryRoom.hpp ${ROOMS_DIR}/Room.hpp ConstNames.hpp | build_dir
	${CC_EXEC} -c ${ROOMS_DIR}/LuxuryRoom.cpp -o ${BUILD_DIR}/LuxuryRoom.o


build_dir:
	mkdir -p ${BUILD_DIR}

clean:
	rm -rf build *.out
