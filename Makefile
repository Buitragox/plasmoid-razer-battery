QML_PATH := $(shell qtpaths6 --query QT_INSTALL_QML 2>/dev/null || /usr/lib/qt6/bin/qtpaths6 --query QT_INSTALL_QML)

.PHONY: build install run clean format tidy

build:
	cmake -B build -DCMAKE_INSTALL_PREFIX=/usr -DKDE_INSTALL_QMLDIR=$(QML_PATH)
	cmake --build build
	# Strip GCC-specific flags that clang-tidy doesn't recognize
	sed -i 's/-mno-direct-extern-access//g' build/compile_commands.json

install: build
	sudo cmake --install build

run: install
	plasmoidviewer -a com.github.buitragox.razerbattery

clean:
	rm -rf build

format:
	clang-format -i plugin/*.h plugin/*.cpp

tidy: build
	clang-tidy -p build plugin/*.cpp
