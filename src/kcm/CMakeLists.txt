add_definitions(-DTRANSLATION_DOMAIN=\"kcm_bolt\")
include_directories(${CMAKE_CURRENT_BINARY_DIR})


add_library(kcm_bolt MODULE)
target_sources(kcm_bolt PRIVATE
    kcm_bolt.cpp
    kcm_bolt.h
)
set_target_properties(kcm_bolt PROPERTIES CXX_STANDARD 14)
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

