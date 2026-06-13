cmake_minimum_required(VERSION 3.16)

project(plasma-razerbattery)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(QT_MIN_VERSION "6.6.0")
set(KF_MIN_VERSION "6.0.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core
    Qml
    DBus
    Quick
)

find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Kirigami
)

find_package(Plasma CONFIG REQUIRED)

add_subdirectory(plugin)

plasma_install_package(
    package
    com.github.buitragox.razerbattery
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
