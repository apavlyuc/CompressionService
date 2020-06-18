message(STATUS "in PreLoad.cmake")

if (WIN32)
    message("WIN32")

	set(CMAKE_GENERATOR "MinGW Makefiles" CACHE PATH "")
	message("set CMAKE_GENERATOR to ${CMAKE_GENERATOR}")
endif(WIN32)