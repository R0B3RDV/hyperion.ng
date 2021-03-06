cmake_minimum_required(VERSION 2.8)
project(hyperion-v4l2)

find_package(Qt5Widgets REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}    ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}/../../libsrc/protoserver
	${PROTOBUF_INCLUDE_DIRS}
	${QT_INCLUDES}
)

set(Hyperion_V4L2_QT_HEADERS
	ScreenshotHandler.h
)

set(Hyperion_V4L2_HEADERS
)

set(Hyperion_V4L2_SOURCES
	hyperion-v4l2.cpp
	ScreenshotHandler.cpp
)

QT5_WRAP_CPP(Hyperion_V4L2_MOC_SOURCES ${Hyperion_V4L2_QT_HEADERS})

add_executable(${PROJECT_NAME}
	${Hyperion_V4L2_HEADERS}
	${Hyperion_V4L2_SOURCES}
	${Hyperion_V4L2_QT_HEADERS}
	${Hyperion_V4L2_MOC_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
	effectengine
	v4l2-grabber
	commandline
	blackborder
	hyperion-utils
	protoserver
	pthread
	${QT_LIBRARIES}
)

qt5_use_modules(${PROJECT_NAME} Core Gui Network)

install ( TARGETS ${PROJECT_NAME} DESTINATION "share/hyperion/bin/" COMPONENT "${PLATFORM}" )
