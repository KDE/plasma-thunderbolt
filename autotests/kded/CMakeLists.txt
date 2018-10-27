include(ECMAddTests)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/kded
)

ecm_add_test(
    kdedtest.cpp
    TEST_NAME kdedtest
    LINK_LIBRARIES kbolt kded_bolt libfakeserver Qt5::Test Qt5::Core Qt5::DBus
    NAME_PREFIX kded-
)

