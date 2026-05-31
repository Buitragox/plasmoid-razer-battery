# Razer Mouse Battery Widget

Simple widget for displaying the battery of your razer mouse in Plasma6 Desktop.

![Example image](images/full-battery-charging.png)

## Install

First, clone the repo and install the widget:
```sh
git clone https://github.com/Buitragox/plasmoid-razer-mouse-battery.git
cd plasmoid-razer-mouse-battery
kpackagetool6 -t Plasma/Applet -i ./package
```

Then:

1. Right click your task bar
2. "Add or Manage Widgets"
3. The widget should show up in the list.
4. Drag and drop it to your task bar.

## Upgrade
After updating the repo (or making changes yourself):
```sh
kpackagetool6 -t Plasma/Applet -u ./package
```

To apply the update you can: 
- Restart your session
- or run `plasmashell --replace`
