pragma ComponentBehavior: Bound

import QtQuick
import org.kde.plasma.plasmoid
import org.kde.plasma.components as PlasmaComponents
import org.kde.kirigami as Kirigami

import com.github.buitragox.private.razerbattery as Razer

PlasmoidItem {
    id: root

    property alias batteryPercent: razerBattery.batteryPercent
    property alias batteryIcon: razerBattery.icon

    Razer.RazerBattery {
        id: razerBattery
    }

    compactRepresentation: Kirigami.Icon {
        source: root.batteryIcon
        implicitWidth: Kirigami.Units.iconSizes.smallMedium
        implicitHeight: Kirigami.Units.iconSizes.smallMedium
    }

    fullRepresentation: PlasmaComponents.Label {
        text: root.batteryPercent >= 0 ? "Battery: " + root.batteryPercent + "%" : "Battery unavailable"
    }

    toolTipMainText: "Razer Battery"
    toolTipSubText: razerBattery.batteryPercent >= 0 ? razerBattery.batteryPercent + "%" : "Unavailable"
}
