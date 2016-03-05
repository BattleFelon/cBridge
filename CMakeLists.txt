#First Try
cmake_minimum_required(VERSION 2.6)
project (BridgeApp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(OpenCV REQUIRED)
find_package(SFML 2 COMPONENTS graphics system window REQUIRED)
find_package(SFGUI REQUIRED)

include_directories(include)

SET(bridge_SRCS 	
	src/main.cpp
	src/Display.cpp
	include/Display.h
)

add_executable(BridgeApp ${bridge_SRCS})


if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
	target_link_libraries(BridgeApp ${SFML_LIBRARIES})
endif()
	
if(OpenCV_FOUND)
	include_directories(${OpenCV_INCLUDE_DIR})
	target_link_libraries(BridgeApp ${OpenCV_LIBRARIES})
endif()

if(SFGUI_FOUND)
	include_directories(${SFGUI_INCLUDE_DIR})
	target_link_libraries(BridgeApp ${SFGUI_LIBRARY})
endif()





