# Copyright (C) 2018 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
# Authors: Jonas Gonzalez & Henry Powell
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.6)
SET(KEYWORD "MANUSYARP")
PROJECT(${KEYWORD})
set(CMAKE_CXX_STANDARD 11)
set(MANUS_LIB_PATH /usr/local/lib)


FIND_PACKAGE(YARP REQUIRED)

#search for includes
INCLUDE_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}/include
        ${YARP_INCLUDE_DIRS}
        ${MANUS_VR_INCLUDE_DIRS}
)

# Search for source code.
FILE(GLOB folder_source src/*.cpp src/*.cc src/*.c)
FILE(GLOB folder_header include/iCub/*.h)
SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})


# Set up our main executable
IF (folder_source)
    ADD_EXECUTABLE(${KEYWORD}
            ${folder_source}
            ${folder_header}
            )

    TARGET_LINK_LIBRARIES( ${KEYWORD}
            ${YARP_LIBRARIES}
            ${MANUS_LIB_PATH}/libmanus.so
            )

    INSTALL_TARGETS(/bin ${KEYWORD})

ELSE (folder_source)
    MESSAGE( "No source code files found. Please add something")

ENDIF (folder_source)