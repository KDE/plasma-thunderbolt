add_definitions(-DTRANSLATION_DOMAIN=\"kcm_bolt\")
include_directories(${CMAKE_CURRENT_BINARY_DIR})


add_library(kcm_bolt MODULE)
kcmutils_generate_desktop_file(kcm_bolt)
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

install(TARGETS kcm_bolt DESTINATION ${KDE_INSTALL_PLUGINDIR}/plasma/kcms/systemsettings)

kpackage_install_package(package kcm_bolt kcms)

