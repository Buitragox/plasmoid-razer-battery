#pragma once

#include <QObject>
#include <QTimer>

class RazerBattery : public QObject {
    Q_OBJECT

    Q_PROPERTY(int batteryPercent READ batteryPercent NOTIFY batteryPercentChanged)
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)

  public:
    explicit RazerBattery(QObject* parent = nullptr);

    [[nodiscard]] int batteryPercent() const;
    [[nodiscard]] QString icon() const;

    Q_INVOKABLE void refresh();

  Q_SIGNALS:
    void batteryPercentChanged();
    void iconChanged();

  private:
    void setBatteryPercent(int value);
    void setCharging(bool value);
    void updateIcon();
    bool resolveDevicePath();

    int m_batteryPercent = -1;
    bool m_charging = false;
    QString m_icon = QStringLiteral("battery-missing");
    QString m_devicePath;

    QTimer m_timer;
};