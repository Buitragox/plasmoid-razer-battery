# Project: Razer Mouse Battery Plasmoid

## Overview
A KDE Plasma 6 widget (plasmoid/applet) that displays battery information for a Razer mouse by communicating with the [OpenRazer](https://openrazer.github.io/) daemon over D-Bus.

**Applet ID:** `com.github.buitragox.razerbattery`
**Author:** Jhoan Buitrago

## Tech Stack
- **C++17** with **Qt 6.6+** and **KDE Frameworks 6 (KF6)**
- **QML** for the UI (Plasma Components, Kirigami)
- **CMake** build system (uses ECM — Extra CMake Modules)
- **D-Bus** to talk to the OpenRazer daemon (`org.razer` service)

## Project Structure
```
.
├── CMakeLists.txt          # Top-level CMake config
├── run.sh                  # Build, install, and launch script
├── .clangd                 # clangd LSP configuration
├── package/                # Plasma package (QML + metadata)
│   ├── metadata.json       # Plugin metadata (name, id, icon, etc.)
│   └── contents/
│       └── ui/
│           └── main.qml    # Widget UI (compact: icon, full: label with %)
├── plugin/                 # C++ QML plugin
│   ├── CMakeLists.txt      # Builds shared lib `razerbatteryplugin`
│   ├── qmldir              # QML module registration
│   ├── razerbattery.h/.cpp # RazerBattery class — polls D-Bus for battery state
│   ├── razerplugin.h/.cpp  # QQmlExtensionPlugin — registers RazerBattery type
└── build/                  # Build output (gitignored)
    └── compile_commands.json
```

## Architecture

### C++ Plugin (`plugin/`)
- **RazerBattery** (`QObject`): Exposes `batteryPercent` (int) and `icon` (QString) as Q_PROPERTY. Polls the OpenRazer D-Bus interface every 30 seconds. Has a `Q_INVOKABLE refresh()` method.
- **RazerPlugin** (`QQmlExtensionPlugin`): Registers `RazerBattery` as a QML type under the URI `com.github.buitragox.private.razerbattery`.
- The compiled shared library is installed to the QML import path.

### D-Bus Interface
- **Service:** `org.razer`
- **Path:** `/org/razer/device/<serial>` (currently hardcoded — TODO: make dynamic)
- **Interface:** `razer.device.power`
- **Methods called:** `getBattery()` → int, `isCharging()` → bool

### QML UI (`package/contents/ui/main.qml`)
- Compact representation: battery icon (changes based on level and charging state)
- Full representation: text label showing percentage
- Tooltip with battery percentage

## Build & Run
```sh
./run.sh
```
This does: `cmake -B build` → `cmake --build build` → `sudo cmake --install build` → `plasmoidviewer -a com.github.buitragox.razerbattery`

CMake is configured to always generate `compile_commands.json` (for clangd).

## Known TODOs
- Device path is hardcoded — should be dynamic/configurable
- `isCharging()` may not be available for all mice — needs fallback
- The `Qt6::DBus` find_package has a TODO about whether it can be removed (it cannot — it's essential)

## Dependencies (system packages)
- Qt 6.6+: Core, Qml, DBus, Gui, Quick
- KDE Frameworks 6: ECM, KI18n, Kirigami
- Plasma 6 libraries
- OpenRazer daemon running with D-Bus session bus
