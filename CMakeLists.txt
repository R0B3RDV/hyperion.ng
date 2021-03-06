# Define the main-project name
project(hyperion)

# define the minimum cmake version (as required by cmake)
cmake_minimum_required(VERSION 2.8)

IF ( POLICY CMP0026 )
	CMAKE_POLICY( SET CMP0026 OLD )
ENDIF()

IF ( POLICY CMP0043 )
	CMAKE_POLICY( SET CMP0043 OLD )
ENDIF()

SET ( HYPERION_VERSION_STABLE OFF )
SET ( HYPERION_VERSION_MAJOR 2 )
SET ( HYPERION_VERSION_MINOR 0 )
SET ( HYPERION_VERSION_PATCH 0 )

SET ( DEFAULT_AMLOGIC    OFF )
SET ( DEFAULT_DISPMANX   OFF )
SET ( DEFAULT_FB         ON )
SET ( DEFAULT_OSX        OFF )
SET ( DEFAULT_X11        OFF )
SET ( DEFAULT_SPIDEV     ON )
SET ( DEFAULT_WS2812BPWM OFF )
SET ( DEFAULT_WS281XPWM  OFF )
SET ( DEFAULT_V4L2       ON  )
SET ( DEFAULT_USE_SHARED_AVAHI_LIBS  OFF )
SET ( DEFAULT_USE_AVAHI_LIBS  ON )
SET ( DEFAULT_USE_SYSTEM_PROTO_LIBS OFF )
SET ( DEFAULT_TESTS      OFF )

if ( NOT DEFINED PLATFORM )
	if ( "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "x86" )
		SET( PLATFORM "x86")
	elseif ( "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm" )
		SET( PLATFORM "rpi")
		EXEC_PROGRAM( "nproc" OUTPUT_VARIABLE SYSTEM_CPU_CORES )
		FILE( READ /proc/cpuinfo SYSTEM_CPUINFO )
		STRING ( TOLOWER "${SYSTEM_CPUINFO}" SYSTEM_CPUINFO )
		if ( "${SYSTEM_CPUINFO}" MATCHES "amlogic" AND ${CMAKE_SIZEOF_VOID_P} EQUAL 4 )
			SET( PLATFORM "amlogic" )
		elseif ( "${SYSTEM_CPUINFO}" MATCHES "amlogic" AND ${CMAKE_SIZEOF_VOID_P} EQUAL 8 )
			SET( PLATFORM "amlogic64" )
		elseif( SYSTEM_CPU_CORES GREATER 1 AND "${SYSTEM_CPUINFO}" MATCHES "bcm" )
			SET( PLATFORM "rpi-pwm" )
		endif()
	endif()
	if ( PLATFORM )
		message( STATUS "PLATFORM is not defined, evaluated platform: ${PLATFORM}")
	else()
		message( FATAL_ERROR "PLATFORM is not defined and could not be evaluated. Set -DPLATFORM=<rpi|pri-pwm|amlogic|amlogic64|x86>")
	endif()
endif()

if ( "${PLATFORM}" STREQUAL "rpi" )
	SET ( DEFAULT_DISPMANX   ON )
elseif ( "${PLATFORM}" STREQUAL "rpi-pwm" )
	SET ( DEFAULT_DISPMANX   ON )
	SET ( DEFAULT_WS2812BPWM ON )
	SET ( DEFAULT_WS281XPWM  ON )
elseif ( "${PLATFORM}" STREQUAL "wetek" OR  "${PLATFORM}" STREQUAL "amlogic" )
	SET ( DEFAULT_AMLOGIC    ON )
elseif ( "${PLATFORM}" STREQUAL "amlogic64" )
	SET ( DEFAULT_AMLOGIC    ON )
	SET ( DEFAULT_USE_SYSTEM_PROTO_LIBS ON )
elseif ( "${PLATFORM}" STREQUAL "x86" )
	SET ( DEFAULT_X11        ON )
	SET ( DEFAULT_USE_SHARED_AVAHI_LIBS  ON )
elseif ( "${PLATFORM}" STREQUAL "x86-dev" )
	SET ( DEFAULT_X11        ON )
	SET ( DEFAULT_AMLOGIC    ON)
	SET ( DEFAULT_WS281XPWM  ON )
	SET ( DEFAULT_USE_SHARED_AVAHI_LIBS  ON )
	SET ( DEFAULT_TESTS      ON )
elseif ( "${PLATFORM}" STREQUAL "imx6" )
	SET ( DEFAULT_FB         ON )
endif()

if (APPLE)
	set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "/usr/local/opt/qt5")
	include_directories("/opt/X11/include/")
	SET ( DEFAULT_OSX    ON )
	SET ( DEFAULT_V4L2   OFF )
	SET ( DEFAULT_SPIDEV OFF )
	SET ( DEFAULT_FB     OFF )
	SET ( DEFAULT_WS281XPWM  OFF )
	SET ( DEFAULT_USE_AVAHI_LIBS  OFF )
	SET ( DEFAULT_USE_SHARED_AVAHI_LIBS  OFF )
