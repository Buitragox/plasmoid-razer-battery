#include "razerbattery.h"
#include "qtmetamacros.h"

#include <qcontainerfwd.h>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QLatin1Char>
#include <QString>
#include <QStringList>
#include <QStringLiteral>
#include <QTimer>
#include <QtLogging>
#include <QtMinMax>

RazerBattery::RazerBattery(QObject* parent) : QObject(parent) {
    connect(&m_timer, &QTimer::timeout, this, &RazerBattery::refresh);
    m_timer.setInterval(30'000);
    m_timer.start();

    refresh();
}

int RazerBattery::batteryPercent() const {
    return m_batteryPercent;
}

QString RazerBattery::icon() const {
    return m_icon;
}

bool RazerBattery::resolveDevicePath() {
    if (!m_devicePath.isEmpty()) {
        return true;
    }

    const QString service = QStringLiteral("org.razer");
    const QString path = QStringLiteral("/org/razer");
    const QString interface = QStringLiteral("razer.devices");

    QDBusInterface devicesInterface(service, path, interface, QDBusConnection::sessionBus());

    if (!devicesInterface.isValid()) {
        qWarning() << "[RazerBattery] D-Bus devices interface not valid." << devicesInterface.lastError().name()
                   << devicesInterface.lastError().message();
        return false;
    }

    const QDBusReply<QStringList> reply = devicesInterface.call(QStringLiteral("getDevices"));

    if (!reply.isValid()) {
        qWarning() << "[RazerBattery] getDevices() failed:" << reply.error().name() << reply.error().message();
        return false;
    }

    const QStringList devices = reply.value();

    if (devices.isEmpty()) {
        qWarning() << "[RazerBattery] No Razer devices found.";
        return false;
    }

    m_devicePath = QStringLiteral("/org/razer/device/%1").arg(devices.first());
    qInfo() << "[RazerBattery] Using device path:" << m_devicePath;

    return true;
}

void RazerBattery::refresh() {
    if (!resolveDevicePath()) {
        setBatteryPercent(-1);
        setCharging(false);
        return;
    }

    const QString service = QStringLiteral("org.razer");
    const QString interface = QStringLiteral("razer.device.power");

    QDBusInterface razerInterface(service, m_devicePath, interface, QDBusConnection::sessionBus());

    if (!razerInterface.isValid()) {
        qWarning() << "[RazerBattery] D-Bus interface not valid." << razerInterface.lastError().name()
                   << razerInterface.lastError().message();
        // Device may have disconnected; clear the path so we re-resolve next
        // time.
        m_devicePath.clear();
        setBatteryPercent(-1);
        setCharging(false);
        return;
    }

    const QDBusReply<double> batteryReply = razerInterface.call(QStringLiteral("getBattery"));

    if (!batteryReply.isValid()) {
        qWarning() << "[RazerBattery] getBattery() failed:" << batteryReply.error().name()
                   << batteryReply.error().message();
        setBatteryPercent(-1);
        setCharging(false);
        return;
    }

    setBatteryPercent(static_cast<int>(batteryReply.value()));

    // isCharging() is always available on the razer.device.power interface
    // alongside getBattery().
    const QDBusReply<bool> chargingReply = razerInterface.call(QStringLiteral("isCharging"));

    if (chargingReply.isValid()) {
        setCharging(chargingReply.value());
    } else {
        qWarning() << "[RazerBattery] isCharging() failed:" << chargingReply.error().name()
                   << chargingReply.error().message();
        setCharging(false);
    }
}

void RazerBattery::setBatteryPercent(int value) {
    value = qBound(-1, value, 100);

    if (m_batteryPercent == value) {
        return;
    }

    m_batteryPercent = value;
    Q_EMIT batteryPercentChanged();

    updateIcon();
}

void RazerBattery::setCharging(bool value) {
    if (m_charging == value) {
        return;
    }

    m_charging = value;
    updateIcon();
}

void RazerBattery::updateIcon() {
    const QString previousIcon = m_icon;

    if (m_batteryPercent < 0) {
        m_icon = QStringLiteral("battery-missing");
    } else {
        // Ceiling-round to the nearest 10 to match available icon levels
        // (battery-010, battery-020, ..., battery-100).
        int bucket = ((m_batteryPercent + 9) / 10) * 10;
        bucket = qBound(10, bucket, 100);

        const QString level = QStringLiteral("%1").arg(bucket, 3, 10, QLatin1Char('0'));

        if (m_charging) {
            m_icon = QStringLiteral("battery-%1-charging").arg(level);
        } else {
            m_icon = QStringLiteral("battery-%1").arg(level);
        }
    }

    if (m_icon != previousIcon) {
        Q_EMIT iconChanged();
    }
}
