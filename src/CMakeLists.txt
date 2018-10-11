include_directories(${CMAKE_CURRENT_BINARY_DIR})


set(KCM_BOLT_SRCS
    dbushelper.cpp
    device.cpp
    enum.cpp
    exceptions.cpp
    manager.cpp
)

qt5_add_dbus_interfaces(
    KCM_BOLT_SRCS

    interfaces/org.freedesktop.bolt1.manager.xml
    interfaces/org.freedesktop.bolt1.device.xml
)

add_library(kcm_bolt MODULE ${KCM_BOLT_SRCS})
target_link_libraries(kcm_bolt
    Qt5::Quick
    Qt5::Core
    Qt5::Qml
    Qt5::DBus
    KF5::KCMUtils
    KF5::I18n
)

install(TARGETS kcm_bolt DESTINATION ${KDE_INSTALL_PLUGINDIR})
