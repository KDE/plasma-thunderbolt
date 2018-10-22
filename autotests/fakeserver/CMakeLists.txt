include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(libfakeserver_SRCS
    exceptions.cpp
    fakedevice.cpp
    fakemanager.cpp
    fakeserver.cpp
)

qt5_add_dbus_adaptor(
    libfakeserver_SRCS
    ${CMAKE_SOURCE_DIR}/src/interfaces/org.freedesktop.bolt1.device.xml
    fakedevice.h FakeDevice fakedeviceadaptor FakeDeviceAdaptor
)
qt5_add_dbus_adaptor(
    libfakeserver_SRCS
    ${CMAKE_SOURCE_DIR}/src/interfaces/org.freedesktop.bolt1.manager.xml
    fakemanager.h FakeManager fakemanageradaptor FakeManagerAdaptor
)

add_library(libfakeserver STATIC ${libfakeserver_SRCS})
target_link_libraries(libfakeserver Qt5::Core Qt5::DBus Qt5::Test)
set_target_properties(libfakeserver PROPERTIES OUTPUT_NAME fakeserver)
target_include_directories(libfakeserver PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>;$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

#######################################################################

set(fakeserver_SRCS
    main.cpp
)

add_executable(fakeserver ${fakeserver_SRCS})
target_link_libraries(fakeserver libfakeserver Qt5::Core Qt5::DBus)
