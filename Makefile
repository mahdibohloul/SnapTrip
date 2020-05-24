CC_EXEC = g++ --std=c++11

BUILD_DIR = ./build
DB_DIR = ./Models
ROOMS_DIR = ${DB_DIR}/Rooms
USER_DIR = ${DB_DIR}/User
FILTER_DIR = ${DB_DIR}/Filters

all: utrip.out

utrip.out: ${BUILD_DIR}/main.o ${BUILD_DIR}/Database.o ${BUILD_DIR}/Hotel.o ${BUILD_DIR}/Room.o ${BUILD_DIR}/StandardRoom.o ${BUILD_DIR}/DeluxeRoom.o ${BUILD_DIR}/LuxuryRoom.o ${BUILD_DIR}/PremiumRoom.o ${BUILD_DIR}/ReserveCase.o ${BUILD_DIR}/User.o ${BUILD_DIR}/Filter.o ${BUILD_DIR}/CityFilter.o ${BUILD_DIR}/AvgPriceFilter.o ${BUILD_DIR}/AdvancedFilter.o ${BUILD_DIR}/Rating.o ${BUILD_DIR}/Comment.o | build_dir
	${CC_EXEC} ${BUILD_DIR}/main.o ${BUILD_DIR}/Database.o ${BUILD_DIR}/Hotel.o ${BUILD_DIR}/Room.o ${BUILD_DIR}/StandardRoom.o ${BUILD_DIR}/DeluxeRoom.o ${BUILD_DIR}/LuxuryRoom.o ${BUILD_DIR}/PremiumRoom.o ${BUILD_DIR}/ReserveCase.o ${BUILD_DIR}/User.o ${BUILD_DIR}/Filter.o ${BUILD_DIR}/CityFilter.o ${BUILD_DIR}/AvgPriceFilter.o ${BUILD_DIR}/AdvancedFilter.o ${BUILD_DIR}/Rating.o ${BUILD_DIR}/Comment.o -o utrip.out

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

${BUILD_DIR}/ReserveCase.o: ${USER_DIR}/ReserveCase.cpp ${USER_DIR}/ReserveCase.hpp ${ROOMS_DIR}/Room.hpp ConstNames.hpp | build_dir
	${CC_EXEC} -c ${USER_DIR}/ReserveCase.cpp -o ${BUILD_DIR}/ReserveCase.o

${BUILD_DIR}/User.o: ${USER_DIR}/User.cpp ${USER_DIR}/User.hpp ${USER_DIR}/ReserveCase.hpp | build_dir
	${CC_EXEC} -c ${USER_DIR}/User.cpp -o ${BUILD_DIR}/User.o

${BUILD_DIR}/Filter.o: ${FILTER_DIR}/Filter.cpp ${FILTER_DIR}/Filter.hpp ${USER_DIR}/User.hpp | build_dir
	${CC_EXEC} -c ${FILTER_DIR}/Filter.cpp -o ${BUILD_DIR}/Filter.o

${BUILD_DIR}/AdvancedFilter.o: ${FILTER_DIR}/AdvancedFilter.cpp ${FILTER_DIR}/AdvancedFilter.hpp ${FILTER_DIR}/Filter.hpp | build_dir
	${CC_EXEC} -c ${FILTER_DIR}/AdvancedFilter.cpp -o ${BUILD_DIR}/AdvancedFilter.o

${BUILD_DIR}/CityFilter.o: ${FILTER_DIR}/CityFilter.cpp ${FILTER_DIR}/CityFilter.hpp ${FILTER_DIR}/Filter.hpp | build_dir
	${CC_EXEC} -c ${FILTER_DIR}/CityFilter.cpp -o ${BUILD_DIR}/CityFilter.o

${BUILD_DIR}/AvgPriceFilter.o: ${FILTER_DIR}/AvgPriceFilter.cpp ${FILTER_DIR}/AvgPriceFilter.hpp ${FILTER_DIR}/Filter.hpp | build_dir
	${CC_EXEC} -c ${FILTER_DIR}/AvgPriceFilter.cpp -o ${BUILD_DIR}/AvgPriceFilter.o


${BUILD_DIR}/Rating.o: ${DB_DIR}/Rating.cpp ${DB_DIR}/Rating.hpp ${USER_DIR}/User.hpp | build_dir
	${CC_EXEC} -c ${DB_DIR}/Rating.cpp -o ${BUILD_DIR}/Rating.o


${BUILD_DIR}/Comment.o: ${DB_DIR}/Comment.cpp ${DB_DIR}/Comment.cpp ${USER_DIR}/User.hpp | build_dir
	${CC_EXEC} -c ${DB_DIR}/Comment.cpp -o ${BUILD_DIR}/Comment.o

build_dir:
	mkdir -p ${BUILD_DIR}

clean:
	rm -rf build *.out
