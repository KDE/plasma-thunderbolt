project(kcm_bolt)


cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)

option(BUILD_TESTING "Build autotests" ON)

#################################################
# Dependencies

set(QT5_VERSION "5.10")
set(KF5_VERSION "5.40")

find_package(ECM ${KF5_VERSION} REQUIRED)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_MODULE_PATH})

include(KDECompilerSettings)
include(ECMQtDeclareLoggingCategory)
include(GenerateExportHeader)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(CheckIncludeFiles)
include(CheckSymbolExists)

kde_enable_exceptions()

#################################################
# Dependencies

find_package(Qt5Core ${QT_VERSION} REQUIRED)
find_package(Qt5Quick ${QT_VERSION} REQUIRED)
find_package(Qt5DBus ${QT_VERSION} REQUIRED)
find_package(Qt5Test ${QT_VERSION} REQUIRED)

find_package(KF5CoreAddons ${KF5_VERSION} REQUIRED)
find_package(KF5KCMUtils ${KF5_VERSION} REQUIRED)
find_package(KF5Declarative ${KF5_VERSION} REQUIRED)
find_package(KF5I18n ${KF5_VERSION} REQUIRED)
find_package(KF5DBusAddons ${KF5_VERSION} REQUIRED)
find_package(KF5Notifications ${KF5_VERSION} REQUIRED)

#################################################

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
add_subdirectory(src)
