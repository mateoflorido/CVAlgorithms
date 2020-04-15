CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## ================
## = Project name =
## ================

PROJECT( Borders )

## ============
## = Packages =
## ============

FIND_PACKAGE( OpenCV REQUIRED )

## ================================================
## = Do not allow to build inside the source tree =
## ================================================

IF(PROJECT_BINARY_DIR STREQUAL ${PROJECT_SOURCE_DIR})
  MESSAGE(FATAL_ERROR "Building in the source tree is not allowed")
ENDIF(PROJECT_BINARY_DIR STREQUAL ${PROJECT_SOURCE_DIR})

## =====================================
## = Where to put executables and libs =
## =====================================

include_directories( ${OpenCV_INCLUDE_DIRS} )

## =======================
## = Programs to compile =
## =======================

ADD_EXECUTABLE( border border.cxx )
TARGET_LINK_LIBRARIES( border ${OpenCV_LIBS} )
