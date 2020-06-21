message(STATUS "in ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")

set(TARGET_NAME "protocol")

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_FILES)

add_library(${TARGET_NAME} SHARED ${SRC_FILES})

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
