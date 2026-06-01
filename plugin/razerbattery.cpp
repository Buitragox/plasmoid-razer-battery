#include "razerbattery.h"

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QtGlobal>

RazerBattery::RazerBattery(QObject *parent)
    : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &RazerBattery::refresh);
    m_timer.setInterval(30'000);
    m_timer.start();

    refresh();
}

int RazerBattery::batteryPercent() const
{
    return m_batteryPercent;
}

QString RazerBattery::icon() const
{
    return m_icon;
}

void RazerBattery::refresh()
{
    const QString service = QStringLiteral("org.razer");

    // TODO: Make this dynamic/configurable
    const QString path = QStringLiteral("/org/razer/device/632305H15702820");
    const QString interface = QStringLiteral("razer.device.power");

    QDBusInterface razerInterface(
        service,
        path,
        interface,
        QDBusConnection::sessionBus()
    );

    if (!razerInterface.isValid()) {
        qWarning() << "[RazerBattery] D-Bus interface not valid."
                   << razerInterface.lastError().name()
                   << razerInterface.lastError().message();
        setBatteryPercent(-1);
        setCharging(false);
        return;
    }

    QDBusReply<double> batteryReply =
        razerInterface.call(QStringLiteral("getBattery"));

    if (!batteryReply.isValid()) {
        qWarning() << "[RazerBattery] getBattery() failed:"
                   << batteryReply.error().name()
                   << batteryReply.error().message();
        setBatteryPercent(-1);
        setCharging(false);
        return;
    }

    setBatteryPercent(static_cast<int>(batteryReply.value()));

    // isCharging() is always available on the razer.device.power interface
    // alongside getBattery().
    QDBusReply<bool> chargingReply =
        razerInterface.call(QStringLiteral("isCharging"));

    if (chargingReply.isValid()) {
        setCharging(chargingReply.value());
    } else {
        qWarning() << "[RazerBattery] isCharging() failed:"
                   << chargingReply.error().name()
                   << chargingReply.error().message();
        setCharging(false);
    }
}

void RazerBattery::setBatteryPercent(int value)
{
    value = qBound(-1, value, 100);

    if (m_batteryPercent == value) {
        return;
    }

    m_batteryPercent = value;
    Q_EMIT batteryPercentChanged();

    updateIcon();
}

void RazerBattery::setCharging(bool value)
{
    if (m_charging == value) {
        return;
    }

    m_charging = value;
    updateIcon();
}

void RazerBattery::updateIcon()
{
    const QString previousIcon = m_icon;

    if (m_batteryPercent < 0) {
        m_icon = QStringLiteral("battery-missing");
    } else {
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
