QML_PATH := $(shell qtpaths6 --query QT_INSTALL_QML 2>/dev/null || /usr/lib/qt6/bin/qtpaths6 --query QT_INSTALL_QML)

.PHONY: build install run clean

build:
	cmake -B build -DCMAKE_INSTALL_PREFIX=/usr -DKDE_INSTALL_QMLDIR=$(QML_PATH)
	cmake --build build

install: build
	sudo cmake --install build

run: install
	plasmoidviewer -a com.github.buitragox.razerbattery

clean:
	rm -rf build
