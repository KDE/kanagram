cmake_minimum_required(VERSION 3.5)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "21")
set(RELEASE_SERVICE_VERSION_MINOR "07")
set(RELEASE_SERVICE_VERSION_MICRO "90")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kanagram VERSION ${RELEASE_SERVICE_VERSION})

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -Wno-switch -O2 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wno-switch -O2")
    set(CMAKE_CXX_FLAGS_DEBUG  "-Wall -Wno-switch -g3 -ggdb -O0" CACHE STRING "Debug options." FORCE)
endif(NOT MSVC)

include(FeatureSummary)
find_package(Qt5 REQUIRED COMPONENTS Core Qml Quick)
find_package(Qt5 QUIET OPTIONAL_COMPONENTS TextToSpeech)
if (NOT Qt5TextToSpeech_FOUND)
    message(STATUS "Qt5TextToSpeech not found, speech feature will be disabled")
else()
    add_definitions(-DHAVE_SPEECH)
endif()

find_package(ECM 5.17.0 NO_MODULE REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} )

include(ECMAddAppIcon)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMInstallIcons)
include(ECMSetupVersion)

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
if (${KF5Config_VERSION} STRGREATER "5.56.0")
        add_definitions(-DQT_NO_FOREACH)
        MESSAGE(STATUS "compile without foreach")
endif()



add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( icons )

ki18n_install(po)
if (KF5DocTools_FOUND)
    kdoctools_install(po)
endif()
install(FILES org.kde.kanagram.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