endif ()

# set the build options
option(ENABLE_AMLOGIC "Enable the AMLOGIC video grabber" ${DEFAULT_AMLOGIC} )
message(STATUS "ENABLE_AMLOGIC = " ${ENABLE_AMLOGIC})

option(ENABLE_DISPMANX "Enable the RPi dispmanx grabber" ${DEFAULT_DISPMANX} )
message(STATUS "ENABLE_DISPMANX = " ${ENABLE_DISPMANX})

option(ENABLE_FB "Enable the framebuffer grabber" ${DEFAULT_FB} )
message(STATUS "ENABLE_FB = " ${ENABLE_FB})

option(ENABLE_OSX "Enable the osx grabber" ${DEFAULT_OSX} )
message(STATUS "ENABLE_OSX = " ${ENABLE_OSX})

option(ENABLE_SPIDEV   "Enable the SPIDEV device" ${DEFAULT_SPIDEV} )
message(STATUS "ENABLE_SPIDEV   = " ${ENABLE_SPIDEV})

option(ENABLE_TINKERFORGE "Enable the TINKERFORGE device" ON)
message(STATUS "ENABLE_TINKERFORGE = " ${ENABLE_TINKERFORGE})

option(ENABLE_V4L2 "Enable the V4L2 grabber" ${DEFAULT_V4L2})
message(STATUS "ENABLE_V4L2 = " ${ENABLE_V4L2})

option(ENABLE_WS2812BPWM   "Enable the WS2812b-PWM device" ${DEFAULT_WS2812BPWM} )
message(STATUS "ENABLE_WS2812BPWM = " ${ENABLE_WS2812BPWM})

option(ENABLE_WS281XPWM   "Enable the WS281x-PWM device" ${DEFAULT_WS281XPWM} )
message(STATUS "ENABLE_WS281XPWM = " ${ENABLE_WS281XPWM})

option(ENABLE_X11 "Enable the X11 grabber" ${DEFAULT_X11})
message(STATUS "ENABLE_X11 = " ${ENABLE_X11})

SET(ENABLE_QT5 ON)

option(ENABLE_TESTS "Compile additional test applications" ${DEFAULT_TESTS})
message(STATUS "ENABLE_TESTS = " ${ENABLE_TESTS})

option(ENABLE_PROFILER "enable profiler capabilities - not for release code" OFF)
message(STATUS "ENABLE_PROFILER = " ${ENABLE_PROFILER})


SET ( PROTOBUF_INSTALL_BIN_DIR ${CMAKE_BINARY_DIR}/proto )
SET ( PROTOBUF_INSTALL_LIB_DIR ${CMAKE_BINARY_DIR}/proto )

# Createt the configuration file

# Add project specific cmake modules (find, etc)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(GitVersion)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file("${PROJECT_SOURCE_DIR}/HyperionConfig.h.in" "${PROJECT_BINARY_DIR}/HyperionConfig.h")
include_directories("${PROJECT_BINARY_DIR}")


# Define the global output path of binaries
SET(LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH})
file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})

# Add the project include directory as additional include path
include_directories(${CMAKE_SOURCE_DIR}/dependencies/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Prefer static linking over dynamic
#set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.so")

# enable C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "No support for C++11 detected. Compilation will most likely fail on your compiler")
endif()

SET(QT_MIN_VERSION "5.2.0")
find_package(Qt5 COMPONENTS Core Gui Network REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}    ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
message( STATUS "Found Qt Version: ${Qt5Core_VERSION}" )
IF ( "${Qt5Core_VERSION}" VERSION_LESS "${QT_MIN_VERSION}" )
	message( FATAL_ERROR "Your Qt version is to old! Minimum required ${QT_MIN_VERSION}" )
ENDIF()


#add libusb and pthreads
find_package(libusb-1.0 REQUIRED)
find_package(Threads REQUIRED)
add_definitions(${QT_DEFINITIONS})

# TODO[TvdZ]: This linking directory should only be added if we are cross compiling
if(NOT APPLE)
	link_directories(${CMAKE_FIND_ROOT_PATH}/lib/arm-linux-gnueabihf)
endif()

if(ENABLE_OSX)
	set(CMAKE_EXE_LINKER_FLAGS "-framework CoreGraphics")
endif()

# Add the source/lib directories
add_subdirectory(dependencies)
add_subdirectory(libsrc)
add_subdirectory(src)
if (ENABLE_TESTS)
	add_subdirectory(test)
endif ()

# Add the doxygen generation directory
add_subdirectory(doc)

# uninstall target
configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

# enable make package - no code after this line !
include (${CMAKE_CURRENT_SOURCE_DIR}/cmake/packages.cmake)


