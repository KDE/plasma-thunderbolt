cmake_minimum_required(VERSION 3.16)

project(kcm_bolt)

set(CMAKE_CXX_STANDARD 14)

#################################################
# Dependencies
set(QT_MIN_VERSION "6.10.0")
set(KF6_VERSION "5.240.0")
set(KDE_COMPILERSETTINGS_LEVEL "5.82")

find_package(ECM ${KF6_VERSION} REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${ECM_MODULE_PATH})

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
include(ECMDeprecationSettings)

kde_enable_exceptions()

#################################################
# Dependencies

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core Quick DBus)
if (BUILD_TESTING)
    find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Test)
endif()

find_package(KF6 ${KF6_VERSION} REQUIRED COMPONENTS
    CoreAddons
    KCMUtils
    I18n
    DBusAddons
    Notifications
)

find_package(Bolt)
set_package_properties(Bolt PROPERTIES DESCRIPTION "Thunderbolt device manager"
                       URL "https://gitlab.freedesktop.org/bolt/bolt"
                       PURPOSE "Runtime-only dependency for Thunderbolt KCM"
                       TYPE RUNTIME)

#################################################
ecm_set_disabled_deprecation_versions(QT 6.9.0
    KF 6.16.0
)


if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
add_subdirectory(src)

################################################

# add clang-format target for all our real source files
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)

ki18n_install(po)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
