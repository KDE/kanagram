cmake_minimum_required(VERSION 3.16)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "22")
set(RELEASE_SERVICE_VERSION_MINOR "11")
set(RELEASE_SERVICE_VERSION_MICRO "70")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kanagram VERSION ${RELEASE_SERVICE_VERSION})

find_package(ECM 5.90.0 NO_MODULE REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} )
set(KDE_COMPILERSETTINGS_LEVEL "5.82")
include(ECMAddAppIcon)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMInstallIcons)
include(ECMSetupVersion)


if(NOT MSVC)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -Wno-switch -O2 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wno-switch -O2")
    set(CMAKE_CXX_FLAGS_DEBUG  "-Wall -Wno-switch -g3 -ggdb -O0" CACHE STRING "Debug options." FORCE)
endif(NOT MSVC)

include(FeatureSummary)
find_package(Qt${QT_MAJOR_VERSION} REQUIRED COMPONENTS Core Qml Quick)
find_package(Qt${QT_MAJOR_VERSION} QUIET OPTIONAL_COMPONENTS TextToSpeech)
if (NOT Qt${QT_MAJOR_VERSION}TextToSpeech_FOUND)
    message(STATUS "Qt${QT_MAJOR_VERSION}TextToSpeech not found, speech feature will be disabled")
else()
    add_definitions(-DHAVE_SPEECH)
endif()


find_package(KF5 REQUIRED COMPONENTS
    I18n
    Crash
    Sonnet
    Config
    ConfigWidgets
    CoreAddons
    DocTools
    KIO
    NewStuff
)
include_directories (${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )

find_package(LibKEduVocDocument REQUIRED)
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)



add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( icons )

ki18n_install(po)
if (KF5DocTools_FOUND)
    kdoctools_install(po)
endif()
install(FILES org.kde.kanagram.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
