cmake_minimum_required(VERSION 2.8)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}

    ${KANAGRAM_ENGINE_INCLUDE_DIRS}
)

set(kanagram-harmattan_SRCS
    main.cpp
    kanagramenginehelper.cpp
)

# generate rules for building source files from the resources
QT4_ADD_RESOURCES(KANAGRAM-HARMATTAN_RCC_SRCS
    kanagram-harmattan.qrc
)

kde4_add_executable(kanagram-harmattan ${kanagram-harmattan_SRCS} ${KANAGRAM-HARMATTAN_RCC_SRCS})

target_link_libraries(kanagram-harmattan
    ${QT_QTDECLARATIVE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
    ${KANAGRAM_ENGINE_LIBRARIES}
    ${KDE4_KDECORE_LIBS}
    kdeclarative
)

install(TARGETS kanagram-harmattan
        DESTINATION ${BIN_INSTALL_DIR}
        COMPONENT kanagramharmattan)

install(FILES kanagram.sh
        DESTINATION ${BIN_INSTALL_DIR}
        COMPONENT kanagramharmattan)

install(FILES kanagram-harmattan.desktop
        DESTINATION ${SHARE_INSTALL_DIR}/applications
        COMPONENT kanagramharmattan)

install(FILES kanagram.conf
        DESTINATION ${SHARE_INSTALL_DIR}/policy/etc/pulse/xpolicy.conf.d
        COMPONENT kanagramharmattan)

if(NOT KANAGRAM_ICONS_INSTALLED)
    add_subdirectory(../../icons icons)
    set(KANAGRAM_ICONS_INSTALLED TRUE)
endif()
