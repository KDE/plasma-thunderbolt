cmake_minimum_required(VERSION 3.16)

project(kcm_bolt)

set(CMAKE_CXX_STANDARD 14)

#################################################
# Dependencies
set(QT_MIN_VERSION "5.15.0")
set(KF5_VERSION "5.86")
set(KDE_COMPILERSETTINGS_LEVEL "5.82")

find_package(ECM ${KF5_VERSION} REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${ECM_MODULE_PATH} ${CMAKE_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEClangFormat)
include(ECMQtDeclareLoggingCategory)
include(GenerateExportHeader)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(FeatureSummary)
include(KDEGitCommitHooks)

kde_enable_exceptions()

#################################################
# Dependencies

find_package(Qt5Core ${QT_VERSION} REQUIRED)
find_package(Qt5Quick ${QT_VERSION} REQUIRED)
find_package(Qt5DBus ${QT_VERSION} REQUIRED)
if (BUILD_TESTING)
    find_package(Qt5Test ${QT_VERSION} REQUIRED)
endif()

find_package(KF5CoreAddons ${KF5_VERSION} REQUIRED)
find_package(KF5KCMUtils ${KF5_VERSION} REQUIRED)
find_package(KF5Declarative ${KF5_VERSION} REQUIRED)
find_package(KF5I18n ${KF5_VERSION} REQUIRED)
find_package(KF5DBusAddons ${KF5_VERSION} REQUIRED)
find_package(KF5Notifications ${KF5_VERSION} REQUIRED)

find_package(Bolt)
set_package_properties(Bolt PROPERTIES DESCRIPTION "Thunderbolt device manager"
                       URL "https://gitlab.freedesktop.org/bolt/bolt"
                       PURPOSE "Runtime-only dependency for Thunderbolt KCM"
                       TYPE RUNTIME)

#################################################
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050c00)
add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x055800)

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
add_subdirectory(src)

################################################

# add clang-format target for all our real source files
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
