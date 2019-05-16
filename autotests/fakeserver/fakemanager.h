/*
 * Copyright (c) 2018 - 2019  Daniel Vrátil <dvratil@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FAKEMANAGER_H_
#define FAKEMANAGER_H_

#include <QObject>
#include <QDBusObjectPath>
#include <QList>
#include <QString>
#include <QJsonObject>

#include <memory>

class FakeManagerException : public std::runtime_error
{
public:
    FakeManagerException(const QString &what)
        : std::runtime_error(what.toStdString())
    {}
};

class FakeDevice;
class FakeManager : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.bolt1.Manager")

    Q_PROPERTY(unsigned int Version READ version CONSTANT)
    Q_PROPERTY(bool Probing READ isProbing CONSTANT)
    Q_PROPERTY(QString DefaultPolicy READ defaultPolicy CONSTANT)
    Q_PROPERTY(QString SecurityLevel READ securityLevel CONSTANT)
    Q_PROPERTY(QString AuthMode READ authMode WRITE setAuthMode)

public:
    explicit FakeManager(const QJsonObject &json, QObject *parent = nullptr);
    explicit FakeManager(QObject *parent = nullptr);
    ~FakeManager() override;

    unsigned int version() const;
    bool isProbing() const;
    QString defaultPolicy() const;
    QString securityLevel() const;
    QString authMode() const;
    void setAuthMode(const QString &authMode);

    FakeDevice *addDevice(std::unique_ptr<FakeDevice> device);
    void removeDevice(const QString &uid);
    QList<FakeDevice *> devices() const;

    Q_INVOKABLE QList<QDBusObjectPath> ListDevices() const;
    Q_INVOKABLE QDBusObjectPath DeviceByUid(const QString &uid) const;
    Q_INVOKABLE QDBusObjectPath EnrollDevice(const QString &uid, const QString &policy, const QString &flags);
    Q_INVOKABLE void ForgetDevice(const QString &uid);

Q_SIGNALS:
    void DeviceAdded(const QDBusObjectPath &device);
    void DeviceRemoved(const QDBusObjectPath &device);

private:
    bool mProbing = false;
    QString mDefaultPolicy = QStringLiteral("auto");
    QString mSecurityLevel = QStringLiteral("none");
    QString mAuthMode = QStringLiteral("enabled");

    QMap<QString /*uid*/, FakeDevice*> mDevices;
};


#endif
