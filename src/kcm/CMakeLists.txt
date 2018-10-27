add_definitions(-DTRANSLATION_DOMAIN=\"kcm_bolt\")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(KCM_BOLT_SRCS
    kcm_bolt.cpp
)

add_library(kcm_bolt MODULE ${KCM_BOLT_SRCS})
target_link_libraries(kcm_bolt
    KF5::QuickAddons
    KF5::I18n
    KF5::Declarative
    kbolt
)

kcoreaddons_desktop_to_json(kcm_bolt "kcm_bolt.desktop" SERVICE_TYPES kcmodule.desktop)

install(FILES kcm_bolt.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(TARGETS kcm_bolt DESTINATION ${KDE_INSTALL_PLUGINDIR}/kcms)

kpackage_install_package(package kcm_bolt kcms)

