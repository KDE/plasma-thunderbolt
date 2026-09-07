add_definitions(-DTRANSLATION_DOMAIN=\"kcm_bolt\")

kcmutils_add_qml_kcm(kcm_bolt SOURCES kcm_bolt.cpp thunderboltmoduledata.cpp)
target_link_libraries(kcm_bolt PRIVATE
    Qt6::Qml
    KF6::KCMUtilsQuick
    KF6::I18n
    kbolt
)
