message(STATUS "in ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

set(TARGET_NAME "Server")

if (WIN32)
	message("WIN32")

	set(Boost_USE_STATIC_LIBS ON)
	message("set Boost_USE_STATIC_LIBS to ${Boost_USE_STATIC_LIBS}")

	set(Boost_ARCHITECTURE "-x64")
	set(Boost_COMPILER "-mgw8")
	message("set Boost_ARCHITECTURE to ${Boost_ARCHITECTURE}\nset Boost_COMPILER to ${Boost_COMPILER}")

	set(BOOST_ROOT "D:/apavlyuc/coding/dev_libs/Boost")
	set(BOOST_LIBRARYDIR "D:/apavlyuc/coding/dev_libs/Boost/lib")
	set(Boost_INCLUDE_DIR "D:/apavlyuc/coding/dev_libs/Boost/include/boost-1_73")
	message("set BOOST_ROOT to ${BOOST_ROOT}\nset BOOST_LIBRARYDIR to ${BOOST_LIBRARYDIR}\nset Boost_INCLUDE_DIR to ${Boost_INCLUDE_DIR}")

	if (MINGW)
		message("MINGW")

		set(MINGW_LIBS "ws2_32")
		message("set MINGW_LIBS to ${MINGW_LIBS}")
	endif()
endif()

find_package(Boost 1.73 REQUIRED
	COMPONENTS
		thread
)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_FILES)

add_executable(${TARGET_NAME} ${SRC_FILES})
target_include_directories(${TARGET_NAME}
	PRIVATE
		${Boost_INCLUDE_DIRS}
		${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(${TARGET_NAME} ${MINGW_LIBS} ${Boost_LIBRARIES})