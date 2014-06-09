# - Try to find the Kanagram Engine library.
#
# Once done this will define
#  KANAGRAM_ENGINE_FOUND - The Kanagram Engine library was found
#  KANAGRAM_ENGINE_INCLUDE_DIR - the Kanagram Engine include directory
#  KANAGRAM_ENGINE_INCLUDE_DIRS - all include directories required for the
#  KANAGRAM_ENGINE_LIBRARY - The Kanagram Engine library location
#  Kanagram Engine library
#  KANAGRAM_ENGINE_LIBRARIES - The libraries to link against to use Kanagram Engine
#
# Copyright (C) 2012 Laszlo Papp
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the COPYING-CMAKE-SCRIPTS file from Gluon's Source tree.

if (KANAGRAM_ENGINE_LIBRARY AND KANAGRAM_ENGINE_INCLUDE_DIR)
  # Already in cache, be silent
  set(KANAGRAM_ENGINE_FOUND TRUE)
endif (KANAGRAM_ENGINE_LIBRARY AND KANAGRAM_ENGINE_INCLUDE_DIR)

if (KanagramEngine_FIND_REQUIRED)
    set(_kanagramEngineReq "REQUIRED")
endif (KanagramEngine_FIND_REQUIRED)

find_path(KANAGRAM_INSTALL_PREFIX
    NAMES
    include/kanagram
)

find_package(LibKdeEdu ${_kanagramEngineReq})

if(NOT LIB_SUFFIX)
    set(_Init_LIB_SUFFIX "")
    if ("${QT_QTCORE_LIBRARY}" MATCHES lib64)
        set(_Init_LIB_SUFFIX 64)
    endif ("${QT_QTCORE_LIBRARY}" MATCHES lib64)
    if ("${QT_QTCORE_LIBRARY}" MATCHES lib32)
        set(_Init_LIB_SUFFIX 32)
    endif ("${QT_QTCORE_LIBRARY}" MATCHES lib32)

    set(LIB_SUFFIX              ${_Init_LIB_SUFFIX} CACHE STRING "The suffix of the system-wide library path")
endif(NOT LIB_SUFFIX)

if(NOT INCLUDE_INSTALL_DIR)
    set(INCLUDE_INSTALL_DIR ${KANAGRAM_INSTALL_PREFIX}/include CACHE PATH "The subdirectory relative to the install prefix where header files will be installed.")
endif()
if(NOT LIB_INSTALL_DIR)
    set(LIB_INSTALL_DIR ${KANAGRAM_INSTALL_PREFIX}/lib${LIB_SUFFIX} CACHE PATH "The subdirectory relative to the install prefix where libraries will be installed.")
endif()
if(NOT SHARE_INSTALL_DIR)
    set(SHARE_INSTALL_DIR ${KANAGRAM_INSTALL_PREFIX}/share CACHE PATH "The subdiractory relative to the install prefix where data and other files will be installed.")
endif()

find_path(KANAGRAM_ENGINE_INCLUDE_DIR
    NAMES
    kanagram/kanagramgame.h
    PATHS
    ${INCLUDE_INSTALL_DIR}
    PATH_SUFFIXES
    kanagram
)

set(KANAGRAM_ENGINE_INCLUDE_DIRS
    ${KANAGRAM_ENGINE_INCLUDE_DIR}
    ${QT_INCLUDES}
    ${KDE4_INCLUDES}
)

find_library(KANAGRAM_ENGINE_LIBRARY
    NAMES
    kanagramengine
    PATHS
    ${LIB_INSTALL_DIR}
)

set(KANAGRAM_ENGINE_LIBRARIES
    ${QT_QTCORE_LIBRARY}
    ${KDE4_KDECORE_LIBS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(KanagramEngine DEFAULT_MSG KANAGRAM_ENGINE_LIBRARY KANAGRAM_ENGINE_INCLUDE_DIR)
