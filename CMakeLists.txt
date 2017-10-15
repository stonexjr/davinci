# Jinrong Xie (stonexjr at gmail.com) 06/08/2015
project(Davinci)

cmake_minimum_required(VERSION 3.1.3 FATAL_ERROR)

#Enable C++11 on Linux
if(${UNIX})
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    elseif(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")
    endif()
    # Qt5 require the source code is compiled with -fPIC
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

IF(WIN32)
    # hide insecure CRT warnings, common practice
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
    #[[
    IF(MSVC)
        SET( CMAKE_DEBUG_POSTFIX "d" )
    ENDIF(MSVC)
    #]]
    
    IF(NOT(MSVC_VERSION LESS "1600"))
        ADD_DEFINITIONS(-D_WIN32_WINNT=0x0601)
        ADD_DEFINITIONS(-DWINVER=0x0601)
    ELSEIF(NOT(MSVC_VERSION LESS "1300"))
        ADD_DEFINITIONS(-D_WIN32_WINNT=0x0501)
        ADD_DEFINITIONS(-DWINVER=0x0501)
    ELSE()
        ADD_DEFINITIONS(-D_WIN32_WINNT=0x0500)
        ADD_DEFINITIONS(-DWINVER=0x0500)
    ENDIF()

ENDIF()

# Find includes in corresponding build directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)


if(NOT DAVINCI_BINARY_DIR)
  set(DAVINCI_BINARY_DIR ${CMAKE_BINARY_DIR} CACHE PATH 
    "Single output directory for all libs and exes.")
endif(NOT DAVINCI_BINARY_DIR)

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE( "64 bits compiler detected" )
    SET( PLATFORM_NAME "x64" )
else( CMAKE_SIZEOF_VOID_P EQUAL 8 ) 
    MESSAGE( "32 bits compiler detected" )
    SET( PLATFORM_NAME "x86" )
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    MESSAGE( "Debug build" )
    SET( CONFIGURATION_NAME "Debug" )
else( CMAKE_BUILD_TYPE STREQUAL "Debug" ) 
    MESSAGE( "Release build" )
    SET( CONFIGURATION_NAME "Release" )
endif( CMAKE_BUILD_TYPE STREQUAL "Debug" )


#Set where the output lib file would be.
set(LIBRARY_OUTPUT_PATH "${DAVINCI_BINARY_DIR}/lib/${PLATFORM_NAME}")
set(EXECUTABLE_OUTPUT_PATH "${DAVINCI_BINARY_DIR}/bin")

set(DAVINCI_ROOT_DIR "${CMAKE_SOURCE_DIR}")
set(DAVINCI_INC_DIR "${DAVINCI_ROOT_DIR}/include")
set(DAVINCI_SRC_DIR "${DAVINCI_ROOT_DIR}/src")
set(DAVINCI_LIB_DIR "${LIBRARY_OUTPUT_PATH}")

#Prompt user to specify glew installation root.
FIND_PACKAGE(GLEW QUIET)
IF(GLEW_FOUND AND GLEW_INCLUDE_DIRS)
    MESSAGE("glew is found!")
    MESSAGE("glew include dir: ${GLEW_INCLUDE_DIRS}")
    MESSAGE("glew lib dir: ${GLEW_LIBRARIES}")
ELSE()
    MESSAGE("\nglew cannot be automatically detected on your machine.")
    MESSAGE("Please install and manually set glew root: GLEW_ROOT_DIR in the cmake")
    SET(GLEW_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glew" CACHE PATH
        "Please specify glew installation directory")
    SET(GLEW_INCLUDE_DIRS "${GLEW_ROOT_DIR}/include")
    SET(GLEW_LIB_DIR "${GLEW_ROOT_DIR}/lib")
    IF(WIN32)
        SET(GLEW_LIBRARIES "glew")
        SET(GLEW_LIB_DIR2 "${GLEW_LIB_DIR}/${PLATFORM_NAME}")
    ELSE()
        SET(GLEW_LIBRARIES "GLEW")
    ENDIF()
    MESSAGE("glew root dir GLEW_ROOT_DIR is set to: ${GLEW_ROOT_DIR}")
    MESSAGE("glew possible lib dir: ${GLEW_LIB_DIR}")
    MESSAGE("glew possible lib dir: ${GLEW_LIB_DIR2}")
ENDIF()

SET(INCS ${INCS} ${GLEW_INCLUDE_DIRS})
SET(LIBS ${LIBS} ${GLEW_LIBRARIES})

#Prompt user to specify freeglut installation root.
FIND_PACKAGE(FREEGLUT QUIET)
IF(FREEGLUT_FOUND AND FREEGLUT_INCLUDE_DIRS)
    MESSAGE("\nfreeglut is auto detected!")
    MESSAGE("freeglut include dir: ${FREEGLUT_INCLUDE_DIRS}")
    MESSAGE("freeglut lib dir: ${FREEGLUT_LIBRARIES}")
ELSE()
    MESSAGE("\nfreeglut cannot be automatically detected on your machine.")
    MESSAGE("Please install and manually set freeglut root: FREEGLUT_ROOT_DIR when you run cmake")
    SET(FREEGLUT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/freeglut" CACHE PATH
        "Please specify freeglut installation directory")
    SET(FREEGLUT_INCLUDE_DIRS "${FREEGLUT_ROOT_DIR}/include")
    SET(FREEGLUT_LIB_DIR "${FREEGLUT_ROOT_DIR}/lib")
    SET(FREEGLUT_LIBRARIES "freeglut")
    IF(WIN32)
        SET(FREEGLUT_LIB_DIR2 "${FREEGLUT_LIB_DIR}/${PLATFORM_NAME}")
    ENDIF()
    MESSAGE("freeglut root dir FREEGLUT_ROOT_DIR is set to: ${FREEGLUT_ROOT_DIR}")
    MESSAGE("freeglut possible lib dir: ${FREEGLUT_LIB_DIR}")
    MESSAGE("freeglut possible lib dir: ${FREEGLUT_LIB_DIR2}")
    MESSAGE("freeglut bin dir: ${FREEGLUT_BIN_DIR}")
ENDIF()

SET(INCS ${INCS} ${FREEGLUT_INCLUDE_DIRS})
SET(LIBS ${LIBS} ${FREEGLUT_LIBRARIES})


#Prompt user to specify freetype installation root.
OPTION(DAVINCI_ENABLE_TEXT_RENDERING "Enable text rendering (optional), requiring installation of freetype first." OFF)

IF(DAVINCI_ENABLE_TEXT_RENDERING)
    ADD_DEFINITIONS(-DENABLE_TEXT_RENDERING)
    FIND_PACKAGE(Freetype QUIET)
    IF(FREETYPE_FOUND AND FREETYPE_INCLUDE_DIRS)
        MESSAGE("\nfreetype is auto detected!")
        MESSAGE("freetype include dir: ${FREETYPE_INCLUDE_DIRS}")
        MESSAGE("freetype lib dir: ${FREETYPE_LIBRARIES}")
    ELSE()
        SET(FREETYPE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/freetype-2.5.2" CACHE PATH
            "Please specify freetype installation directory")
        SET(FREETYPE_INCLUDE_DIRS "${FREETYPE_ROOT_DIR}/include")
        SET(FREETYPE_LIB_DIR "${FREETYPE_ROOT_DIR}/lib")
        SET(FREETYPE_LIBRARIES "freetype")
        IF(WIN32)
            SET(FREETYPE_LIB_DIR2 "${FREETYPE_ROOT_DIR}/lib/${PLATFORM_NAME}")
        ENDIF()
        MESSAGE("freetype root dir: ${FREETYPE_ROOT_DIR}")
        MESSAGE("freetype lib dir: ${FREETYPE_LIB_DIR}")
        MESSAGE("freetype lib dir: ${FREETYPE_LIB_DIR2}")
    ENDIF()
ENDIF()
SET(INCS ${INCS} ${FREETYPE_INCLUDE_DIRS})
SET(LIBS ${LIBS} ${FREETYPE_LIBRARIES})


INCLUDE_DIRECTORIES(
"${DAVINCI_INC_DIR}" 
${INCS}
)

LINK_DIRECTORIES(
${GLEW_LIB_DIR} 
${GLEW_LIB_DIR2} 
${FREEGLUT_LIB_DIR} 
${FREEGLUT_LIB_DIR2} 
${FREETYPE_LIB_DIR}
${FREETYPE_LIB_DIR2}
)

ADD_SUBDIRECTORY(src)

INSTALL(DIRECTORY ./include DESTINATION ./)
#"Debug" or "Release" will automatically appended 
#INSTALL(FILES ${LIBRARY_OUTPUT_PATH}/${CONFIGURATION_NAME}/DAVINCI.lib DESTINATION ./lib/${PLATFORM_NAME}/${CONFIGURATION_NAME})
INSTALL(DIRECTORY ${LIBRARY_OUTPUT_PATH} DESTINATION ./lib)
