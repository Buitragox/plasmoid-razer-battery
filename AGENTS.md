# Project: Razer Battery Plasmoid

## Overview
A KDE Plasma 6 widget (plasmoid/applet) that displays battery information for a Razer device by communicating with the [OpenRazer](https://openrazer.github.io/) daemon over D-Bus.

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
├── Makefile                # Build/install/run/format/tidy targets
├── .clang-format           # clang-format configuration (C++17, 4-space indent, 120 col)
├── .clang-tidy             # clang-tidy configuration (checks, naming conventions)
├── .clangd                 # clangd LSP configuration
├── .qmlls.ini              # QML language server configuration (import paths)
├── package/                # Plasma package (QML + metadata)
│   ├── metadata.json       # Plugin metadata (name, id, icon, etc.)
│   └── contents/
│       └── ui/
│           └── main.qml    # Widget UI (compact: icon, full: label with %)
├── plugin/                 # C++ QML plugin
│   ├── CMakeLists.txt      # Builds shared lib `razerbatteryplugin`
│   ├── qmldir              # QML module registration + typeinfo reference
│   ├── plugins.qmltypes    # Type info for QML tooling (qmlls, IDE support)
│   ├── razerbattery.h/.cpp # RazerBattery class — polls D-Bus for battery state
│   ├── razerplugin.h/.cpp  # QQmlExtensionPlugin — registers RazerBattery type
└── build/                  # Build output (gitignored)
    └── compile_commands.json
```

## Architecture

### C++ Plugin (`plugin/`)
- **RazerBattery** (`QObject`): Exposes `batteryPercent` (int) and `icon` (QString) as Q_PROPERTY. Polls the OpenRazer D-Bus interface every 30 seconds. Has a `Q_INVOKABLE refresh()` method. Dynamically discovers the first connected device via `getDevices()` and re-resolves if it disconnects.
- **RazerPlugin** (`QQmlExtensionPlugin`): Registers `RazerBattery` as a QML type under the URI `com.github.buitragox.private.razerbattery`.
- The compiled shared library is installed to the QML import path.

### D-Bus Interface
- **Service:** `org.razer`
- **Path:** `/org/razer/device/<serial>` (resolved dynamically via `razer.devices.getDevices()`)
- **Interface:** `razer.device.power`
- **Methods called:**
  - `razer.devices.getDevices()` → QStringList (serial numbers) — on `/org/razer`
  - `razer.device.power.getBattery()` → double (0–100) — on `/org/razer/device/<serial>`
  - `razer.device.power.isCharging()` → bool — on `/org/razer/device/<serial>`

### QML UI (`package/contents/ui/main.qml`)
- Compact representation: battery icon (changes based on level and charging state)
- Full representation: text label showing percentage
- Tooltip with battery percentage

## Build & Run
```sh
make install   # build + install
make run       # build + install + launch plasmoidviewer
make clean     # remove build directory
make format    # run clang-format on plugin/*.h and plugin/*.cpp
make tidy      # run clang-tidy on plugin/*.cpp (requires build)
```

**Important:** `KDE_INSTALL_QMLDIR` must match Qt6's actual QML import path (check with `qtpaths6 --query QT_INSTALL_QML`). ECM's default may differ.

CMake is configured to always generate `compile_commands.json` (for clangd).

## Known TODOs
- Add settings UI to select a specific device when multiple are connected

## Dependencies (system packages)
- Qt 6.6+: Core, Qml, DBus, Quick
- KDE Frameworks 6: ECM, Kirigami
- Plasma 6 libraries
- OpenRazer daemon running with D-Bus session bus

## Dev Environment
- **C++ LSP:** clangd — configured via `.clangd` to use `build/compile_commands.json`
- **QML LSP:** qmlls — configured via `.qmlls.ini` (import paths + build dir)
- **QML type info:** `plugin/plugins.qmltypes` describes C++ types for qmlls; referenced from `qmldir` via `typeinfo` directive
- After `make install`, restart qmlls if it doesn't pick up changes to the installed `.qmltypes` file
