cmake_minimum_required(VERSION 3.16)

project(kcm_bolt)

set(CMAKE_CXX_STANDARD 14)

#################################################
# Dependencies

set(QT5_VERSION "5.10")
set(KF5_VERSION "5.70")

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
if(ECM_VERSION VERSION_GREATER_EQUAL 5.79)
    include(KDEGitCommitHooks)
endif()

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

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
add_subdirectory(src)

################################################

# add clang-format target for all our real source files
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
if(ECM_VERSION VERSION_GREATER_EQUAL 5.79)
    kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
