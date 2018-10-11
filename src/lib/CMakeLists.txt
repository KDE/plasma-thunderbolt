include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(LIBKBOLT_SRCS
    dbushelper.cpp
    device.cpp
    devicemodel.cpp
    enum.cpp
    exceptions.cpp
    manager.cpp
)

ecm_qt_declare_logging_category(LIBKBOLT_SRCS
    HEADER libkbolt_debug.h
    IDENTIFIER log_libkbolt
    CATEGORY_NAME org.kde.libkbolt
)

qt5_add_dbus_interfaces(
    LIBKBOLT_SRCS

    ../interfaces/org.freedesktop.bolt1.manager.xml
    ../interfaces/org.freedesktop.bolt1.device.xml
)

add_library(kbolt SHARED ${LIBKBOLT_SRCS})
generate_export_header(kbolt)
target_link_libraries(kbolt
    Qt5::Core
    Qt5::DBus
    KF5::I18n
    stdc++
)


install(TARGETS kbolt DESTINATION ${KDE_INSTALL_LIBDIR})
