include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(fakeserver_SRCS
    exceptions.cpp
    fakedevice.cpp
    fakemanager.cpp
    main.cpp
)

qt5_add_dbus_adaptor(
    fakeserver_SRCS
    ${CMAKE_SOURCE_DIR}/src/interfaces/org.freedesktop.bolt1.device.xml
    fakedevice.h FakeDevice fakedeviceadaptor FakeDeviceAdaptor
)
qt5_add_dbus_adaptor(
    fakeserver_SRCS
    ${CMAKE_SOURCE_DIR}/src/interfaces/org.freedesktop.bolt1.manager.xml
    fakemanager.h FakeManager fakemanageradaptor FakeManagerAdaptor
)

add_executable(fakeserver ${fakeserver_SRCS})
target_link_libraries(fakeserver Qt5::Core Qt5::DBus stdc++)
