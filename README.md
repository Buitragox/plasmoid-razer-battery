# Razer Mouse Battery Widget

Simple widget for displaying the battery of your Razer mouse in Plasma 6 Desktop. Communicates with the [OpenRazer](https://openrazer.github.io/) daemon over D-Bus.

![Example image](images/full-battery-charging.png)

## Dependencies

- Qt 6.6+
- KDE Frameworks 6 (ECM, Kirigami)
- Plasma 6
- [OpenRazer](https://openrazer.github.io/) daemon running

## Install

```sh
git clone https://github.com/Buitragox/plasmoid-razer-mouse-battery.git
cd plasmoid-razer-mouse-battery
make install
```

This builds and installs both the C++ plugin and the QML package. You can then add the widget to your panel:

1. Right click your task bar
2. "Add or Manage Widgets"
3. The widget should show up in the list.
4. Drag and drop it to your task bar.

## Upgrade

After updating the repo (or making changes yourself), run `make install` again.

To apply the update you can:
- Restart your session
- or run `plasmashell --replace`

## Configuration

The widget automatically discovers connected Razer devices via `razer.devices.getDevices()` and uses the first one found. If the device disconnects, it will re-resolve on the next poll cycle.

A future update will add a settings UI to select a specific device when multiple are connected.
