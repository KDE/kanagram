include_directories(
    ${KDE4_INCLUDES}
    ${QT_INCLUDES}
)

add_subdirectory(src)

install(
    DIRECTORY package/
    DESTINATION ${DATA_INSTALL_DIR}/plasma/packages/org.kde.kanagram)

install(
    FILES kanagram-active.desktop
    DESTINATION ${XDG_APPS_INSTALL_DIR}
)
