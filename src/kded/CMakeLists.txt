add_definitions(-DTRANSLATION_DOMAIN=\"kded_bolt\")

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(KDED_BOLT_SRCS
    kded_bolt.cpp
)

ecm_qt_declare_logging_category(KDED_BOLT_SRCS
    HEADER kded_bolt_debug.h
    IDENTIFIER log_kded_bolt
    CATEGORY_NAME org.kde.bolt.kded
)

add_library(kded_bolt MODULE ${KDED_BOLT_SRCS})
kcoreaddons_desktop_to_json(kded_bolt kded_bolt.desktop)
target_link_libraries(kded_bolt
    KF5::DBusAddons
    KF5::I18n
    KF5::Notifications
    KF5::CoreAddons
    kbolt
)

install(TARGETS kded_bolt DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf5/kded)
