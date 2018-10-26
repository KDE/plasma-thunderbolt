include(ECMAddTests)

ecm_add_test(
    managertest.cpp
    LINK_LIBRARIES kbolt libfakeserver Qt5::Test Qt5::Core Qt5::DBus
    NAME_PREFIX lib-
)

ecm_add_test(
    devicetest.cpp
    LINK_LIBRARIES kbolt libfakeserver Qt5::Test Qt5::Core Qt5::DBus
    NAME_PREFIX lib-
)
