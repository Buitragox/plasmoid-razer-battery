#!/usr/bin/env python3
import json
from openrazer.client import DeviceManager


def battery_icon(level: int, charging: bool) -> str:
    level = round(level, -1)

    icon = "battery-"

    if level < 100:
        icon += "0"
    icon += str(level)

    if charging:
        icon += "-charging"

    return icon


def mouse_info() -> dict:
    try:
        device_manager = DeviceManager()
        devices = [d for d in device_manager.devices if d.type == "mouse"]

        if not devices:
            return {"percentage": 0, "icon": "battery-missing"}

        device = devices[0]
        icon = battery_icon(device.battery_level, device.is_charging)
        return {"percentage": device.battery_level, "icon": icon}
    except Exception:
        return {"percentage": 0, "icon": "battery-missing"}


if __name__ == "__main__":
    print(json.dumps(mouse_info()))
