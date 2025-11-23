cmake_minimum_required(VERSION 3.16)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "25")
set(RELEASE_SERVICE_VERSION_MINOR "11")
set(RELEASE_SERVICE_VERSION_MICRO "90")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kanagram VERSION ${RELEASE_SERVICE_VERSION})

set(QT_REQUIRED_VERSION "6.4.0")
set(KF_MIN_VERSION "6.3.0")

find_package(ECM ${KF_MIN_VERSION} NO_MODULE REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} )

include(ECMAddAppIcon)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMInstallIcons)
include(ECMSetupVersion)
include(ECMDeprecationSettings)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -Wno-switch -O2 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wno-switch -O2")
    set(CMAKE_CXX_FLAGS_DEBUG  "-Wall -Wno-switch -g3 -ggdb -O0" CACHE STRING "Debug options." FORCE)
endif(NOT MSVC)

include(FeatureSummary)
find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick)
find_package(Qt6 QUIET OPTIONAL_COMPONENTS TextToSpeech)
if (NOT Qt6TextToSpeech_FOUND)
    message(STATUS "Qt6TextToSpeech not found, speech feature will be disabled")
else()
    add_definitions(-DHAVE_SPEECH)
endif()


find_package(KF6 REQUIRED COMPONENTS
    I18n
    Crash
    Sonnet
    Config
    ConfigWidgets
    CoreAddons
    DocTools
    KIO
    NewStuff
    XmlGui
)
include_directories (${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )

find_package(LibKEduVocDocument REQUIRED)

ecm_set_disabled_deprecation_versions(QT 6.9.0  KF 6.5.0)

add_subdirectory( doc )
add_subdirectory( src )
add_subdirectory( icons )

ki18n_install(po)
if (KF6DocTools_FOUND)
    kdoctools_install(po)
endif()
install(FILES org.kde.kanagram.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
