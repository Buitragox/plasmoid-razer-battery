#!/bin/sh
set -e

cmake -B build -DCMAKE_INSTALL_PREFIX=/usr -DKDE_INSTALL_QMLDIR=$(qtpaths6 --qml-import-path | head -1)
cmake --build build
sudo cmake --install build

plasmoidviewer -a com.github.buitragox.razerbattery
