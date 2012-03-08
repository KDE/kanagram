include_directories(
    ${KDE4_INCLUDES}
    ${QT_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(kanagram-active_SRCS
    kdeclarativemainwindow.cpp
    kdeclarativeview.cpp
    main.cpp
    mainwindow.cpp
)

kde4_add_executable(kanagram-active ${gluonplayer-active_SRCS})

target_link_libraries(kanagram-active
    ${QT_QTDECLARATIVE_LIBRARY}
    ${QT_QTSCRIPT_LIBRARY}
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_PLASMA_LIBS}
    ${KANAGRAM_ENGINE_LIBRARIES}
    kdeclarative
)

install(
    TARGETS
    kanagram-active
    ${INSTALL_TARGETS_DEFAULT_ARGS}
)
