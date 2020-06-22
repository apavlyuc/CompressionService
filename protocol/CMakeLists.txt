message(STATUS "in ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

set(TARGET_NAME "protocol")

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.hpp)

add_library(${TARGET_NAME} SHARED ${SRC_FILES})

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

## install rules
if (WIN32)
	set(CMAKE_INSTALL_PREFIX "C:/my_libs")
	message("set CMAKE_INSTALL_PREFIX to ${CMAKE_INSTALL_PREFIX}")

	install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
	install(FILES ${HEADER_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/protocol)
endif()
