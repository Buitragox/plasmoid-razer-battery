pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.plasma.plasmoid
import org.kde.plasma.components as PlasmaComponents
import org.kde.plasma.plasma5support as Plasma5Support

PlasmoidItem {
    id: main

    // Internal state
    property int batteryLevel: 0
    property string icon: "battery-missing"

    // Timer to refresh every 30 seconds
    Timer {
        id: timer
        interval: 30000
        running: true
        repeat: true
        triggeredOnStart: true
        onTriggered: executable.exec()
    }

    Plasma5Support.DataSource {
        id: executable
        engine: "executable"
        connectedSources: []
        onNewData: (sourceName, data) => {
            try {
                console.log("data:", data.stdout)
                let res = JSON.parse(data.stdout);
                main.batteryLevel = res.percentage;
                main.icon = res.icon;
            } catch (e) {
                console.log("Error parsing Razer battery data:", e);
                console.log(data.stdout);
            }
            disconnectSource(sourceName);
        }

        function exec() {
            const url = Qt.resolvedUrl("../../mouse_battery.py")
            const path = String(url).replace("file://", "")
            connectSource(path);
        }
    }

    // The visual representation in the panel
    RowLayout {
        anchors.centerIn: parent
        spacing: 6

        PlasmaComponents.Label {
            text: main.batteryLevel + "%"
            // color: main.batteryLevel < 15 ? "red" : "white"
        }
        Kirigami.Icon {
            source: main.icon
            implicitWidth: 16
            implicitHeight: 16
        }
    }
}
